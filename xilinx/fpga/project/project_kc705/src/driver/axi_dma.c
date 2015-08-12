#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/kthread.h>

#include "utils/xiaofei_debug.h"

#include "pcie.h"
#include "dma_common.h"

//AXI DMA Register Summary
#define MM2S_DMACR 0x00 //MM2S DMA Control register
#define MM2S_DMASR 0x04 //MM2S DMA Status register
//08h – 14h Reserved N/A
#define MM2S_SA 0x18 //MM2S Source Address. Lower 32 bits of address.
#define MM2S_SA_MSB 0x1C //MM2S Source Address. Upper 32 bits of address.
#define MM2S_LENGTH 0x28 //MM2S Transfer Length (Bytes)
#define S2MM_DMACR 0x30 //S2MM DMA Control register
#define S2MM_DMASR 0x34 //S2MM DMA Status register38h – 44h Reserved N/A
#define S2MM_DA 0x48 //S2MM Destination Address. Lower 32 bit address.
#define S2MM_DA_MSB 0x4C //S2MM Destination Address. Upper 32 bit address.
#define S2MM_LENGTH 0x58 //S2MM Buffer Length (Bytes)

static irqreturn_t process_isr(void *ppara) {
	irqreturn_t status = IRQ_NONE;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;


	value = readl(dma_base_vaddr + MM2S_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		complete(&dma->tx_cmp);
		//mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		complete(&dma->tx_cmp);
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	}

	value = readl(dma_base_vaddr + S2MM_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		complete(&dma->rx_cmp);
		//mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		complete(&dma->rx_cmp);
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	}

	/* Handle DMA and any user interrupts */
	return status;
}

static int prepare_bars_map(pcie_dma_t *dma, uint64_t tx_bar_map_addr, uint64_t rx_bar_map_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *bar_vddr_map_ctrl_reg = (uint32_t *)(base_vaddr + OFFSET_AXI_PCIe_CTL + dma->pcie_bar_map_ctl_offset);

	write_addr_to_reg(bar_vddr_map_ctrl_reg, tx_bar_map_addr);
	bar_vddr_map_ctrl_reg += 2;
	write_addr_to_reg(bar_vddr_map_ctrl_reg, rx_bar_map_addr);

	return 0;
}

static int init_dma(pcie_dma_t *dma) {
	int ret;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 

	uint32_t value;
	uint32_t mask;
	uint32_t default_value = 0x00010002;//0000_0000_0000_0001_0000_0000_0000_0010

	//reset dma
	mask = BITMASK(2)/*reset bit*/;
	value = default_value | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	ret = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMACR, mask, 0);
	if(ret != 0) {
		mydebug("time out!\n");
	}

	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	ret = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMASR, mask, 0);
	if(ret != 0) {
		mydebug("time out!\n");
	}
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);

	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	ret = wait_for_iostatus_timeout(0, dma_base_vaddr + S2MM_DMASR, mask, 0);
	if(ret != 0) {
		mydebug("time out!\n");
	}
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);

	return ret;
}

static int transfer_des_setting(pcie_dma_t *dma, uint64_t tx_axi_addr, uint64_t rx_axi_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	value = (uint32_t)(tx_axi_addr);
	writel(value, dma_base_vaddr + MM2S_SA);

	value = (uint32_t)(rx_axi_addr);
	writel(value, dma_base_vaddr + S2MM_DA);

	return 0;
}

static int dma_prepare_transfer(pcie_dma_t *dma, uint64_t tx_bar_map_addr, uint64_t rx_bar_map_addr, uint64_t tx_axi_addr, uint64_t rx_axi_addr) {
	int ret = 0;

	prepare_bars_map(dma, tx_bar_map_addr, rx_bar_map_addr);
	transfer_des_setting(dma, tx_axi_addr, rx_axi_addr);
	return ret;
}

static int start_dma_mm2s(pcie_dma_t *dma, int tx_size) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	init_completion(&dma->tx_cmp);
	value = (uint32_t)tx_size;
	writel(value, dma_base_vaddr + MM2S_LENGTH);

	return 0;
}

static int start_dma_s2mm(pcie_dma_t *dma, int rx_size) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	init_completion(&dma->rx_cmp);
	value = (uint32_t)rx_size;
	writel(value, dma_base_vaddr + S2MM_LENGTH);

	return 0;
}

static int dma_trans_sync(pcie_dma_t *dma) {
	unsigned long tmo;
	int ret = 0;

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&dma->tx_cmp, tmo);
	if (0 == tmo) {
		printk("tx transfer timed out!\n");
		ret = -1;
	}

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&dma->rx_cmp, tmo);
	if (0 == tmo) {
		printk("rx transfer timed out!\n");
		ret = -1;
	}

	return ret;
}

static int get_dma_data(pcie_dma_t *dma) {
	int ret = 0;
	buffer_node_t write;

	uint8_t *tx_bar_map_memory;
	uint8_t *rx_bar_map_memory;
	uint64_t tx_bar_map_addr;
	uint64_t rx_bar_map_addr;

	uint64_t tx_axi_addr;
	uint64_t rx_axi_addr;

	//mydebug("\n");
	ret = get_buffer_node_info(&write, NULL, dma->list);
	if(ret != 0) {
		ret = -1;
		return ret;
	}

	tx_bar_map_memory = (uint8_t *)(dma->bar_map_memory[1] + write.write_offset);
	rx_bar_map_memory = (uint8_t *)(write.buffer + write.write_offset);
	prepare_test_data(tx_bar_map_memory, DMA_BLOCK_SIZE, rx_bar_map_memory, DMA_BLOCK_SIZE);

	tx_bar_map_addr = (uint64_t)dma->bar_map_addr[1];
	rx_bar_map_addr = (uint64_t)write.buffer_addr;
	tx_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_0 + write.write_offset);
	rx_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_1 + write.write_offset);
	dma_prepare_transfer(dma, tx_bar_map_addr, rx_bar_map_addr, tx_axi_addr, rx_axi_addr);

	start_dma_mm2s(dma, DMA_BLOCK_SIZE);
	start_dma_s2mm(dma, DMA_BLOCK_SIZE);
	dma_trans_sync(dma);
	write_buffer(NULL, DMA_BLOCK_SIZE, dma->list);
	test_result(tx_bar_map_memory, DMA_BLOCK_SIZE, rx_bar_map_memory, DMA_BLOCK_SIZE);
	//read_buffer(NULL, DMA_BLOCK_SIZE, dma->list);

	return ret;
}

void inc_dma_op_count(void);
static int dma_worker_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;

	init_dma(dma);

	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		//init_dma(dma);
		//dump_memory(dma->bar_map_memory[0], 4 * 0x40);
		get_dma_data(dma);
		inc_dma_op_count();
		//mydebug("\n");
	}

	return ret;
}

dma_op_t axi_dma_op = {
	.process_isr = process_isr,
	.thread_func = dma_worker_thread,
};

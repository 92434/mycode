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

/*
 *dma->bar_map_memory[0]-----------unused
 *dma->bar_map_memory[1]-----------for tx test data
 *dma->bar_map_memory[2-MAX_BAR_MAP_MEMORY]-----------for rx data
 *
 *
 *
 * */

static int init_dma(void *ppara) {
	int ret;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
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

static int prepare_bars_map(pcie_dma_t *dma, uint64_t tx_src_bar_map_addr, uint64_t rx_dest_bar_map_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *bar_vddr_map_ctrl_reg = (uint32_t *)(base_vaddr + OFFSET_AXI_PCIe_CTL + dma->pcie_bar_map_ctl_offset);

	write_addr_to_reg(bar_vddr_map_ctrl_reg, tx_src_bar_map_addr);
	bar_vddr_map_ctrl_reg += 2;
	write_addr_to_reg(bar_vddr_map_ctrl_reg, rx_dest_bar_map_addr);

	return 0;
}

static int transfer_des_setting(pcie_dma_t *dma, uint64_t tx_src_axi_addr, uint64_t rx_dest_axi_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	value = (uint32_t)(tx_src_axi_addr);
	writel(value, dma_base_vaddr + MM2S_SA);

	value = (uint32_t)(rx_dest_axi_addr);
	writel(value, dma_base_vaddr + S2MM_DA);

	return 0;
}

static int start_dma_mm2s(pcie_dma_t *dma, int tx_size) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	value = (uint32_t)tx_size;
	writel(value, dma_base_vaddr + MM2S_LENGTH);

	return 0;
}

static int start_dma_s2mm(pcie_dma_t *dma, int rx_size) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 
	uint32_t value;

	value = (uint32_t)rx_size;
	writel(value, dma_base_vaddr + S2MM_LENGTH);

	return 0;
}

static int dma_trans_sync(pcie_dma_t *dma, int tx_size, int rx_size) {
	unsigned long tmo;
	int ret = 0;

	if(tx_size != 0) {
		tmo = msecs_to_jiffies(10);
		tmo = wait_for_completion_timeout(&dma->tx_cmp, tmo);
		if (0 == tmo) {
			myprintf("%s:tx transfer timed out!\n", dma->dev_name);
			ret = -1;
		}
	}

	if(rx_size != 0) {
		tmo = msecs_to_jiffies(10);
		tmo = wait_for_completion_timeout(&dma->rx_cmp, tmo);
		if (0 == tmo) {
			myprintf("%s:rx transfer timed out!\n", dma->dev_name);
			ret = -1;
		}
	}

	return ret;
}

void inc_dma_op_tx_count(pcie_dma_t *dma, long unsigned int count);
int tr_wakeup(struct completion *tr_cmp);
static int dma_tr(void *ppara) {
	int ret = 0;


	pcie_tr_t *tr = (pcie_tr_t *)ppara;
	pcie_dma_t *dma = tr->dma;
	//uint64_t tx_dest_axi_addr = tr->tx_dest_axi_addr;
	//uint64_t rx_src_axi_addr = tr->rx_src_axi_addr;
	int tx_size = tr->tx_size;
	int rx_size = tr->rx_size;
	uint8_t *tx_data = tr->tx_data;
	//uint8_t *rx_data = tr->rx_data;
	struct completion *tr_cmp = tr->tr_cmp;

	buffer_node_t write;
	uint64_t tx_src_bar_map_addr;
	uint64_t rx_dest_bar_map_addr;
	uint64_t tx_src_axi_addr;
	uint64_t rx_dest_axi_addr;
	uint8_t *tx_src_bar_map_memory;
	uint8_t *rx_dest_bar_map_memory;

	ret = get_buffer_node_info(&write, NULL, dma->list);
	if(ret != 0) {
		return ret;
	}

	tx_src_bar_map_memory = (uint8_t *)(dma->bar_map_memory[1] + write.write_offset);
	rx_dest_bar_map_memory = (uint8_t *)(write.buffer + write.write_offset);
	prepare_test_data(tx_src_bar_map_memory, tx_size, rx_dest_bar_map_memory, rx_size, tx_data);

	tx_src_bar_map_addr = (uint64_t)dma->bar_map_addr[1];
	rx_dest_bar_map_addr = (uint64_t)write.buffer_addr;
	prepare_bars_map(dma, tx_src_bar_map_addr, rx_dest_bar_map_addr);

	tx_src_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_0 + write.write_offset);
	rx_dest_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_1 + write.write_offset);
	transfer_des_setting(dma, tx_src_axi_addr, rx_dest_axi_addr);

	if(tx_size != 0) {
		init_completion(&dma->tx_cmp);
		start_dma_mm2s(dma, tx_size);
	}

	if(rx_size != 0) {
		init_completion(&dma->rx_cmp);
		start_dma_s2mm(dma, rx_size);
	}

	ret = dma_trans_sync(dma, tx_size, rx_size);
	if(ret != 0) {
		dma->dma_op.init_dma(dma);
	}

	//get_result(tx_src_bar_map_memory, tx_size, rx_dest_bar_map_memory, rx_size, rx_data);
	write_buffer(NULL, rx_size, dma->list);
	//read_buffer(NULL, rx_size, dma->list);
	inc_dma_op_tx_count(dma, tx_size);

	tr_wakeup(tr_cmp);
	
	return ret;
}

dma_op_t axi_dma_op = {
	.init_dma = init_dma,
	.process_isr = process_isr,
	.dma_tr = dma_tr,
};

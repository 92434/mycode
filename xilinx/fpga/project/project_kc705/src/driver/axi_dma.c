#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/kthread.h>

#include "utils/xiaofei_debug.h"

#include "pcie.h"
#include "dma_common.h"

#define BASE_AXI_DMA_LITE 0x8100c000
#define OFFSET_AXI_DMA_LITE (BASE_AXI_DMA_LITE - BASE_AXI_PCIe)

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

static struct completion tx_cmp;
static struct completion rx_cmp;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
irqreturn_t isr(int irq, void *dev_id, struct pt_regs *regs)
#else
irqreturn_t isr(int irq, void *dev_id)
#endif
{
	irqreturn_t status = IRQ_NONE;

	struct pci_dev *pdev = dev_id;
	kc705_pci_dev_t * kc705_pci_dev = pci_get_drvdata(pdev);
	uint8_t *base_vaddr;
	uint8_t *dma_base_vaddr;
	uint32_t value;

	if(kc705_pci_dev == NULL) {
		mydebug("\n");
		return status;
	}

	base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 

	value = readl(dma_base_vaddr + MM2S_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		complete(&tx_cmp);
		//mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		complete(&tx_cmp);
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	}

	value = readl(dma_base_vaddr + S2MM_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		complete(&rx_cmp);
		//mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		complete(&rx_cmp);
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	}

	/* Handle DMA and any user interrupts */
	return status;
}


int prepare_bars_map(kc705_pci_dev_t *kc705_pci_dev) {
	uint64_t addr;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *bar_vddr_map_ctrl_reg = (uint32_t *)(base_vaddr + OFFSET_AXI_PCIe_CTL + AXIBAR2PCIEBAR_0U);

	//bind bar0 
	addr = (uint64_t)kc705_pci_dev->bar_map_addr[1];
	writel((uint32_t)(addr >> 32), bar_vddr_map_ctrl_reg);
	bar_vddr_map_ctrl_reg++;

	writel((uint32_t)addr, bar_vddr_map_ctrl_reg);
	bar_vddr_map_ctrl_reg++;

	//bind bar1 
	addr = (uint64_t)kc705_pci_dev->bar_map_addr[2];
	writel((uint32_t)(addr >> 32), bar_vddr_map_ctrl_reg);
	bar_vddr_map_ctrl_reg++;

	writel((uint32_t)addr, bar_vddr_map_ctrl_reg);

	return 0;
}

int init_dma(kc705_pci_dev_t *kc705_pci_dev) {
	int rtn;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
	//uint64_t addr_tx = (uint64_t)BASE_AXI_PCIe_BAR0;
	//uint64_t addr_rx = (uint64_t)BASE_AXI_PCIe_BAR1;

	uint32_t value;
	uint32_t mask;
	uint32_t default_value = 0x00010002;//0000_0000_0000_0001_0000_0000_0000_0010

	//reset dma
	mask = BITMASK(2)/*reset bit*/;
	value = default_value | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMACR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}

	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMASR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	//value = (uint32_t)(addr_tx);
	//writel(value, dma_base_vaddr + MM2S_SA);


	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + S2MM_DMASR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);
	//value = (uint32_t)(addr_rx);
	//writel(value, dma_base_vaddr + S2MM_DA);

	return rtn;
}

static int dma_prepare_transfer(kc705_pci_dev_t *kc705_pci_dev, uint64_t addr_tx, uint64_t addr_rx) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
	uint32_t value;

	value = (uint32_t)(addr_tx);
	writel(value, dma_base_vaddr + MM2S_SA);

	value = (uint32_t)(addr_rx);
	writel(value, dma_base_vaddr + S2MM_DA);

	return 0;
}

static int start_dma_mm2s(kc705_pci_dev_t *kc705_pci_dev) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
	uint32_t value;

	init_completion(&tx_cmp);
	value = DM_CHANNEL_TX_SIZE;
	writel(value, dma_base_vaddr + MM2S_LENGTH);

	return 0;
}

static int start_dma_s2mm(kc705_pci_dev_t *kc705_pci_dev) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
	uint32_t value;

	init_completion(&rx_cmp);
	value = DM_CHANNEL_RX_SIZE;
	writel(value, dma_base_vaddr + S2MM_LENGTH);

	return 0;
}

static int dma_trans_sync(kc705_pci_dev_t *kc705_pci_dev) {
	unsigned long tmo;
	int rtn = 0;

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&rx_cmp, tmo);
	if (0 == tmo) {
		printk("rx transfer timed out!\n");
		rtn = -1;
	}

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&tx_cmp, tmo);
	if (0 == tmo) {
		printk("tx transfer timed out!\n");
		rtn = -1;
	}

	return rtn;
}

int alloc_sg_list_chain(uint32_t tx_axiaddr, uint32_t rx_axiaddr) {
	return 0;
}

void free_sg_list_chain(void) {
}

void inc_dma_op_count(void);
int dma_worker_thread(void *ppara) {
	int rtn = 0;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)ppara;
	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		//init_dma(kc705_pci_dev);
		//dump_memory(kc705_pci_dev->bar_map_memory[0], 4 * 0x40);
		prepare_test_data(kc705_pci_dev);
		dma_prepare_transfer(kc705_pci_dev, BASE_AXI_PCIe_BAR0, BASE_AXI_PCIe_BAR1);
		start_dma_mm2s(kc705_pci_dev);
		start_dma_s2mm(kc705_pci_dev);
		dma_trans_sync(kc705_pci_dev);
		test_result(kc705_pci_dev);
		inc_dma_op_count();
		//mydebug("\n");
	}

	return rtn;
}

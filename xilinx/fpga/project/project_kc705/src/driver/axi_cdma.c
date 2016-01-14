#include "pcie.h"
#include "utils/xiaofei_debug.h"
#include "pcie_dma.h"

//AXI CDMA Register Summary
#define CDMA_CR 0x00 //CDMA Control.
#define CDMA_SR 0x04 //CDMA Status.
#define CDMA_CURDESC_PNTR 0x08 //Current Descriptor Pointer.
//#define Reserved 0x0C //N/A
#define CDMA_TAILDESC_PNTR 0x10 //Tail Descriptor Pointer.
//#define Reserved 0x14 //N/A
#define CDMA_SA 0x18 //Source Address.
//#define Reserved 0x1C //N/A
#define CDMA_DA 0x20 //Destination Address.
//#define Reserved 0x24 //N/A
#define CDMA_BTT 0x28 //Bytes to Transfer.

#define SG_DESCRIPTOR_SIZE 0x40

////Transfer Descriptor Word Summary
//#define DES_NXTDESC_PNTR 0x00 //[31:5]
////#define DES_RESERVED 0x04
//#define DES_SA 0x08
////#define DES_RESERVED 0x0c
//#define DES_DA 0x10
////#define DES_RESERVED 0x14
//#define DES_CONTROL 0x18 //[22:0] size to transfer
//#define DES_STATUS 0x1c //bit31:Cmplt bit30:DMADecErr bit29:DMASlvErr bit28:DMAIntErr

/*
 *dma->bar_map_memory[0]-----------for sg
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
	uint8_t *cdma_base_vaddr = (uint8_t *)(base_vaddr + dma->dma_lite_offset); 

	//reset cdma
	uint32_t default_value = 0x00010002;//0000_0000_0000_0001(irq threshold)_0000_0000_0000_001(tail ptr enable)0
	uint32_t value;
	
	//reset
	value = default_value | BITMASK(2);//reset bit
	writel(value, cdma_base_vaddr + CDMA_CR);
	ret = wait_for_iostatus_timeout(0, cdma_base_vaddr + CDMA_CR, BITMASK(2), 0);
	if(ret != 0) {
		mydebug("time out!\n");
	}

	//enable sg mode
	value = readl(cdma_base_vaddr + CDMA_CR) & (~(BITMASK(3)));//sg mode
	writel(value, cdma_base_vaddr + CDMA_CR);

	value = readl(cdma_base_vaddr + CDMA_CR) | BITMASK(3);//sg mode
	writel(value, cdma_base_vaddr + CDMA_CR);

	//enable interrupt
	value = readl(cdma_base_vaddr + CDMA_CR) | BITMASK(12)/*ioc interrupt*/ | BITMASK(14)/*err interrupt*/;
	writel(value, cdma_base_vaddr + CDMA_CR);

	value = readl(cdma_base_vaddr + CDMA_CR) | (2 << 16)/*IRQThreshold:2*/;
	writel(value, cdma_base_vaddr + CDMA_CR);


	return ret;
}

static irqreturn_t process_isr(void *ppara) {
	irqreturn_t status = IRQ_NONE;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *cdma_base_vaddr = (uint8_t *)(base_vaddr + dma->dma_lite_offset); 
	uint32_t value;

	value = readl(cdma_base_vaddr + CDMA_SR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		complete(&dma->tx_cmp);
		complete(&dma->rx_cmp);
		//mydebug("\n");
		value |= BITMASK(12);
		writel(value, cdma_base_vaddr + CDMA_SR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		complete(&dma->tx_cmp);
		complete(&dma->rx_cmp);
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, cdma_base_vaddr + CDMA_SR);
		status = IRQ_HANDLED;
	}
	/* Handle DMA and any user interrupts */
	return status;
}

typedef union {
	struct {
		uint32_t NXTDESC_PNTR;
		uint32_t PAD0;
		uint32_t SA;
		uint32_t PAD1;
		uint32_t DA;
		uint32_t PAD2;
		uint32_t CONTROL;
		uint32_t STATUS;
	} des;
	unsigned char des_space[0x40];
} kc705_transfer_descriptor_t;

typedef struct {
	struct list_head sg_item;
	kc705_transfer_descriptor_t des;
} kc705_sg_item_info_t;

static int alloc_sg_des_item(uint32_t SA, uint32_t DA, uint32_t CONTROL, struct list_head *sg_list) {
	int ret = 0;
	kc705_sg_item_info_t *sg_item = (kc705_sg_item_info_t *)vzalloc(sizeof(kc705_sg_item_info_t));

	if(sg_item == NULL) {
		ret = -1;
		mydebug("alloc sg_item failed.\n");
		return ret;
	}

	sg_item->des.des.SA = SA;
	sg_item->des.des.DA = DA;
	sg_item->des.des.CONTROL = CONTROL;
	list_add_tail(&sg_item->sg_item, sg_list);
	return 0;
}

static void free_sg_des_items(struct list_head *sg_list) {
	kc705_sg_item_info_t *sg_item, *sg_item_next;
	list_for_each_entry_safe(sg_item, sg_item_next, sg_list, sg_item) {
		list_del(&sg_item->sg_item);

		vfree(sg_item);
	}
}

static uint32_t flush_sg_des_items(pcie_dma_t *dma, struct list_head *sg_list) {
	kc705_transfer_descriptor_t *pdes = dma->bar_map_memory[0]; //buffer for sg, axi address 0x80800000
	kc705_sg_item_info_t *info;
	uint32_t cdma_tail_des_axi_addr = 0;
	uint32_t bar_sg_next_addr_offset = dma->pcie_map_bar_axi_addr_0 + SG_DESCRIPTOR_SIZE;

	list_for_each_entry(info, sg_list, sg_item) {
		//kc705_sg_item_info_t *next_info = list_next_entry(info, sg_item);

		if(info->sg_item.next != sg_list) {
		//if(&(next_info->sg_item) != sg_list) {
			info->des.des.NXTDESC_PNTR = bar_sg_next_addr_offset;
			cdma_tail_des_axi_addr = bar_sg_next_addr_offset;
			bar_sg_next_addr_offset += SG_DESCRIPTOR_SIZE;
		} else {
			info->des.des.NXTDESC_PNTR = dma->pcie_map_bar_axi_addr_0;
		}

		pdes->des = info->des.des;

		pdes++;

	}

	free_sg_des_items(sg_list);

	return cdma_tail_des_axi_addr;
}

static int start_cdma(pcie_dma_t *dma, uint32_t cdma_tail_des_axi_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *cdma_base_vaddr = (uint8_t *)(base_vaddr + dma->dma_lite_offset); 

	init_completion(&dma->tx_cmp);
	init_completion(&dma->rx_cmp);

	//update cdma curdesc pointer
	writel(dma->pcie_map_bar_axi_addr_0, cdma_base_vaddr + CDMA_CURDESC_PNTR);

	writel(cdma_tail_des_axi_addr, cdma_base_vaddr + CDMA_TAILDESC_PNTR);
	return 0;
}

static int dma_trans_sync(pcie_tr_t *tr) {
	pcie_dma_t *dma = tr->dma;
	int tx_size = tr->tx_size;
	int rx_size = tr->rx_size;
	unsigned long tmo;
	int ret = 0;

	if(tx_size != 0) {
		tmo = msecs_to_jiffies(10);
		tmo = wait_for_completion_timeout(&dma->tx_cmp, tmo);
		myprintf_once((0 == tmo), "%s:tx transfer timed out!\n", dma->devname);
		if (0 == tmo) {
			tr->tx_size = 0;
			ret = -1;
		}
	}

	if(rx_size != 0) {
		tmo = msecs_to_jiffies(10);
		tmo = wait_for_completion_timeout(&dma->rx_cmp, tmo);
		myprintf_once((0 == tmo), "%s:rx transfer timed out!\n", dma->devname);
		if (0 == tmo) {
			tr->rx_size = 0;
			ret = -1;
		}
	}

	return ret;
}

static int prepare_bars_map(pcie_dma_t *dma, uint64_t tx_src_bar_map_addr, uint64_t rx_dest_bar_map_addr) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *bram_vddr_reg = (uint32_t *)(base_vaddr + OFFSET_Translation_BRAM);
	uint32_t *bar_vddr_map_ctrl_reg = (uint32_t *)(base_vaddr + OFFSET_AXI_PCIe_CTL + dma->pcie_bar_map_ctl_offset_0);

	//bind sg 
	write_addr_to_reg(bar_vddr_map_ctrl_reg, (uint64_t)dma->bar_map_addr[0]);

	//prepare tx && rx buffer address for dm map ctl register at TR BRAM
	write_addr_to_reg(bram_vddr_reg, tx_src_bar_map_addr);
	bram_vddr_reg += 2;
	write_addr_to_reg(bram_vddr_reg, rx_dest_bar_map_addr);

	return 0;
}

static int dma_tr(void *ppara) {
	int ret = 0;

	pcie_tr_t *tr = (pcie_tr_t *)ppara;
	pcie_dma_t *dma = tr->dma;
	uint64_t tx_dest_axi_addr = tr->tx_dest_axi_addr;
	uint64_t rx_src_axi_addr = tr->rx_src_axi_addr;
	int tx_size = tr->tx_size;
	int rx_size = tr->rx_size;
	//uint8_t *tx_data = tr->tx_data;
	//uint8_t *rx_data = tr->rx_data;

	buffer_node_t write;
	uint64_t tx_src_bar_map_addr;
	uint64_t rx_dest_bar_map_addr;
	uint64_t tx_src_axi_addr;
	uint64_t rx_dest_axi_addr;
	uint8_t *tx_src_bar_map_memory;
	uint8_t *rx_dest_bar_map_memory;
	int offset = 0;

	LIST_HEAD(sg_descripter_list);

	dma->dma_op.init_dma(dma);

	reset_list_buffer(dma->list);
	ret = get_buffer_node_info(&write, NULL, dma->list);
	if(ret != 0) {
		return ret;
	}

	tx_src_bar_map_memory = (uint8_t *)(dma->bar_map_memory[1]);
	rx_dest_bar_map_memory = (uint8_t *)(write.buffer + write.write_offset);

	tx_src_bar_map_addr = (uint64_t)dma->bar_map_addr[1];
	rx_dest_bar_map_addr = (uint64_t)write.buffer_addr;
	prepare_bars_map(dma, tx_src_bar_map_addr, rx_dest_bar_map_addr);//bind sg to bar0, write addr to bram

	tx_src_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_1);
	rx_dest_axi_addr = (uint64_t)(dma->pcie_map_bar_axi_addr_1 + write.write_offset);

	if(tx_size != 0) {
		ret = alloc_sg_des_item(BASE_Translation_BRAM + offset,
				BASE_AXI_PCIe_CTL + dma->pcie_bar_map_ctl_offset_1,
				sizeof(uint64_t),
				&sg_descripter_list);
		if(ret != 0) {
			goto exit;
		}

		ret = alloc_sg_des_item(tx_src_axi_addr, tx_dest_axi_addr, tx_size, &sg_descripter_list);
		if(ret != 0) {
			goto exit;
		}
	}

	offset += sizeof(uint64_t);

	if(rx_size != 0) {
		ret = alloc_sg_des_item(BASE_Translation_BRAM + offset,
				BASE_AXI_PCIe_CTL + dma->pcie_bar_map_ctl_offset_1,
				sizeof(uint64_t),
				&sg_descripter_list);
		if(ret != 0) {
			goto exit;
		}

		ret = alloc_sg_des_item(rx_src_axi_addr, rx_dest_axi_addr, rx_size, &sg_descripter_list);
		if(ret != 0) {
			goto exit;
		}
	}

	ret = flush_sg_des_items(dma, &sg_descripter_list);
	if(ret == 0) {
		goto exit;
	}

	start_cdma(dma, ret);

	ret = dma_trans_sync(tr);
	if(ret != 0) {
		dma->dma_op.init_dma(dma);
	}
exit:
	free_sg_des_items(&sg_descripter_list);

	return ret;
}

dma_op_t axi_cdma_op = {
	.init_dma = init_dma,
	.process_isr = process_isr,
	.dma_tr = dma_tr,
};

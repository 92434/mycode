#include "linux/kthread.h"

#include "pcie.h"
#include "pcie_tr_thread.h"
#include "utils/xiaofei_debug.h"
#if defined(KC705_DVBS2)
#include "dvbs2_dma_config.h"
#elif defined(KC705_CSA)
#include "csa_dma_config.h"
#endif
#include "axi_cdma.h"
#include "axi_dma.h"
#include "pseudo_dma.h"

#define DMA_BAR_MEM_START_INDEX 1
static int alloc_dma_memory(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	int i, j;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		dma->kc705_pci_dev = kc705_pci_dev;

		dma->list = init_list_buffer();
		if(dma->list == NULL) {
			ret = -1;
			return ret;
		}

		dma->dma_lite_offset = dma_info[i].dma_lite_offset;
		dma->pcie_bar_map_ctl_offset_0 = dma_info[i].pcie_bar_map_ctl_offset_0;
		dma->pcie_bar_map_ctl_offset_1 = dma_info[i].pcie_bar_map_ctl_offset_1;
		dma->pcie_map_bar_axi_addr_0 = dma_info[i].pcie_map_bar_axi_addr_0;
		dma->pcie_map_bar_axi_addr_1 = dma_info[i].pcie_map_bar_axi_addr_1;
		dma->dma_bar_map_num = dma_info[i].dma_bar_map_num;

		dma->dma_type = dma_info[i].dma_type;
		dma->dma_op = (dma_info[i].dma_type == AXI_DMA) ? axi_dma_op :
					(dma_info[i].dma_type == AXI_CDMA) ? axi_cdma_op :
					(dma_info[i].dma_type == PSEUDO_DMA) ? pseudo_dma_op :
					pseudo_dma_op;
		dma->dma_thread = dma_info[i].dma_thread;
		dma->dma_thread_count = dma_info[i].dma_thread_count;
		kc705_pci_dev->total_dma_thread_count += dma_info[i].dma_thread_count;
		dma->is_auto_receive = dma_info[i].is_auto_receive;

		dma->receive_bulk_size = dma_info[i].receive_bulk_size;
		dma->send_bulk_size = dma_info[i].send_bulk_size;
		dma->devname = dma_info[i].devname;

		for(j = 0; j < dma->dma_bar_map_num; j++) {
			dma->bar_map_memory_size[j] = PCIe_MAP_BAR_SIZE;
		}


		//alloc memory cdma
		for(j = 0; j < MAX_BAR_MAP_MEMORY; j++) {
			if(dma->bar_map_memory_size[j] != 0) {
				dma->bar_map_memory[j] = dma_zalloc_coherent(&(kc705_pci_dev->pdev->dev), dma->bar_map_memory_size[j], &(dma->bar_map_addr[j]), GFP_KERNEL);
				if(dma->bar_map_memory[j] == NULL) {
					ret = -1;
					mydebug("dma_zalloc_coherent failed.\n");
					return ret;
				} else {
					//mydebug("dma->bar_map_memory[%d]:%p\n", j, dma->bar_map_memory[j]);
					//mydebug("dma->bar_map_addr[%d]:%p\n", j, (void *)dma->bar_map_addr[j]);
					//mydebug("dma->bar_map_memory_size[%d]:%x\n", j, dma->bar_map_memory_size[j] - (dma->bar_map_memory_size[j] % dma->receive_bulk_size));
					if(j > DMA_BAR_MEM_START_INDEX) {//
						int unused_size = dma->bar_map_memory_size[j] % dma->receive_bulk_size;

						add_list_buffer_item((char *)dma->bar_map_memory[j], (void *)dma->bar_map_addr[j], dma->bar_map_memory_size[j] - unused_size, dma->list);
					}
				}
			}
		}
	}

	return ret;
}

static void free_dma_memory(kc705_pci_dev_t *kc705_pci_dev) {
	int i, j;

	if(kc705_pci_dev->dma == NULL) {
		return;
	}

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		for(j = 0; j < MAX_BAR_MAP_MEMORY; j++) {
			if(dma->bar_map_memory[j] != NULL) {
				dma_free_coherent(&(kc705_pci_dev->pdev->dev), dma->bar_map_memory_size[j], dma->bar_map_memory[j], dma->bar_map_addr[j]);
				dma->bar_map_memory[j] = NULL;
			}
		}

		if(dma->list != NULL) {
			uninit_list_buffer(dma->list);
			dma->list = NULL;
		}
	}

}

int alloc_kc705_dma(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;

	if(DMA_MAX == 0) {
		return ret;
	}

	kc705_pci_dev->dma = (pcie_dma_t *)vzalloc(sizeof(pcie_dma_t) * DMA_MAX);
	if(kc705_pci_dev->dma == NULL) {
		mydebug("alloc kc705_pci_dev->dma failed.\n");
		ret = -1;
		goto alloc_kc705_pci_dma_failed;
	}

	kc705_pci_dev->dma_count = DMA_MAX;

	ret = alloc_dma_memory(kc705_pci_dev);
	if(ret != 0) {
		mydebug("alloc_dma_memory_failed!\n");
		goto alloc_dma_memory_failed;
	}

	return ret;

alloc_dma_memory_failed:
	free_dma_memory(kc705_pci_dev);
	kc705_pci_dev->dma_count = 0;
	vfree(kc705_pci_dev->dma);
	kc705_pci_dev->dma = NULL;
alloc_kc705_pci_dma_failed:
	return ret;
}

void free_kc705_dma(kc705_pci_dev_t *kc705_pci_dev) {
	free_dma_memory(kc705_pci_dev);
	if(kc705_pci_dev->dma == NULL) {
		kc705_pci_dev->dma_count = 0;
		vfree(kc705_pci_dev->dma);
		kc705_pci_dev->dma = NULL;
	}
}

#include "pcie_dma_device.h"
void add_dma_device(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		setup_pcie_dma_dev(dma);
	}
}

void remove_dma_device(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		uninstall_pcie_dma_dev(dma);
	}
}

irqreturn_t dma_process_isr(kc705_pci_dev_t *kc705_pci_dev) {
	irqreturn_t status = IRQ_NONE;
	int i;

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		if(dma->dma_op.process_isr(dma) == IRQ_HANDLED) {
			status = IRQ_HANDLED;
		}
	}

	return status;
}

int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection) {
#define XILINX_VDMA_LOOP_COUNT 1000000
	uint32_t value = 0;
	int ret = 0;

	if(count == 0) {
		count = XILINX_VDMA_LOOP_COUNT;
	}

	do {
		value = readl(paddr) & mask;
		count--;
	} while(count != 0 && value != expection);

	if(count == 0) {
		ret = -1;
	}

	return ret;
}

int write_addr_to_reg(uint32_t *reg, uint64_t addr) {
	writel((uint32_t)((addr >> 32) & 0xffffffff), reg);
	reg++;
	writel((uint32_t)(addr & 0xffffffff), reg);

	return 0;
}


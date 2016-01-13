#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/kthread.h>

#include "pcie.h"
#include "pcie_dma.h"
#include "pcie_performance.h"
#include "pcie_tr_thread.h"

/*
 *dma->bar_map_memory[0]-----------unused
 *dma->bar_map_memory[1]-----------for tx test data
 *dma->bar_map_memory[2-MAX_BAR_MAP_MEMORY]-----------for rx data
 *
 *
 *
 * */

static int init_dma(void *ppara) {
	return 0;
}

static irqreturn_t process_isr(void *ppara) {
	irqreturn_t status = IRQ_NONE;

	/* Handle DMA and any user interrupts */
	return status;
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

	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = base_vaddr + dma->dma_lite_offset; 

	uint8_t *tx_src_memory = (uint8_t *)dma->bar_map_memory[1];
	uint8_t *rx_dest_memory = (uint8_t *)dma->bar_map_memory[2];

	uint8_t *tx_dest_memory;
	uint8_t *rx_src_memory;
	int i;
	int pos;

	tx_dest_memory = (uint8_t *)(dma_base_vaddr + tx_dest_axi_addr);
	rx_src_memory = (uint8_t *)(dma_base_vaddr + rx_src_axi_addr);

	if(tx_size != 0) {
		for(i = 0; i + 4 <= tx_size; i += 4) {
			writel(*((uint32_t *)(tx_src_memory + i)), tx_dest_memory + i);
			myprintf("%08x->%p\n", *((uint32_t *)(tx_src_memory + i)), (void *)(tx_dest_memory + i));
		}
		pos = i;
		for(i = pos; i + 2 <= tx_size; i += 2) {
			writew(*((uint16_t *)(tx_src_memory + i)), tx_dest_memory + i);
			myprintf("%04x->%p\n", *((uint16_t *)(tx_src_memory + i)), (void *)(tx_dest_memory + i));
		}
		pos = i;
		for(i = pos; i + 1 <= tx_size; i += 1) {
			writeb(*((uint8_t *)(tx_src_memory + i)), tx_dest_memory + i);
			myprintf("%02x->%p\n", *((uint8_t *)(tx_src_memory + i)), (void *)(tx_dest_memory + i));
		}
	}

	if(rx_size != 0) {
		for(i = 0; i + 4 <= rx_size; i += 4) {
			*((uint32_t *)(rx_dest_memory + i)) = readl(rx_src_memory + i);
			myprintf("%08x<-%p\n", *((uint32_t *)(rx_dest_memory + i)), (void *)(rx_src_memory + i));
		}
		pos = i;
		for(i = pos; i + 2 <= rx_size; i += 2) {
			*((uint16_t *)(rx_dest_memory + i)) = readw(rx_src_memory + i);
			myprintf("%04x<-%p\n", *((uint16_t *)(rx_dest_memory + i)), (void *)(rx_src_memory + i));
		}
		pos = i;
		for(i = pos; i + 1 <= rx_size; i += 1) {
			*((uint8_t *)(rx_dest_memory + i)) = readb(rx_src_memory + i);
			myprintf("%02x<-%p\n", *((uint8_t *)(rx_dest_memory + i)), (void *)(rx_src_memory + i));
		}
	}

	return ret;
}

dma_op_t pseudo_dma_op = {
	.init_dma = init_dma,
	.process_isr = process_isr,
	.dma_tr = dma_tr,
};

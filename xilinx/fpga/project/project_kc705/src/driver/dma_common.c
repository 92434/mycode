#include <linux/pci.h>

#include "utils/xiaofei_debug.h"

#include "pcie.h"
#include "dma_common.h"


struct completion cmp;

int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection) {
#define XILINX_VDMA_LOOP_COUNT 1000000
	uint32_t value = 0;
	int rtn = 0;

	if(count == 0) {
		count = XILINX_VDMA_LOOP_COUNT;
	}

	do {
		value = readl(paddr) & mask;
		count--;
	} while(count != 0 && value != expection);

	if(count == 0) {
		rtn = -1;
	}

	return rtn;
}

int write_addr_to_reg(uint32_t *reg, uint64_t addr) {
	writel((uint32_t)((addr >> 32) & 0xffffffff), reg);
	reg++;
	writel((uint32_t)(addr & 0xffffffff), reg);

	return 0;
}

int dma_trans_sync(kc705_pci_dev_t *kc705_pci_dev) {
	unsigned long tmo;
	int rtn = 0;

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&cmp, tmo);
	if (0 == tmo) {
		mydebug("transfer timed out!\n");
		rtn = -1;
	}

	return rtn;
}

void prepare_test_data(kc705_pci_dev_t *kc705_pci_dev) {
	uint8_t *tx_addr = kc705_pci_dev->bar_map_memory[1];
	uint8_t *rx_addr = kc705_pci_dev->bar_map_memory[2];
	int i;

	for(i = 0; i < kc705_pci_dev->bar_map_memory_size[1]; i++) {
		tx_addr[i] = 8 + i;
	}

	for(i = 0; i < kc705_pci_dev->bar_map_memory_size[2]; i++) {
		rx_addr[i] = 0;
	}
}

void test_result(kc705_pci_dev_t *kc705_pci_dev) {
	uint8_t *tx_addr = kc705_pci_dev->bar_map_memory[1];
	uint8_t *rx_addr = kc705_pci_dev->bar_map_memory[2];

	int i;
	bool success = true;

	//mydebug("tx_addr:%p\n", (void *)tx_addr);
	//for(i = 0; i < DM_CHANNEL_TX_SIZE; i++) {
	//	if((i != 0) && (i % 16 == 0)) {
	//		printk("\n");
	//	}
	//	printk("%02x ", tx_addr[i]);
	//}

	//printk("\n");

	//mydebug("rx_addr:%p\n", (void *)rx_addr);
	//for(i = 0; i < DM_CHANNEL_RX_SIZE; i++) {
	//	if((i != 0) && (i % 16 == 0)) {
	//		printk("\n");
	//	}
	//	printk("%02x ", rx_addr[i]);
	//}
	//printk("\n");

	for(i = 0; i < DM_CHANNEL_RX_SIZE; i++) {
		if(rx_addr[i] != i + 8) {
			success = false;
			break;
		}
	}

	if(success == false) {
		for(i = 0; i < DM_CHANNEL_RX_SIZE; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", rx_addr[i]);
		}
		printk("\n");
		printk("\n");
	} else {
		printk("!\n");
	}
}

#include <linux/pci.h>

#include "utils/xiaofei_debug.h"

#include "pcie.h"
#include "dma_common.h"


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

void prepare_test_data(uint8_t *tx_memory, int tx_size, uint8_t *rx_memory, int rx_size) {
	uint8_t *tx_addr = tx_memory;
	uint8_t *rx_addr = rx_memory;
	int i;

	for(i = 0; i < tx_size; i++) {
		tx_addr[i] = 8 + i;
	}

	for(i = 0; i < rx_size; i++) {
		rx_addr[i] = 0;
	}
}

void test_result(uint8_t *memory_tx, int tx_size, uint8_t *memory_rx, int rx_size) {
	uint8_t *tx_addr = memory_tx;
	uint8_t *rx_addr = memory_rx;

	int i;
	bool success = true;
	int pos;

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

	for(i = 0; i < rx_size; i++) {
		if(rx_addr[i] != tx_addr[i]) {
			success = false;
			pos = i;
			break;
		}
	}

	if(success == false) {
		printk("!\n");
		mydebug("pos:%d\n", pos);
		for(i = pos; i < 8; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", rx_addr[i]);
		}
		printk("\n");
	}
}

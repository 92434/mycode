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

void prepare_test_data(uint8_t *tx_memory, int tx_size, uint8_t *rx_memory, int rx_size, uint8_t *tx_data) {
	int i;

	if(tx_data != NULL) {
		if(tx_data != tx_memory) {
			memcpy(tx_memory, tx_data, tx_size);
		}
	} else {
		for(i = 0; i < tx_size; i++) {
			tx_memory[i] = 8 + i;
		}
	}

	for(i = 0; i < rx_size; i++) {
		rx_memory[i] = 0;
	}
}

void test_result(uint8_t *tx_memory, int tx_size, uint8_t *rx_memory, int rx_size) {
	int i;
	bool success = true;
	int pos;

	for(i = 0; i < rx_size; i++) {
		if(rx_memory[i] != tx_memory[i]) {
			mydebug("%d:%d\n", tx_memory[i], rx_memory[i]);
			success = false;
			pos = i;
			break;
		}
	}

	if(success == false) {
		printk("!\n");
		mydebug("pos:%d\n", pos);
		for(i = pos; i < pos + 8; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", tx_memory[i]);
		}
		printk("\n");
		for(i = pos; i < pos + 8; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", rx_memory[i]);
		}
		printk("\n");
	}

}

void get_result(uint8_t *rx_memory, int rx_size, uint8_t *rx_data) {
	if(rx_data != NULL) {
		if(rx_data != rx_memory) {
			memcpy(rx_data, rx_memory, rx_size);
		}
	}

}

void inc_dma_op_tx_count(pcie_dma_t *dma, long unsigned int count) {
	dma->tx_count += count;
}

void inc_dma_op_rx_count(pcie_dma_t *dma, long unsigned int count) {
	dma->rx_count += count;
}

long unsigned int get_op_tx_count(pcie_dma_t *dma) {
	long unsigned int count = dma->tx_count;
	dma->tx_count = 0;
	return count;
}

long unsigned int get_op_rx_count(pcie_dma_t *dma) {
	long unsigned int count = dma->rx_count;
	dma->rx_count = 0;
	return count;
}

int tr_wait(pcie_dma_t *dma, struct completion *tr_cmp) {
	int ret = 0;
	//unsigned long tmo;

	//tmo = msecs_to_jiffies(1000);
	//tmo = wait_for_completion_timeout(tr_cmp, tmo);
	//if (0 == tmo) {
	//	myprintf("(%d)%s:tr_wait timed out!\n", task_pid_nr(current), dma->dev_name);
	//	ret = -1;
	//}

	wait_for_completion(tr_cmp);

	return ret;
}

int tr_wakeup(struct completion *tr_cmp) {
	int ret = 0;

	if(tr_cmp != NULL) {
		//mydebug("tr_cmp:%p\n", tr_cmp);
		complete(tr_cmp);
	}
	return ret;
}

#ifndef _FMC_GPIO_DMA_THREAD_CONFIG_H
#define _FMC_GPIO_DMA_THREAD_CONFIG_H
#include <linux/delay.h>

static int dma_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	uint32_t *tx_src_bar_map_memory = (uint32_t *)(dma->bar_map_memory[1]);

	uint64_t tx_dest_axi_addr = 0;
	uint64_t rx_src_axi_addr = 0;

	int i;

	for(i = 0; i < (dma->send_bulk_size / sizeof(uint32_t)); i++) {
		tx_src_bar_map_memory[i] = i;
	}

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 

		memset(dma->bar_map_memory[2], 0, dma->receive_bulk_size);

		put_pcie_tr(dma, tx_dest_axi_addr, rx_src_axi_addr, dma->send_bulk_size, dma->receive_bulk_size, NULL, NULL);
		
		if(memcmp(dma->bar_map_memory[1], dma->bar_map_memory[2], dma->receive_bulk_size) != 0) {
			mydebug("failed!tx_dest_axi_addr:%x(%d) rx_src_axi_addr:%x(%d)\n", (int)tx_dest_axi_addr, (int)tx_dest_axi_addr, (int)rx_src_axi_addr, (int)rx_src_axi_addr);
			set_current_state(TASK_UNINTERRUPTIBLE);  
			schedule_timeout(msecs_to_jiffies(1000)); 
		} else {
			tx_dest_axi_addr += dma->send_bulk_size;
			if(tx_dest_axi_addr >= 1024 * 1024) {
				tx_dest_axi_addr = 0;
			}

			rx_src_axi_addr += dma->receive_bulk_size;
			if(rx_src_axi_addr >= 1024 * 1024) {
				rx_src_axi_addr = 0;
			}
		}
	}

	mydebug("\n");

	return ret;
}

static dma_thread_info_t dma_threads[] = {
	{
		.t = &dma_thread,
		.thread_name = "dma_thread"
	},
};

#define DMA_THREAD_COUNT (sizeof(dma_threads) / sizeof(dma_thread_info_t))
#endif//#ifndef _FMC_GPIO_DMA_THREAD_CONFIG_H

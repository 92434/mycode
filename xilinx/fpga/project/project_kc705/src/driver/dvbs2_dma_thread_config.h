#ifndef _DVBS2_DMA_THREAD_CONFIG_H
#define _DVBS2_DMA_THREAD_CONFIG_H
#include "eg9013f_nz_fb.h"

static int dma_thread(void *ppara) {
	int ret = 0;
	//pcie_dma_t *dma = (pcie_dma_t *)ppara;

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(msecs_to_jiffies(10)); 

		//put_pcie_tr(dma, 0, 0, 0, dma->receive_bulk_size, NULL, NULL);
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

#define REG_RANGE 4096
typedef enum {
	ADDR_LCM_CLK_LEVEL_DELAY = 0,
	ADDR_CHANNEL_INDEX,
} addr_lcd_controler_config_t;

#define ADDR_OFFSET(addr) (addr * 4)

static int flush_data_to_lcd(pcie_dma_t *dma, char *buffer, int size) {
	int ret = 0;
	uint32_t channel_idx = 1;
	uint32_t lcm_clk_level_delay = 64;
	char *start_pos = buffer;
	char *end_pos = buffer + size;

	memcpy(dma->bar_map_memory[1] + ADDR_OFFSET(ADDR_LCM_CLK_LEVEL_DELAY), &lcm_clk_level_delay, sizeof(uint32_t));
	if(put_pcie_tr(
			dma,
			ADDR_OFFSET(ADDR_LCM_CLK_LEVEL_DELAY),
			0,
			sizeof(uint32_t),
			0,
			NULL,
			NULL
		) < 0) {

		mydebug("!\n");
		ret = -1;
		return ret;
	}

	while(start_pos < end_pos) {
		int flush_size;

		memcpy(dma->bar_map_memory[1] + ADDR_OFFSET(ADDR_CHANNEL_INDEX), &channel_idx, sizeof(uint32_t));
		if(put_pcie_tr(
				dma,
				ADDR_OFFSET(ADDR_CHANNEL_INDEX),
				0,
				sizeof(uint32_t),
				0,
				NULL,
				NULL
			) < 0) {
			mydebug("!\n");
			ret = -1;
			return ret;
		}

		
		flush_size = (end_pos - start_pos >= REG_RANGE) ? REG_RANGE : (end_pos - start_pos);

		memcpy(dma->bar_map_memory[1], start_pos, flush_size);
		if(put_pcie_tr(
				dma,
				0,
				0,
				flush_size,
				0,
				NULL,
				NULL
			) < 0) {
			mydebug("!\n");
			ret = -1;
			return ret;
		}


		start_pos += flush_size;
		channel_idx++;
	}
	
	return ret;
}

static int eg9013f_nz_fb_dma_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = dma->kc705_pci_dev;
	int buffer_size = EG9013F_NZ_SIZE;
	char *eg9013f_nz_buffer = kc705_pci_dev->eg9013f_nz_buffer;
	char *eg9013f_nz_buffer_buckup = (char *)vzalloc(buffer_size);

	if(eg9013f_nz_buffer_buckup == NULL) {
		mydebug("\n");
		ret = -1;
		return ret;
	}

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(msecs_to_jiffies(50)); 

		if(memcmp(eg9013f_nz_buffer_buckup, eg9013f_nz_buffer, buffer_size) != 0) {
			memcpy(eg9013f_nz_buffer_buckup, eg9013f_nz_buffer, buffer_size);
			flush_data_to_lcd(dma, eg9013f_nz_buffer_buckup, buffer_size);
		}
	}

	vfree(eg9013f_nz_buffer_buckup);
	mydebug("\n");

	return ret;
}

static dma_thread_info_t eg9013f_nz_fb_thread[] = {
	{
		.t = &eg9013f_nz_fb_dma_thread,
		.thread_name = "eg9013f_nz"
	},
};

#define DMA_THREAD_EG9013F_NZ_COUNT (sizeof(eg9013f_nz_fb_thread) / sizeof(dma_thread_info_t))

#endif//#ifndef _DVBS2_DMA_THREAD_CONFIG_H

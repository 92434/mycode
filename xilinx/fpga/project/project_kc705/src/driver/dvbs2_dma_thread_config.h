#ifndef _DVBS2_DMA_THREAD_CONFIG_H
#define _DVBS2_DMA_THREAD_CONFIG_H
#include "eg9013f_nz_fb.h"

static int dma_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 

		put_pcie_tr(dma, 0, 0, 0, dma->receive_bulk_size, NULL, NULL);
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
	ADDR_BYTE_ORDER,
	ADDR_READ_CHANNEL_INDEX,
	ADDR_WRITE_CHANNEL_INDEX,
} addr_lcd_controler_config_t;

#define ADDR_OFFSET(addr) (addr * 4)

static int flush_data_to_lcd(pcie_dma_t *dma, char *buffer, int size) {
	int ret = 0;
	uint32_t channel_idx = 1;
	uint32_t lcm_clk_level_delay = 64;
	char *start_pos = buffer;
	char *end_pos = buffer + size;
	uint8_t *tx_bar_mem = (uint8_t *)dma->bar_map_memory[1];

	memcpy(tx_bar_mem, &lcm_clk_level_delay, sizeof(uint32_t));
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

		memcpy(tx_bar_mem, &channel_idx, sizeof(uint32_t));
		if(put_pcie_tr(
				dma,
				ADDR_OFFSET(ADDR_WRITE_CHANNEL_INDEX),
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

		memcpy(tx_bar_mem, start_pos, flush_size);
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

#define REFRESH_DURATION_BY_MSECS 50
#define FORCE_REFRESH_COUNT (1000 / REFRESH_DURATION_BY_MSECS)
static int eg9013f_nz_fb_dma_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = dma->kc705_pci_dev;
	int buffer_size = EG9013F_NZ_SIZE;
	char *eg9013f_nz_buffer = kc705_pci_dev->eg9013f_nz_buffer;
	char *eg9013f_nz_buffer_buckup = (char *)vzalloc(buffer_size);
	int force_refresh_count = 0;

	if(down_trylock(&dma->dma_dev_sema) != 0) {
		ret = -EBUSY;
		return ret;
	}

	if(eg9013f_nz_buffer_buckup == NULL) {
		mydebug("\n");
		ret = -1;
		return ret;
	}

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(msecs_to_jiffies(REFRESH_DURATION_BY_MSECS)); 

		if((memcmp(eg9013f_nz_buffer_buckup, eg9013f_nz_buffer, buffer_size) != 0) || (force_refresh_count == 0)) {
			memcpy(eg9013f_nz_buffer_buckup, eg9013f_nz_buffer, buffer_size);
			flush_data_to_lcd(dma, eg9013f_nz_buffer_buckup, buffer_size);
		}

		force_refresh_count++;
		if(force_refresh_count == FORCE_REFRESH_COUNT) {
			force_refresh_count = 0;
		}
	}

	vfree(eg9013f_nz_buffer_buckup);
	up(&dma->dma_dev_sema);

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

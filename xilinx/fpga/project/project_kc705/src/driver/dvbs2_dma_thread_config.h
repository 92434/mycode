#ifndef _DVBS2_DMA_THREAD_CONFIG_H
#define _DVBS2_DMA_THREAD_CONFIG_H

static int dma_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;

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

#endif//#ifndef _DVBS2_DMA_THREAD_CONFIG_H

#ifndef _DVBS2_DMA_THREAD_CONFIG_H
#define _DVBS2_DMA_THREAD_CONFIG_H

unsigned long long i2s_read_count = 0;

static int i2s_dma_receiver_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 

		put_pcie_tr(dma, 0, 0, 0, dma->receive_bulk_size, NULL, NULL);
		i2s_read_count++;
	}

	mydebug("\n");

	return ret;
}

dma_thread_info_t dvbs2_dma_threads[] = {
	{
		.t = &i2s_dma_receiver_thread,
		.thread_name = "i2s_receiver"
	},
};

#define DVBS2_DMA_THREADS (sizeof(dvbs2_dma_threads) / sizeof(dma_thread_info_t))

#endif//#ifndef _DVBS2_DMA_THREAD_CONFIG_H

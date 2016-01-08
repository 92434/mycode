#ifndef _CSA_DMA_THREAD_CONFIG_H
#define _CSA_DMA_THREAD_CONFIG_H

typedef int (*pcie_dma_thread_t)(void *ppara);

typedef struct _dma_thread_info {
	pcie_dma_thread_t t;
	int dma_index;
	char *thread_name;
} dma_thread_info_t;

static int csa_dma_receiver_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 

		put_pcie_tr(dma, 0, 0, 0, (4 * 3 * 4), NULL, NULL, true);
	}

	mydebug("\n");

	return ret;
}

dma_thread_info_t dma_threads[] = {
	{
		.t = &csa_dma_receiver_thread,
		.dma_index = 0,
		.thread_name = "csa_receiver"
	},
};

#define MAX_DMA_THREAD (sizeof(dma_threads) / sizeof(dma_thread_info_t))

#endif//#ifndef _CSA_DMA_THREAD_CONFIG_H

#ifndef _CSA_DEBUG_DMA_THREAD_CONFIG_H
#define _CSA_DEBUG_DMA_THREAD_CONFIG_H

static int csa_debug_dma_receiver_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	//kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;

	while(!kthread_should_stop()) {
		//set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 
		
		put_pcie_tr(dma, 0, 0, 0, dma->receive_bulk_size, NULL, NULL);
	}

	mydebug("\n");

	return ret;
}

dma_thread_info_t csa_debug_dma_threads[] = {
	{
		.t = &csa_debug_dma_receiver_thread,
		.thread_name = "csa_debug_receiver"
	},
};

#define CSA_DEBUG_DMA_THREAD (sizeof(csa_debug_dma_threads) / sizeof(dma_thread_info_t))

#endif//#ifndef _CSA_DEBUG_DMA_THREAD_CONFIG_H

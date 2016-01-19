#ifndef _CSA_DMA_THREAD_CONFIG_H
#define _CSA_DMA_THREAD_CONFIG_H

static bool csa_is_ready_for_write(void *ppara) {
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *csa_busy_reg = base_vaddr + kc705_pci_dev->dma[1].dma_lite_offset + 0;
	
	uint32_t value = readl(csa_busy_reg) & 0x1;

	return (value == 0);
}

static bool csa_is_ready_for_read(void *ppara) {
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *csa_ready_reg = base_vaddr + kc705_pci_dev->dma[1].dma_lite_offset + 4;

	uint32_t value = readl(csa_ready_reg) & 0x1;

	return (value == 1);
}

static int csa_dma_receiver_thread(void *ppara) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)ppara;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;

	while(!kthread_should_stop()) {
		//set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(msecs_to_jiffies(10)); 
		
		put_pcie_tr(dma, 0, 0, 0, dma->receive_bulk_size, NULL, NULL);
	}

	mydebug("\n");

	return ret;
}

dma_thread_info_t csa_dma_threads[] = {
	{
		.t = &csa_dma_receiver_thread,
		.thread_name = "csa_receiver"
	},
};

#define CSA_DMA_THREAD (sizeof(csa_dma_threads) / sizeof(dma_thread_info_t))

#endif//#ifndef _CSA_DMA_THREAD_CONFIG_H

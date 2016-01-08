#include "pcie.h"
#include "pcie_tr_thread.h"

#include "csa_dma_thread_config.h"

int alloc_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	kc705_pci_dev->dma_thread = (struct task_struct **)vzalloc(sizeof(struct task_struct *) * MAX_DMA_THREAD);
	if(kc705_pci_dev->dma_thread == NULL) {
		mydebug("alloc kc705_pci_dev->dma_thread failed.\n");
		ret = -1;
	}
	kc705_pci_dev->dma_thread_count = MAX_DMA_THREAD;

	return ret;
}

void free_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->dma_thread != NULL) {
		kc705_pci_dev->dma_thread_count = 0;
		vfree(kc705_pci_dev->dma_thread);
		kc705_pci_dev->dma_thread = NULL;
	}
}

void start_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->dma_thread_count; i++) {
		if(kc705_pci_dev->dma_thread[i] == NULL) {
			kc705_pci_dev->dma_thread[i] = alloc_work_thread(dma_threads[i].t, kc705_pci_dev->dma + dma_threads[i].dma_index, "%s", dma_threads[i].thread_name);
		}
	}
}

void stop_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->dma_thread_count; i++) {
		if(kc705_pci_dev->dma_thread[i] != NULL) {
			free_work_thread(kc705_pci_dev->dma_thread[i]);
			kc705_pci_dev->dma_thread[i] = NULL;
		}
	}
}


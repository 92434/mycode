#include "pcie.h"
#include "pcie_tr_thread.h"

int alloc_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;

	if(kc705_pci_dev->total_dma_thread_count == 0) {
		return ret;
	}

	kc705_pci_dev->dma_thread = (struct task_struct **)vzalloc(sizeof(struct task_struct *) * kc705_pci_dev->total_dma_thread_count);
	if(kc705_pci_dev->dma_thread == NULL) {
		mydebug("alloc kc705_pci_dev->dma_thread failed.\n");
		ret = -1;
	}

	return ret;
}

void free_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->dma_thread != NULL) {
		vfree(kc705_pci_dev->dma_thread);
		kc705_pci_dev->dma_thread = NULL;
	}
}

void start_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int i;
	int j;
	int k = 0;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		for(j = 0; j < dma->dma_thread_count; j++) {
			if(kc705_pci_dev->dma_thread[k] == NULL) {
				kc705_pci_dev->dma_thread[k] = alloc_work_thread(dma->dma_thread[i].t, dma, "%s", dma->dma_thread[j].thread_name);
				k++;
			}
		}
	}
}

void stop_dma_thread(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->total_dma_thread_count; i++) {
		if(kc705_pci_dev->dma_thread[i] != NULL) {
			free_work_thread(kc705_pci_dev->dma_thread[i]);
			kc705_pci_dev->dma_thread[i] = NULL;
		}
	}
}


#include "pcie.h"

int tr_wait(pcie_dma_t *dma, struct completion *tr_cmp) {
	int ret = 0;
	//unsigned long tmo;

	//tmo = msecs_to_jiffies(1000);
	//tmo = wait_for_completion_timeout(tr_cmp, tmo);
	//if (0 == tmo) {
	//	myprintf("(%d)%s:tr_wait timed out!\n", task_pid_nr(current), dma->dev_name);
	//	ret = -1;
	//}

	wait_for_completion(tr_cmp);

	return ret;
}

int tr_wakeup(struct completion *tr_cmp) {
	int ret = 0;

	if(tr_cmp != NULL) {
		//mydebug("tr_cmp:%p\n", tr_cmp);
		complete(tr_cmp);
	}
	return ret;
}

int alloc_pcie_tr(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	pcie_tr_t *tr_buffer;

	spin_lock_init(&kc705_pci_dev->pcie_tr_list_lock);

	kc705_pci_dev->pcie_tr_list = init_list_buffer();
	if(kc705_pci_dev->pcie_tr_list == NULL) {
		ret = -1;
		goto alloc_pcie_tr_list_failed;
	} else {
		disable_list_buffer_overwrite(kc705_pci_dev->pcie_tr_list, true);
	}

	tr_buffer = (pcie_tr_t *)vzalloc(sizeof(pcie_tr_t) * 1024);
	if(tr_buffer == NULL) {
		mydebug("alloc tr_buffer failed.\n");
		ret = -1;
		goto alloc_pcie_tr_buffer_failed;
	}
	add_list_buffer_item((char *)tr_buffer, (void *)NULL, sizeof(pcie_tr_t) * 1024, kc705_pci_dev->pcie_tr_list);

alloc_pcie_tr_buffer_failed:
	uninit_list_buffer(kc705_pci_dev->pcie_tr_list);
	kc705_pci_dev->pcie_tr_list = NULL;
alloc_pcie_tr_list_failed:
	return ret;
}

void free_pcie_tr(kc705_pci_dev_t *kc705_pci_dev) {
	buffer_node_t *node;
	
	//only 1 node
	if(kc705_pci_dev->pcie_tr_list->first != NULL) {
		node = list_entry(kc705_pci_dev->pcie_tr_list->first, buffer_node_t, list);
		vfree(node->buffer);
	}
	
	uninit_list_buffer(kc705_pci_dev->pcie_tr_list);
	kc705_pci_dev->pcie_tr_list = NULL;
}

int put_pcie_tr(pcie_dma_t *dma,
		uint64_t tx_dest_axi_addr,
		uint64_t rx_src_axi_addr,
		int tx_size,
		int rx_size,
		uint8_t *tx_data,
		uint8_t *rx_data,
		bool wait) {
	int ret = 0;

	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)dma->kc705_pci_dev;

	pcie_tr_t tr;
	tr.dma = dma;
	tr.tx_dest_axi_addr = tx_dest_axi_addr;
	tr.rx_src_axi_addr = rx_src_axi_addr;
	tr.tx_size = tx_size;
	tr.rx_size = rx_size;
	tr.tx_data = tx_data;
	tr.rx_data = rx_data;

	if(wait) {
		struct completion *tr_cmp = (struct completion *)vzalloc(sizeof(struct completion));
		if(tr_cmp == NULL) {
			mydebug("alloc tr_cmp failed.\n");
			ret = -1;
			goto exit;
		} else {
			tr.tr_cmp = tr_cmp;
			init_completion(tr.tr_cmp);
		}
	} else {
		tr.tr_cmp = NULL;
	}

	spin_lock(&kc705_pci_dev->pcie_tr_list_lock);
	ret = write_buffer((char *)&tr, sizeof(pcie_tr_t), kc705_pci_dev->pcie_tr_list);
	spin_unlock(&kc705_pci_dev->pcie_tr_list_lock);

	if(tr.tr_cmp != NULL) {
		tr_wait(tr.dma, tr.tr_cmp);
		vfree(tr.tr_cmp);
	}

exit:
	return ret;
}

static void reset_pcie_tr(kc705_pci_dev_t *kc705_pci_dev) {
	spin_lock(&kc705_pci_dev->pcie_tr_list_lock);
	buffer_reset(kc705_pci_dev->pcie_tr_list);
	spin_unlock(&kc705_pci_dev->pcie_tr_list_lock);
}

static int get_pcie_tr(kc705_pci_dev_t *kc705_pci_dev, pcie_tr_t *tr) {
	int ret;

	ret = read_buffer((char *)tr, sizeof(pcie_tr_t), kc705_pci_dev->pcie_tr_list);

	return ret;
}

static int pcie_tr_thread(void *ppara) {
	int ret = 0;
	int i;
	int size;
	pcie_tr_t tr;

	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)ppara;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		dma->dma_op.init_dma(dma);
	}

	while(!kthread_should_stop()) {
		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		
		size = get_pcie_tr(kc705_pci_dev, &tr);
		if(size != 0) {
			tr.dma->dma_op.dma_tr(&tr);
		} else {
			schedule_timeout(msecs_to_jiffies(10)); 
		}

	 }

	while(get_pcie_tr(kc705_pci_dev, &tr) != 0) {
		struct completion *tr_cmp = tr.tr_cmp;
		tr_wakeup(tr_cmp);
	}

	return ret;
}

void alloc_tr_thread(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->pcie_tr_thread == NULL) {
		kc705_pci_dev->pcie_tr_thread = alloc_work_thread(pcie_tr_thread, kc705_pci_dev, "%s", "pcie_tr");
	}
}

void free_tr_thread(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->pcie_tr_thread != NULL) {
		free_work_thread(kc705_pci_dev->pcie_tr_thread);
		kc705_pci_dev->pcie_tr_thread = NULL;
	}
	reset_pcie_tr(kc705_pci_dev);
}

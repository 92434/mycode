#ifndef _PCIE_TR_THREAD_H
#define _PCIE_TR_THREAD_H
int tr_wait(pcie_dma_t *dma, struct completion *tr_cmp);
int tr_wakeup(struct completion *tr_cmp);
int alloc_pcie_tr(kc705_pci_dev_t *kc705_pci_dev);
void free_pcie_tr(kc705_pci_dev_t *kc705_pci_dev);
int put_pcie_tr(pcie_dma_t *dma,
		uint64_t tx_dest_axi_addr,
		uint64_t rx_src_axi_addr,
		int tx_size,
		int rx_size,
		uint8_t *tx_data,
		uint8_t *rx_data);
void alloc_tr_thread(kc705_pci_dev_t *kc705_pci_dev);
void free_tr_thread(kc705_pci_dev_t *kc705_pci_dev);
#endif//#ifndef _PCIE_TR_THREAD_H

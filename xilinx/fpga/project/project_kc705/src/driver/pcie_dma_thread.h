#ifndef _PCIE_DMA_THREAD_H
#define _PCIE_DMA_THREAD_H
int alloc_dma_thread(kc705_pci_dev_t *kc705_pci_dev);
void free_dma_thread(kc705_pci_dev_t *kc705_pci_dev);
void start_dma_thread(kc705_pci_dev_t *kc705_pci_dev);
void stop_dma_thread(kc705_pci_dev_t *kc705_pci_dev);
#endif//#ifndef _PCIE_TR_THREAD_H

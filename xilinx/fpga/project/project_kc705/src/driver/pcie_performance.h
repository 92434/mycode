#ifndef _PCIE_PERFORMANCE_H
#define _PCIE_PERFORMANCE_H
void start_statistic_timer(kc705_pci_dev_t *kc705_pci_dev);
void stop_statistic_timer(kc705_pci_dev_t *kc705_pci_dev);
void inc_dma_op_tx_count(pcie_dma_t *dma, long unsigned int count);
void inc_dma_op_rx_count(pcie_dma_t *dma, long unsigned int count);
long unsigned int get_op_tx_count(pcie_dma_t *dma);
long unsigned int get_op_rx_count(pcie_dma_t *dma);
#endif//#ifndef _PCIE_PERFORMANCE_H

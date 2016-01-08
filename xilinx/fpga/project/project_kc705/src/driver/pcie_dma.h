#ifndef _PCIE_DMA_H
#define _PCIE_DMA_H
int alloc_kc705_dma(kc705_pci_dev_t *kc705_pci_dev);
void free_kc705_dma(kc705_pci_dev_t *kc705_pci_dev);
void add_dma_device(kc705_pci_dev_t *kc705_pci_dev);
void remove_dma_device(kc705_pci_dev_t *kc705_pci_dev);
irqreturn_t dma_process_isr(kc705_pci_dev_t *kc705_pci_dev);
irqreturn_t dma_process_isr(kc705_pci_dev_t *kc705_pci_dev);
int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection);
int write_addr_to_reg(uint32_t *reg, uint64_t addr);
#endif//#ifndef _PCIE_DMA_H

#ifndef _DMA_COMMON_H
#define _DMA_COMMON_H

#include <linux/version.h>

int dma_trans_sync(kc705_pci_dev_t *kc705_pci_dev);
int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection);
int write_addr_to_reg(uint32_t *reg, uint64_t addr);
void prepare_test_data(kc705_pci_dev_t *kc705_pci_dev);
void test_result(kc705_pci_dev_t *kc705_pci_dev);

#endif //#define _DMA_COMMON_H

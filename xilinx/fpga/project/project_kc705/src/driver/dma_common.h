#ifndef _DMA_COMMON_H
#define _DMA_COMMON_H

#include <linux/version.h>

int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection);
int write_addr_to_reg(uint32_t *reg, uint64_t addr);
void prepare_test_data(kc705_pci_dev_t *kc705_pci_dev);
void test_result(kc705_pci_dev_t *kc705_pci_dev);


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
irqreturn_t isr(int irq, void *dev_id, struct pt_regs *regs);
#else
irqreturn_t isr(int irq, void *dev_id);
#endif
int prepare_bars_map(kc705_pci_dev_t *kc705_pci_dev);
int alloc_sg_list_chain(uint32_t tx_axiaddr, uint32_t rx_axiaddr);
void free_sg_list_chain(void);
int dma_worker_thread(void *ppara);
int init_dma(kc705_pci_dev_t *kc705_pci_dev);

#endif //#define _DMA_COMMON_H

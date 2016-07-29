#ifndef _PCIE_GPIO_H
#define _PCIE_GPIO_H
int alloc_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev);
void free_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev);

void add_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev);
void remove_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev);

#endif//#ifndef _PCIE_GPIO_H

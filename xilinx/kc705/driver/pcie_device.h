#ifndef _PCIE_DEVICE_H
#define _PCIE_DEVICE_H

int init_kc705_dev_class(kc705_pci_dev_t *kc705_pci_dev);
void uninit_kc705_dev_class(kc705_pci_dev_t *kc705_pci_dev);
int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);
void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);

#endif//#ifndef _PCIE_DEVICE_H

#ifndef _PCIE_DEVICE_H
#define _PCIE_DEVICE_H

int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev, char *namefmt, ...);
void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);

#endif//#ifndef _PCIE_DEVICE_H

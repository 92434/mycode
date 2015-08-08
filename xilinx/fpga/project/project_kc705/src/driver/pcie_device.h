#ifndef _PCIE_DEVICE_H
#define _PCIE_DEVICE_H

//driver struct
#define PCIE_DEVICE_IOCTL_BASE	'W'
#define PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE _IO(PCIE_DEVICE_IOCTL_BASE, 0)
#define PCIE_DEVICE_IOCTL_GET_NODE_INFO _IO(PCIE_DEVICE_IOCTL_BASE, 1)

typedef struct _node_info {
	int size;
	int read_offset;
	int avail_for_read;
	int base_addr_of_list_buffer;
} node_info_t;
//driver struct

int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);
void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);
#endif//#ifndef _PCIE_DEVICE_H

#ifndef _PCIE_DMA_DEVICE_H
#define _PCIE_DMA_DEVICE_H

//driver struct
#define PCIE_DEVICE_IOCTL_BASE	'W'
#define PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE _IO(PCIE_DEVICE_IOCTL_BASE, 0)
#define PCIE_DEVICE_IOCTL_GET_NODE_INFO _IO(PCIE_DEVICE_IOCTL_BASE, 1)
#define PCIE_DEVICE_IOCTL_PSEUDO_DMA_TR _IO(PCIE_DEVICE_IOCTL_BASE, 2)

typedef struct _node_info {
	int size;
	int read_offset;
	int avail_for_read;
	int base_addr_of_list_buffer;
} node_info_t;

typedef struct _pseudo_dma_tr {
	int tx_size;
	int rx_size;
	uint8_t *tx_data;
	uint8_t *rx_data;
	int tx_dest_offset;
	int rx_src_offset;
} pseudo_dma_tr_t;
//driver struct

#endif//#ifndef _PCIE_DMA_DEVICE_H

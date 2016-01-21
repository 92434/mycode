#ifndef _KC705_H
#define _KC705_H
#include <linux/version.h>

#define BITMASK(loc) (1 << loc)

//#if defined(i386)
//#else
//#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
#else
#define __devinit
#define __devexit
#define __devexit_p(f) f
#endif

/** Driver information */
#define MODULE_NAME "kc705_pcie"
#define DRIVER_DESCRIPTION "kc705 pcie driver"
#define DRIVER_VERSION "1.0"


//pcie device define
#define PCIE_DEVICE_IOCTL_BASE	'W'
#define PCIE_DEVICE_IOCTL_START_DMA_THREAD _IO(PCIE_DEVICE_IOCTL_BASE, 0)
#define PCIE_DEVICE_IOCTL_STOP_DMA_THREAD _IO(PCIE_DEVICE_IOCTL_BASE, 1)

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
#endif //#define _KC705_H

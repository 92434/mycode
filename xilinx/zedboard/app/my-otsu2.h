#ifndef __MY_OTSU2_H

#define USE_VDMA
#define RST_ACTIVE	0
#define RST_INACTIVE	1

#ifdef KERNEL_DIGILENT
#define UIO_DEV "/dev/uio1"
#define UIO_ADDR "/sys/class/uio/uio0/maps/map0/addr"
#define UIO_SIZE "/sys/class/uio/uio0/maps/map0/size"
#else
#define KMEM_DEV "/dev/kmem"
#endif

//typedef unsigned int dma_cookie_t;
typedef struct _image_buffer_info {
	int size;
	char *buffer;
	unsigned int phy_out_buffer;
	unsigned int k_in_buffer;
	unsigned int phy_in_buffer;
	unsigned int k_out_buffer;
} image_buffer_info_t;

typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XOtsu2_Config;

#include "xotsu2.h"
#endif//#ifndef __MY_OTSU2_H

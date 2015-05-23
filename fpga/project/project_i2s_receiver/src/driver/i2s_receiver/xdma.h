#ifndef XDMA_H
#define XDMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/types.h>
#include <asm/ioctl.h>

#define MODULE_NAME "xdma"
//#define DMA_LENGTH (32*1024*1024)
#define AXI_BUS_WIDTH_BY_BYTE (32 / 8)
#define I2S_DATA_COUNT_EACH_GROUP (1024)
#define DMA_LENGTH (AXI_BUS_WIDTH_BY_BYTE * I2S_DATA_COUNT_EACH_GROUP)	// max image is 1024x1024 for now!
#define MAX_DEVICES 4
#define MAX_NAME_SIZE 64

#define XDMA_IOCTL_BASE	'W'
#define XDMA_GET_NUM_DEVICES _IO(XDMA_IOCTL_BASE, 0)
#define XDMA_GET_DEV_INFO _IO(XDMA_IOCTL_BASE, 1)
#define XDMA_DEVICE_CONTROL _IO(XDMA_IOCTL_BASE, 2)
#define XDMA_PREP_BUF _IO(XDMA_IOCTL_BASE, 3)
#define XDMA_START_TRANSFER _IO(XDMA_IOCTL_BASE, 4)
#define XDMA_STOP_TRANSFER _IO(XDMA_IOCTL_BASE, 5)


#ifdef __cplusplus
}
#endif

#endif				/* XDMA_H */

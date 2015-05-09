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
#define I2S_DATA_COUNT_EACH_GROUP (8)
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

	struct xdma_dev {
		u32 tx_chan;	/* (struct dma_chan *) */
		u32 tx_cmp;	/* (struct completion *) callback_param */
		u32 rx_chan;	/* (struct dma_chan *) */
		u32 rx_cmp;	/* (struct completion *) callback_param */
		char dma_name[MAX_NAME_SIZE];
	};

	struct xdma_chan_cfg {
		u32 chan;	/* (struct dma_chan *) */

		enum dma_transfer_direction dir;	/* Channel direction */
		int coalesc;	/* Interrupt coalescing threshold */
		int delay;	/* Delay counter */
		int reset;	/* Reset Channel */
	};

	struct xdma_buf_info {
		u32 chan;	/* (struct dma_chan *) */
		u32 completion;	/* (struct completion *) callback_param */

		dma_cookie_t cookie;
		u32 buf_offset;
		u32 buf_size;
		enum dma_transfer_direction dir;
	};

	struct xdma_transfer {
		u32 chan;	/* (struct dma_chan *) */
		u32 completion;	/* (struct completion *) callback_param */

		dma_cookie_t cookie;
		u32 wait;	/* true/false */
	};

#ifdef __cplusplus
}
#endif

#endif				/* XDMA_H */

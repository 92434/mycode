#ifndef __MY_SOBEL_H

#define USE_VDMA
#define RST_ACTIVE	0
#define RST_INACTIVE	1

//#define UIO_DEV "/dev/uio1"
#define UIO_DEV "/dev/kmem"
#define UIO_ADDR "/sys/class/uio/uio1/maps/map0/addr"
#define UIO_SIZE "/sys/class/uio/uio1/maps/map0/size"

enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};

enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};

/* DMA Device configuration structure
 *
 * Xilinx CDMA and Xilinx DMA only use interrupt coalescing and delay counter
 * settings.
 *
 * If used to start/stop parking mode for Xilinx VDMA, vsize must be -1
 * If used to set interrupt coalescing and delay counter only for
 * Xilinx VDMA, hsize must be -1 */
struct xilinx_dma_config {
	enum dma_transfer_direction direction; /* Channel direction */
	int vsize;                         /* Vertical size */
	int hsize;                         /* Horizontal size */
	int stride;                        /* Stride */
	int frm_dly;                       /* Frame delay */
	int gen_lock;                      /* Whether in gen-lock mode */
	int master;                        /* Master that it syncs to */
	int frm_cnt_en;                    /* Enable frame count enable */
	int park;                          /* Whether wants to park */
	int park_frm;                      /* Frame to park on */
	int coalesc;                       /* Interrupt coalescing threshold */
	int delay;                         /* Delay counter */
	int disable_intr;                  /* Whether use interrupts */
	int reset;			   /* Reset Channel */
	int ext_fsync;			   /* External Frame Sync */
};

//typedef unsigned int dma_cookie_t;
typedef struct _image_buffer_info {
	int size;
	char *buffer;
	unsigned int phy_in_buffer;
	unsigned int phy_out_buffer;
} image_buffer_info_t;
#ifdef USE_VDMA
#include "xvdma.h"
#else//#ifdef USE_VDMA
#endif//#ifdef USE_VDMA

#endif//#ifndef __MY_SOBEL_H

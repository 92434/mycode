#ifndef __XVDMA_H
#define DRIVER_NAME 	"xvdma"
#define XVDMA_SUSPEND	NULL
#define XVDMA_RESUME	NULL

#define XVDMA_MAJOR	10
#define XVDMA_MINOR	224

#define MAX_DEVICES	4
#define MAX_FRAMES	5
#define MAX_NAME_SIZE       64

#define XVDMA_IOCTL_BASE	'W'
#define XVDMA_GET_NUM_DEVICES	_IO(XVDMA_IOCTL_BASE, 0)
#define XVDMA_GET_DEV_INFO	_IO(XVDMA_IOCTL_BASE, 1)
#define XVDMA_DEVICE_CONTROL	_IO(XVDMA_IOCTL_BASE, 2)
#define XVDMA_PREP_BUF		_IO(XVDMA_IOCTL_BASE, 3)
#define XVDMA_START_TRANSFER	_IO(XVDMA_IOCTL_BASE, 4)
#define XVDMA_STOP_TRANSFER	_IO(XVDMA_IOCTL_BASE, 5)
#define XVDMA_GET_DMA_MEM_ADDR	_IO(XVDMA_IOCTL_BASE, 6)

#define IMAGE_WIDTH	1024
#define IMAGE_HEIGHT	768
#define IMAGE_PIXSIZE	1
#define IMAGE_SIZE	(IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_PIXSIZE)
#define BUFFER_SIZE	(IMAGE_SIZE * 2)
#define DMA_SOBEL_NAME	"43040000.axivdma"


struct xvdma_dev
{
	u32 tx_chan;
	u32 rx_chan;
	char dma_name[MAX_NAME_SIZE];
};

struct xvdma_chan_cfg {
	struct xilinx_dma_config config;
	u32 chan;
};

struct xvdma_buf_info {
	u32 chan;
	u32 direction;
	u32 mem_type;
	u32 buf_size;
	//u32 addr_base;
	u32 frm_cnt;
	u32 callback;
};

struct xvdma_transfer {
	u32 chan;
	u32 wait;
};

typedef struct _xvdma_buffer_addr {
	unsigned int in_addr;
	unsigned int out_addr;
} xvdma_buffer_addr_t;

#endif //#ifndef __XVDMA_H

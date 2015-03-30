#ifndef __XOTSU2_H
#define DRIVER_NAME 	"xotsu2"
#define XOTSU2_IOCTL_BASE	'W'
#define XOTSU_GET_DMA_MEM_ADDR	_IO(XOTSU2_IOCTL_BASE, 0)
#define XOTSU_WAIT_FOR_RESULT	_IO(XOTSU2_IOCTL_BASE, 1)

#define IMAGE_WIDTH	1024
#define IMAGE_HEIGHT	768
#define IMAGE_PIXSIZE	1
#define IMAGE_SIZE	(IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_PIXSIZE)
#define BUFFER_SIZE	(IMAGE_SIZE * 2)

typedef struct _xotsu2_buffer_addr {
	unsigned int k_in_addr;
	unsigned int phy_in_addr;
	unsigned int k_out_addr;
	unsigned int phy_out_addr;
} xotsu2_buffer_addr_t;

typedef struct _xotsu2_result {
	int value;
	bool is_timeout;
} xotsu2_result_t;

//#define KERNEL_DIGILENT

#endif//#ifndef __XOTSU2_H

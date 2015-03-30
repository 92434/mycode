#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "gpio_utils.h"
#include "ximage_filter.h"
#include "my_sobel.h"
#include "VDMA.h"

#define VDMABaseAddr 0x43040000

#define myexec(x) \
	do { \
		x; \
		printf("%s:"#x":passed!\n", __func__); \
	}while(0)

static image_buffer_info_t image_buffer_info = {0,0};

void dumpMemory()
{
	char *virtual_addr_in = image_buffer_info.buffer;
	remove("data0.bin");
	int fd_data0 = open("data0.bin", O_WRONLY | O_CREAT);
	if(fd_data0 != -1) {
		//for(i = IMAGE_SIZE ; i < BUFFER_SIZE ; i++)
		//for(i = 0 ; i < IMAGE_SIZE ; i++)
		{
			write(fd_data0, virtual_addr_in + IMAGE_SIZE, IMAGE_SIZE);
			//write(fd_data0, virtual_addr_in, IMAGE_SIZE);
		}
		close(fd_data0);
	} else {
		printf("xiaofei:open data0.bin failed!(%s)\n", strerror(errno));
	}
}

void gpio_reset(int level)
{
	// reset SOBEL
	gpio_export(SOBEL_RST_PIN);
	gpio_dir_out(SOBEL_RST_PIN);
	gpio_value(SOBEL_RST_PIN, level);
	gpio_unexport(SOBEL_RST_PIN);

	//printf("\nReset Done\n");
}

void vdma_reset(const char *dma_name, enum dma_transfer_direction vdma_dir)
{
	int fd_vdma;
	struct xvdma_chan_cfg chan_cfg;
	struct xvdma_dev xdma_device_info;

	if ((fd_vdma = open("/dev/xvdma", O_RDWR)) < 0)
	{
		printf("Cannot open device node xvdma\n");
		//exit(1);
	}

	strcpy(xdma_device_info.dma_name, dma_name);
	if (ioctl(fd_vdma, XVDMA_GET_DEV_INFO, &xdma_device_info) < 0)
	{
		printf("%s: Failed to get info for device id:%s", __func__, dma_name);
	}

	if(vdma_dir == DMA_DEV_TO_MEM)
	{// rx
		chan_cfg.chan = xdma_device_info.rx_chan;
	}
	else
	{
		chan_cfg.chan = xdma_device_info.tx_chan;
	}

	/* Set up hardware configuration information */
	chan_cfg.config.direction = vdma_dir;
	chan_cfg.config.vsize = IMAGE_WIDTH;
	chan_cfg.config.hsize = IMAGE_HEIGHT; // width length in bytes (Assuming always a pixel is of 32bpp / 24bpp unpacked)
	chan_cfg.config.stride = IMAGE_WIDTH  * IMAGE_PIXSIZE;		// stride length in bytes (Assuming always a pixel is of 32bpp / 24bpp unpacked)

	chan_cfg.config.frm_cnt_en = 1;
	chan_cfg.config.coalesc = 0;
	chan_cfg.config.delay = 0;
	chan_cfg.config.frm_dly = 0;
	chan_cfg.config.gen_lock = 0; /* Gen-Lock */
	chan_cfg.config.master = 0;
	chan_cfg.config.park_frm = 0;
	chan_cfg.config.disable_intr = 0;
	chan_cfg.config.reset = 1;
	if(chan_cfg.config.direction == DMA_MEM_TO_DEV) {
		chan_cfg.config.park = 1; /* circular_buf_en(0), park_mode(1) */
		chan_cfg.config.ext_fsync = 1; //fsync type
	}
	else {
		chan_cfg.config.park = 1; /* circular_buf_en(0), park_mode(1) */
		chan_cfg.config.ext_fsync = 1; //fsync type
	}

	//if(ioctl(fd_vdma, XVDMA_STOP_TRANSFER, &(chan_cfg.chan)) < 0)
	//	printf("VDMA: XVDMA_STOP_TRANSFER calling failed\n");


	//if (ioctl(fd_vdma, XVDMA_DEVICE_CONTROL, &chan_cfg) < 0)
	//	printf("VDMA: XVDMA_DEVICE_CONTROL calling failed\n");

	close(fd_vdma);

	//printf("\n VDMA Reset Done\r\n" );
}

void configureVDMA(const char *dma_name, enum dma_transfer_direction vdma_direction)
{
	struct xvdma_dev xvdma_dev;
	struct xvdma_chan_cfg chan_cfg;
	struct xvdma_buf_info buf_info;
	int fd_vdma;

	if ((fd_vdma = open("/dev/xvdma", O_RDWR)) < 0)
	{
		printf("Cannot open device node xvdma\n");
		exit(1);
	}

	strcpy(xvdma_dev.dma_name, dma_name);

	if (ioctl(fd_vdma, XVDMA_GET_DEV_INFO, &xvdma_dev) < 0)
	{
		printf("%s: Failed to get info for device id:%s", __func__, dma_name);
	}

	if(vdma_direction == DMA_DEV_TO_MEM)		// frome device to memory
	{// rx
		chan_cfg.chan = xvdma_dev.rx_chan;
		buf_info.chan = xvdma_dev.rx_chan;
		buf_info.mem_type = DMA_FROM_DEVICE;
	}
	else										// from memory to devie
	{
		chan_cfg.chan = xvdma_dev.tx_chan;
		buf_info.chan = xvdma_dev.tx_chan;
		buf_info.mem_type = DMA_TO_DEVICE;
	}

	/* Set up hardware configuration information */

	chan_cfg.config.direction = vdma_direction;
	chan_cfg.config.vsize = IMAGE_WIDTH;
	chan_cfg.config.hsize = IMAGE_HEIGHT; // width length in bytes (Assuming always a pixel is of 32bpp / 24bpp unpacked)
	chan_cfg.config.stride = IMAGE_WIDTH  * IMAGE_PIXSIZE;		// stride length in bytes (Assuming always a pixel is of 32bpp / 24bpp unpacked)

	chan_cfg.config.frm_cnt_en = 1;
	chan_cfg.config.coalesc = 0;
	chan_cfg.config.delay = 0;
	chan_cfg.config.frm_dly = 0;
	chan_cfg.config.gen_lock = 0; /* Gen-Lock */
	chan_cfg.config.master = 0;
	chan_cfg.config.park_frm = 0;
	chan_cfg.config.disable_intr = 0;
	chan_cfg.config.reset = 1;
	if(chan_cfg.config.direction == DMA_MEM_TO_DEV) {
		chan_cfg.config.park = 1; /* circular_buf_en(0), park_mode(1) */
		chan_cfg.config.ext_fsync = 1; //fsync type
	}
	else {
		chan_cfg.config.park = 1; /* circular_buf_en(0), park_mode(1) */
		chan_cfg.config.ext_fsync = 1; //fsync type
	}

	if (ioctl(fd_vdma, XVDMA_DEVICE_CONTROL, &chan_cfg) < 0)			// --1--
		printf("VDMA: XVDMA_DEVICE_CONTROL calling failed\n");

	buf_info.direction = vdma_direction;
	buf_info.buf_size = IMAGE_SIZE;
	buf_info.frm_cnt = 1;
	if (ioctl(fd_vdma, XVDMA_PREP_BUF, &buf_info) < 0)
		printf("%s: Calling XVDMA_PREP_BUF failed\n",__func__);

	close(fd_vdma);
}

void startVDMA(const char *dma_name, int vdma_direction)
{
	struct xvdma_dev xvdma_dev;
	int fd_vdma;
	struct xvdma_transfer xfer_param;
	if ((fd_vdma = open("/dev/xvdma", O_RDWR)) < 0)
	{
		printf("Cannot open device node xvdma\n");
		exit(1);
	}

	strcpy(xvdma_dev.dma_name, dma_name);

	if (ioctl(fd_vdma, XVDMA_GET_DEV_INFO, &xvdma_dev) < 0)
	{
		printf("%s: Failed to get info for device id:%s", __func__, dma_name);
	}

	switch(vdma_direction)
	{
		case DMA_DEV_TO_MEM:
			{
				xfer_param.chan =  xvdma_dev.rx_chan;
				xfer_param.wait = 1;
			}
			break;

		case DMA_MEM_TO_DEV:
			{
				xfer_param.chan =  xvdma_dev.tx_chan;
				xfer_param.wait = 0;
			}
			break;

		default:
			perror("Invalid direction for start transfer");
			break;

	}
	if (ioctl(fd_vdma, XVDMA_START_TRANSFER, &xfer_param) < 0)
		printf("%s: Calling XVDMA_START_TRANSFER failed\n",__func__);

	close(fd_vdma);
}

int hw_sobel_filter(char *filter_name) {
	XImage_filter xfilter;
	XImage_filter_Initialize(&xfilter, filter_name);

	XImage_filter_SetRows(&xfilter, IMAGE_HEIGHT);
	XImage_filter_SetCols(&xfilter, IMAGE_WIDTH);

	XImage_filter_InterruptGlobalEnable(&xfilter);
	XImage_filter_InterruptEnable(&xfilter, 3);
	XImage_filter_EnableAutoRestart(&xfilter);
	XImage_filter_Start(&xfilter);

	configureVDMA(DMA_SOBEL_NAME, DMA_MEM_TO_DEV);
	configureVDMA(DMA_SOBEL_NAME, DMA_DEV_TO_MEM);
	startVDMA(DMA_SOBEL_NAME, DMA_MEM_TO_DEV);
	startVDMA(DMA_SOBEL_NAME, DMA_DEV_TO_MEM);

	XImage_filter_Release(&xfilter);

	return 0;
}

void resetStop_DMA_ALL(void)
{
	//gpio_reset(RST_ACTIVE);	// pull the reset line.

	// Reset Sobel DMA (IN and OUT)
	vdma_reset(DMA_SOBEL_NAME, DMA_DEV_TO_MEM);		// sobel out
	vdma_reset(DMA_SOBEL_NAME, DMA_MEM_TO_DEV);		// Soble in

	//gpio_reset(RST_INACTIVE);
}

int free_image_buffer() {
	int ret = 0;
	munmap(image_buffer_info.buffer, image_buffer_info.size);
	image_buffer_info.buffer = 0;
	image_buffer_info.size = 0;
	image_buffer_info.phy_in_buffer = 0;
	image_buffer_info.phy_out_buffer = 0;

	return ret;
}

int get_vmda_buffer_addr_direct(unsigned int *in, unsigned int *out) {
	int ret = 0;
	int fd_vdma;
	xvdma_buffer_addr_t addr;

	if ((ret = open("/dev/xvdma", O_RDWR)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto open_xvdma_failed;
	}
	fd_vdma = ret;

	if((ret = ioctl(fd_vdma, XVDMA_GET_DMA_MEM_ADDR, &addr)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto ioctl_failed;
	}

	*in = addr.in_addr;
	*out = addr.out_addr;

ioctl_failed:
	close(fd_vdma);
open_xvdma_failed:
	return ret;
}

int alloc_image_buffer() {
	unsigned int in_buffer = 0;
	unsigned int out_buffer = 0;

	int uio_fd, addr_fd, size_fd;
	int uio_size;
	void *uio_addr;
	char uio_addr_buf[16], uio_size_buf[16];
	int ret = 0;

	ret = get_vmda_buffer_addr_direct(&in_buffer, &out_buffer);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}

	image_buffer_info.phy_in_buffer = in_buffer;
	image_buffer_info.phy_out_buffer = out_buffer;

	uio_fd = open(UIO_DEV, /*O_RDONLY*/O_RDWR);
	if(uio_fd < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto uio_fd_error;
	}
	addr_fd = open(UIO_ADDR, O_RDONLY);
	if(addr_fd < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto addr_fd_error;
	}
	size_fd = open(UIO_SIZE, O_RDONLY);
	if(size_fd < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto size_fd_error;
	}

	read(addr_fd, uio_addr_buf, sizeof(uio_addr_buf));
	read(size_fd, uio_size_buf, sizeof(uio_size_buf));
	uio_addr = (void*)strtoul(uio_addr_buf, NULL, 0);
	uio_size = (int)strtol(uio_size_buf, NULL, 0);

	image_buffer_info.size = uio_size;
	image_buffer_info.buffer = (char *)mmap(NULL, uio_size, PROT_READ | PROT_WRITE, MAP_SHARED, uio_fd, (__off_t)uio_addr);
	if ((int)image_buffer_info.buffer == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto mmap_image_buffer_error;
	}

	printf("The image_buffer address %p (lenth %x(%d)) can be accessed over logical address %p\n", uio_addr, uio_size, uio_size, image_buffer_info.buffer);


mmap_image_buffer_error:
	close(size_fd);
size_fd_error:
	close(addr_fd);
addr_fd_error:
	close(uio_fd);
uio_fd_error:
	return ret;
}

int init_dma_direct(VDMA_info *info, unsigned int baseAddr, int width, int height, int pixelLength, char *fb1Addr, char *fb2Addr) {
	int ret = 0;

	info->baseAddr=baseAddr;//?
	info->width=width;
	info->height=height;
	info->pixelLength=pixelLength;
	info->fbLength=pixelLength*width*height;
	info->fb1VirtualAddress = (unsigned int *)fb1Addr;
	info->fb2VirtualAddress = (unsigned int *)fb2Addr;

	ret = open("/dev/mem", O_RDWR);
	if(info->vdmaHandler < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto open_mem_failed;
	}
	info->vdmaHandler = ret;

	ret = (int)mmap(NULL, VDMAMapLen, PROT_READ | PROT_WRITE, MAP_SHARED, info->vdmaHandler, (off_t)info->baseAddr);
	if(ret < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto mmap_failed;
	}
	info->vdmaVirtualAddress = (unsigned int *)ret;

mmap_failed:
	ret = close(info->vdmaHandler);
open_mem_failed:
	return ret;
}

int uninit_dma_direct(VDMA_info *info) {
	int ret;

	ret = munmap((void *)info->vdmaVirtualAddress, VDMAMapLen);
	if(ret < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	}

	return ret;
}


bool is_timeout = false;
void default_handle(int signo) 
{ 
	is_timeout = true;
} 

int wait_for_vdma_timeout(VDMA_info *info) {
	int ret = 0;
	struct sigaction act; 
	struct itimerval value; 

	printf("Waiting for end...\n");

	act.sa_handler= default_handle; 
	act.sa_flags=0; 
	ret = sigemptyset(&act.sa_mask); 
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto sig_handle_regist_failed;
	}

	ret = sigaction(SIGPROF,&act,NULL); //设置信号 SIGPROF 的处理函数为 default_handle
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto sigaction_regist_failed;
	}

	value.it_value.tv_sec=3; 
	value.it_value.tv_usec=0; 
	value.it_interval=value.it_value; 
	ret = setitimer(ITIMER_PROF,&value,NULL); //初始化 timer，到期发送 SIGPROF 信号
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto setitimer_failed;
	}

	while(VDMA_IsDone(info) == 0) {
		if(is_timeout) {
			printf("time out!\n");
			ret = -1;
			break;
		}
	}

setitimer_failed:
sigaction_regist_failed:
sig_handle_regist_failed:
	return ret;
}

int dma_direct(image_buffer_info_t image_buffer_info) {
	int ret = 0;
	VDMA_info info;
	unsigned int in;
	unsigned int out;

	ret = init_dma_direct(&info, VDMABaseAddr, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_PIXSIZE, image_buffer_info.buffer, image_buffer_info.buffer + IMAGE_SIZE);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		goto init_dma_failed;
	}

	ret = get_vmda_buffer_addr_direct(&in, &out);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		goto get_vdma_buffer_addr_failed;
	}
	VDMA_Start(&info, in);
	VDMA_OutStart(&info, out, 0);

	ret = wait_for_vdma_timeout(&info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

get_vdma_buffer_addr_failed:
	ret = uninit_dma_direct(&info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}
init_dma_failed:
	return ret;
}

int sobel_filter_direct() {
	int ret = 0;

	XImage_filter xfilter;
	XImage_filter_Initialize(&xfilter, NULL);

	XImage_filter_SetRows(&xfilter, IMAGE_HEIGHT);
	XImage_filter_SetCols(&xfilter, IMAGE_WIDTH);

	XImage_filter_InterruptGlobalEnable(&xfilter);
	XImage_filter_InterruptEnable(&xfilter, 3);
	XImage_filter_EnableAutoRestart(&xfilter);
	XImage_filter_Start(&xfilter);

	dma_direct(image_buffer_info);

	XImage_filter_Release(&xfilter);

	dumpMemory();

	return ret;
}

int hw_sobel_process() {
	int ret = 0;
	ret = alloc_image_buffer();
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}
	//resetStop_DMA_ALL();
	ret = sobel_filter_direct();
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}

	free_image_buffer();
	return ret;
}

int main(int argc, char **argv) {
	hw_sobel_process();
	return 0;
}

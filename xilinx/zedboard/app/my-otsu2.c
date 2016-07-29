#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>

#include "gpio_utils.h"
#include "xotsu2/xotsu2.h"
#include "my-otsu2.h"
#include "vdma/VDMA.h"

#define XOTSU2_REG_ADDR_BASE	0x43c00000
#define XOTSU2_REG_ADDR_RANGE	0x10000
#define VDMA_REG_ADDR_BASE	0x43000000
#define VDMA_REG_ADDR_RANGE	0x10000

static image_buffer_info_t image_buffer_info = {0,0,0,0};

void gpio_reset(int level)
{
	// reset SOBEL
	gpio_export(SOBEL_RST_PIN);
	gpio_dir_out(SOBEL_RST_PIN);
	gpio_value(SOBEL_RST_PIN, level);
	gpio_unexport(SOBEL_RST_PIN);

	//printf("\nReset Done\n");
}

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

void initMemory() {
	int i;

	for(i = 0 ; i < IMAGE_SIZE ; i++)
	{
		//image_buffer_info.buffer[i] = (unsigned char)i;
		image_buffer_info.buffer[i] = (unsigned char)150;
	}

	for(i = IMAGE_SIZE ; i < BUFFER_SIZE ; i++)
	{
		image_buffer_info.buffer[i] = 0;
	}
}

int get_kbuffer_for_image(xotsu2_buffer_addr_t *paddr) {
	int ret = 0;
	int fd_xotsu2;

	if ((ret = open("/dev/xotsu2", O_RDWR)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto open_xotsu2_failed;
	}
	fd_xotsu2 = ret;

	if((ret = ioctl(fd_xotsu2, XOTSU_GET_DMA_MEM_ADDR, paddr)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto ioctl_failed;
	}

ioctl_failed:
	close(fd_xotsu2);
open_xotsu2_failed:
	return ret;
}

int alloc_image_buffer() {
	int ret = 0;
#ifdef KERNEL_DIGILENT
	int uio_fd, addr_fd, size_fd;
	void *uio_addr;
	int uio_size;
	char uio_addr_buf[16], uio_size_buf[16];
#else
	int kmem_fd;
#endif
	xotsu2_buffer_addr_t buffer_addr;

	ret = get_kbuffer_for_image(&buffer_addr);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		goto get_kbuffer_for_image_failed;
	}

	image_buffer_info.phy_in_buffer = buffer_addr.phy_in_addr;
	image_buffer_info.phy_out_buffer = buffer_addr.phy_out_addr;
#ifdef KERNEL_DIGILENT
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
	image_buffer_info.buffer = (char *)mmap(NULL, image_buffer_info.size, PROT_READ | PROT_WRITE, MAP_SHARED, uio_fd, (__off_t)uio_addr);
#else
	kmem_fd = open(KMEM_DEV, /*O_RDONLY*/O_RDWR);
	if(kmem_fd < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto open_kmem_error;
	}

	printf("xiaofei: %s:%d image_buffer_info.size:%x buffer_addr.k_out_addr:%x\n", __func__, __LINE__,
		image_buffer_info.size,
		buffer_addr.k_out_addr
		);
	image_buffer_info.size = BUFFER_SIZE;
	image_buffer_info.buffer = (char *)mmap(NULL, image_buffer_info.size, PROT_READ | PROT_WRITE, MAP_SHARED, kmem_fd, (__off_t)buffer_addr.k_out_addr);
#endif
	if ((int)image_buffer_info.buffer == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto mmap_image_buffer_error;
	}

	printf("The image_buffer physical address %p"
		" (lenth %x)"
		"can be accessed over logical address %p\n", 
		buffer_addr.phy_out_addr,
		image_buffer_info.size, 
		image_buffer_info.buffer);

mmap_image_buffer_error:
#ifdef KERNEL_DIGILENT
	close(size_fd);
size_fd_error:
	close(addr_fd);
addr_fd_error:
	close(uio_fd);
uio_fd_error:
#else
	close(kmem_fd);
open_kmem_error:
#endif
get_kbuffer_for_image_failed:
	return ret;
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

int init_vdma_direct(VDMA_info *info) {
	int ret = 0;

	info->baseAddr = VDMA_REG_ADDR_BASE;
	info->width = IMAGE_WIDTH;
	info->height = IMAGE_HEIGHT;
	info->pixelLength = IMAGE_PIXSIZE;
	info->fbLength = IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_PIXSIZE;
	info->fb1VirtualAddress = (unsigned int *)image_buffer_info.buffer;
	info->fb2VirtualAddress = (unsigned int *)(image_buffer_info.buffer + IMAGE_SIZE);

	info->vdmaHandler = open("/dev/mem", O_RDWR);
	if(info->vdmaHandler  < 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
		return ret;
	}

	info->vdmaVirtualAddress = (unsigned int *)mmap(NULL, VDMA_REG_ADDR_RANGE, PROT_READ | PROT_WRITE, MAP_SHARED, info->vdmaHandler, (off_t)info->baseAddr);
	if(info->vdmaVirtualAddress == (void *)-1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
	}

	close(info->vdmaHandler);
	info->vdmaHandler = -1;
	return ret;
}

int uninit_vdma_direct(VDMA_info *info) {
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

int wait_for_vdma_timeout(XOtsu2 *xotsu2) {
	int ret = 0;
	struct sigaction act; 
	struct itimerval value; 

	printf("Waiting for end...\n");

	act.sa_handler= default_handle; 
	act.sa_flags=0; 
	ret = sigemptyset(&act.sa_mask); 
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		return ret;
	}

	ret = sigaction(SIGPROF,&act,NULL); //设置信号 SIGPROF 的处理函数为 default_handle
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		return ret;
	}

	value.it_value.tv_sec=3; 
	value.it_value.tv_usec=0; 
	value.it_interval=value.it_value; 
	ret = setitimer(ITIMER_PROF,&value,NULL); //初始化 timer，到期发送 SIGPROF 信号
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		return ret;
	}

	//while(VDMA_IsDone(info) == 0) {
	while(XOtsu2_IsDone(xotsu2) == 1) {
		if(is_timeout) {
			printf("time out!\n");
			ret = -1;
			break;
		}
	}

	printf("result is %d\n", (int)XOtsu2_GetReturn(xotsu2));

	return ret;
}

int wait_for_xotsu2_intr(xotsu2_result_t *result) {
	int ret = 0;
	int fd_xotsu2;

	if ((fd_xotsu2 = open("/dev/xotsu2", O_RDWR)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
		goto open_xotsu2_failed;
	}

	if((ret = ioctl(fd_xotsu2, XOTSU_WAIT_FOR_RESULT, result)) < 0)
	{
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		goto ioctl_failed;
	}

	if(result->is_timeout) {
		printf("xiaofei: %s:%d timeout!\n", __func__, __LINE__);
		ret = -1;
	} else {
		printf("result is %d\n", result->value);
	}

ioctl_failed:
	close(fd_xotsu2);
open_xotsu2_failed:
	return ret;
}

int start_dma_direct(XOtsu2 *xotsu2, VDMA_info *info) {
	int ret = 0;
	xotsu2_result_t result;

	//VDMA_Start(info, image_buffer_info.phy_in_buffer);
	VDMA_OutStart(info, image_buffer_info.phy_out_buffer, 0);
	//ret = wait_for_vdma_timeout(xotsu2);
	ret = wait_for_xotsu2_intr(&result);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	return ret;
}

int uninitilize_device(XOtsu2_Config *xotsu2_config) {
	int ret = 0;
	ret = munmap((void*)xotsu2_config->Control_bus_BaseAddress, XOTSU2_REG_ADDR_RANGE);
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	}

	return ret;
}

int my_XOtsu2_Initialize(XOtsu2_Config *xotsu2_config, XOtsu2 *xotsu2) {
    Xil_AssertNonvoid(xotsu2 != NULL);
    Xil_AssertNonvoid(xotsu2_config != NULL);

    xotsu2->Control_bus_BaseAddress = xotsu2_config->Control_bus_BaseAddress;
    xotsu2->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}

int start_hw_process(XOtsu2_Config *xotsu2_config, XOtsu2 *xotsu2) {
	int ret = 0;

	my_XOtsu2_Initialize(xotsu2_config, xotsu2);

	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XOtsu2_SetRows(xotsu2, IMAGE_HEIGHT);
	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XOtsu2_SetCols(xotsu2, IMAGE_WIDTH);

	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XOtsu2_InterruptGlobalEnable(xotsu2);
	XOtsu2_InterruptEnable(xotsu2, 3);
	XOtsu2_EnableAutoRestart(xotsu2);
	XOtsu2_Start(xotsu2);
	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));

	return ret;
}

int hw_process_direct(XOtsu2_Config *xotsu2_config, XOtsu2 *xotsu2, VDMA_info *info) {
	int ret = 0;

	initMemory();

	ret = start_hw_process(xotsu2_config, xotsu2);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}

	ret = start_dma_direct(xotsu2, info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}


	//dumpMemory();

	return ret;
}

int initilize_device(XOtsu2_Config *xotsu2_config) {
	int ret = 0;
	int fd = -1;

	fd = open("/dev/mem", O_RDWR);
	if(fd == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
		return ret;
	}

	xotsu2_config->Control_bus_BaseAddress = (unsigned int)mmap(NULL, XOTSU2_REG_ADDR_RANGE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)XOTSU2_REG_ADDR_BASE);
	if(xotsu2_config->Control_bus_BaseAddress == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
	}
	//printf("xiaofei: %s:%d (off_t)XOTSU2_REG_ADDR_BASE:%x\n", __func__, __LINE__, (off_t)XOTSU2_REG_ADDR_BASE);
	//printf("xiaofei: %s:%d xotsu2_config->Control_bus_BaseAddress:%x\n", __func__, __LINE__, xotsu2_config->Control_bus_BaseAddress);

	close(fd);

	return ret;
}

int init_hw_porcess(XOtsu2_Config *xotsu2_config, VDMA_info *info) {
	int ret = 0;

	ret = alloc_image_buffer();
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		goto prepare_image_failed;
	}

	ret = init_vdma_direct(info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		goto init_vdma_failed;
	}

	ret = initilize_device(xotsu2_config);
	if(ret != 0) {
		goto init_hw_failed;
	}
	return ret;

init_hw_failed:
	ret = uninit_vdma_direct(info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}
init_vdma_failed:
	free_image_buffer();
prepare_image_failed:
	return ret;
}

int uninit_hw_process(XOtsu2_Config *xotsu2_config, VDMA_info *info) {
	int ret = 0;
	ret = uninitilize_device(xotsu2_config);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	ret = uninit_vdma_direct(info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	free_image_buffer();

	return ret;
}

int hw_porcess() {
	int ret = 0;
	XOtsu2_Config xotsu2_config;
	XOtsu2 xotsu2;
	VDMA_info info;

	ret = init_hw_porcess(&xotsu2_config, &info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}

	ret = hw_process_direct(&xotsu2_config, &xotsu2, &info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	ret = uninit_hw_process(&xotsu2_config, &info);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	return ret;
}

int main(int argc, char **argv) {
	int ret = 0;

	ret = hw_porcess();
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}
	return ret;
}

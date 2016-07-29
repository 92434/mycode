#include <stdio.h>
#include <errno.h>
//#include <sys/ioctl.h>
//#include <signal.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/mman.h>

#include "image-filter.h"

#define XFILTER_REG_ADDR_BASE	0x43c00000
#define XFILTER_REG_ADDR_RANGE	0x10000

int uninitilize_device(XImagefilter_Config *xfilter_config) {
	int ret = 0;
	ret = munmap((void*)xfilter_config->Control_bus_BaseAddress, XFILTER_REG_ADDR_RANGE);
	if(ret != 0) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	}

	return ret;
}

int my_XImage_filter_Initialize(XImagefilter_Config *xfilter_config, XImage_filter *xfilter) {
    Xil_AssertNonvoid(xfilter != NULL);
    Xil_AssertNonvoid(xfilter_config != NULL);

    xfilter->Control_bus_BaseAddress = xfilter_config->Control_bus_BaseAddress;
    xfilter->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}

int start_hw_process(XImagefilter_Config *xfilter_config, XImage_filter *xfilter) {
	int ret = 0;

	my_XImage_filter_Initialize(xfilter_config, xfilter);

	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XImage_filter_SetRows(xfilter, IMAGE_HEIGHT);
	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XImage_filter_SetCols(xfilter, IMAGE_WIDTH);

	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
	XImage_filter_InterruptGlobalEnable(xfilter);
	XImage_filter_InterruptEnable(xfilter, 3);
	XImage_filter_EnableAutoRestart(xfilter);
	XImage_filter_Start(xfilter);
	printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));

	return ret;
}

int hw_process_direct(XImagefilter_Config *xfilter_config, XImage_filter *xfilter) {
	int ret = 0;

	ret = start_hw_process(xfilter_config, xfilter);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	return ret;
}

int initilize_device(XImagefilter_Config *xfilter_config) {
	int ret = 0;
	int fd = -1;

	fd = open("/dev/mem", O_RDWR);
	if(fd == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
		return ret;
	}

	xfilter_config->Control_bus_BaseAddress = (unsigned int)mmap(NULL, XFILTER_REG_ADDR_RANGE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)XFILTER_REG_ADDR_BASE);
	if(xfilter_config->Control_bus_BaseAddress == -1) {
		printf("xiaofei: %s:%d %s\n", __func__, __LINE__, strerror(errno));
		ret = -1;
	}
	//printf("xiaofei: %s:%d (off_t)XFILTER_REG_ADDR_BASE:%x\n", __func__, __LINE__, (off_t)XFILTER_REG_ADDR_BASE);
	//printf("xiaofei: %s:%d xfilter_config->Control_bus_BaseAddress:%x\n", __func__, __LINE__, xfilter_config->Control_bus_BaseAddress);

	close(fd);

	return ret;
}

int init_hw_porcess(XImagefilter_Config *xfilter_config) {
	int ret = 0;

	ret = initilize_device(xfilter_config);
	return ret;
}

int uninit_hw_process(XImagefilter_Config *xfilter_config) {
	int ret = 0;
	ret = uninitilize_device(xfilter_config);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	return ret;
}

int hw_porcess() {
	int ret = 0;
	XImagefilter_Config xfilter_config;
	XImage_filter xfilter;

	ret = init_hw_porcess(&xfilter_config);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
		return ret;
	}

	ret = hw_process_direct(&xfilter_config, &xfilter);
	if(ret != 0) {
		printf("xiaofei: %s:%d\n", __func__, __LINE__);
	}

	ret = uninit_hw_process(&xfilter_config);
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

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>

#include "glyphy.h"
#include "glyphy_test.h"

static int init_one_glyphy_dir_row(char *buffer, struct content_info *pcontent_info, int x, int y, unsigned char value) {
	int ret = 0;
	int h;
	struct glyphy_info glyphy_info = pcontent_info->glyphy_info;
	int glyphy_width_by_byte = (glyphy_info.pix_width + (BIT_PER_BYTE - 1)) / BIT_PER_BYTE;
	int content_step_by_byte = glyphy_width_by_byte * pcontent_info->width;

	for(h = 0; h < glyphy_info.pix_height; h++) {
		int w;
		char *start = buffer;

		start += content_step_by_byte * glyphy_info.pix_height * y;
		start += content_step_by_byte * h;
		start += glyphy_width_by_byte * x;

		for(w = 0; w < glyphy_width_by_byte; w++) {
			//printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
			//printf("xiaofei: start + w:%p value:%08x\n", start + w, value);
			start[w] = value;
		}
	}

	return ret;
}

static int init_glyphy(char *buffer, struct content_info *pcontent_info, int x, int y, unsigned char value, enum out_direction dir) {
	int ret = 0;

	if(dir == DIR_ROW) {
		init_one_glyphy_dir_row(buffer, pcontent_info, x, y, value);
	} else if(dir == DIR_COL) {
	} else {
	}

	return ret;
}



int main(int argc, char **argv) {
	int ret = 0;

	int fd;
	char *buffer;
	int buffer_size;
	struct content_info content_info;
	int glyphy_width_by_byte;
	int x;
	int y;

	content_info.width = 2;
	content_info.height = 2;
	content_info.glyphy_info.pix_width = FPGA_PROCESS_WIDTH;
	content_info.glyphy_info.pix_height = FPGA_PROCESS_HEIGHT;
	content_info.dir = DIR_ROW;

	if ((ret = open(DEV_FILE, O_RDWR)) < 0)
	{
		printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printf("strerror(errno):%s\n", strerror(errno));
		ret = -1;
		goto dev_open_failed;
	}

	fd = ret;

	glyphy_width_by_byte = (content_info.glyphy_info.pix_width + (BIT_PER_BYTE - 1)) / BIT_PER_BYTE;

	buffer_size = glyphy_width_by_byte * content_info.glyphy_info.pix_height *  content_info.width * content_info.height;
	buffer = (char *)mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(buffer == (void *)-1) {
		printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printf("strerror(errno):%s\n", strerror(errno));
		ret = -1;
		goto dev_mmap_failed;
	}

	for(y = 0; y < content_info.height; y++) {
		for(x = 0; x < content_info.width; x++) {
			unsigned char value = x | (y << 4);

			init_glyphy(buffer, &content_info, x, y, value, DIR_ROW);
		}
	}

	if((ret = ioctl(fd, GLYPHY_IOCTL_SET_CONTENT_INFO, &content_info)) < 0)
	{
		printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printf("strerror(errno):%s\n", strerror(errno));
		goto ioctl_failed;
	}

	if((ret = ioctl(fd, GLYPHY_IOCTL_CONTENT_OUT_START, 0)) < 0)
	{
		printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printf("strerror(errno):%s\n", strerror(errno));
		goto ioctl_failed;
	}


ioctl_failed:
	if((ret = ioctl(fd, GLYPHY_IOCTL_CONTENT_OUT_START, 0)) < 0)
	{
		printf("xiaofei:%s:%s:%d\n", __FILE__, __func__, __LINE__);
		printf("strerror(errno):%s\n", strerror(errno));
		goto ioctl_failed;
	}
dev_mmap_failed:
	close(fd);
dev_open_failed:
	return ret;
}

#ifndef __GLYPHY_H
#define DRIVER_NAME "glyphy"
#define GLYPHY_IOCTL_BASE 'W'
#define GLYPHY_IOCTL_SET_CONTENT_INFO _IO(GLYPHY_IOCTL_BASE, 0)
#define GLYPHY_IOCTL_CONTENT_OUT_START _IO(GLYPHY_IOCTL_BASE, 1)

#define FPGA_PROCESS_WIDTH 8
#define FPGA_PROCESS_HEIGHT 8
#define BIT_PER_BYTE 8
#define FPGA_PROCESS_WIDTH_BY_BYTE ((FPGA_PROCESS_WIDTH + BIT_PER_BYTE - 1) / BIT_PER_BYTE)

struct glyphy_info {
	int pix_width;
	int pix_height;
};

enum out_direction {
	DIR_ROW = 0,
	DIR_COL,
	DIR_NONE,
};

struct content_info {
	int width;
	int height;
	struct glyphy_info glyphy_info;
	enum out_direction dir;
};

typedef struct _glyphy_result {
	bool is_timeout;
} glyphy_result_t;

#endif//#ifndef __GLYPHY_H

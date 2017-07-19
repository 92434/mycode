

/*================================================================
 *   Copyright (C) 2017年07月19日 肖飞 All rights reserved
 *   
 *   文件名称：hardware_base.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月19日 星期三 09时09分53秒
 *   修改日期：2017年07月19日 星期三 09时14分23秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_BASE_H
#define _HARDWARE_BASE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "FpSensorLib.h"

#ifdef __cplusplus
}
#endif

#include <fstream>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "filesystem.h"
#include "settings.h"
#pragma pack(1)
struct BMP_FILEHDR {                     // BMP file header
	char   bfType[2];                   // "BM"
	int  bfSize;                      // size of file
	short  bfReserved1;
	short  bfReserved2;
	int  bfOffBits;                   // pointer to the pixmap bits
};
#pragma pack()

struct BMP_INFOHDR {                     // BMP information header
	int  biSize;                      // size of this struct
	int  biWidth;                     // pixmap width
	int  biHeight;                    // pixmap height
	short  biPlanes;                    // should be 1
	short  biBitCount;                  // number of bits per pixel
	int  biCompression;               // compression method
	int  biSizeImage;                 // size of image
	int  biXPelsPerMeter;             // horizontal resolution
	int  biYPelsPerMeter;             // vertical resolution
	int  biClrUsed;                   // number of colors used
	int  biClrImportant;              // number of important colors
};

struct BGR_PALETTE {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char unused;
};

class hardware_base
{
protected:

	int width;
	int height;

	static BMP_FILEHDR bmp_file_header;
	static BMP_INFOHDR bmp_info_header;
	static BGR_PALETTE bmp_palette[256];

	std::string logfile_hardware;
	static std::ofstream ofs_hardware;

	static std::string cur_bmp_path;

	hardware_base();

	hardware_base(int width, int height);

	~hardware_base();

public:

	int hardware_init();

	static int save_bmp(char *label, char *buffer, int len);

	int set_log_file_name(std::string filename);

	int log_file_start();

	static void hardware_log(const char *fmt, ...);

	int log_file_end();

	int set_save_bmp();

	int set_image(std::string bmp_path);

	int get_image(char *buffer, int len);

	static void hw_usleep(__ft_u32 usec);

	static __ft_s32 get_mcu_status(void);

	static __ft_u64 get_system_time(void);

	int write_flash(int offset, char *buffer, int len);

	int read_flash(int offset, char *buffer, int len);

	int enroll(unsigned short finger_id, unsigned char enroll_index, unsigned char *penroll_coverage);

	int idnetify(unsigned char *finger_id, unsigned char *update_template);

	int update_template(int update, int *finger_id);

	int delete_template(char fingerId);
};
#endif //HARDWARE_BASE_H

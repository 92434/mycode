

/*================================================================
 *   Copyright (C) 2017年07月20日 肖飞 All rights reserved
 *
 *   文件名称：hardware.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月20日 星期四 17时55分24秒
 *   修改日期：2017年07月25日 星期二 10时26分28秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_H
#define _HARDWARE_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <fstream>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "filesystem.h"
#include "settings.h"
#include "hw.h"

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

class hardware : public hardware_base
{
private:
	static hardware *hw;
	static BMP_FILEHDR bmp_file_header;
	static BMP_INFOHDR bmp_info_header;
	static BGR_PALETTE bmp_palette[256];

	std::string logfile_hardware;
	static std::ofstream ofs_hardware;

	static std::string cur_bmp_path;


	hardware();

	~hardware();

public:
	static hardware *get_instance();

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

	int save_one_template(unsigned short finger_id);

	int enroll(unsigned short finger_id, unsigned char enroll_index, unsigned char *penroll_coverage);

	int identify(unsigned char *finger_id, unsigned char *update, unsigned char *update_outside, int *update_template_finger_id);

	int update_template(int update, int *finger_id);

	int delete_template(char fingerId);

	int clear_all_template();
};
#endif //HARDWARE_H

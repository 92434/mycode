

/*================================================================
 *   Copyright (C) 2017年07月20日 肖飞 All rights reserved
 *
 *   文件名称：hardware.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月20日 星期四 17时55分24秒
 *   修改日期：2017年08月14日 星期一 18时25分12秒
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

class bmp_cache_item
{
public:
	std::string bmp_path;
	BMP_FILEHDR bmp_file_header;
	BMP_INFOHDR bmp_info_header;
	BGR_PALETTE bmp_palette[256];
	char *buffer;
	int len;

	bmp_cache_item();

	~bmp_cache_item();

	int alloc_bmp_buffer(int len);
};

class hardware : public hardware_base
{
private:
	static hardware *hw;

	std::string logfile_hardware;
	static std::ofstream ofs_hardware;

	static bmp_cache_item *cache_item;
	std::map<std::string, bmp_cache_item *> bmp_cache;

	int clear_cache();

	hardware();

	~hardware();
public:
	static hardware *get_instance();

	static void release_instance();

	int hardware_init();

	static int save_bmp(char *label, const char *buffer, int len);

	int image_v_mirror(char *buffer, int width, int height, int bytes_per_pixel);
	int image_h_mirror(char *buffer, int width, int height, int bytes_per_pixel);

	int get_image_info(std::string bmp_path);

	int set_log_file_name(std::string filename);

	int log_file_start();

	static void hardware_log(const char *fmt, ...);

	int log_file_end();

	int set_save_bmp();

	int set_image(std::string bmp_path);

	static void hw_usleep(__ft_u32 usec);

	static __ft_s32 get_mcu_status(void);

	static __ft_u64 get_system_time(void);

	int save_one_template(unsigned short finger_id);

	int enroll(unsigned short finger_id, unsigned char enroll_index, unsigned char *penroll_coverage);

	int identify(unsigned char *finger_id, unsigned char *update, unsigned char *update_outside, int *update_template_finger_id);

	int update_template(int update, int *finger_id);

	int delete_template(char fingerId);
};
#endif //HARDWARE_H

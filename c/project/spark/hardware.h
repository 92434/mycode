

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *   
 *   文件名称：hardware.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时26分15秒
 *   修改日期：2017年07月17日 星期一 14时09分17秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_H
#define _HARDWARE_H
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
#include <sys/time.h>

#include "filesystem.h"

class hardware_base
{
protected:

	int width;
	int height;

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

	static __ft_u64 get_system_time(void);

	int write_flash(int offset, char *buffer, int len);

	int read_flash(int offset, char *buffer, int len);

};
#endif //HARDWARE_H

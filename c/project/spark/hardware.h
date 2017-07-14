

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：hardware.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时26分40秒
 *   修改日期：2017年07月14日 星期五 18时18分41秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_H
#define _HARDWARE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "ft_lib.h"

#ifdef __cplusplus
}
#endif

#include <string>

class hardware
{
private:
	int width;
	int height;
	static hardware *hw;

	hardware();

	hardware(int width, int height);

	~hardware();

public:
	static hardware *get_instance(int width, int height);

	int hardware_init();

	int set_log(ft_printf_t ft_printf);

	int set_save_bmp(save_bmp_t save_bmp);

	int set_image(std::string bmp_path);

	int get_image(char *buffer, int len);

	int write_flash(int offset, char *buffer, int len);
	int read_flash(int offset, char *buffer, int len);
};

#define HW_GET_INSTANCE() hardware::get_instance(96, 96)
#endif //HARDWARE_H

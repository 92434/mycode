

/*================================================================
 *   Copyright (C) 2017年07月20日 肖飞 All rights reserved
 *   
 *   文件名称：hardware_9348.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月20日 星期四 17时52分18秒
 *   修改日期：2017年07月20日 星期四 18时18分26秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_9348_H
#define _HARDWARE_9348_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "FpSensorLib.h"

#ifdef __cplusplus
}

#include <stdio.h>

#define FT_TPL_MAX_SIZE FOCAL_PER_TEMPLATE_SIZE

typedef struct fp_alg_tpl {
	unsigned char tpl_type;
	unsigned short tpl_id;
	unsigned int tpl_size;
	unsigned char tpl_data[FT_TPL_MAX_SIZE];
} fp_alg_tpl_t;

class hardware_base
{
protected:

	int width;
	int height;

	hardware_base();

	~hardware_base();

public:

	static __ft_s32 spi_write(__ft_u8 *buffer, __ft_u32 len);

	static __ft_s32 spi_read(__ft_u8 *buffer_out, __ft_u8 *buffer_in, __ft_u32 len);

	int write_flash(int offset, char *buffer, int len);

	int read_flash(int offset, char *buffer, int len);

};
#endif
#endif //HARDWARE_9348_H

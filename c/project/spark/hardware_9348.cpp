

/*================================================================
 *   Copyright (C) 2017年07月20日 肖飞 All rights reserved
 *   
 *   文件名称：hardware_9348.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月20日 星期四 17时54分32秒
 *   修改日期：2017年07月22日 星期六 09时53分50秒
 *   描    述：
 *
 *================================================================*/
#include "hardware_9348.h"

hardware_base::hardware_base()
{
	width = 96;
	height = 96;
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware_base::~hardware_base()
{
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}


__ft_s32 hardware_base::spi_write(__ft_u8 *buffer, __ft_u32 len)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 hardware_base::spi_read(__ft_u8 *buffer_out, __ft_u8 *buffer_in, __ft_u32 len)
{
	__ft_s32 ret = 0;
	return ret;
}

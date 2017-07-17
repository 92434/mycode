

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *
 *   文件名称：hardware_9348.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时28分58秒
 *   修改日期：2017年07月17日 星期一 14时25分28秒
 *   描    述：
 *
 *================================================================*/
#include "hardware_9348.h"

hardware *hardware::hw = NULL;

hardware::hardware()
{
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware::hardware(int width, int height) : hardware_base(width, height)
{
	ft_lib_init();

	//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, this->width, this->height);
}

hardware::~hardware()
{
	ft_lib_uninit();
	//printf("%s:%s:%d:width\n", __FILE__, __func__, __LINE__);
}

hardware *hardware::get_instance()
{
	if(hw == NULL) {
		hw = new hardware(96, 96);
	}

	//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, width, height);

	return hw;
}

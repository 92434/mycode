

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *
 *   文件名称：hardware_9348.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时28分07秒
 *   修改日期：2017年07月17日 星期一 13时17分31秒
 *   描    述：
 *
 *================================================================*/
#ifndef _HARDWARE_9348_H
#define _HARDWARE_9348_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include "hardware.h"

class hardware : public hardware_base
{
private:
	static hardware *hw;

	hardware();

	hardware(int width, int height);

	~hardware();

public:
	static hardware *get_instance();

};
#endif //HARDWARE_9348_H

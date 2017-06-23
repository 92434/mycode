

/*================================================================
 *   Copyright (C) 2017年06月23日 肖飞 All rights reserved
 *
 *   文件名称：task_info.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月23日 星期五 12时59分07秒
 *   修改日期：2017年06月23日 星期五 13时28分36秒
 *   描    述：
 *
 *================================================================*/
#include "task_info.h"

task_info::task_info()
{
	clear_enroll_map();
	clear_identify_for_fr_map();
	clear_identify_for_fa_map();
}

task_info::~task_info()
{
	clear_enroll_map();
	clear_identify_for_fr_map();
	clear_identify_for_fa_map();
}

void task_info::clear_enroll_map()
{
	enroll_map.clear();
}

void task_info::clear_identify_for_fr_map()
{
	identify_for_fr_map.clear();
}

void task_info::clear_identify_for_fa_map()
{
	identify_for_fa_map.clear();
}

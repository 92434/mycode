

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *   
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时52分09秒
 *   修改日期：2017年07月14日 星期五 13时35分51秒
 *   描    述：
 *
 *================================================================*/
#include "settings.h"
#include "samples_list.h"

int main(int argc, char **argv)
{
	int ret = 0;

	settings *g_settings = settings::get_instance();
	ret = g_settings->parse_args_from_configuration(argc, argv);

	if(ret != 0) {
		return ret;
	}

	samples_list samples_list;
	ret = samples_list.update_samples_list();

	if(ret != 0) {
		return ret;
	}

	//samples_list.p_result();
	samples_list.start_test_task();

	printf("Done!\n");

	return ret;
}



/*================================================================
 *   Copyright (C) 2017年06月28日 肖飞 All rights reserved
 *
 *   文件名称：test_config.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月28日 星期三 08时54分44秒
 *   修改日期：2017年06月28日 星期三 14时46分35秒
 *   描    述：
 *
 *================================================================*/
//#include "test_config.h"
#include "configure/configure.h"

int main(int argc, char **argv)
{
	int ret = 0;
	std::string filename;
	std::string save_filename;
	std::string value;

	if(argc < 3) {
		filename = "test_config/test_config.ini";
		save_filename = "new.ini";
	} else {
		filename = argv[1];
		save_filename = argv[2];
	}


	configure cfg;
	cfg.p_configure();
	cfg.load(filename);
	cfg.p_configure();
	value = cfg.get("key1", "xiaofei5");
	if(value.size()) {
		printf("get class:%s, key:%s, value:%s\n", "key1", "xiaofei5", value.c_str());
	} else {
		printf("no class:%s, key:%s!!!\n", "key1", "xiaofei5");
	}
	value = cfg.get("key2", "xiaofei3");
	if(value.size()) {
		printf("get class:%s, key:%s, value:%s\n", "key2", "xiaofei3", value.c_str());
	} else {
		printf("no class:%s, key:%s!!!\n", "key2", "xiaofei3");
	}
	value = cfg.get("key1", "xiaofei3");
	if(value.size()) {
		printf("get class:%s, key:%s, value:%s\n", "key1", "xiaofei3", value.c_str());
	} else {
		printf("no class:%s, key:%s!!!\n", "key1", "xiaofei3");
	}
	cfg.save(save_filename);

	return ret;
}

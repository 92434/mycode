

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：regexp.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 16时06分14秒
 *   修改日期：2017年06月26日 星期一 17时49分01秒
 *   描    述：
 *
 *================================================================*/
#include "regexp/regexp.h"

int main(int argc, char **argv)
{
	int ret = 0;

	if(argc < 3) {
		printf("argc:%d, not enough args!!!\n", argc);
		ret = -1;
		return ret;
	}

	regexp r;
	std::string content = argv[1];
	std::string pattern = argv[2];

	printf("content:%s\n", content.c_str());
	printf("pattern:\"%s\"\n", pattern.c_str());
	std::vector<std::string> matched_list = r.match(content, pattern);
	std::vector<std::string>::iterator it;

	for(it = matched_list.begin(); it != matched_list.end(); it++) {
		printf("result:%s\n", it->c_str());
	}

	return ret;
}

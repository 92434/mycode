

/*================================================================
 *   Copyright (C) 2017年06月24日 肖飞 All rights reserved
 *
 *   文件名称：scanfiles.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月24日 星期六 09时10分21秒
 *   修改日期：2017年06月26日 星期一 17时50分40秒
 *   描    述：
 *
 *================================================================*/
//#include "scanfiles.h"
#include "filesystem/filesystem.h"

int main(int argc, char **argv)
{
	int ret = 0;

	std::vector<std::string> file_list;
	std::vector<std::string>::iterator it;
	std::string dirname;
	std::vector<std::string> filter_list;
	struct stat sb;

	if(argc < 2) {
		printf("argc:%d, not enough args!!!\n", argc);
		ret = -1;
		return ret;
	}

	dirname = argv[1];

	printf("Directory scan of %s\n", dirname.c_str());

	filter_list.push_back(".cpp");
	filter_list.push_back("Makefile");
	filesystem fs;
	file_list = fs.dir_scan_files(dirname, filter_list);

	for(std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		printf("file in list:%s\n", it->c_str());
	}

	printf("Done!\n");

	return ret;
}

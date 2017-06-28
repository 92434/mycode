

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 *   修改日期：2017年06月28日 星期三 11时23分35秒
 *   描    述：
 *
 *================================================================*/
#include "main.h"
#include "optparse/optparse.h"
#include "filesystem/filesystem.h"

int parse_args(int argc, char **argv)
{
	int ret = 0;
	command_parser *cp1;

	command_parser lcp("dryrun");
	lcp.add_long_option("", false, false, 'a', "test long_a");
	cp1 = lcp.add_sub_command("sub1");

	if(cp1 == 0) {
		ret = -1;
		return ret;
	}

	cp1->add_long_option("long_b", false, false, 'b', "test long_b");
	if(lcp.get_long_option(argc, argv) != 0) {
		lcp.p_help();

		ret = -1;
		return ret;
	}

	lcp.p_result();
	cp1->p_result();
}

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

	filter_list.push_back(".bmp");
	filesystem fs;
	file_list = fs.dir_scan_files(dirname, filter_list);

	for(std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		printf("file in list:%s\n", it->c_str());
	}

	printf("Done!\n");

	return ret;
}

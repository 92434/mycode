

/*================================================================
 *   Copyright (C) 2017年06月24日 肖飞 All rights reserved
 *
 *   文件名称：scanfiles.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月24日 星期六 09时10分21秒
 *   修改日期：2017年06月26日 星期一 09时17分47秒
 *   描    述：
 *
 *================================================================*/
//#include "scanfiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <string>
#include <errno.h>

std::vector<std::string> scan_files(std::string dirname, std::vector<std::string> &filter_list)
{
	DIR *d;
	struct dirent *entry;

	std::vector<std::string> file_list;

	d = opendir(dirname.c_str());

	if(d == NULL) {
		printf("dirname:%s:%s\n", dirname.c_str(), strerror(errno));
		return file_list;
	}

	while(entry = readdir(d)) {
		if(entry->d_type == DT_REG) {
			std::string filename = entry->d_name;

			//printf("file:%s\n", filename.c_str());

			if(!filter_list.empty()) {
				int pos = filename.rfind('.');
				std::string suffix;
				std::vector<std::string>::iterator it;
				bool suffix_match = false;
				//printf("pos:%d\n", pos);

				if(pos == std::string::npos) {
					pos = 0;
				}

				suffix = filename.substr(pos);
				//printf("suffix:%s\n", suffix.c_str());


				for(it = filter_list.begin(); it != filter_list.end(); it++) {
					if(it->compare(suffix) == 0) {
						suffix_match = true;
					}
				}

				if(!suffix_match) {
					continue;
				}
			}

			printf("add file:%s%s\n", dirname.c_str(), filename.c_str());
			file_list.push_back(dirname + filename);
		} else if(entry->d_type == DT_DIR) {
			if(0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name)) {
			} else {
				//printf("dir:%s\n", entry->d_name);
				std::vector<std::string> sub_file_list = scan_files(dirname + entry->d_name + "/", filter_list);
				file_list.insert(file_list.end(), sub_file_list.begin(), sub_file_list.end());
			}
		} else {
		}
	}

	closedir(d);

	return file_list;
}

int main(int argc, char **argv)
{
	int ret = 0;

	std::vector<std::string> file_list;
	std::string::reverse_iterator rit;
	std::vector<std::string>::iterator it;
	std::string topdir;
	std::vector<std::string> filter_list;
	struct stat sb;

	if(argc < 2) {
		return ret;
	}

	topdir = argv[1];

	ret = stat(topdir.c_str(), &sb);

	if(ret != 0) {
		printf("%s\n", strerror(errno));
	}

	if(!S_ISDIR(sb.st_mode)) {
		printf("%s\n", "not dir!!!");
		return ret;
	}

	rit = topdir.rbegin();

	if(*rit != '/') {
		topdir += "/";
	}

	printf("Directory scan of %s\n", topdir.c_str());


	filter_list.push_back(".cpp");
	filter_list.push_back("Makefile");
	file_list = scan_files(topdir, filter_list);

	for(std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		printf("file in list:%s\n", it->c_str());
	}

	printf("Done!\n");

	return ret;
}

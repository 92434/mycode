

/*================================================================
 *   Copyright (C) 2017年06月24日 肖飞 All rights reserved
 *
 *   文件名称：scanfiles.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月24日 星期六 09时10分21秒
 *   修改日期：2017年06月24日 星期六 18时42分34秒
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

std::vector<std::string> scan_files(std::string &dirname, std::vector<std::string> &filter_list)
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
		switch(entry->d_type) {
			case DT_CHR:
				break;

			case DT_BLK:
				break;

			case DT_REG: {
				//printf("file:%s\n", entry->d_name);
				file_list.push_back(dirname + entry->d_name);
			}
			break;

			case DT_DIR: {
				if(0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name)) {
				} else {
					//printf("dir:%s\n", entry->d_name);
					std::vector<std::string> sub_file_list = scan_files(dirname + entry->d_name + "/", filter_list);
					file_list.insert(file_list.end(), sub_file_list.begin(), sub_file_list.end());
				}
			}

			break;

			case DT_FIFO:
				break;

			case DT_LNK:
				break;

			case DT_SOCK:
				break;

			case DT_UNKNOWN:
				break;

			default:
				break;
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


	filter_list.push_back(".bmp");
	file_list = scan_files(topdir);

	for(std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		printf("file in list:%s\n", it->c_str());
	}

	printf("Done\n");
	return ret;
}

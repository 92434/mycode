

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：filesystem.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 09时39分21秒
 *   修改日期：2017年06月29日 星期四 18时09分50秒
 *   描    述：
 *
 *================================================================*/
#include "filesystem.h"
//#define ENABLE_INFO

filesystem::filesystem()
{
}

filesystem::~filesystem()
{
}

std::vector<std::string> filesystem::scan_files(std::string dirname, std::vector<std::string> &filter_list)
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

#if defined(ENABLE_INFO)
			printf("add file:%s%s\n", dirname.c_str(), filename.c_str());
#endif
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

std::vector<std::string> filesystem::dir_scan_files(std::string dirname, std::vector<std::string> &filter_list)
{
	std::vector<std::string> file_list;
	std::string::reverse_iterator rit;
	struct stat sb;
	int error = 0;

	error = stat(dirname.c_str(), &sb);

	if(error != 0) {
		printf("dirname:%s:%s\n", dirname.c_str(), strerror(errno));
		return file_list;
	}

	if(!S_ISDIR(sb.st_mode)) {
		printf("%s\n", "not dir!!!");
		return file_list;
	}

	rit = dirname.rbegin();

	if(*rit != '/') {
		dirname += "/";
	}

	file_list = scan_files(dirname, filter_list);

	return file_list;
}

int filesystem::mkdirs(std::string dirname, mode_t flags)
{
	int ret = 0;
	size_t pos = dirname.find('/', 0);
#if defined(ENABLE_INFO)
	printf("pos:%d\n", (int)pos);
#endif

	while(std::string::npos != pos) {
		std::string dirname_dirname = dirname.substr(0, pos);
#if defined(ENABLE_INFO)
		printf("dirname_dirname:%s\n", dirname_dirname.c_str());
#endif

		if(dirname_dirname.size() != 0) {
			ret = access(dirname_dirname.c_str(), F_OK);

			if(ret != 0) {
				ret = mkdir(dirname_dirname.c_str(), flags);

				if(ret != 0) {
					printf("mkdir:%s failed!!! (%s)!\n", dirname_dirname.c_str(), strerror(errno));
					break;
				}
			}
		}

		pos += 1;
		pos = dirname.find('/', pos);
#if defined(ENABLE_INFO)
		printf("pos:%d\n", (int)pos);
#endif
	}

	return ret;
}

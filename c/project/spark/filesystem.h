

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：filesystem.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 11时14分12秒
 *   修改日期：2017年07月14日 星期五 13时48分44秒
 *   描    述：
 *
 *================================================================*/
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <vector>
#include <string>
#include <dirent.h>//DIR
#include <stdio.h>//printf
#include <string.h>//strerror
#include <errno.h>//errno
#include <sys/stat.h>//stat
#include <unistd.h>//access

class filesystem
{
private:
	std::vector<std::string> scan_files(std::string dirname, std::vector<std::string> &filter_list);

public:
	filesystem();
	~filesystem();
	std::vector<std::string> dir_scan_files(std::string dirname, std::vector<std::string> &filter_list);
	int mkdirs(std::string dirname, mode_t flags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
};
#endif //FILESYSTEM_H

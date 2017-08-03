

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *   
 *   文件名称：configuration.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时18分47秒
 *   修改日期：2017年08月03日 星期四 10时42分42秒
 *   描    述：
 *
 *================================================================*/
#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <string.h>
#include <errno.h>
#include "regexp.h"
#include "constant.h"

class configure
{
private:
	std::map<std::string, std::map<std::string, std::string> > m_configuration;
	std::string current_class;

private:
	int match_class(std::string content);
	int match_key_value(std::string content);
public:
	configure();
	~configure();
	int load(std::string filename);
	int save(std::string filename);
	int add(std::string class_name, std::string key, std::string value);
	std::map<std::string, std::string> get_class(std::string class_name);
	std::string get(std::string class_name, std::string key);
	int p_configure();
};
#endif //CONFIGURATION_H

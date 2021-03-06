

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *   
 *   文件名称：configuration.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时22分47秒
 *   修改日期：2017年08月16日 星期三 18时16分37秒
 *   描    述：
 *
 *================================================================*/
#include "configuration.h"
//#define ENABLE_INFO

configure::configure()
{
}

configure::~configure()
{
}

int configure::add(std::string class_name, std::string key, std::string value)
{
	int ret = 0;
	std::map<std::string, std::string> key_value_set;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;

	it = m_configuration.find(class_name);

	if(it != m_configuration.end()) {
		key_value_set = it->second;
	}

	key_value_set[key] = value;
	m_configuration[class_name] = key_value_set;

	return ret;
}

std::string configure::get(std::string class_name, std::string key)
{
	std::string value;
	std::map<std::string, std::string> key_value_set;
	std::map<std::string, std::map<std::string, std::string> >::iterator it1;
	std::map<std::string, std::string>::iterator it2;

	it1 = m_configuration.find(class_name);

	if(it1 == m_configuration.end()) {
		printf("can not get class \"%s\"!\n", class_name.c_str());
		exit(1);
	}

	key_value_set = it1->second;

	it2 = key_value_set.find(key);

	if(it2 == key_value_set.end()) {
		printf("can not get class \"%s\", key \"%s\"!\n", class_name.c_str(), key.c_str());
		exit(1);
	}

	value = it2->second;

	return value;
}

std::map<std::string, std::string> configure::get_class(std::string class_name)
{
	std::map<std::string, std::string> key_value_set;
	std::map<std::string, std::map<std::string, std::string> >::iterator it1;

	it1 = m_configuration.find(class_name);

	if(it1 == m_configuration.end()) {
		return key_value_set;
	}

	key_value_set = it1->second;

	return key_value_set;
}

int configure::match_class(std::string content)
{
	int ret = 0;
	std::vector<std::string> matched_list;
	std::string pattern = "^[[:space:]]*\\[[[:space:]]*([^[:space:]]*)[[:space:]]*\\][[:space:]]*$";
	regexp r;

	matched_list = r.match(content, pattern);

	if(matched_list.size() != 2) {
		ret = -1;
		return ret;
	}

	current_class = matched_list.at(1);

	return ret;
}

int configure::match_key_value(std::string content)
{
	int ret = 0;
	std::vector<std::string> matched_list;
	std::string pattern = "^[[:space:]]*([^[:space:]]+)[[:space:]]*=[[:space:]]*(.*[^[:space:]])[[:space:]]*$";
	regexp r;

	matched_list = r.match(content, pattern);

	if(matched_list.size() != 3) {
		ret = -1;
		return ret;
	}

	add(current_class, matched_list.at(1), matched_list.at(2));
#if defined(ENABLE_INFO)
	printf("add:class:%s, key:%s, value:%s\n", current_class.c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str());
#endif

	return ret;
}

int configure::load(std::string filename)
{
	int ret = 0;
	std::ifstream ifs;
	char buffer[BUFFER_LEN];

	ifs.open(filename.c_str());

	if(!ifs.good()) {
		printf("open:%s failed!!! (%s)!\n", filename.c_str(), strerror(errno));
		ret = -1;
		return ret;
	}

	while(ifs.good()) {
		std::string content;
		size_t pos;

		ifs.getline(buffer, BUFFER_LEN);
		content = buffer;

		pos = content.find('#');

		if(std::string::npos != pos) {
			content = content.substr(0, pos);
		}

#if defined(ENABLE_INFO)
		printf("content:\"%s\", size:%d\n", buffer, (int)content.size());
#endif
		match_class(content);
		match_key_value(content);

#if defined(ENABLE_INFO)
		printf("\n");
#endif
	}

	ifs.close();

	return ret;
}

int configure::save(std::string filename)
{
	int ret = 0;
	std::ofstream ofs;
	char buffer[BUFFER_LEN];
	int len = 0;

	ofs.open(filename.c_str());

	if(!ofs.good()) {
		printf("open:%s failed!!! (%s)!\n", filename.c_str(), strerror(errno));
		ret = -1;
		return ret;
	}

	for(std::map<std::string, std::map<std::string, std::string> >::iterator it1 = m_configuration.begin(); it1 != m_configuration.end(); it1++) {
		len = snprintf(buffer, BUFFER_LEN, "[%s]\n", it1->first.c_str());
		buffer[len] = 0;
		ofs.write(buffer, len);

		for(std::map<std::string, std::string>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			len = snprintf(buffer, BUFFER_LEN, "%s=%s\n", it2->first.c_str(), it2->second.c_str());
			buffer[len] = 0;
			ofs.write(buffer, len);
		}
	}

	ofs.close();

	return ret;
}

int configure::p_configure()
{
	for(std::map<std::string, std::map<std::string, std::string> >::iterator it1 = m_configuration.begin(); it1 != m_configuration.end(); it1++) {
		printf("class:\"%s\":\n", it1->first.c_str());

		for(std::map<std::string, std::string>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			printf("key:\"%s\", value:\"%s\"\n", it2->first.c_str(), it2->second.c_str());
		}
	}
}

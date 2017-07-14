

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：settings.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时43分03秒
 *   修改日期：2017年07月14日 星期五 15时56分07秒
 *   描    述：
 *
 *================================================================*/
#include "settings.h"
settings *settings::g_settings = NULL;

settings::settings()
{
	max_proc_number = 32;
	max_number_of_id_per_proc = 5;
	max_number_of_catagory_per_proc = 1;
	fr_select_type = SELECT_SAME_CATAGORY;
	fa_select_type = SELECT_SAME_CATAGORY;
}

settings::~settings()
{
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	abort();

	if(g_settings == NULL) {
		delete g_settings;
		g_settings = NULL;
	}
}

settings *settings::get_instance()
{
	if(g_settings == NULL) {
		g_settings = new settings();
	}

	return g_settings;
}

int settings::check_configuration()
{
	int ret = 0;

	bool have_dir = false;
	bool have_enroll_pattern = false;
	bool have_fr_pattern = false;
	bool have_fa_pattern = false;

	if(dirname.size()) {
		have_dir = true;
	} else {
		printf("no dirname!\n");
	}

	if(enroll_pattern.size()) {
		have_enroll_pattern = true;
	} else {
		printf("no enroll_pattern!\n");
	}

	if(fr_pattern.size()) {
		have_fr_pattern = true;
	} else {
		printf("no fr pattern!\n");
	}

	if(fa_pattern.size()) {
		have_fa_pattern = true;
	} else {
		printf("no fa pattern!\n");
	}

	if(have_dir && have_enroll_pattern && (have_fr_pattern || have_fa_pattern)) {
	} else {
		ret = -1;
		printf("%s!!!\n", "参数信息不足");
	}

	return ret;
}

std::vector<pattern_info> settings::get_pattern_info(configure &cfg, std::string pattern_base)
{
	std::vector<pattern_info> vec_pattern;
	std::map<std::string, std::string> pattern_class = cfg.get_class("resource_pattern");
	std::map<std::string, std::string>::iterator pattern_class_it;

	std::string pattern = pattern_base + "_([[:digit:]]+)$";
	std::string pattern_success = pattern_base + "_success_([[:digit:]]+)$";
	std::string pattern_fail = pattern_base + "_fail_([[:digit:]]+)$";

	std::set<std::string> set_pattern_key;
	std::set<std::string> set_state_pattern_key;

	std::set<std::string>::iterator set_it;

	printf("%s:%s:%d:add pattern for %s!\n", __FILE__, __func__, __LINE__, pattern_base.c_str());

	for(pattern_class_it = pattern_class.begin(); pattern_class_it != pattern_class.end(); pattern_class_it++) {
		regexp r;
		std::string key = pattern_class_it->first;

		std::vector<std::string> matched_list;

		matched_list = r.match(key, pattern);

		if(matched_list.size() == 2) {
			//printf("%s:%s:%d:add pattern:%s\n", __FILE__, __func__, __LINE__, key.c_str());
			set_pattern_key.insert(matched_list.at(1));
		}

		matched_list = r.match(key, pattern_success);

		if(matched_list.size() == 2) {
			//printf("%s:%s:%d:add success pattern:%s\n", __FILE__, __func__, __LINE__, key.c_str());
			set_state_pattern_key.insert(matched_list.at(1));
		}

		matched_list = r.match(key, pattern_fail);

		if(matched_list.size() == 2) {
			//printf("%s:%s:%d:add fail pattern:%s\n", __FILE__, __func__, __LINE__, key.c_str());
			set_state_pattern_key.insert(matched_list.at(1));
		}
	}

	for(set_it = set_pattern_key.begin(); set_it != set_pattern_key.end(); set_it++) {
		pattern_info pattern_info;
		std::string pattern_key = pattern_base + "_" + *set_it;
		std::string pattern_key_order = pattern_base + "_order_" + *set_it;

		pattern_info.pattern = cfg.get("resource_pattern", pattern_key);
		pattern_info.pattern_order = cfg.get("resource_pattern", pattern_key_order);

		printf(
			"%s:%s:%d:pattern:%s, pattern_order:%s\n",
			__FILE__,
			__func__,
			__LINE__,
			pattern_info.pattern.c_str(),
			pattern_info.pattern_order.c_str());

		if((pattern_info.pattern.size() != 0) && (pattern_info.pattern_order.size() != 0)) {
			vec_pattern.push_back(pattern_info);
		}
	}

	for(set_it = set_state_pattern_key.begin(); set_it != set_state_pattern_key.end(); set_it++) {
		pattern_info pattern_info;
		std::string success_pattern_key = pattern_base + "_success_" + *set_it;
		std::string success_pattern_key_order = pattern_base + "_success_order_" + *set_it;

		std::string fail_pattern_key = pattern_base + "_fail_" + *set_it;
		std::string fail_pattern_key_order = pattern_base + "_fail_order_" + *set_it;

		pattern_info.success_pattern = cfg.get("resource_pattern", success_pattern_key);
		pattern_info.success_pattern_order = cfg.get("resource_pattern", success_pattern_key_order);
		pattern_info.fail_pattern = cfg.get("resource_pattern", fail_pattern_key);
		pattern_info.fail_pattern_order = cfg.get("resource_pattern", fail_pattern_key_order);
		printf(
			"%s:%s:%d:success_pattern:%s, success_pattern_order:%s, fail_pattern:%s, fail_pattern_order:%s\n",
			__FILE__,
			__func__,
			__LINE__,
			pattern_info.success_pattern.c_str(),
			pattern_info.success_pattern_order.c_str(),
			pattern_info.fail_pattern.c_str(),
			pattern_info.fail_pattern_order.c_str());

		if(
			(pattern_info.success_pattern.size() != 0)
			&& (pattern_info.success_pattern_order.size() != 0)
			&& (pattern_info.fail_pattern.size() != 0)
			&& (pattern_info.fail_pattern_order.size() != 0)
		) {
			vec_pattern.push_back(pattern_info);
		}
	}

	return vec_pattern;
}

int settings::get_pattern_from_configuration(configure &cfg)
{
	int ret = 0;

	enroll_pattern = get_pattern_info(cfg, "enroll_pattern");
	fr_pattern = get_pattern_info(cfg, "fr_pattern");
	fa_pattern = get_pattern_info(cfg, "fa_pattern");

	return ret;
}

int settings::parse_args_from_configuration(int argc, char **argv)
{
	int ret = 0;
	bool have_configure_file = false;
	optparse opt;
	opt.add_long_option("configure_file", true, false, 'f', "assign configuration file");

	ret = opt.get_long_option(argc, argv);

	if(ret != 0) {
		opt.p_help();
	} else {
		have_configure_file = true;

		if(opt.have_option("f")) {
			configure_file = opt.option_value("f");
		} else {
			configure_file = "default.ini";
		}
	}

	if(have_configure_file) {
		configure cfg;
		ret = cfg.load(configure_file);

		if(ret == 0) {
			cfg.p_configure();
			dirname = cfg.get("dir", "directory");

			ret = get_pattern_from_configuration(cfg);

		} else {
			printf("load configuration file failed!!!\n");
		}

		if(ret == 0) {
			ret = check_configuration();
		}
	}

	return ret;
}

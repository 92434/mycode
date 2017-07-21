

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：settings.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时43分03秒
 *   修改日期：2017年07月21日 星期五 14时50分09秒
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
	fa_select_type = SELECT_DIFFERENT_ID;

	log_dirname = get_timestamp();
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

double settings::value_strtod(std::string number)
{
	double ret = 0;
	char *invalid_pos;

	if(number.size() == 0) {
		printf("parameter not set!!!\n");
	}

	ret = strtod(number.c_str(), &invalid_pos);

	return ret;
}

int settings::get_time_val(struct timeval *timeval)
{
	int ret = 0;
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	*timeval = tv;

	return ret;
}

std::string settings::get_timestamp()
{
	std::string ret;
	char buffer[1024];
	int len = 0;
	struct tm *tm;
	struct timeval tv;

	get_time_val(&tv);

	tm = localtime(&tv.tv_sec);

	len = snprintf(buffer, 1023, "%04d%02d%02d%02d%02d%02d_%06d",
				   tm->tm_year + 1900,
				   tm->tm_mon + 1,
				   tm->tm_mday,
				   tm->tm_hour,
				   tm->tm_min,
				   tm->tm_sec,
				   (int)tv.tv_usec
				  );
	buffer[len] = 0;
	ret = buffer;
	return ret;
}

int settings::check_configuration()
{
	int ret = 0;

	bool have_dir = false;
	bool have_enroll_pattern = false;
	bool have_fr_pattern = false;
	bool have_fa_pattern = false;

	if(pictures_dirname.size()) {
		have_dir = true;
	} else {
		printf("no pictures pictures_dirname!\n");
	}

	if(enroll_pattern.size()) {
		have_enroll_pattern = true;
	} else {
		printf("no enroll pattern!\n");
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

int settings::get_app_settings_from_configuration(configure &cfg)
{
	int ret = 0;
	pictures_dirname = cfg.get("app", "pictures_directory");
	test_type = cfg.get("app", "test_type");

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

	//printf("%s:%s:%d:add pattern for %s!\n", __FILE__, __func__, __LINE__, pattern_base.c_str());

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

		//printf(
		//	"%s:%s:%d:pattern:%s, pattern_order:%s\n",
		//	__FILE__,
		//	__func__,
		//	__LINE__,
		//	pattern_info.pattern.c_str(),
		//	pattern_info.pattern_order.c_str());

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
		//printf(
		//	"%s:%s:%d:success_pattern:%s, success_pattern_order:%s, fail_pattern:%s, fail_pattern_order:%s\n",
		//	__FILE__,
		//	__func__,
		//	__LINE__,
		//	pattern_info.success_pattern.c_str(),
		//	pattern_info.success_pattern_order.c_str(),
		//	pattern_info.fail_pattern.c_str(),
		//	pattern_info.fail_pattern_order.c_str());

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

int settings::get_sensor_lib_settings_from_configuration(configure &cfg)
{
	int ret = 0;
	debug_switch = cfg.get("sensor_lib", "debug_switch");
	algorithm_mode = cfg.get("sensor_lib", "algorithm_mode");
	enroll_max_templates = cfg.get("sensor_lib", "enroll_max_templates");
	algorithm_max_templates = cfg.get("sensor_lib", "algorithm_max_templates");
	spa_enable = cfg.get("sensor_lib", "spa_enable");
	algorithm_far_level = cfg.get("sensor_lib", "algorithm_far_level");
	update_template_far_level = cfg.get("sensor_lib", "update_template_far_level");
	update_template_threshold = cfg.get("sensor_lib", "update_template_threshold");
	verify_quickly_enable = cfg.get("sensor_lib", "verify_quickly_enable");
	update_template_outside_enable = cfg.get("sensor_lib", "update_template_outside_enable");
	image_quality_score = cfg.get("sensor_lib", "image_quality_score");
	verify_image_quality_score = cfg.get("sensor_lib", "verify_image_quality_score");
	enroll_duplicate_area_check_enable = cfg.get("sensor_lib", "enroll_duplicate_area_check_enable");
	valid_area_scale = cfg.get("sensor_lib", "valid_area_scale");
	enrollment_tips_enable = cfg.get("sensor_lib", "enrollment_tips_enable");
	enrollment_tips_parameter1 = cfg.get("sensor_lib", "enrollment_tips_parameter1");
	enrollment_tips_parameter2 = cfg.get("sensor_lib", "enrollment_tips_parameter2");
	enrollment_tips_parameter3 = cfg.get("sensor_lib", "enrollment_tips_parameter3");
	verify_improve_enable = cfg.get("sensor_lib", "verify_improve_enable");
	verify_improve_level = cfg.get("sensor_lib", "verify_improve_level");
	mcu_image_bit = cfg.get("sensor_lib", "mcu_image_bit");
	mcu_interrupt_mode = cfg.get("sensor_lib", "mcu_interrupt_mode");
	mcu_state_check_mode = cfg.get("sensor_lib", "mcu_state_check_mode");
	repeat_get_image_count = cfg.get("sensor_lib", "repeat_get_image_count");
	template_buffer_enable = cfg.get("sensor_lib", "template_buffer_enable");
	transfer_bytes_max = cfg.get("sensor_lib", "transfer_bytes_max");
	config_debuginfo_switch = cfg.get("sensor_lib", "config_debuginfo_switch");
	return ret;
}


int settings::get_task_settings_from_configuration(configure &cfg)
{
	int ret = 0;
	std::string value;
	max_number_of_id_per_proc = (int)value_strtod(cfg.get("settings", "max_number_of_id_per_proc"));
	max_number_of_catagory_per_proc = (int)value_strtod(cfg.get("settings", "max_number_of_catagory_per_proc"));
	value = cfg.get("settings", "fr_select_type");
	fr_select_type = (value == "SELECT_ALL") ? SELECT_ALL
					 : (value == "SELECT_SAME_CATAGORY") ? SELECT_SAME_CATAGORY
					 : SELECT_DIFFERENT_ID;
	value = cfg.get("settings", "fa_select_type");
	fa_select_type = (value == "SELECT_ALL") ? SELECT_ALL
					 : (value == "SELECT_SAME_CATAGORY") ? SELECT_SAME_CATAGORY
					 : SELECT_DIFFERENT_ID;

	max_proc_number = (int)value_strtod(cfg.get("settings", "max_proc_number"));
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
			//cfg.p_configure();
			ret = get_app_settings_from_configuration(cfg);
			ret = get_pattern_from_configuration(cfg);
			ret = get_sensor_lib_settings_from_configuration(cfg);
			ret = get_task_settings_from_configuration(cfg);
		} else {
			printf("load configuration file failed!!!\n");
		}

		if(ret == 0) {
			ret = check_configuration();
		}
	}

	return ret;
}

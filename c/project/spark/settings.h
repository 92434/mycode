

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：settings.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时25分41秒
 *   修改日期：2017年07月24日 星期一 12时47分35秒
 *   描    述：
 *
 *================================================================*/
#ifndef _SETTINGS_H
#define _SETTINGS_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <string>
#include <vector>
#include <set>
#include <stdlib.h>
#include <sys/time.h>
#include "configuration.h"
#include "optparse.h"

typedef enum _select_type {
	SELECT_ALL = 0,
	SELECT_SAME_CATAGORY,
	SELECT_DIFFERENT_ID,
} select_type_t;

struct pattern_info {
	std::string pattern;
	std::string pattern_order;

	std::string success_pattern;
	std::string success_pattern_order;
	std::string fail_pattern;
	std::string fail_pattern_order;

	pattern_info()
	{
		pattern.clear();
		pattern_order.clear();

		success_pattern.clear();
		success_pattern_order.clear();
		fail_pattern.clear();
		fail_pattern_order.clear();
	}
	~pattern_info()
	{
	}
};

class settings
{
public:
	bool dryrun;
	std::string configure_file;
	std::string pictures_dirname;
	std::string test_type;
	std::string log_dirname;

	std::vector<pattern_info> enroll_pattern;
	std::vector<pattern_info> fr_pattern;
	std::vector<pattern_info> fa_pattern;

	int max_number_of_id_per_proc;
	int max_number_of_catagory_per_proc;
	select_type_t fr_select_type;
	select_type_t fa_select_type;
	int max_proc_number;

	std::string debug_switch;
	std::string algorithm_mode;
	std::string enroll_max_templates;
	std::string algorithm_max_templates;
	std::string spa_enable;
	std::string algorithm_far_level;
	std::string update_template_far_level;
	std::string update_template_threshold;
	std::string verify_quickly_enable;
	std::string update_template_outside_enable;
	std::string image_quality_score;
	std::string verify_image_quality_score;
	std::string enroll_duplicate_area_check_enable;
	std::string valid_area_scale;
	std::string enrollment_tips_enable;
	std::string enrollment_tips_parameter1;
	std::string enrollment_tips_parameter2;
	std::string enrollment_tips_parameter3;
	std::string verify_improve_enable;
	std::string verify_improve_level;
	std::string mcu_image_bit;
	std::string mcu_interrupt_mode;
	std::string mcu_state_check_mode;
	std::string repeat_get_image_count;
	std::string template_buffer_enable;
	std::string transfer_bytes_max;
	std::string config_debuginfo_switch;

	static settings *g_settings;

	settings();

	~settings();
public:
	static settings *get_instance();

	double value_strtod(std::string number);

	int get_time_val(struct timeval *timeval);

	std::string get_timestamp();

	int check_configuration();

	int get_app_settings_from_configuration(configure &cfg);

	std::vector<pattern_info> get_pattern_info(configure &cfg, std::string pattern_base);

	int get_pattern_from_configuration(configure &cfg);

	int get_sensor_lib_settings_from_configuration(configure &cfg);

	int get_task_settings_from_configuration(configure &cfg);

	int parse_args_from_configuration(int argc, char **argv);

};
#endif //SETTINGS_H

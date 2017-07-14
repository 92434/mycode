

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：settings.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时25分41秒
 *   修改日期：2017年07月14日 星期五 15时52分52秒
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
#include "configuration.h"
#include "optparse.h"

typedef enum _select_type {
	SELECT_ALL = 0,
	SELECT_SAME_CATAGORY,
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
	std::string configure_file;
	std::string dirname;

	std::vector<pattern_info> enroll_pattern;
	std::vector<pattern_info> fr_pattern;
	std::vector<pattern_info> fa_pattern;

	int max_number_of_id_per_proc;
	int max_number_of_catagory_per_proc;
	select_type_t fr_select_type;
	select_type_t fa_select_type;
	int max_proc_number;

	static settings *g_settings;

	settings();

	~settings();
public:
	static settings *get_instance();

	int check_configuration();

	std::vector<pattern_info> get_pattern_info(configure &cfg, std::string pattern_base);

	int get_pattern_from_configuration(configure &cfg);

	int parse_args_from_configuration(int argc, char **argv);

};
#endif //SETTINGS_H

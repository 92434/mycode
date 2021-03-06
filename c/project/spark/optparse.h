

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：optparse.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 11时07分30秒
 *   修改日期：2017年07月14日 星期五 13时57分32秒
 *   描    述：
 *
 *================================================================*/
#ifndef _OPTPARSE_H
#define _OPTPARSE_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <getopt.h>
#include <strings.h>

typedef struct _long_option_des {
	std::string optstring;
	std::string long_optstring;
	bool has_arg;
	bool has_retval;
	std::string info;
} long_option_des_t;

typedef enum _short_option_mode {
	MODE_NOMAL = 0,
	MODE_NOPARA,
	MODE_PARA,
} short_option_mode_t;

class optparse
{
protected:
	std::vector<long_option_des_t> vlong_option;
	std::map<std::string, std::string> moption;

	int initilize_optstring(char *optstring, short_option_mode_t mode);
	int parse_option(int argc, char **argv, char *optstring);
	int initilize_long_option(struct option *long_options, char *optstring, int *vals, short_option_mode_t mode);
	int parse_long_option(int argc, char **argv, char *optstring, struct option *long_options, int *vals);
public:

	optparse()
	{
		vlong_option.clear();
	}

	~optparse() {}

	int add_long_option(std::string long_optstring, bool has_arg, bool has_retval, char opt, std::string info);
	//MODE_NOMAL:nonal mode(parameter is left for name argument),
	//MODE_NOPARA:optstring start with -(parameter is left for 1),
	//MODE_PARA:optstring start with +(stop at first error,left is parameter for name argument),
	int get_long_option(int argc, char **argv, short_option_mode_t mode = MODE_PARA);

	int p_help();
	int p_result(std::string parsername = "");

	bool have_option(std::string s);
	std::string option_value(std::string s);
};
#endif //OPTPARSE_H

#ifndef __OPTPARSE_H
#define __OPTPARSE_H
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <getopt.h>
#include <strings.h>

typedef struct _option_des {
	std::string optstring;
	bool has_arg;
	std::string info;
} option_des_t;

typedef struct _long_option_des {
	std::string long_optstring;
	bool has_arg;
	bool has_retval;
	std::string optstring;
	std::string info;
} long_option_des_t;

typedef enum _short_option_mode {
	MODE_NOMAL = 0,
	MODE_NOPARA,
	MODE_PARA,
} short_option_mode_t;

class optparse {
private:
	std::vector<option_des_t> voption;
	std::vector<long_option_des_t> vlong_option;
	std::map<std::string, std::string> moption;

	int initilize_optstring(char *optstring, short_option_mode_t mode);
	int parse_option(int argc, char **argv, char *optstring);
	int initilize_long_option(struct option *long_options, char *optstring, int *vals, short_option_mode_t mode);
	int parse_long_option(int argc, char **argv, char *optstring, struct option *long_options, int *vals);
public:

	optparse() {
		voption.clear();
		vlong_option.clear();
		moption.clear();
	}

	~optparse() {}

	int add_option(char opt, bool has_arg, std::string info);

	//MODE_NOMAL:nonal mode(parameter is left for name argument),
	//MODE_NOPARA:optstring start with -(parameter is left for 1),
	//MODE_PARA:optstring start with +(stop at first error,left is parameter for name argument),
	int add_long_option(std::string long_optstring, bool has_arg, bool has_retval, char opt, std::string info);
	int get_option(int argc, char **argv, short_option_mode_t mode = MODE_PARA);
	int get_long_option(int argc, char **argv, short_option_mode_t mode = MODE_PARA);
	int p_help();
	int p_result();

	bool have_option(std::string s);
	std::string option_value(std::string s);
};
#endif//#ifndef __OPTPARSE_H

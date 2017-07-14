

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：optparse.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 11时12分25秒
 *   修改日期：2017年07月14日 星期五 13时57分24秒
 *   描    述：
 *
 *================================================================*/
#include "optparse.h"

//#define ENABLE_INFO
int optparse::add_long_option(std::string long_optstring, bool has_arg, bool has_retval, char opt, std::string info)
{
	int ret = 0;
	long_option_des_t des;
	des.long_optstring = long_optstring;
	des.has_arg = has_arg;
	des.has_retval = has_retval;
	des.optstring.push_back(opt);
	des.info = info;
	vlong_option.push_back(des);

	return ret;
}

int optparse::initilize_long_option(struct option *long_options, char *optstring, int *vals, short_option_mode_t mode)
{
	int ret = 0;
	int i;
	int index = 0;

	switch(mode) {
		default:
		case MODE_NOMAL:
			break;

		case MODE_NOPARA:
			optstring[index++] = '-';
			break;

		case MODE_PARA:
			optstring[index++] = '+';
			break;
	}


	for(i = 0; i < (int)vlong_option.size(); i++) {
		long_options[i].name = vlong_option[i].long_optstring.c_str();

		if(vlong_option[i].has_retval) {
			long_options[i].flag = vals + i;
		} else {
			long_options[i].flag = 0;
		}

		if(vlong_option[i].optstring[0] != ' ') {
			long_options[i].val = (int)vlong_option[i].optstring[0];
			optstring[index++] = vlong_option[i].optstring[0];
		} else {
			long_options[i].val = 0;
		}

		if(vlong_option[i].has_arg) {
			long_options[i].has_arg = required_argument;

			if(long_options[i].val) {
				optstring[index++] = ':';
			}
		} else {
			long_options[i].has_arg = no_argument;
		}

	}

	optstring[index++] = '\0';

	bzero(&long_options[i], sizeof(struct option));

	return ret;
}

int optparse::parse_long_option(int argc, char **argv, char *optstring, struct option *long_options, int *vals)
{
	int ret = 0;
	int opt;
	int option_index = 0;
	int i;

#if defined(ENABLE_INFO)
	printf("%-40s:%d\n", "argc", argc);
	printf("%-40s:", "argv");

	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}

	printf("\n");
	printf("%-40s:%s\n", "optstring", optstring);
	printf("%-40s:\n", "long_options");

	for(i = 0; i < (int)vlong_option.size(); i++) {
		printf("%-40s:%20s%20s%20s%5c\n",
			   "",
			   long_options[i].name,
			   (long_options[i].has_arg == no_argument) ? "no_argument" : (long_options[i].has_arg == required_argument) ? "required_argument" : "optional_argument",
			   long_options[i].flag ? "use value" : "not use value",
			   long_options[i].val ? long_options[i].val : ' '
			  );
	}

#endif

	optind = 1;

	while((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1) {
#if defined(ENABLE_INFO)
		printf("option is %c(%x), option_index is %d, long option arg is %s, optind=%d\n",
			   opt,
			   opt,
			   option_index,
			   optarg,
			   optind
			  );
#endif

		if(opt == 0) {
			//printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
			if(long_options[option_index].flag != 0) {//default use short option name
				std::string o;
				o.push_back(vals[option_index]);
				moption[o] = optarg ? optarg : "";
			} else {//if not exist short option name, use long option name
				moption[long_options[option_index].name] = optarg ? optarg : "";
			}

		} else if(opt == 1) {
			if(moption["para"].size()) {
				moption["para"] += " ";
			}

			moption["para"] += optarg ? optarg : "";
			ret = -1;
		} else if(opt == '?') {
			ret = -1;
		} else {
			std::string o;
			o.push_back(opt);
			moption[o] = optarg ? optarg : "[set]";
		}
	}

#if defined(ENABLE_INFO)

	if (optind < argc) {
		printf("name argument =");

		for(i = optind; i < argc; i++) {
			printf(" %s", argv[i]);

			if(moption["para"].size()) {
				moption["para"] += " ";
			}

			moption["para"] += argv[i];
		}

		printf("\n");
	}

#endif

	return ret;
}

int optparse::get_long_option(int argc, char **argv, short_option_mode_t mode)
{
	int ret = 0;

	char *optstring;
	struct option *long_options;
	int *vals;

	optstring = new char[vlong_option.size() * 2 + 1];

	if(optstring == 0) {
		ret = -1;
		goto l1;
	}

	long_options = new struct option[vlong_option.size() + 1];

	if(long_options == 0) {
		ret = -1;
		goto l2;
	}

	vals = new int[vlong_option.size() + 1];

	if(vals == 0) {
		ret = -1;
		goto l3;
	}

	ret = initilize_long_option(long_options, optstring, vals, mode);

	if(ret != 0) {
		goto l4;
	}

	ret = parse_long_option(argc, argv, optstring, long_options, vals);

l4:
	delete []vals;
l3:
	delete []long_options;
l2:
	delete []optstring;
l1:

	return ret;
}

int optparse::p_help()
{
	int ret = 0;
	int i;
	printf("%40s==help==\n", "");

	for(i = 0; i < (int)vlong_option.size(); i++) {
		std::string s = vlong_option[i].long_optstring;

		if(vlong_option[i].optstring[0] != ' ') {
			s = vlong_option[i].optstring  + "," + s;
		}

		if(vlong_option[i].has_arg) {
			s += "[para]";
		}

		printf("%40s%5s%s\n", s.c_str(), "", vlong_option[i].info.c_str());
	}

	return ret;
}

int optparse::p_result(std::string parsername)
{
	int ret = 0;
	std::map<std::string, std::string>::iterator it;

	if(moption.size() == 0) {
		ret = -1;
		return ret;
	}

	printf("[result:%s]:\n", parsername.c_str());

	for(it = moption.begin(); it != moption.end(); it++) {
		if(it->second.size() != 0) {
			printf("%40s:%s\n", it->first.c_str(), it->second.c_str());
		}
	}

	return ret;
}

bool optparse::have_option(std::string s)
{
	bool ret = false;
	std::map<std::string, std::string>::iterator it;

	for(it = moption.begin(); it != moption.end(); it++) {
		if(it->first.compare(s) == 0) {
			ret = true;
		}
	}

	return ret;
}

std::string optparse::option_value(std::string s)
{
	std::string ret;
	std::map<std::string, std::string>::iterator it;

	for(it = moption.begin(); it != moption.end(); it++) {
		if(it->first.compare(s) == 0) {
			ret = it->second;
		}
	}

	return ret;
}

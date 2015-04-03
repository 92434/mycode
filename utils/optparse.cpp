#include "optparse.h"

//#define printf(args...) printf(args)
//#define printf(format, ...) fprintf (stderr, format, ## __VA_ARGS__)
#define printf(args...)

int optparse::add_option(char opt, bool has_arg, std::string info) {
	int ret  = 0;
	option_des_t des;
	des.optstring.push_back(opt);
	des.has_arg = has_arg;
	if(has_arg) {
		des.optstring.push_back(':');
	}
	des.info = info;
	voption.push_back(des);
	return ret;
}

int optparse::initilize_optstring(char *optstring, short_option_mode_t mode) {
	int ret = 0;
	int index = 0, i;
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

	for(i = 0; i < (int)voption.size(); i++) {
		optstring[index++] = voption[i].optstring[0];
		if(voption[i].has_arg) {
			optstring[index++] = voption[i].optstring[1];
		}
	}
	optstring[index] = '\0';

	return ret;
}

int optparse::parse_option(int argc, char **argv, char *optstring) {
	int ret = 0;
	int opt;
	int i;
	printf("%-20s:%d\n", "argc", argc);
	printf("%-20s:", "argv");
	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	printf("%-20s:%s\n", "optstring", optstring);
	optind = 1;

	while ((opt = getopt(argc, argv, optstring)) != -1) {
		printf("option is %c(%x), option argument is %s; optind=%d\n", opt, opt, optarg, optind);

		if(opt == 1) {
			if(moption["para"].size()) {
				moption["para"] += " ";
			}
			moption["para"] += optarg ? optarg : "";
			ret = -1;
		} else if(opt != '?') {
			std::string o;
			o.push_back(opt);
			moption[o] = optarg ? optarg : "";
		} else {
			ret = -1;
		}

	}

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
	return ret;
}

//MODE_NOMAL:nomal mode(parameter is left for name argument),
//MODE_NOPARA:optstring start with -(parameter is left for 1),
//MODE_PARA:optstring start with +(stop at first error,left is parameter for name argument),
int optparse::get_option(int argc, char **argv, short_option_mode_t mode) {
	int ret = 0;
	char *optstring;

	optstring = new char[voption.size() * 2];
	if(optstring == 0) {
		ret = -1;
		goto l1;
	}

	ret = initilize_optstring(optstring, mode);
	if(ret != 0) {
		goto l2;
	}

	ret = parse_option(argc, argv, optstring);

l2:
	delete []optstring;
l1:

	return ret;
}

int optparse::add_long_option(std::string long_optstring, bool has_arg, bool has_retval, char opt, std::string info) {
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

int optparse::initilize_long_option(struct option *long_options, char *optstring, int *vals, short_option_mode_t mode) {
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

int optparse::parse_long_option(int argc, char **argv, char *optstring, struct option *long_options, int *vals) {
	int ret = 0;
	int opt;
	int option_index = 0;
	int i;

	printf("%-20s:%d\n", "argc", argc);
	printf("%-20s:", "argv");
	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	printf("%-20s:%s\n", "optstring", optstring);
	printf("%-20s:\n", "long_options");
	for(i = 0; long_options[i].name != 0; i++) {
		printf("%-20s:%20s%20s%20s%5c\n",
				"", 
				long_options[i].name, 
				(long_options[i].has_arg == no_argument) ? "no_argument" : (long_options[i].has_arg == required_argument) ? "required_argument" : "optional_argument", 
				long_options[i].flag ? "use value" : "not use value", 
				long_options[i].val ? long_options[i].val : ' '
		      );
	}
	optind = 1;

	while((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1) {
		printf("option is %c(%x), option_index is %d, long option arg is %s, optind=%d\n",
				opt,
				opt,
				option_index,
				optarg,
				optind
		      );

		if(opt == 0) {
			printf("opt is %c(%x)\n", vals[option_index], vals[option_index]);

			if(long_options[option_index].flag != 0) {//default use short option name
				std::string o;
				o.push_back(vals[option_index]);
				moption[o] = optarg ? optarg : "";
			} else {//if not exist short option name, use long option name
				moption[long_options[option_index].name] = optarg ? optarg : "";
			}
		} else if(opt == 1) {
			printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
			if(moption["para"].size()) {
				moption["para"] += " ";
			}
			moption["para"] += optarg ? optarg : "";
			ret = -1;
		} else if(opt == '?') {
			ret = -1;
		} else {
			printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
			std::string o;
			o.push_back(opt);
			moption[o] = optarg ? optarg : "";
		}
	}

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

	return ret;
}

int optparse::get_long_option(int argc, char **argv, short_option_mode_t mode) {
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

int optparse::p_help() {
	int ret = 0;
	int i;
	printf("%20s%s%20s\n", "","==help==","");
	for(i = 0; i < (int)voption.size(); i++) {
		printf("%20c%5s%s\n", voption[i].optstring[0], "", voption[i].info.c_str());
	}
	for(i = 0; i < (int)vlong_option.size(); i++) {
		std::string s = vlong_option[i].long_optstring;
		if(vlong_option[i].optstring[0] != ' ') {
			s = vlong_option[i].optstring  + "," + s;
		}
		if(vlong_option[i].has_arg) {
			s += "[para]";
		}
		printf("%20s%5s%s\n", s.c_str(), "", vlong_option[i].info.c_str());
	}

	return ret;
}

int optparse::p_result(std::string parsername) {
	int ret = 0;
	std::map<std::string, std::string>::iterator it;
	if(moption.size() == 0) {
		ret = -1;
		return ret;
	}

	printf("%s:\n", parsername.c_str());
	for(it = moption.begin(); it != moption.end(); it++) {
		printf("%20s:%s\n", it->first.c_str(), it->second.c_str());
	}
	return ret;
}

bool optparse::have_option(std::string s) {
	bool ret = false;
	std::map<std::string, std::string>::iterator it;
	for(it = moption.begin(); it != moption.end(); it++) {
		if(it->first.compare(s) == 0) {
			ret = true;
		}
	}

	return ret;
}

std::string optparse::option_value(std::string s) {
	std::string ret;
	std::map<std::string, std::string>::iterator it;
	for(it = moption.begin(); it != moption.end(); it++) {
		if(it->first.compare(s) == 0) {
			ret = it->second;
		}
	}

	return ret;
}

command_parser *command_parser::cur_parser = NULL;
command_parser *command_parser::add_sub_command(std::string sub_command) {
	command_parser *parser = new command_parser(sub_command);

	if(parser != 0) {
		msub_commands[sub_command] = parser;
	}

	return parser;
}
int command_parser::get_option(int argc, char **argv, short_option_mode_t mode) {
	int ret = 0;
	ret = optparse::get_option(argc, argv, MODE_PARA);
	if(ret != 0) {
		return ret;
	}

	if(optind >= argc) {
		return ret;
	}

	std::map<std::string, command_parser *>::iterator it;
	for(it = msub_commands.begin(); it != msub_commands.end(); it++) {
		if(it->first.compare(argv[optind]) == 0) {
			optind += 1;
			moption["para"] = "";
			cur_parser = it->second;
			break;
		}
	}

	if(cur_parser != this) {
		printf("optind:%d,argc:%d, argv[0]:%s\n",optind, argc - (optind - 1), *(argv + (optind - 1)));
		ret = cur_parser->get_option(argc - (optind - 1), argv + (optind - 1));
	}

	return ret;
}
int command_parser::get_long_option(int argc, char **argv, short_option_mode_t mode) {
	int ret = 0;
	ret = optparse::get_long_option(argc, argv, MODE_PARA);
	if(ret != 0) {
		return ret;
	}

	if(optind >= argc) {
		return ret;
	}

	std::map<std::string, command_parser *>::iterator it;
	for(it = msub_commands.begin(); it != msub_commands.end(); it++) {
		if(it->first.compare(argv[optind]) == 0) {
			optind += 1;
			moption["para"] = "";
			cur_parser = it->second;
			break;
		}
	}

	if(cur_parser != this) {
		//printf("optind:%d,argc:%d, argv[0]:%s\n",optind, argc - (optind - 1), *(argv + (optind - 1)));
		ret = cur_parser->get_long_option(argc - (optind - 1), argv + (optind - 1));
	}


	return ret;
}

int command_parser::p_help() {
	if(cur_parser == this) {
		optparse::p_help();

		std::string sub_commands;
		std::map<std::string, command_parser *>::iterator it;
		for(it = msub_commands.begin(); it != msub_commands.end(); it++) {
			if(sub_commands.size() != 0) {
				sub_commands += " ";
			}
			sub_commands += it->first;
		}
		if(sub_commands.size() != 0) {
			printf("%20s%5s%s\n", "subcommands:", "", sub_commands.c_str());
		}
	} else {
		cur_parser->p_help();
	}
}

int command_parser::p_result(std::string parsername) {
	if(parsername.size() == 0) {
		parsername = command_name;
	}
	optparse::p_result(parsername);
}

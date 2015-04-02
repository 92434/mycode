#include "optparse.h"
#include <stdlib.h>

//--long_e --long_f -g  xiaofei test -t --long_g xx --long_d x -x xiaofei -c
//-a xiaofei -b -c xiaofei
//

int test_option_parser(int argc, char **argv) {
	int ret = -1;

	optparse opt;
	opt.add_option('a', true, "test a with arg");
	opt.add_option('b', false, "test b");
	opt.get_option(argc, argv);
	opt.p_help();
	printf("================================================\n");
	opt.p_result();

	return ret;
}

int test_long_option_parser(int argc, char **argv) {
	int ret = -1;

	optparse opt1;
	opt1.add_long_option("long_a", false, false, 'a', "test long_a");
	opt1.add_long_option("long_b", true, false, ' ', "test long_b with arg");
	opt1.add_long_option("long_c", false, true, 'c', "test long_c");
	opt1.add_long_option("long_d", false, false, 'd', "test long_d");
	opt1.add_long_option("long_e", true, false, 'e', "test long_e with arg");
	opt1.get_long_option(argc, argv);
	opt1.p_help();
	printf("================================================\n");
	opt1.p_result();

	return ret;
}

int test_command_parser(int argc, char **argv) {
	int ret = -1;
	command_parser *cp1, *cp2;

	command_parser cp("main");
	cp.add_option('a', true, "test a with arg");
	cp1 = cp.add_sub_command("sub1");
	if(cp1 == 0) {
		ret = -1;
		return ret;
	}
	cp1->add_option('b', false, "test b");
	cp2 = cp1->add_sub_command("sub2");
	if(cp2 == 0) {
		ret = -1;
		return ret;
	}
	cp2->add_option('c', true, "test c with arg");
	if(cp.get_option(argc, argv) != 0) {
		cp.p_help();

		ret = -1;
		return ret;
	}

	if(cp.get_curparser() != cp2) {
		cp.p_help();

		return ret;
	}

	cp.p_result();
	cp1->p_result();
	cp2->p_result();

	return ret;
}

int test_long_command_parser(int argc, char **argv) {
	int ret = -1;
	command_parser *cp1, *cp2;

	command_parser lcp("main");
	lcp.add_long_option("long_a", false, false, 'a', "test long_a");
	cp1 = lcp.add_sub_command("sub1");
	if(cp1 == 0) {
		ret = -1;
		return ret;
	}
	cp1->add_long_option("long_b", false, false, 'b', "test long_b");
	cp2 = cp1->add_sub_command("sub2");
	cp2->add_long_option("long_c", true, false, ' ', "test long_c with arg");
	cp2->add_long_option("long_d", false, true, 'd', "test long_d");
	cp2->add_long_option("long_e", false, false, 'e', "test long_e");
	cp2->add_long_option("long_f", true, false, 'f', "test long_f with arg");

	if(lcp.get_long_option(argc, argv) != 0) {
		lcp.p_help();

		ret = -1;
		return ret;
	}

	if(lcp.get_curparser() != cp2) {
		lcp.p_help();

		return ret;
	}

	lcp.p_result();
	cp1->p_result();
	cp2->p_result();

	return ret;
}

int main(int argc, char **argv) {
	int ret = 0;
	optparse opt;
	opt.add_option('t', true, "test case:1->optparse, 2->long optparse, 3->commandparse, 4->long commandparse");
	opt.get_option(argc, argv);

	int type = -1;

	if(opt.have_option("t")) {
		//std::string::size_type sz;//save value of end ptr
		//type = std::stoi(opt.option_value("t"), &sz, 10);

		char *pend;
		type = strtol(opt.option_value("t").c_str(), &pend, 10);
	}

	switch(type) {
		case 1:
			printf("xiaofei\n");
			test_option_parser(argc - 2, argv + 2);
			break;
		case 2:
			test_long_option_parser(argc - 2, argv + 2);
			break;
		case 3:
			test_command_parser(argc - 2, argv + 2);
			break;
		case 4:
			test_long_command_parser(argc - 2, argv + 2);
			break;
		default:
			opt.p_help();
			break;
	}

	return ret;
}

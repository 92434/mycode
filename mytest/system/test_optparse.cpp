#include "optparse.h"

//--long_e --long_f -g  xiaofei test -t --long_g xx --long_d x -x xiaofei -c
//-a xiaofei -b -c xiaofei
int main(int argc, char **argv) {
	int ret = 0;
	optparse opt;
	opt.add_option('a', true, "test a with arg");
	opt.add_option('b', false, "test b");
	opt.get_option(argc, argv);

	opt.add_long_option("long_c", false, false, 'c', "test long_c");
	opt.add_long_option("long_d", true, false, ' ', "test long_d with arg");
	opt.add_long_option("long_e", false, true, 'e', "test long_e");
	opt.add_long_option("long_f", false, false, 'f', "test long_f");
	opt.add_long_option("long_g", true, false, 'g', "test long_g with arg");
	opt.get_long_option(argc, argv);

	opt.p_help();
	opt.p_result();
	return ret;
}

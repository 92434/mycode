#include "log.h"
#include "data_process.h"
#include "optparse.h"
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
	int ret = 0;

	std::string infile = "xiaofei.db";
	std::string outfile = "hex.bin";

	optparse opt;
	opt.add_long_option("compress", false, false, 'C', "compress");
	opt.add_long_option("decompress", false, false, 'D', "decompress");
	opt.add_long_option("input-file", true, false, 'I', "input file name");
	opt.add_long_option("out-file", true, false, 'O', "output file name");

	opt.add_long_option("level", true, false, 'l', "output file name");
	//opt.add_long_option("dictsize", true, false, 'd', "(1 << 12) <= dictSize <= (1 << 27) for 32-bit version\n (1 << 12) <= dictSize <= (1 << 30) for 64-bit version\n default = (1 << 24)---for compress");
	opt.add_long_option("dictsize", true, false, 'd', "(1 << 12) <= dictSize <= (1 << 27) for 32-bit version, (1 << 12) <= dictSize <= (1 << 30) for 64-bit version, default = (1 << 24)---for compress");
	opt.add_long_option("lc", true, false, 'c', "0 <= lc <= 8, default = 3---for compress");
	opt.add_long_option("lp", true, false, 'p', "0 <= lp <= 4, default = 0---for compress");
	opt.add_long_option("pb", true, false, 'b', "0 <= pb <= 4, default = 2---for compress");
	opt.add_long_option("fb", true, false, 'f', "5 <= fb <= 273, default = 32---for compress");

	if(opt.get_long_option(argc, argv) != 0) {
		printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	opt.p_help();
	opt.p_result();

	//bool b_encrypt = opt.have_option("e");
	//bool b_decrypt = opt.have_option("d");
	//if(b_encrypt && b_decrypt) {
	//	printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//	ret = -1;
	//	return ret;
	//}

	//if(opt.have_option("i") && opt.have_option("o")) {
	//	infile = opt.option_value("i");
	//	outfile = opt.option_value("o");
	//}

	//printf("infile:%s, outfile:%s\n", infile.c_str(), outfile.c_str());

	//if(b_encrypt) {
	//	if(access(infile.c_str(), F_OK) != 0) {
	//		printf("error: %s(errno: %d)\n",strerror(errno),errno);
	//		ret = -1;
	//		return ret;
	//	}
	//	data_encrypt de(infile.c_str(), outfile.c_str());
	//	while(de.do_cache_file() != -1) {
	//		de.cache_process();
	//	}
	//	remove(infile.c_str());
	//}

	//if(b_decrypt) {
	//	if(access(outfile.c_str(), F_OK) != 0) {
	//		printf("error: %s(errno: %d)\n",strerror(errno),errno);
	//		ret = -1;
	//		return ret;
	//	}
	//	data_decrypt dd(outfile.c_str(), infile.c_str());
	//	while(dd.do_cache_file() != -1) {
	//		dd.cache_process();
	//	}
	//	remove(outfile.c_str());
	//}

	//log log;
	//log.set_logfile("test.txt");
	//std::cout << "This is written to the file" << std::endl;
	//log.reset_logfile();

	return ret;
}

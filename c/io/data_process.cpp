#include "log.h"
#include "data_process.h"
#include "optparse.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int ret = 0;

	std::string infile = "xiaofei.db";
	std::string outfile = "hex.bin";

	optparse opt;
	opt.add_option('e', false, "encrypt");
	opt.add_option('d', false, "decrypt");
	opt.add_option('i', true, "decrypt file name");
	opt.add_option('o', true, "encrypt file name");

	if(opt.get_option(argc, argv) != 0) {
		printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	opt.p_help();
	opt.p_result();

	bool b_encrypt = opt.have_option("e");
	bool b_decrypt = opt.have_option("d");
	if(b_encrypt && b_decrypt) {
		printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		ret = -1;
		return ret;
	}

	if(opt.have_option("i") && opt.have_option("o")) {
		infile = opt.option_value("i");
		outfile = opt.option_value("o");
	}

	printf("infile:%s, outfile:%s\n", infile.c_str(), outfile.c_str());

	if(b_encrypt) {
		if(access(infile.c_str(), F_OK) != 0) {
			printf("error: %s(errno: %d)\n",strerror(errno),errno);
			ret = -1;
			return ret;
		}
		data_encrypt de(infile.c_str(), outfile.c_str());
		while(de.do_cache_file() != -1) {
			de.cache_process();
		}
		remove(infile.c_str());
	}

	if(b_decrypt) {
		if(access(outfile.c_str(), F_OK) != 0) {
			printf("error: %s(errno: %d)\n",strerror(errno),errno);
			ret = -1;
			return ret;
		}
		data_decrypt dd(outfile.c_str(), infile.c_str());
		while(dd.do_cache_file() != -1) {
			dd.cache_process();
		}
		remove(outfile.c_str());
	}

	//log log;
	//log.set_logfile("test.txt");
	//std::cout << "This is written to the file" << std::endl;
	//log.reset_logfile();

	return ret;
}

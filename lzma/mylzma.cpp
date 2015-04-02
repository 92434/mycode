#include "log.h"
#include "data_process.h"
#include "optparse.h"
#include <string.h>
#include <errno.h>
#include "C/LzmaLib.h"

int mycompress(std::string in, std::string out, int level, unsigned dictsize, int lc, int lp, int pb, int fb) {
	int ret = 0;
	unsigned char *src_buffer;
	unsigned char *compress_buffer;
	size_t filesize;
	unsigned char prop[5];
	size_t prop_size = 5;
	size_t out_size;
	int numthreads = 2;/* 1 or 2, default = 2 */


	std::ifstream ifs(in.c_str());
	std::ofstream ofs(out.c_str());
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l1;
	}

	ifs.seekg (0, ifs.end);
	filesize = ifs.tellg();
	ifs.seekg (0, ifs.beg);
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l2;
	}

	src_buffer = new unsigned char[filesize]; //原始文件数据
	if(src_buffer == NULL) {
		ret = -1;
		goto l3;
	}
	compress_buffer = new unsigned char[filesize * 2]; //存放压缩数据
	if(compress_buffer == NULL) {
		ret = -1;
		goto l4;
	}

	ifs.read((char *)src_buffer, filesize);
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l4;
	}

	if (SZ_OK != LzmaCompress(compress_buffer, &out_size, src_buffer, filesize, prop, &prop_size, level, dictsize, lc, lp, pb, fb, numthreads)) {//出错了
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("压缩时出错！");
		ret = -1;
		goto l4;
	}

	if(ofs.good()) {
		ofs.write((const char *)compress_buffer, out_size);//写入压缩后的数据
	}

	if(!ofs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("创建文件出错！");
		ret = -1;
		goto l5;
	}
	

l5:
	ofs.close();
l4:
	delete [] compress_buffer;
l3:
	delete [] src_buffer;
l2:
	ifs.close();
l1:
	return ret;
}

int mydecompress(std::string in, std::string out) {
	int ret = -1;

	unsigned char *src_buffer;
	unsigned char *decompress_buffer;
	size_t filesize;
	size_t out_size;
	unsigned char prop[5];
	size_t prop_size = 5;

	std::ifstream ifs(in.c_str());
	std::ofstream ofs(out.c_str());
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l1;
	}

	ifs.seekg (0, ifs.end);
	filesize = ifs.tellg();
	ifs.seekg (0, ifs.beg);
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l2;
	}

	src_buffer = new unsigned char[filesize]; //原始文件数据
	if(src_buffer == NULL) {
		ret = -1;
		goto l3;
	}
	decompress_buffer = new unsigned char[filesize * 10]; //存放压缩数据
	if(decompress_buffer == NULL) {
		ret = -1;
		goto l4;
	}
	ifs.read((char *)src_buffer, filesize);
	if(!ifs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		ret = -1;
		goto l4;
	}

	if (SZ_OK != LzmaUncompress(decompress_buffer, &out_size, src_buffer, &filesize, prop, prop_size)) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("解压缩时出错！");
		ret = -1;
		goto l4;
	}

	if(ofs.good()) {
		ofs.write((char *)decompress_buffer, out_size);//写入压缩后的数据
	}
	if(!ofs.good()) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("创建文件出错！");
		ret = -1;
		goto l5;
	}

l5:
	ofs.close();
l4:
	delete [] decompress_buffer;
l3:
	delete [] src_buffer;
l2:
	ifs.close();
l1:
	return ret;
}

int main(int argc, char **argv) {
	int ret = 0;

	command_parser *cp1, *cp2;
	command_parser lcp("main");

	lcp.add_long_option("input-file", true, false, 'I', "input file name");
	lcp.add_long_option("out-file", true, false, 'O', "output file name");

	cp1 = lcp.add_sub_command("compress");
	if(cp1 == 0) {
		ret = -1;
		return ret;
	}
	cp1->add_long_option("level", true, false, 'l', "0 <= level <= 9, default = 5---for compress");
	cp1->add_long_option("dictsize", true, false, 'd', "(1 << 12) <= dictSize <= (1 << 27) for 32-bit version\n (1 << 12) <= dictSize <= (1 << 30) for 64-bit version\n default = (1 << 24)---for compress");
	cp1->add_long_option("lc", true, false, 'c', "0 <= lc <= 8, default = 3---for compress");
	cp1->add_long_option("lp", true, false, 'p', "0 <= lp <= 4, default = 0---for compress");
	cp1->add_long_option("pb", true, false, 'b', "0 <= pb <= 4, default = 2---for compress");
	cp1->add_long_option("fb", true, false, 'f', "5 <= fb <= 273, default = 32---for compress");

	cp2 = lcp.add_sub_command("decompress");
	if(cp2 == 0) {
		ret = -1;
		return ret;
	}


	if(lcp.get_long_option(argc, argv) != 0) {
		printf("%s:%s:%d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		lcp.p_help();

		ret = -1;
		return ret;
	}

	if(lcp.get_curparser() == &lcp) {
		lcp.p_help();

		ret = -1;
		return ret;
	}

	lcp.p_result();
	cp1->p_result();
	cp2->p_result();

	std::string infile;
	std::string outfile;

	if(!lcp.have_option("I")) {
		printf("%s\n", "no input file");
		ret = -1;
		infile = lcp.option_value("I");
		return ret;
	}

	if(!lcp.have_option("O")) {
		ret = -1;
		printf("%s\n", "no output file");
		outfile = lcp.option_value("O");
		return ret;
	}

	if(lcp.get_curparser() == cp1) {
		int level;
		unsigned dictsize;
		int lc;
		int lp;
		int pb;
		int fb;
		char *pend;
		level = cp1->have_option("l") ? strtol(cp1->option_value("t").c_str(), &pend, 10) : (5);
		dictsize = cp1->have_option("d") ? strtol(cp1->option_value("d").c_str(), &pend, 10) : (1 << 24);
		lc = cp1->have_option("c") ? strtol(cp1->option_value("c").c_str(), &pend, 10) : (3);
		lp = cp1->have_option("p") ? strtol(cp1->option_value("p").c_str(), &pend, 10) : (0);
		pb = cp1->have_option("b") ? strtol(cp1->option_value("b").c_str(), &pend, 10) : (2);
		fb = cp1->have_option("f") ? strtol(cp1->option_value("f").c_str(), &pend, 10) : (32);

		ret = mycompress(infile, outfile, level, dictsize, lc, lp, pb, fb);
	} else if(lcp.get_curparser() == cp2) {
		ret = mydecompress(infile, outfile);
	}


	return ret;
}

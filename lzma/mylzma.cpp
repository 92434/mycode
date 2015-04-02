#include "log.h"
#include "data_process.h"
#include "optparse.h"
#include <string.h>
#include <errno.h>

int mycompress(std::string in, std::string out, int level, int lc, int lp, int pb, int fb) {
	size_t destLen = filesize*2;
	unsigned char *psrcRead;
	unsigned char *pLzma;

	std::ifstream ifs(in);
	std::ofstream ofs(out);

	if(ifs.good()) {
		ifs.seekg (0, ifs.end);
		int length = ifs.tellg();
		ifs.seekg (0, ifs.beg);

		unsigned char *psrcRead = new unsigned char[length]; //原始文件数据
		unsigned char *pLzma = new unsigned char[length]; //存放压缩数据
		ifs.read(psrcRead, length);
		ifs.close();
	}

	unsigned char prop[5];
	size_t sizeProp = 5;

	if (SZ_OK != LzmaCompress(pLzma,&destLen,psrcRead,filesize,prop,&sizeProp,5,(1<<24),3,0,2,32,2)) {//出错了
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("压缩时出错！");
		delete psrcRead;
		delete pDecomress;
		delete pLzma;
		fclose(binfile);
		return -1;
	}

	FILE *compressfile = fopen("compress.dat","wb+"); //写入压缩后的数据
	if (compressfile == NULL)
	{
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("创建文件出错！");
		delete psrcRead;
		delete pDecomress;
		delete pLzma;
		fclose(binfile);
		return -1;
	}
	fwrite(pLzma,sizeof(char),destLen,compressfile);
	fclose(compressfile);
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
	cp1->add_long_option("level", true, false, 'l', "output file name");
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
		int level, lc, lp, pb, fb;
		level = cp1->have_option("l") ? cp1->option_value("l") : (1 << 24);
		lc = cp1->have_option("c") ? cp1->option_value("c") : (3);
		lp = cp1->have_option("p") ? cp1->option_value("p") : (0);
		pb = cp1->have_option("b") ? cp1->option_value("b") : (2);
		fb = cp1->have_option("f") ? cp1->option_value("f") : (32);
	}

	//if(lcp.get_curparser() == cp1) {

	//	unsigned char *psrcRead = new unsigned char[filesize]; //原始文件数据
	//	unsigned char *pDecomress = new unsigned char[filesize]; //存放解压缩数据
	//	unsigned char *pLzma = new unsigned char[destLen]; //存放压缩数据

	//	fread(psrcRead,sizeof(char),filesize,binfile);

	//	unsigned char prop[5];
	//	size_t sizeProp = 5;

	//	if (SZ_OK != LzmaCompress(pLzma,&destLen,psrcRead,filesize,prop,&sizeProp,5,(1<<24),3,0,2,32,2)) {//出错了
	//		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//		printf("%s:%m\n", strerror(errno));
	//		printf("压缩时出错！");
	//		delete psrcRead;
	//		delete pDecomress;
	//		delete pLzma;
	//		fclose(binfile);
	//		return -1;
	//	}
	//}

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

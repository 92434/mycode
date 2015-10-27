#ifndef __DATA_PROCESS_H
#define __DATA_PROCESS_H

#include "file_cache.h"

class data_process : public file_cache {
private:
	data_process() {
		std::cout << __PRETTY_FUNCTION__ << " not permit!!!" << std::endl;
	}
protected:
	std::ofstream ofs;
public:
	data_process(std::string infile, std::string outfile);
	~data_process();
	int right_roll(unsigned char *p, unsigned int n);

	int left_roll(unsigned char *p, unsigned int n);
	int xor_value(unsigned char *p, unsigned char mask);
};

class data_encrypt : public data_process {
private:
	data_encrypt() : data_process("", ""){
		std::cout << __PRETTY_FUNCTION__ << " not permit!!!" << std::endl;
	}

	int cache_update();
public:
	data_encrypt(std::string infile, std::string outfile) : data_process(infile, outfile) {
	}
};

class data_decrypt : public data_process {
private:
	data_decrypt() : data_process("", ""){
		std::cout << __PRETTY_FUNCTION__ << " not permit!!!" << std::endl;
	}

	int cache_update();
public:
	data_decrypt(std::string infile, std::string outfile) : data_process(infile, outfile) {
	}
};

#endif //#ifndef __DATA_PROCESS_H

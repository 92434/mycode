#include "data_process.h"

data_process::data_process(std::string infile, std::string outfile) {
	open(infile);
	ofs.open(outfile.c_str());
}

data_process::~data_process() {
	ofs.close();
}

int data_process::right_roll(unsigned char *p, unsigned int n) {
	int ret = 0;
	unsigned char ch = *p;
	unsigned char cl = *p;
	unsigned int i = sizeof(*p) * 8;
	n %= i;

	ch = *p << (i - n);
	cl = *p >> n;
	*p = ch | cl;

	return ret;
}

int data_process::left_roll(unsigned char *p, unsigned int n) {
	int ret = 0;
	unsigned char ch = *p;
	unsigned char cl = *p;
	unsigned int i = sizeof(*p) * 8;
	n %= i;

	ch = *p >> (i - n);
	cl = *p << n;
	*p = ch | cl;
	return ret;
}

int data_process::xor_value(unsigned char *p, unsigned char c) {
	int ret = 0;
	*p = *p ^ c;
	return ret;
}

int data_encrypt::cache_update() {
	int ret = 0;

	while(cache.cache_offset + 1 <= cache.cached) {
		unsigned int pos = cache.file_offset - cache.cached + cache.cache_offset;
		if(pos % 2 == 0) {
			left_roll(cache.content + cache.cache_offset, 3);
			xor_value(cache.content + cache.cache_offset, 0x55);
		} else {
			left_roll(cache.content + cache.cache_offset, 5);
			xor_value(cache.content + cache.cache_offset, 0xaa);
		}

		cache.cache_offset += 1;
	}

	if(ofs.good()) {
		ofs.write((char *)cache.content, cache.cached);
	} else {
		ifs.close();
		ret = -1;
	}

	return ret;
}

int data_decrypt::cache_update() {
	int ret = 0;

	while(cache.cache_offset + 1 <= cache.cached) {
		unsigned int pos = cache.file_offset - cache.cached + cache.cache_offset;
		if(pos % 2 == 0) {
			xor_value(cache.content + cache.cache_offset, 0x55);
			right_roll(cache.content + cache.cache_offset, 3);
		} else {
			xor_value(cache.content + cache.cache_offset, 0xaa);
			right_roll(cache.content + cache.cache_offset, 5);
		}

		cache.cache_offset += 1;
	}

	if(ofs.good()) {
		ofs.write((char *)cache.content, cache.cached);
	} else {
		ifs.close();
		ret = -1;
	}

	return ret;
}

#ifndef __FILE_CACHE_H
#define __FILE_CACHE_H
#include <stdio.h>
#include <strings.h>
#include <string.h>

#include <stdlib.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#define FILE_CONTENT_CACHE (1024 * 1024)

typedef struct _file_cache {
	int file_length;
	int file_offset;

	unsigned int file_cache_size;
	unsigned int cached;
	unsigned int cache_offset;
	unsigned char *content;
} file_cache_t;

class file_cache {
private:
	virtual int do_match();
	virtual int cache_update();
	int cache_match();
	int cache_clean();

protected:
	std::ifstream ifs;

	file_cache_t cache;

public:
	file_cache();
	~file_cache();
	int open(std::string s);
	int do_cache_file();
	int cache_search();
	int cache_process();
};
#endif//ifndef __FILE_CACHE_H

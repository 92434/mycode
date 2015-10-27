#ifndef __HEX_CHAR_SEARCH_H
#define __HEX_CHAR_SEARCH_H
#include "file_cache.h"

typedef struct _hex_char {
	bool valid;
	unsigned char value;
} hex_char_t;

class hex_char_search : public file_cache {
private:
	std::vector<hex_char_t> vhc;

	hex_char_search() {
		std::cout << __PRETTY_FUNCTION__ << " not permit!!!" << std::endl;
	}
	int do_match();


public:
	hex_char_search(std::string infile);

	int get_hex_string(const std::string &s);

	int p_hex_string();
};
#endif//ifndef __HEX_CHAR_SEARCH_H

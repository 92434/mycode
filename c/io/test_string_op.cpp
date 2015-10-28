#include "hex_char_search.h"

int main(int argc, char **argv) {
	printf("argc:%d\n", argc);
	if(argc != 3) {
		printf("error: para error!\n");
		return -1;
	}
	char *filename = argv[1];
	const char *hex_string = argv[2];

	hex_char_search hss(filename);
	hss.get_hex_string(hex_string);
	hss.p_hex_string();

	while(hss.do_cache_file() == 0) {
		hss.cache_search();
	}

	return 0;
}

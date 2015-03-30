#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 256
#define FILE_CONTENT_CACHE 1024 * 1024

char *get_char_buffer(int size) {
	char *buffer = (char *)malloc(size);
	if(buffer) {
		memset(buffer, 0, size);
	}

	return buffer;
}

typedef struct _hex_char {
	bool valid;
	unsigned char value;
} hex_char_t;

typedef struct _hex_chars {
	int count;
	hex_char_t hex_char[INPUT_BUFFER_SIZE];
} hex_chars_t;

typedef struct _file_cache {
	int totalread;
	int cached;
	int pos;
	unsigned char content[FILE_CONTENT_CACHE];
} file_cache_t;

unsigned char char2hex(char ch) {
	if(ch >= '0' && ch <= '9') {
		return ch - 0x30;
	} else if(ch >= 'a' && ch <= 'f') {
		return ch -'a' + 0x0a;
	} else if(ch >= 'A' && ch <= 'F') {
		return ch -'A' + 0x0a;
	} else {
		printf("error: input is not correct!(2)\n");
		exit(-1);
	}
}

unsigned char *get_hex_char(unsigned char *buffer, hex_chars_t *hex_chars) {
	if(*buffer == 0) {
		return NULL;
	} else if(*buffer == '.') {
		hex_chars->hex_char[hex_chars->count].valid = false;
		printf(".");
		hex_chars->count++;
		++buffer;
		return buffer;
	} else if(*(buffer+1) == '.') {
		printf("error: input is not correct!(1)\n");
		printf("pos:%s\n", buffer);
		exit(-1);
	}

	hex_chars->hex_char[hex_chars->count].valid = true;
	unsigned char high = char2hex(*buffer);
	unsigned char low = char2hex(*(buffer + 1));
	hex_chars->hex_char[hex_chars->count].value = high << 4 | low;
	//printf("pointer: %p\n", &(hex_chars->hex_char[hex_chars->count]));
	printf("%02x", hex_chars->hex_char[hex_chars->count].value);
	hex_chars->count++;
	buffer += 2;

	return buffer;
}

bool match(hex_chars_t *hex_chars, unsigned char * content) {
	bool ret = true;
	
	int i = 0;
	unsigned char *temp = content;

	while(i < hex_chars->count) {
		hex_char_t hex_char = hex_chars->hex_char[i];
		if(hex_char.valid) {
			if((*temp & 0xfc) != (hex_char.value & 0xfc)) {
				ret = false;
				break;
			}
		}
		
		i++;
		temp++;
	}

	return ret;
}

void cache_search(file_cache_t *file_cache, hex_chars_t *hex_chars) {
	static int index = 0;
	//printf("file cached %d\n", file_cache->cached);
	while(file_cache->pos + hex_chars->count  <= file_cache->cached) {
		if(match(hex_chars, file_cache->content + file_cache->pos)) {
			unsigned int addr_virtual_base = 0x80010000;
			unsigned int phy_addr = file_cache->totalread - file_cache->cached + file_cache->pos;
			printf("[%-10d]:%08x\n", index++, phy_addr);
			printf("print___:0x%08x\n", (((phy_addr + addr_virtual_base) & 0x0fffffff) >> 2) | 0x0c000000);

			file_cache->pos += hex_chars->count;
		} else {
			file_cache->pos++;
		}
	}

	if(file_cache->pos < file_cache->cached) {
		memcpy(file_cache->content, file_cache->content + file_cache->pos, file_cache->cached - file_cache->pos);
	}

	file_cache->cached = file_cache->cached - file_cache->pos;
	//printf("file cached %d\n", file_cache->cached);
	file_cache->pos = 0;
}

int main(int argc, char **argv) {
#if 0
	unsigned char *input = (unsigned char *)get_char_buffer(INPUT_BUFFER_SIZE);

	if(input == NULL) {
		printf("error: %m\n");
		return -1;
	}
	printf("input hex pattern>:");
	scanf("%s", input);

#else
	printf("argc:%d\n", argc);
	if(argc != 3) {
		printf("error: para error!\n");
		return -1;
	}
	unsigned char *input = (unsigned char *)argv[2];
#endif
	hex_chars_t *hex_chars = (hex_chars_t *)get_char_buffer(sizeof(hex_chars_t));
	if(hex_chars == NULL) {
		printf("error: %m\n");
		return -1;
	}

	printf("find:");
	unsigned char *buffer = input;
	while(buffer && *buffer) {
		buffer = get_hex_char(buffer, hex_chars);
	}
	printf("\n");
	printf("hex value count: %d\n", hex_chars->count);

	//for(int i = 0; i < hex_chars->count; i++) {
	//	hex_char_t hex_char = hex_chars->hex_char[i];
	//	if(hex_char.valid) {
	//		printf("%02x", hex_char.value);
	//	} else {
	//		printf(".");
	//	}
	//}
	//printf("\n");

	//unsigned char test_content[] = {0x88, 0xff, 0x88, 0xff, 0x88, 0xff, 0xff, 0x88};
	//printf("match: %s\n", match(hex_chars, test_content) ? "true" : "false");


	file_cache_t *file_cache = (file_cache_t *)get_char_buffer(sizeof(file_cache_t));
	if(file_cache == NULL) {
		printf("error: %m\n");
		return -1;
	}

#if 0
	printf("input bin file>:");
	scanf("%s", input);
#else
	input = (unsigned char *)argv[1];
#endif
	printf("open %s\n", input);
	FILE * binfile = fopen((const char *)input, "rb");
	if(binfile == NULL) {
		printf("error: %m\n");
		return -1;
	}

	if(fseek(binfile, 0, SEEK_END) != 0) {
		printf("error: %m\n");
		return -1;
	}

	int filelength = ftell(binfile);
	//printf("file length %d\n", filelength);

	if(fseek(binfile, 0, SEEK_SET) != 0) {
		printf("error: %m\n");
		return -1;
	}

	while(file_cache->totalread < filelength){
		int read = fread(file_cache->content + file_cache->cached, sizeof(unsigned char), FILE_CONTENT_CACHE - file_cache->cached, binfile);
		if(read <= 0) {
			printf("error: %m\n");
			exit(-1);
		}
		file_cache->totalread += read;
		file_cache->cached += read;
		//printf("read count %d\n", read);

		cache_search(file_cache, hex_chars);
	}

	fclose(binfile);

#if 0
	free(input);
#endif
	free(hex_chars);
	free(file_cache);

	return 0;
}


//diag_printf function pattern:...27...00...8f...27...af...af...af...00...00...00...af...0c...af...8f...03...27

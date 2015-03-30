#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <bitset>

#define FILE_CONTENT_CACHE 1024 * 1024

typedef struct _cmd {
	unsigned int part3 : 16;
	unsigned int part2 : 5;
	unsigned int part1 : 5;
	unsigned int opcode : 6;
} cmd_t;

typedef struct _file_cache {
	int totalread;
	int cached;
	int pos;
	unsigned char content[FILE_CONTENT_CACHE];
} file_cache_t;

const char *reg_name[] = {
	"zero",//	Constant 0
	"at",//	Reserved for assembler
	"v0",//	Expression evaluation and results of a function
	"v1",//	Expression evaluation and results of a function
	"a0",//	Argument 1
	"a1",//	Argument 2
	"a2",//	Argument 3
	"a3",//	Argument 4
	"t0",//	Temporary (not preserved across call)
	"t1",//	Temporary (not preserved across call)
	"t2",//	Temporary (not preserved across call)
	"t3",//	Temporary (not preserved across call)
	"t4",//	Temporary (not preserved across call)
	"t5",//	Temporary (not preserved across call)
	"t6",//	Temporary (not preserved across call)
	"t7",//	Temporary (not preserved across call)
	"s0",//	Saved temporary (preserved across call)
	"s1",//	Saved temporary (preserved across call)
	"s2",//	Saved temporary (preserved across call)
	"s3",//	Saved temporary (preserved across call)
	"s4",//	Saved temporary (preserved across call)
	"s5",//	Saved temporary (preserved across call)
	"s6",//	Saved temporary (preserved across call)
	"s7",//	Saved temporary (preserved across call)
	"t8",//	Temporary (not preserved across call)
	"t9",//	Temporary (not preserved across call)
	"k0",//	Reserved for OS kernel
	"k1",//	Reserved for OS kernel
	"gp",//	Pointer to global area
	"sp",//	Stack pointer
	"fp(s8)",//	Frame pointer
	"ra",//	Return address (used by function call)
};

int code_pos = 0;
int code_off = 0;

class c_common{
public:
	c_common(unsigned int opcode = 0xffffffff) : cmd_opcode(opcode) {
		//printf("c_common::c_commonc_common(cmd_t &cmd_in)\n");
	}

	unsigned int opcode() {
		//printf("c_common::opcode():%x\n", cmd_opcode);
		return cmd_opcode;
	}

	virtual int decode(cmd_t *cmd) {
		//printf("c_common::decode()\n");
		//

		printf("%08x:", code_pos);

		std::bitset<6> bits_opcode(cmd->opcode);
		std::bitset<5> bits_part1(cmd->part1);
		std::bitset<5> bits_part2(cmd->part2);
		std::bitset<16> bits_part3(cmd->part3);
		printf("%s %s %s %s//",
			bits_opcode.to_string().c_str(),
			bits_part1.to_string().c_str(),
			bits_part2.to_string().c_str(),
			bits_part3.to_string().c_str()
			);
		printf("0x%02x 0x%02x 0x%02x 0x%08x//",
			cmd->opcode,
			cmd->part1,
			cmd->part2,
			cmd->part3
		);
		return 0;
	}
private:
	int cmd_opcode;
};

class c_001101 : public c_common {
public:
	c_001101() : c_common(0x0d) {
		//printf("c_001101::c_001101(cmd_t &cmd_in)\n");
	}

	int decode(cmd_t *cmd) {
		//printf("c_001101::decode()\n");
		c_common::decode(cmd);

		printf("ori %s, %s, 0x%08x//",
			reg_name[cmd->part2],
			reg_name[cmd->part1],
			cmd->part3
		);
		printf("%s = %s | 0x%08x//",
			reg_name[cmd->part2],
			reg_name[cmd->part1],
			cmd->part3
		);
	}
};

class c_001111 : public c_common {
public:
	c_001111() : c_common(0x0f) {
		//printf("c_001101::c_001101(cmd_t &cmd_in)\n");
	}

	int decode(cmd_t *cmd) {
		//printf("c_001101::decode()\n");
		c_common::decode(cmd);

		printf("lui %s, 0x%08x//",
			reg_name[cmd->part2],
			cmd->part3
		);
		printf("%s = 0x%08x//",
			reg_name[cmd->part2],
			cmd->part3 << 16
		);
	}
};

class c_000100 : public c_common {
public:
	c_000100() : c_common(0x04) {
		//printf("c_001101::c_001101(cmd_t &cmd_in)\n");
	}

	int decode(cmd_t *cmd) {
		//printf("c_001101::decode()\n");
		c_common::decode(cmd);

		if(cmd->part1 == 0 && cmd->part2 == 0) {
			printf("b 0x%08x//",
				cmd->part3
			);
			printf("PC = 0x%08x//",
				code_pos + 4 * 2 + cmd->part3 * 4
			);
		} else {
			printf("beq %s, %s, 0x%08x//",
				reg_name[cmd->part1],
				reg_name[cmd->part2],
				cmd->part3
			);
			printf("if %s == %s then PC = 0x%08x//",
				reg_name[cmd->part1],
				reg_name[cmd->part2],
				code_pos + 4 * 2 + cmd->part3 * 4
			);
		}
	}
};

class c_000010 : public c_common {
public:
	c_000010() : c_common(0x02) {
		//printf("c_001101::c_001101(cmd_t &cmd_in)\n");
	}

	int decode(cmd_t *cmd) {
		//printf("c_001101::decode()\n");
		c_common::decode(cmd);

		printf("j 0x%08x//",
			cmd->part1 << 21 | cmd->part2 << 16 | cmd->part3
		);
		printf("PC = 0x%08x//",
			//((cmd->part1 << 21 | cmd->part2 << 16 | cmd->part3) << 2) | (code_pos & 0xfc000000)
			((cmd->part1 << 21 | cmd->part2 << 16 | cmd->part3) << 2) | (0x80008000 & 0xfc000000)
		);
	}
};

class c_100000 : public c_common {
public:
	c_100000() : c_common(0x20) {
		//printf("c_001101::c_001101(cmd_t &cmd_in)\n");
	}

	int decode(cmd_t *cmd) {
		//printf("c_001101::decode()\n");
		c_common::decode(cmd);

		printf("lb %s, 0x%08x(%s)//",
			reg_name[cmd->part2],
			cmd->part3,
			reg_name[cmd->part1]
		);
		printf("%s = %s[0x%08x]//",
			reg_name[cmd->part2],
			reg_name[cmd->part1],
			cmd->part3
		);
	}
};

char *get_char_buffer(int size) {
	char *buffer = (char *)malloc(size);
	if(buffer) {
		memset(buffer, 0, size);
	}

	return buffer;
}

std::map<int, c_common *> cmd_map;

void pool_add_cmd_type(c_common *cmd) {
	//printf("key:%x value:%p\n", cmd->opcode(), cmd);
	cmd_map[cmd->opcode()] = cmd;
}

void init_cmd_pool() {
	for(std::map<int, c_common *>::iterator i = cmd_map.begin(); i != cmd_map.end(); i++) {
		delete i->second;
	}

	cmd_map.clear();

	pool_add_cmd_type(new c_common());
	pool_add_cmd_type(new c_001101());
	pool_add_cmd_type(new c_001111());
	pool_add_cmd_type(new c_000100());
	pool_add_cmd_type(new c_000010());
	pool_add_cmd_type(new c_100000());
}

int cmd_process(cmd_t *pcmd_in){
	std::map<int, c_common *>::iterator i = cmd_map.find(pcmd_in->opcode);
	if(i == cmd_map.end()) {
		i = cmd_map.find(0xffffffff);
	}
	if(i == cmd_map.end()) {
		printf("error: !!!\n");
	}
	i->second->decode(pcmd_in);
	printf("\n");
	return 0;
}

void cache_process(file_cache_t *file_cache) {
	static int index = 0;
	printf("cmd_t size is: %d\n", sizeof(cmd_t));
	while(file_cache->pos + sizeof(cmd_t) <= file_cache->cached) {
		cmd_t *cmd = (cmd_t *)(file_cache->content + file_cache->pos);
		code_pos = file_cache->totalread - file_cache->cached + file_cache->pos + code_off;
		cmd_process(cmd);
		file_cache->pos += sizeof(cmd_t);
	}

	if(file_cache->pos < file_cache->cached) {
		memcpy(file_cache->content, file_cache->content + file_cache->pos, file_cache->cached - file_cache->pos);
	}

	file_cache->cached = file_cache->cached - file_cache->pos;
	//printf("file cached %d\n", file_cache->cached);
	file_cache->pos = 0;
}

int file_process(unsigned char *filename, int start, int size) {
	int cache_size = size > FILE_CONTENT_CACHE ? FILE_CONTENT_CACHE : size;
	FILE * binfile = fopen((const char *)filename, "rb");
	if(binfile == NULL) {
		printf("error: %m\n");
		return -1;
	}

	if(size == 0) {
		if(fseek(binfile, 0, SEEK_END) != 0) {
			printf("error: %m\n");
			return -1;
		}

		size = ftell(binfile);
	}

	if(size <= 0) {
		return -1;
	}

	if(fseek(binfile, start, SEEK_SET) != 0) {
		printf("error: %m\n");
		return -1;
	}

	code_off = start;

	file_cache_t *file_cache = (file_cache_t *)get_char_buffer(sizeof(file_cache_t));
	if(file_cache == NULL) {
		printf("error: %m\n");
		return -1;
	}

	while(file_cache->totalread < size){
		int read = fread(file_cache->content + file_cache->cached, sizeof(unsigned char), cache_size, binfile);
		if(read <= 0) {
			printf("error: %m\n");
			return -1;
		}
		file_cache->totalread += read;
		file_cache->cached += read;
		//printf("read count %d\n", read);

		cache_process(file_cache);
	}

	free(file_cache);

	fclose(binfile);
}

int main(int argc, char **argv) {
	if(argc != 4) {
		printf("error: para error!\n");
		return -1;
	}

	char *pend;
	unsigned char *filename = (unsigned char *)argv[1];
	int start = strtol(argv[2], &pend, 16);
	int size = strtol(argv[3], &pend, 16);
	printf("filename:%s start:%08x size:%08x\n", filename, start, size);

	init_cmd_pool();
	file_process(filename, start, size);

	return 0;
}

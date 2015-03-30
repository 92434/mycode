#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_char_buffer(int size) {
	char *buffer = (char *)malloc(size);
	if(buffer) {
		memset(buffer, 0, size);
	}

	return buffer;
}

static int myread(FILE *binfile, char *buffer, int offset, int size) {
	if(fseek(binfile, offset, SEEK_SET) != 0) {
		printf("error: %m\n");
		exit(-1);
	}

	int toread = size;
	while(toread > 0) {
		int read = fread(buffer, sizeof(unsigned char), toread, binfile);
		if(read <= 0) {
			printf("error: %m\n");
			exit(-1);
		}
		toread -= read;
		buffer += read;
	}

	return size - toread;

}

static int mywrite(FILE *binfile, char *buffer, int offset, int size) {
	if(fseek(binfile, offset, SEEK_SET) != 0) {
		printf("error: %m\n");
		exit(-1);
	}

	int towrite = size;
	while(towrite > 0) {
		int count;
		count = fwrite(buffer , sizeof(char), towrite, binfile);
		if(count == 0) {
			printf("error: %m\n");
			exit(-1);
		}

		towrite -= count;
		buffer += count;
	}

	return size - towrite;
}

int main(int argc, char **argv) {
	printf("argc:%d\n", argc);
	if(argc != 7) {
		printf("error: para error!\n");
		return -1;
	}


	char *pend;
	char *infile = argv[1];
	int instart = strtol(argv[2], &pend, 16);
	int insize = strtol(argv[3], &pend, 16);
	char *outfile = argv[4];
	int outstart = strtol(argv[5], &pend, 16);
	int outsize = strtol(argv[6], &pend, 16);
	printf("infile:%s, instart:%08x, insize:%08x, outfile:%s, outstart:%08x, outsize:%08x\n", infile, instart, insize, outfile, outstart, outsize);


	FILE * inbinfile = fopen((const char *)infile, "rb");
	if(inbinfile == NULL) {
		printf("error: %m\n");
		return -1;
	}

	if(insize == 0) {
		if(fseek(inbinfile, 0, SEEK_END) != 0) {
			printf("error: %m\n");
			exit(-1);
		}

		if((insize = ftell(inbinfile)) == -1) {
			printf("error: %m\n");
			exit(-1);
		}
	}

	char *buffer = get_char_buffer(insize);

	insize = myread(inbinfile, buffer, instart, insize);

	printf("read 0x%08x bytes!\n", insize);

	fclose(inbinfile);

	if(outsize == 0 || outsize > insize) {
		outsize = insize;
	}

	FILE * outbinfile = fopen((const char *)outfile, "rb+");
	if(outbinfile == NULL) {
		printf("error: %m\n");
		outbinfile = fopen((const char *)outfile, "wb");
	}
	if(outbinfile == NULL) {
		printf("error: %m\n");
		exit(-1);
	}

	outsize = mywrite(outbinfile, buffer, outstart, outsize);

	printf("write 0x%08x bytes!\n", outsize);

	fclose(outbinfile);
	return 0;
}

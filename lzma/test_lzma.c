// 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "C/LzmaLib.h"

int main(int argc, char** argv)
{
	FILE *binfile = fopen("file.dat", "rb+");
	if (binfile == NULL)
	{
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		return -1;
	}

	if(fseek(binfile, 0, SEEK_END) != 0) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		return -1;
	}

	size_t filesize = ftell(binfile);
	if(filesize == -1) {
		return -1;
	}

	rewind(binfile);

	size_t destLen = filesize*2;
	printf("destLen:%d\n", destLen);
	unsigned char *psrcRead = new unsigned char[filesize]; //原始文件数据
	unsigned char *pDecomress = new unsigned char[filesize]; //存放解压缩数据
	unsigned char *pLzma = new unsigned char[destLen]; //存放压缩数据

	fread(psrcRead,sizeof(char),filesize,binfile);

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
	printf("destLen:%d\n", destLen);
	fwrite(pLzma,sizeof(char),destLen,compressfile);
	fclose(compressfile);

	FILE *decompressfile = fopen("decompress.dat", "wb+"); //写入解压缩数据
	if (decompressfile == NULL) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("创建文件出错！");
		delete psrcRead;
		delete pDecomress;
		delete pLzma;
		fclose(binfile);
		return -1;
	}

	if (SZ_OK != LzmaUncompress(pDecomress,&filesize,pLzma,&destLen,prop,5)) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s:%m\n", strerror(errno));
		printf("解压缩时出错！");
		delete psrcRead;
		delete pDecomress;
		delete pLzma;
		fclose(decompressfile);
		fclose(binfile);
		return -1;
	}
	fwrite(pDecomress,sizeof(char),filesize,decompressfile);
	delete psrcRead;
	delete pDecomress;
	delete pLzma;
	fclose(decompressfile);

//	fclose(binfile);

	return 0;
}

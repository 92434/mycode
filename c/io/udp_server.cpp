#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include<unistd.h>

int port = 10000;

int check_buffer(unsigned int *pdata, int count, unsigned char *pre_value) {
	int rtn = 0;
	int i;
	unsigned char c0, c1;

	c0 = c1 = *pre_value;

	for(i = 0; i < count; i++) {
		c1 = pdata[i] & 0xff;
		switch(c0) {
			case 0x12:
				if(c1 != 0x34) {
					goto failed;
				}
				break;
			case 0x34:
				if(c1 != 0x56) {
					goto failed;
				}
				break;
			case 0x56:
				if(c1 != 0x78) {
					goto failed;
				}
				break;
			case 0x78:
				if(c1 != 0x90) {
					goto failed;
				}
				break;
			case 0x90:
				if(c1 != 0x12) {
					goto failed;
				}
				break;
			default:
				break;
		}
		c0 = c1;
	}

	*pre_value = c0;
	//printf("!!!success!!!\n");
	return rtn;
failed:
	printf("!!!failed!!!(%d)\n", i);
	rtn = -1;
	return rtn;
}

static int receive_data(int sockfd, unsigned char *buf, unsigned int len, struct sockaddr *sockaddr) {
	FILE *f = fopen("i2s.dat", "wb+");
	if(f == 0) {
		printf("open file error:%s\n", strerror(errno));
	}

	while(1)
	{
		int i;
		int z;

		z = recvfrom(sockfd, buf, sizeof(buf), 0, sockaddr, &len);
		if (z < 0)
		{
			perror("recvfrom 出错");
			exit(1);
		}
		//buf[z] = 0;
		//printf("接收:%s",buf);

		//if (strncmp(buf, "stop", 4) == 0)
		//{
		//	printf("结束..\n");
		//	break;
		//}
		//printf("received len %d\n", z);
		unsigned int *pdata;
		pdata = (unsigned int *)buf;
		static bool value001 = false;
		static bool value002 = false;
		static bool disable_write = false;

		static unsigned char pre_value = 0;
		check_buffer(pdata, z / sizeof(unsigned int), &pre_value);

		for(i = 0; i < z / sizeof(unsigned int); i++) {
			if((i != 0) && (i % 16 == 0)) {
				//printf("\n");
			}
			//printf("%08x ", pdata[i] & 0x7fffffff);
			unsigned short value0 = (pdata[i] >> 8) & 0xffff;
			unsigned short value = (value0 << 8) | (value0 >> 8);
			if(value == 0) {
				if(value001 == false) {
					value001 = true;
				} else {
					if(value002 == false) {
						value002 = true;
						fwrite("!!", sizeof(unsigned char), 2, f);
					} else {
						disable_write = true;
					}
				}
			} else {
				disable_write = value001 = value002 = false;
			}
			if(disable_write == true) {
				continue;
			}
			fwrite((char *)&value, sizeof(unsigned short), 1, f);
		}
		//printf("\n");
	}
	close(sockfd);
	fclose(f);
}

int main()
{
	int sockfd;
	unsigned int len;
	unsigned char buf[256];
	struct sockaddr_in adr_inet;
	struct sockaddr_in adr_clnt;
	printf("等待客户端...\n");
	//建立IP地址
	
	adr_inet.sin_family = AF_INET;
	adr_inet.sin_port   = htons(port);
	adr_inet.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(adr_inet.sin_zero),8);
	len = sizeof(adr_clnt);
	//建立socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		perror("socket 出错");
		exit(1);
	}

	int b;

	//bind socket
	b = bind (sockfd, (struct sockaddr *)&adr_inet, sizeof (adr_inet));
	if (b == -1)
	{
		perror("bind出错");
		exit(1);
	}

	receive_data(sockfd, buf, len, (struct sockaddr *)&adr_clnt);

	exit(0);
}

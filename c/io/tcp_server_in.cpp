#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAXLINE 4096

//int check_buffer(unsigned int *pdata, int count, unsigned char *pre_value) {
//	int rtn = 0;
//	int i;
//	unsigned char c0, c1;
//
//	c0 = c1 = *pre_value;
//
//	for(i = 0; i < count; i++) {
//		c1 = pdata[i] & 0xff;
//		switch(c0) {
//			case 0x12:
//				if(c1 != 0x34) {
//					goto failed;
//				}
//				break;
//			case 0x34:
//				if(c1 != 0x56) {
//					goto failed;
//				}
//				break;
//			case 0x56:
//				if(c1 != 0x78) {
//					goto failed;
//				}
//				break;
//			case 0x78:
//				if(c1 != 0x90) {
//					goto failed;
//				}
//				break;
//			case 0x90:
//				if(c1 != 0x12) {
//					goto failed;
//				}
//				break;
//			default:
//				break;
//		}
//		c0 = c1;
//	}
//
//	*pre_value = c0;
//	//printf("!!!success!!!\n");
//	return rtn;
//failed:
//	printf("!!!failed!!!(%d)\n", i);
//	rtn = -1;
//	return rtn;
//}

static int receive_data(int sockfd, unsigned char *buf, unsigned int len) {
	int rtn = 0;
	//FILE *f = fopen("i2s.dat", "wb+");
	//if(f == 0) {
	//	printf("open file error:%s\n", strerror(errno));
	//}

	while(1)
	{
		int i;
		int z;

		z = recv(sockfd, buf, len, 0);
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

		//static unsigned char pre_value = 0;
		//check_buffer(pdata, z / sizeof(unsigned int), &pre_value);

		for(i = 0; i < z / sizeof(unsigned int); i++) {
			if((i != 0) && (i % 10 == 0)) {
				printf("\n");
			}
			printf("%10d ", pdata[i] & 0x7fffffff);
			//fwrite((char *)pdata, sizeof(unsigned int), 1, f);
		}
		printf("\n");
	}
	close(sockfd);
	//fclose(f);

	return rtn;
}

int main(int argc, char** argv)
{
	int    listenfd, connfd;
	struct sockaddr_in     servaddr;
	char    buff[4096];

	if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	//协议地址
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(10000);//
	//bind把一个地址族中的特定地址赋给socket
	if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if( listen(listenfd, 10) == -1){
	//listen 监听
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	printf("======waiting for client's request======\n");
	while(1){
		if((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
			printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
			continue;
		}

		//int     n;
		//n = recv(connfd, buff, MAXLINE, 0);
		//buff[n] = '\0';
		//printf("recv msg from client: %s\n", buff);
		//close(connfd);

		receive_data(connfd, (unsigned char *)buff, MAXLINE);
	 }

		close(listenfd);
}

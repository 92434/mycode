#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main()
{
	int fd_socket;
	int fd_connect;
	int fd_server;
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	fd_socket= socket(AF_INET, SOCK_STREAM, 0);
	if(fd_socket < 0)
	{
		printf("Init socket failed!\n");
		return -1;
	}
	int iOption_value = 1;
	int iLength = sizeof(int);
	if(setsockopt(fd_socket,SOL_SOCKET,SO_REUSEADDR,&iOption_value, iLength)<0)
	{
		printf("setsockopt error\n");
		return -1;
	}
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(6666);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(fd_socket,(struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
	{
		perror("bind");
		return -1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1234);
	serv_addr.sin_addr.s_addr = inet_addr("10.10.10.66");
	if(connect(fd_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("connect error\n");
		return -1;
	}
	memset(&client_addr, 0, sizeof(client_addr));
	int iAddrLen = sizeof(client_addr);
	if(getsockname(fd_socket, (struct sockaddr *)&client_addr, &iAddrLen) < 0)
	{
		printf("getsockname\n");
		return -1;
	}
	printf("the port is %d\n", htons(client_addr.sin_port));
	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
	{
		printf("Init socket failed!\n");
		return -1;
	}
	if(setsockopt(fd_server,SOL_SOCKET,SO_REUSEADDR,&iOption_value, iLength)<0)
	{
		printf("setsockopt error\n");
		return -1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = client_addr.sin_port;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(fd_server,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("bind");
		return -1;
	}
	if(listen(fd_server, 12) < 0)
	{
		perror("listen");        
	}
	printf("listen success\n");
	sleep(20);

	return 0;
}

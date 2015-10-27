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
	struct sockaddr_in serv_addr;
	fd_socket= socket(AF_INET, SOCK_STREAM, 0);
	if(fd_socket < 0)
	{
		printf("Init socket failed!\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1043);
	serv_addr.sin_addr.s_addr = inet_addr("10.10.10.77");
	if(connect(fd_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("connect error\n");
		return -1;
	}
	printf("connect success\n");
	sleep(20);        
	return 0;
}

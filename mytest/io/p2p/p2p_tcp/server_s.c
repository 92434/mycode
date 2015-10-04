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
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	socklen_t cli_len;

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
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1234);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(fd_socket, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("bind");
		printf("Bind failed!\n");
		return -2;
	}
	if(listen(fd_socket, 4) < 0)
	{
		printf("Listen failed!\n");
	}
	printf("start to accept!\n");
	while(1)
	{
		fd_connect = accept(fd_socket, (struct sockaddr *)&client_addr, &cli_len);
		printf("the ServerA ip is %s, port is %d\n",inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));
		usleep(0);
	}
	printf("over\n");

	return 0;
}

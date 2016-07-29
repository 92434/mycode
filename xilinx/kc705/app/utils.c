#include <stdio.h>     
#include <unistd.h>    
#include <signal.h>
#include <stdlib.h>   
#include <errno.h>    
#include <string.h>
#include <fcntl.h>    
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "utils.h"

static void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);
	/* Ensure it's our signal */
	exit(0);

	return;
}

int catch_signal(sig_action_t sig_action) {
	int ret = 0;
	struct sigaction sig_act;

	if(sig_action == 0) {
		sig_action = default_sig_action;
	}

	/* Set up the signal handler */
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_SIGINFO;
	sig_act.sa_sigaction = sig_action;

	/* Map the Signal to the Signal Handler */
	if((ret = sigaction(SIGINT, &sig_act, NULL)) == -1) {
		perror("sigaction");
		return ret;
	}

	return ret;
}


int init_udp_client(unsigned char *ip_addr, unsigned int port, struct sockaddr_in *sock_addr) {
	int ret = 0;
	memset(sock_addr, 0, sizeof(*sock_addr));

	//协议地址
	sock_addr->sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr->sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr->sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	return ret;
}

int init_tcp_client(unsigned char *ip_addr, unsigned int port, struct sockaddr_in *sock_addr) {
	int ret = 0;
	memset(sock_addr, 0, sizeof(*sock_addr));

	//协议地址
	sock_addr->sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr->sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr->sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if(connect(ret, (struct sockaddr *)sock_addr, sizeof(*sock_addr)) < 0){
		//连接请求
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	}

	return ret;
}

int udp_send_data(int fd, unsigned char *buffer, unsigned int len, struct sockaddr_in *sock_addr) {
	int ret = 0;
	unsigned int step = 256;

	while(len != 0) {
		ret = sendto(fd, buffer, step, 0, (struct sockaddr *)sock_addr, sizeof(*sock_addr));
		if(ret <= 0) {
			printf("err: para error!:%s\n", strerror(errno));
		} else {
			//printf("send data len %d!\n", ret);
		}

		len -= step;
		buffer += step;
	}
	return ret;
}

int tcp_send_data(int fd, unsigned char *buffer, unsigned int len, struct sockaddr_in *sock_addr) {
	int ret = 0;
	unsigned int step = 1024;

	while(len != 0) {
		ret = send(fd, buffer, step, 0);
		if(ret <= 0) {
			printf("err: para error!:%s\n", strerror(errno));
		} else {
			//printf("send data len %d!\n", ret);
		}

		len -= step;
		buffer += step;
	}
	return ret;
}

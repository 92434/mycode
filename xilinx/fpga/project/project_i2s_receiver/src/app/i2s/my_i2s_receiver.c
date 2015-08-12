#include <stdio.h>     
#include <unistd.h>    
#include <signal.h>
#include <stdlib.h>   
#include <errno.h>    
#include <string.h>
#include <fcntl.h>    

#include <netinet/in.h>

#include "xdma.h"

#define BUFSIZE (DMA_LENGTH)

static int stop = 0;

typedef void (*sig_action_t)(int, siginfo_t *, void *);

void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called!\n", __func__);
	/* Ensure it's our signal */
	if(stop == 0) {
		stop = 1;
	} else {
		exit(0);
	}

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

int check_buffer(unsigned int *pdata, int count, unsigned int *pre_value) {
	int ret = 0;
	int i;
	unsigned int ui0, ui1;

	ui0 = ui1 = *pre_value;

	for(i = 0; i < count; i++) {
		ui1 = pdata[i] & 0x7fffffff;
		
		if(ui1 == ui0 + 1) {
		} else if(ui1 == 0) {
		} else if(ui0 == 0) {
		} else {
			ret = -1;
			printf("!!!failed!!!(%d)\n", i);
			printf("ui0:%010d ui1:%010d\n", ui0, ui1);
		}

		ui0 = ui1;
	}

	*pre_value = ui0;
	return ret;
}

static struct sockaddr_in sock_addr;

static int init_udp_client(unsigned char *ip_addr, unsigned int port) {
	int ret = 0;
	memset(&sock_addr, 0, sizeof(sock_addr));

	//协议地址
	sock_addr.sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr.sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr.sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	return ret;
}

static int init_tcp_client(unsigned char *ip_addr, unsigned int port) {
	int ret = 0;
	memset(&sock_addr, 0, sizeof(sock_addr));

	//协议地址
	sock_addr.sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr.sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr.sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if(connect(ret, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0){
		//连接请求
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	}

	return ret;
}

static int udp_send_data(int fd, unsigned char *buffer, unsigned int len) {
	int ret = 0;
	unsigned int step = 256;

	while(len != 0) {
		ret = sendto(fd, buffer, step, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
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

static int tcp_send_data(int fd, unsigned char *buffer, unsigned int len) {
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

#if 0
static read_buffer(int fd, int sock_fd, unsigned char *read_buf) {
	struct timeval tv;
	fd_set fds;

	tv.tv_sec=0;
	tv.tv_usec=10;
	while(stop == 0) {
		//printf("wait...\n");
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		if(select(fd + 1, &fds, NULL, NULL, &tv) > 0) {
			if(FD_ISSET(fd, &fds)) {
				int nread;
				unsigned int *pdata = (unsigned int *)read_buf;
				int i;
				static unsigned int pre_value = 0;

				nread = read(fd, read_buf, BUFSIZE);
				//printf("nread:%d\n", nread);
				if(nread <= 0) {
					continue;
				}

				//udp_send_data(sock_fd, read_buf, nread);
				tcp_send_data(sock_fd, read_buf, nread);
				//for(i = 0; i < nread / sizeof(unsigned int); i++) {
				//	if(i % 16 == 0) {
				//		printf("\n");
				//	}
				//	printf("%02x ", (pdata[i] >> 8) & 0xff);
				//}
				//printf("\n");
				//check_buffer(pdata, nread / sizeof(unsigned int), &pre_value);
			}
		}
	}
}
#else
static read_buffer(int fd, int sock_fd, unsigned char *read_buf) {
	struct timeval tv;
	fd_set fds;

	while(stop == 0) {
		//printf("wait...\n");
		int nread;
		unsigned int *pdata = (unsigned int *)read_buf;
		int i;
		static unsigned int pre_value = 0;

		nread = read(fd, read_buf, BUFSIZE);
		//printf("nread:%d\n", nread);
		if(nread <= 0) {
			continue;
		}

		//udp_send_data(sock_fd, read_buf, nread);
		tcp_send_data(sock_fd, read_buf, nread);
		//for(i = 0; i < nread / sizeof(unsigned int); i++) {
		//	if(i % 16 == 0) {
		//		printf("\n");
		//	}
		//	printf("%02x ", (pdata[i] >> 8) & 0xff);
		//}
		//printf("\n");
		//check_buffer(pdata, nread / sizeof(unsigned int), &pre_value);
	}
}
#endif

int main(int argc, char **argv) {
	int ret = 0;
	int fd, sock_fd;
	unsigned char *read_buf;
	char *dev = argv[1];

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	//sock_fd = init_udp_client("192.168.1.170", 10000);
	//sock_fd = init_udp_client("192.168.1.210", 10000);
	//sock_fd = init_tcp_client("192.168.1.210", 10000);
	sock_fd = init_tcp_client("192.168.1.175", 10000);

	if ((fd = open(dev, O_RDONLY))<0) {
		printf("err: can't open serial port!(%s)\n", strerror(errno));
		ret = -1;
		return ret;
	}

	int flags = fcntl(fd, F_GETFL, 0);

	//用以下方法将socket设置为非阻塞方式
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	if(catch_signal(0) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	read_buf = (unsigned char *)malloc(BUFSIZE);
	if(read_buf == 0) {
		printf("err: no memory for read_buf!\n");
		ret = -1;
		return ret;
	}

	read_buffer(fd, sock_fd, read_buf);

	free(read_buf);

	close(fd);

	return ret;
}

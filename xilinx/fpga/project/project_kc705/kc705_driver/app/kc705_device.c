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

#define BUFSIZE 0x1000

#include "kc705.h"

typedef void (*sig_action_t)(int, siginfo_t *, void *);

static int stop = 0;
static char *mmap_memory = NULL;
static int mmap_size = 0;


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

int main(int argc, char **argv) {
	int ret = 0;
	int fd, sock_fd;
	unsigned char *read_buf;
	char *dev = argv[1];
	char *op = argv[2];
	int flags;
	int opcode = 0;

	if(argc < 3) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((fd = open(dev, O_RDONLY))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev, strerror(errno));
		ret = -1;
		return ret;
	}

	flags = fcntl(fd, F_GETFL, 0);
	//用以下方法将socket设置为非阻塞方式
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	if(strncmp(op, "start", strlen("start")) == 0) { 
		opcode = PCIE_DEVICE_IOCTL_START_DMA_THREAD;
	} else if(strncmp(op, "stop", strlen("stop")) == 0) {
		opcode = PCIE_DEVICE_IOCTL_STOP_DMA_THREAD;
	} else {
		printf("[%s:%s:%d]:%s(%s)\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, "kc705 ioctl command error", op);
	}
	ret = ioctl(fd, opcode);
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	if(catch_signal(0) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	close(fd);

	return ret;
}

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
#include <pthread.h>
#include "utils.h"
#include "i2s_dma.h"
static thread_arg_t i2s_thread;
static int i2s_stop = 0;


void *i2s_task(void *arg);
static void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);
	i2s_stop = 1;

	return;
}
int init_i2s_dma(char* dma_dev){
	int ret = 0;
	pthread_t tid;
	if ((i2s_thread.fd = open(dma_dev, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dma_dev, strerror(errno));
		ret = -1;
		return ret;
	}
	if(catch_signal(default_sig_action) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	i2s_thread.buffer = (unsigned char *)malloc(I2S_BUFSIZE);
	if(i2s_thread.buffer == 0) {
		printf("err: no memory for i2s_thread.buffer!\n");
		ret = -1;
		return ret;
	}
	ret = pthread_create(&tid, NULL, i2s_task, &i2s_thread);
	if (ret != 0) {
		printf("can't create thread: %s\n", strerror(errno));
	}
	return ret;
}

void *i2s_task(void *arg) {
	struct timeval tv;
	fd_set rfds;
	fd_set efds;
	int nread;

	printids("i2s_task: ");

	tv.tv_sec=1;
	tv.tv_usec=0;
	while(i2s_stop == 0) {
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		FD_SET(i2s_thread.fd, &rfds);
		FD_SET(i2s_thread.fd, &efds);

		if(select(i2s_thread.fd + 1, &rfds, NULL, &efds, &tv) > 0) {
			if(FD_ISSET(i2s_thread.fd, &rfds)) {
				printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
				nread = read(i2s_thread.fd, i2s_thread.buffer, I2S_BUFSIZE);
				if(nread < 0) {
					printf("%s\n", strerror(errno));
				} else {
					int i;
					uint32_t *data = (uint32_t *)i2s_thread.buffer;

					printf("read %d!\n", nread);
					for(i = 0; i < nread / sizeof(uint32_t); i++) {
						if((i != 0) && (i % 12 == 0)) {
							printf("\n");
						}
						printf("%04x ", data[i] & 0xffff);
					}
					printf("\n");
				}
			}
			if(FD_ISSET(i2s_thread.fd, &efds)) {
				printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	return 0;
}

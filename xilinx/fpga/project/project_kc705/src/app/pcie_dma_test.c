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
#include "kc705.h"

#define BUFSIZE 64
static int stop = 0;

static void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);
	stop = 1;

	return;
}

void printids(const char *s) {
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int) pid, (unsigned int) tid, (unsigned int) tid);
}

typedef struct {
	int fd;
	unsigned char *buffer;
} thread_arg_t;

void *read_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	struct timeval tv;
	fd_set rfds;
	fd_set efds;
	int nread;

	printids("read_fn: ");

	tv.tv_sec=1;
	tv.tv_usec=0;

	while(stop == 0) {
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		FD_SET(targ->fd, &rfds);
		FD_SET(targ->fd, &efds);

		if(select(targ->fd + 1, &rfds, NULL, &efds, &tv) > 0) {
			if(FD_ISSET(targ->fd, &rfds)) {
				//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
				nread = read(targ->fd, targ->buffer, BUFSIZE);
				if(nread < 0) {
					printf("%s\n", strerror(errno));
				} else {
					int i;
					uint32_t *data = (uint32_t *)targ->buffer;

					//printf("read %d!\n", nread);
					for(i = 0; i < nread / sizeof(uint32_t); i++) {
						if((i != 0) && (i % 16 == 0)) {
							printf("\n");
						}
						printf("%04x ", data[i] & 0xffff);
					}
					printf("\n");
				}
			}
			if(FD_ISSET(targ->fd, &efds)) {
				//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	targ = targ;
	int nwrite;

	printids("write_fn: ");

	while(stop == 0) {
		//memset(buffer, 0xff, BUFSIZE);
		//nwrite = write(fd, buffer, BUFSIZE);
		//if(nwrite < 0) {
		//	printf("%s\n", strerror(errno));
		//} else {
		//	printf("write %d!\n", nwrite);
		//}
		usleep(1000000);
	}
	return NULL;
}

int read_write(thread_arg_t *targ) {
	int err;
	pthread_t rtid;
	pthread_t wtid;

	err = pthread_create(&rtid, NULL, read_fn, targ);
	if (err != 0) {
		printf("can't create thread: %s\n", strerror(err));
	}

	err = pthread_create(&wtid, NULL, write_fn, targ);
	if (err != 0) {
		printf("can't create thread: %s\n", strerror(err));
	}

	printids("main thread:");
	pthread_join(rtid,NULL);
	pthread_join(wtid,NULL);

	return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	int ret = 0;

	char *dev = argv[1];

	thread_arg_t targ;
	int flags;

	int mmap_size;
	char *mmap_memory;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((targ.fd = open(dev, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev, strerror(errno));
		ret = -1;
		return ret;
	}

	//flags = fcntl(targ.fd, F_GETFL, 0);
	////用以下方法将socket设置为非阻塞方式
	//fcntl(targ.fd, F_SETFL, flags | O_NONBLOCK);

	ret = ioctl(targ.fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, targ.fd, (off_t)0);
	if(mmap_memory == (void *)-1) {
		printf("xiaofei: %s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		return ret;
	}

	if(catch_signal(default_sig_action) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	targ.buffer = (unsigned char *)malloc(BUFSIZE);
	if(targ.buffer == 0) {
		printf("err: no memory for targ.buffer!\n");
		ret = -1;
		return ret;
	}

	read_write(&targ);

	free(targ.buffer);

	close(targ.fd);

	return ret;
}

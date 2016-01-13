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
#include "kc705.h"

#define BUFSIZE 8
static int stop = 0;

static void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);
	stop = 1;

	return;
}

static read_write(int fd, int sock_fd, unsigned char *buffer) {
	struct timeval tv;
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	int nread, nwrite;
	int i;

	tv.tv_sec=1;
	tv.tv_usec=0;
	while(stop == 0) {
		memset(buffer, 0xff, BUFSIZE);
		nwrite = write(fd, buffer, BUFSIZE);
		if(nwrite < 0) {
			printf("%s\n", strerror(errno));
		} else {
			printf("write %d!\n", nwrite);
		}

		lseek(fd, 4, SEEK_SET);
		memset(buffer, 0, BUFSIZE - 1);
		nread = read(fd, buffer, BUFSIZE - 1);
		if(nread < 0) {
			printf("%s\n", strerror(errno));
		} else {
			printf("read %d!\n", nread);
		}

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		FD_SET(fd, &rfds);
		FD_SET(fd, &wfds);
		FD_SET(fd, &efds);

		if(select(fd + 1, &rfds, &wfds, &efds, &tv) > 0) {
			if(FD_ISSET(fd, &rfds)) {
				printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
			if(FD_ISSET(fd, &wfds)) {
				printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
			if(FD_ISSET(fd, &efds)) {
				printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
			printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		}

		usleep(1000000);
		printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

int main(int argc, char **argv) {
	int ret = 0;

	char *dev = argv[1];
	int fd;
	int flags;

	int mmap_size;
	char *mmap_memory;

	unsigned char *buffer;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((fd = open(dev, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev, strerror(errno));
		ret = -1;
		return ret;
	}

	//flags = fcntl(fd, F_GETFL, 0);
	////用以下方法将socket设置为非阻塞方式
	//fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	ret = ioctl(fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, fd, (off_t)0);
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

	buffer = (unsigned char *)malloc(BUFSIZE);
	if(buffer == 0) {
		printf("err: no memory for buffer!\n");
		ret = -1;
		return ret;
	}

	read_write(fd, 0, buffer);

	free(buffer);

	close(fd);

	return ret;
}

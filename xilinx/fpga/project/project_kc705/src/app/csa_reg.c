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

typedef enum {
	ADDR_CSA_BUSY = 0,
	ADDR_CSA_READY,
	ADDR_CHANNEL_INDEX,
	ADDR_IN_DATA_VALID,
	ADDR_IN_DATA_0,
	ADDR_IN_DATA_1,
	ADDR_IN_DATA_2,
	ADDR_IN_DATA_3,
	ADDR_IN_DATA_4,
	ADDR_OUT_DATA_VALID,
	ADDR_OUT_DATA_0,
	ADDR_OUT_DATA_1,
	ADDR_OUT_DATA_2,
	ADDR_CALC_TIMES,
	ADDR_BUFFER_DATA0,
	ADDR_BUFFER_DATA1,
	ADDR_BUFFER_DATA2,
	ADDR_BUFFER_DATA3,
	ADDR_BUFFER_DATA4,
	TOTAL_REGS,
} addr_t;

char *reg_name[] = {
	"ADDR_CSA_BUSY",
	"ADDR_CSA_READY",
	"ADDR_CHANNEL_INDEX",
	"ADDR_IN_DATA_VALID",
	"ADDR_IN_DATA_0",
	"ADDR_IN_DATA_1",
	"ADDR_IN_DATA_2",
	"ADDR_IN_DATA_3",
	"ADDR_IN_DATA_4",
	"ADDR_OUT_DATA_VALID",
	"ADDR_OUT_DATA_0",
	"ADDR_OUT_DATA_1",
	"ADDR_OUT_DATA_2",
	"ADDR_CALC_TIMES",
	"ADDR_BUFFER_DATA0",
	"ADDR_BUFFER_DATA1",
	"ADDR_BUFFER_DATA2",
	"ADDR_BUFFER_DATA3",
	"ADDR_BUFFER_DATA4",
	"TOTAL_REGS",
};

#define ADDR_OFFSET(addr) (addr * 4)

#define BUFSIZE ((ADDR_CALC_TIMES + 1) * 4)

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

#define read_regs(addr) \
do { \
	lseek(targ->fd, ADDR_OFFSET(addr), SEEK_SET); \
	nread = read(targ->fd, targ->buffer, sizeof(uint32_t)); \
} while(0)

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
		uint32_t *data = (uint32_t *)targ->buffer;
		int i;

		usleep(1000000);

		for(i = 0; i < TOTAL_REGS; i++) {
			read_regs(i);
			printf("%s: %08x(%d)\n", reg_name[i], data[0], data[0]);
		}

		for(i = TOTAL_REGS; i < TOTAL_REGS + (40 * 2); i++) {
			if((i - TOTAL_REGS) % 2 == 0) {
				read_regs(i);
				printf("data:%08x\n", data[0]);
			} else {
				read_regs(i);
				printf("aresetn:%d\t", (data[0] >> 0) & 0x1);
				printf("tready:%d\t", (data[0] >> 1) & 0x1);
				printf("tlast:%d\t", (data[0] >> 2) & 0x1);
				printf("tvalid:%d\n", (data[0] >> 3) & 0x1);
			}
		}

		return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	targ = targ;
	int nwrite;

	printids("write_fn: ");
	int times = 50000;
	int channel = 1;

	while(stop == 0) {
		lseek(targ->fd, ADDR_OFFSET(ADDR_CALC_TIMES), SEEK_SET);
		nwrite = write(targ->fd, &times, sizeof(int));

		lseek(targ->fd, ADDR_OFFSET(ADDR_CHANNEL_INDEX), SEEK_SET);
		nwrite = write(targ->fd, &channel, sizeof(int));
		//usleep(1000000);
		return NULL;
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
		//return ret;
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

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
	MOD_MODE_CFG_REG = 0,
	LDPC_MODE_CFG_REG,
	FRAME_MODE_CFG_REG,
	PILOT_MODE_CFG_REG,
	SRRC_MODE_REG,
	DVB_S_CONVOLUTION_MODE_REG,
	DVB_S_MODE_REG,
	TS_SOURCE_MODE_REG,
	SYS_FREQ_NUM_REG,
	SYS_BAUD_NUM_REG,
	FREQ_INV_MODE_REG,
	FS_EN2_COUNT_REG,
	FS_EN_COUNT_REG,
	MPEG_BYTES_COUNT_REG,
	SYMBOL_1X_OE_COUNT_REG,
	SYMBOL_2X_OE_COUNT_REG,
	TOTAL_REGS,
} addr_t;

char *reg_name[] = {
	"MOD_MODE_CFG_REG",
	"LDPC_MODE_CFG_REG",
	"FRAME_MODE_CFG_REG",
	"PILOT_MODE_CFG_REG",
	"SRRC_MODE_REG",
	"DVB_S_CONVOLUTION_MODE_REG",
	"DVB_S_MODE_REG",
	"TS_SOURCE_MODE_REG",
	"SYS_FREQ_NUM_REG",
	"SYS_BAUD_NUM_REG",
	"FREQ_INV_MODE_REG",
	"FS_EN2_COUNT_REG",
	"FS_EN_COUNT_REG",
	"MPEG_BYTES_COUNT_REG",
	"SYMBOL_1X_OE_COUNT_REG",
	"SYMBOL_2X_OE_COUNT_REG",
};

#define ADDR_OFFSET(addr) (addr * 4)

#define BUFSIZE (TOTAL_REGS * 4)

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

	//printids("read_fn: ");

	tv.tv_sec=1;
	tv.tv_usec=0;

	while(stop == 0) {
		uint32_t *data = (uint32_t *)targ->buffer;
		int i;

		usleep(10);

		for(i = 0; i < TOTAL_REGS; i++) {
			read_regs(i);
			printf("%40s(%03x): %16x(%16u)\n", reg_name[i], i * 4, data[0], data[0]);
		}

		return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	targ = targ;
	int nwrite;

	uint32_t ts_source_mode_reg = 2;
	uint32_t sys_freq_num_reg = 12500;
	uint32_t sys_baud_num_reg = 1000;

	printids("write_fn: ");

	while(stop == 0) {
		lseek(targ->fd, ADDR_OFFSET(TS_SOURCE_MODE_REG), SEEK_SET);
		nwrite = write(targ->fd, &ts_source_mode_reg, sizeof(uint32_t));
		lseek(targ->fd, ADDR_OFFSET(SYS_FREQ_NUM_REG), SEEK_SET);
		nwrite = write(targ->fd, &sys_freq_num_reg, sizeof(uint32_t));
		lseek(targ->fd, ADDR_OFFSET(SYS_BAUD_NUM_REG), SEEK_SET);
		nwrite = write(targ->fd, &sys_baud_num_reg, sizeof(uint32_t));

		return NULL;
	}
	return NULL;
}

int main_proc(thread_arg_t *targ) {
	write_fn(targ);
	read_fn(targ);
}

int read_write(thread_arg_t *targ) {
	int err;
	pthread_t rtid;
	pthread_t wtid;

	//err = pthread_create(&rtid, NULL, read_fn, targ);
	//if (err != 0) {
	//	printf("can't create thread: %s\n", strerror(err));
	//}

	//err = pthread_create(&wtid, NULL, write_fn, targ);
	//if (err != 0) {
	//	printf("can't create thread: %s\n", strerror(err));
	//}

	////printids("main thread:");
	//pthread_join(rtid,NULL);
	//pthread_join(wtid,NULL);
	main_proc(targ);

	return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	int ret = 0;

	char *dev = argv[1];

	thread_arg_t targ;
	int flags;

	int mmap_size;
	//char *mmap_memory;

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

	//mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, targ.fd, (off_t)0);
	//if(mmap_memory == (void *)-1) {
	//	printf("xiaofei: %s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
	//	ret = -1;
	//	return ret;
	//}

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

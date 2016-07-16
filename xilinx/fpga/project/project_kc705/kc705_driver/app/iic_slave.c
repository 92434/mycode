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
#include <time.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>
#include "kc705.h"

typedef enum {
	POWER_ON_SWITCH_REG = 0,
	MOD_MODE_CFG_REG,
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
	"POWER_ON_SWITCH_REG",
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
	int fd_master;
	int fd_slave;
	unsigned char *buffer;
} thread_arg_t;

#define read_regs(addr) \
do { \
	lseek(targ->fd_slave, ADDR_OFFSET(addr), SEEK_SET); \
	nread = read(targ->fd_slave, targ->buffer, sizeof(uint32_t)); \
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

		//usleep(10);

		//for(i = 0; i < TOTAL_REGS; i++) {
		//	read_regs(i);
		//	printf("%s: %08x(%d)\n", reg_name[i], data[0], data[0]);
		//}

		return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	targ = targ;
	int nwrite;

	//printids("write_fn: ");
	int channel = 3;
	int reset = 1;

	while(stop == 0) {
		//lseek(targ->fd_slave, ADDR_OFFSET(ADDR_CHANNEL_INDEX), SEEK_SET);
		//nwrite = write(targ->fd_slave, &channel, sizeof(int));
		//lseek(targ->fd_slave, ADDR_OFFSET(ADDR_RESET), SEEK_SET);
		//nwrite = write(targ->fd_slave, &reset, sizeof(int));

		return NULL;
	}
	return NULL;
}

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	int nwrite;
	int nread;
	int count = 0;
	int start = 1;

	//printids("main_proc: ");
	
        struct timeval tv0, tv1;
        struct timezone tz0, tz1;

        gettimeofday(&tv0, &tz0);

	while(stop == 0) {
		unsigned char buff[2 + BUFSIZE] = {0};
		uint16_t *paddr = (uint16_t *)buff;
		uint32_t *pdata = (uint32_t *)(buff + 2);
		uint32_t *pdata_r = (uint32_t *)targ->buffer;
		int i;

		//iic_slave_dump
		printf("%s\n", "-------------------------------------------------");
		for(i = 0; i < TOTAL_REGS; i++) {
			read_regs(i);
			printf("dump %s: %08x(%d)\n", reg_name[i], pdata_r[0], pdata_r[0]);
		}

		//write iic_slave
		printf("%s\n", "-------------------------------------------------");
		*paddr = 0x0000;
		for(i = 0; i < TOTAL_REGS; i++) {
			pdata[i] = count + i;
			printf("write %s: %08x(%d)\n", reg_name[i], pdata[i], pdata[i]);
		}
		count++;

		write(targ->fd_master, buff, 2 + BUFSIZE);

		//iic_slave_dump
		printf("%s\n", "-------------------------------------------------");
		for(i = 0; i < TOTAL_REGS; i++) {
			read_regs(i);
			printf("dump %s: %08x(%d)\n", reg_name[i], pdata_r[0], pdata_r[0]);
		}

		//read iic_slave
		printf("%s\n", "-------------------------------------------------");
		for(i = 0; i < TOTAL_REGS; i++) {
			pdata[i] = 0;
		}
		write(targ->fd_master, buff, 2);
		read(targ->fd_master, buff + 2, BUFSIZE);
		for(i = 0; i < TOTAL_REGS; i++) {
			printf("read %s: %08x(%d)\n", reg_name[i], pdata[i], pdata[i]);
		}

		sleep(1);

		//break;
	}
        gettimeofday(&tv1, &tz1);

        //printf("tv0.tv_sec:%d\n", (int)tv0.tv_sec);
        //printf("tv0.tv_usec:%d\n", (int)tv0.tv_usec);
        //printf("tz0.tz_minuteswest:%d\n", (int)tz0.tz_minuteswest);
        //printf("tz0.tz_dsttime:%d\n", (int)tz0.tz_dsttime);

        //printf("tv1.tv_sec:%d\n", (int)tv1.tv_sec);
        //printf("tv1.tv_usec:%d\n", (int)tv1.tv_usec);
        //printf("tz1.tz_minuteswest:%d\n", (int)tz1.tz_minuteswest);
        //printf("tz1.tz_dsttime:%d\n", (int)tz1.tz_dsttime);
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

	main_proc(targ);

	//printids("main thread:");
	pthread_join(rtid,NULL);
	pthread_join(wtid,NULL);

	return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
	int ret = 0;
	char *dev_master; 
	char *dev_slave;

	if(argc == 3) {
		dev_master = argv[1];
		dev_slave = argv[2];
	} else {
		dev_master = "/dev/i2c-7";
		dev_slave = "/dev/iic_slave_reg";
	}

	thread_arg_t targ;
	int flags;

	int mmap_size;
	//char *mmap_memory;

	if(argc != 3 && argc != 1) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((targ.fd_master = open(dev_master, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev_master, strerror(errno));
		ret = -1;
		return ret;
	}

	ret = ioctl(targ.fd_master, I2C_SLAVE_FORCE, 0x3c);
	if(ret < 0){
		printf("err: can't ioctl device(%s)(%s)\n", dev_master, strerror(errno));
		return ret;
	}

	if ((targ.fd_slave = open(dev_slave, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev_slave, strerror(errno));
		ret = -1;
		return ret;
	}

	//flags = fcntl(targ.fd_master, F_GETFL, 0);
	////用以下方法将socket设置为非阻塞方式
	//fcntl(targ.fd_master, F_SETFL, flags | O_NONBLOCK);

	//flags = fcntl(targ.fd_slave, F_GETFL, 0);
	////用以下方法将socket设置为非阻塞方式
	//fcntl(targ.fd_slave, F_SETFL, flags | O_NONBLOCK);

	ret = ioctl(targ.fd_slave, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	//mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, targ.fd_slave, (off_t)0);
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

	close(targ.fd_slave);
	close(targ.fd_master);

	return ret;
}

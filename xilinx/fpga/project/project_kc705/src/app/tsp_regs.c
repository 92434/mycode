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

#define	PACK_BYTE_SIZE 188
#define	PACK_WORD_SIZE (PACK_BYTE_SIZE / sizeof(uint32_t))

typedef enum {
	ADDR_INDEX = 0,
	ADDR_PID_INDEX,
	ADDR_PID,
	ADDR_MATCH_ENABLE,
	ADDR_READ_REQUEST,
	ADDR_TS_DATA_BASE = 128,
	ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE * 2,
} addr_t;

#define ADDR_OFFSET(addr) (addr * 4)
#define PID_INFO(ENABLE, PID) ((ENABLE << 16)/*match enable*/ + PID)

#define BUFSIZE (PACK_BYTE_SIZE * 2)

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

int read_regs(thread_arg_t *targ, int addr, int count) {
	int nread;

	lseek(targ->fd, ADDR_OFFSET(addr), SEEK_SET);
	nread = read(targ->fd, targ->buffer, count);

	return nread;
}

int write_regs(thread_arg_t *targ, int addr, int count) {
	int nwrite;

	lseek(targ->fd, ADDR_OFFSET(addr), SEEK_SET);
	nwrite = write(targ->fd, targ->buffer, count);

	return nwrite;
}

void *read_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("read_fn: ");

	while(stop == 0) {
		return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	while(stop == 0) {
		return NULL;
	}
	return NULL;
}

int test_pid_op(thread_arg_t *targ) {
	int ret = 0;

	int i;

	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	int pid = 0x1570;

	for(i = 0; i < 2 + 16 + 1; i ++) {
		int j;
		int pid_slot = (i == 18) ? 16 : 1;

		*pbuffer = i;//
		ret = write_regs(targ, ADDR_INDEX, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		
		for(j = 0; j < pid_slot; j++) {
			*pbuffer = j;//
			ret = write_regs(targ, ADDR_PID_INDEX, sizeof(uint32_t));
			if (ret < 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

			*pbuffer = PID_INFO(1, pid);
			//pid++;
			ret = write_regs(targ, ADDR_PID, sizeof(uint32_t));
			if (ret < 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

		}

		*pbuffer = (i == 0 || i == 1) ? 1 : 0;//
		ret = write_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

	}

	for(i = 0; i < 2 + 16 + 1; i++) {
		int j;
		int pid_slot = (i == 18) ? 16 : 1;

		*pbuffer = i;//
		ret = write_regs(targ, ADDR_INDEX, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		for(j = 0; j < pid_slot; j++) {
			*pbuffer = j;//
			ret = write_regs(targ, ADDR_PID_INDEX, sizeof(uint32_t));
			if (ret < 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

			ret = read_regs(targ, ADDR_PID, sizeof(uint32_t));
			if (ret < 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}
			printf("slot %d pid_index %d pid:%08x\n", i, j, *pbuffer);
		}

		ret = read_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
		printf("slot %d match enable:%s\n", i, *pbuffer == 1 ? "true" : *pbuffer == 0 ? "false" : "other");

	}
	return ret;
}

int write_ts_pack(thread_arg_t *targ, int slot) {
	int ret = 0;
	int i;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);
	static int loc = 0;

	int tx_size = (slot == 18) ? 188 * 2 : (slot >= 2 && slot < 18) ? 188 : 0;

	*pbuffer = slot;//replacer #17
	ret = write_regs(targ, ADDR_INDEX, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	for(i = 0; i < tx_size; i++) {
		targ->buffer[i] = loc + i;
	}
	if(tx_size != 0) {
		loc++;
	}
	ret = write_regs(targ, ADDR_TS_DATA_BASE, tx_size);
	if (ret < 0) {
		printf("tx_size:%d\n", tx_size);
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int read_ts_pack(thread_arg_t *targ, int slot) {
	int ret = 0;

	int i;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	int rx_size = (slot == 18) ? 188 * 2 : (slot >= 0 && slot < 18) ? 188 : 0;

	*pbuffer = 0;//any value
	ret = write_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*pbuffer = 0;
	while(*pbuffer == 0) {
		ret = read_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
	}

	ret = read_regs(targ, ADDR_TS_DATA_BASE, rx_size);//will valid after 3 clock
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	printf("\n");
	for(i = 0; i < rx_size; i++) {
		if((i != 0) && (i % 16 == 0)) {
			printf("\n");
		}
		printf("%02x ", targ->buffer[i]);
	}
	printf("\n");

	return ret;
}

int test_ts_pack_write_read(thread_arg_t *targ) {
	int ret = 0;
	int i;

	for(i = 0; i < 19; i++) {
		ret = write_ts_pack(targ, i);
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		ret = read_ts_pack(targ, i);
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
	}

	return ret;
}

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("main_proc: ");

	while(stop == 0) {
		test_pid_op(targ);
		test_ts_pack_write_read(targ);

		return;
	}
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

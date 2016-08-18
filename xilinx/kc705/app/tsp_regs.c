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

typedef int bool;
#define true (1 == 1)
#define false (1 == 0)

typedef enum {
	ADDR_INDEX = 0,
	ADDR_PID_INDEX,
	ADDR_PID,
	ADDR_MATCH_ENABLE,
	ADDR_READ_REQUEST,
	ADDR_PTS_LOW,
	ADDR_PTS_HIGH,
	ADDR_MATCHED_COUNT,
	ADDR_TS_DATA_BASE = 128,
	ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE * 2,
} addr_t;

#define ADDR_OFFSET(addr) (addr * 4)
#define PID_INFO(CHANGE, ENABLE, PID) (((CHANGE == 0 ? 0 : 1) << 17)/*change enable*/ + ((ENABLE == 0 ? 0 : 1) << 16)/*match enable*/ + PID)

#define BUFSIZE (PACK_BYTE_SIZE * 2)

#define MONITOR_SIZE 1
#define REPLACER_SIZE 8
#define COMMON_REPLACER_SIZE 1

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

int select_slot(thread_arg_t *targ, uint32_t slot) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = slot;//
	ret = write_regs(targ, ADDR_INDEX, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int select_pid_slot(thread_arg_t *targ, uint32_t pid_slot) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = pid_slot;//
	ret = write_regs(targ, ADDR_PID_INDEX, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int set_pid(thread_arg_t *targ, uint32_t pid, bool pid_enable, bool pid_change) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = PID_INFO(pid_change, pid_enable, pid);
	ret = write_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_pid(thread_arg_t *targ, uint32_t *pid, bool *pid_enable, bool *pid_change) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*pid = *pbuffer & 0xffff;
	*pid_enable = (((*pbuffer >> 16) & 1) == 0) ? false : true;
	*pid_change = (((*pbuffer >> 17) & 1) == 0) ? false : true;
	return ret;
}

int get_matched_count(thread_arg_t *targ, uint32_t *matched_count) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_MATCHED_COUNT, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*matched_count = *pbuffer; 
	return ret;
}

int set_pts(thread_arg_t *targ, uint32_t pts_low, uint32_t pts_high) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = pts_low;
	ret = write_regs(targ, ADDR_PTS_LOW, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*pbuffer = pts_high;
	ret = write_regs(targ, ADDR_PTS_HIGH, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_pts(thread_arg_t *targ, uint32_t *pts_low, uint32_t *pts_high) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_PTS_LOW, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*pts_low = *pbuffer;

	ret = read_regs(targ, ADDR_PTS_HIGH, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*pts_high = *pbuffer;
	return ret;
}

int set_slot_enable(thread_arg_t *targ, bool slot_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = slot_enable ? 1 : 0;//
	ret = write_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_slot_enable(thread_arg_t *targ, bool *slot_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*slot_enable = (*pbuffer == 1) ? true : false;

	return ret;
}

int write_ts_data(thread_arg_t *targ, int tx_size, unsigned char slot) {
	int ret = 0;
	int i;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	if(tx_size <= 0) {
		return ret;
	}

	for(i = 0; i < tx_size; i++) {
		switch(i) {
			case 0:
				targ->buffer[i] = 0x47;
				break;
			case 1:
				targ->buffer[i] = 0x1f;
				break;
			case 2:
				targ->buffer[i] = 0xff;
				break;
			case 3:
				targ->buffer[i] = 0xff;
				break;
			case 188:
				targ->buffer[i] = 0x47;
				break;
			case 189:
				targ->buffer[i] = 0x1f;
				break;
			case 190:
				targ->buffer[i] = 0xff;
				break;
			case 191:
				targ->buffer[i] = 0xff;
				break;
			default:
				targ->buffer[i] = 0;
				break;
		}
	}

	ret = write_regs(targ, ADDR_TS_DATA_BASE, tx_size);
	if (ret < 0) {
		printf("tx_size:%d\n", tx_size);
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int read_ts_data(thread_arg_t *targ, int rx_size) {
	int ret = 0;
	int i;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	if(rx_size <= 0) {
		return ret;
	}

	*pbuffer = 0;//any value
	ret = write_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*pbuffer = 0;
	i = 0;
	while(*pbuffer != 1) {
		ret = read_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
		if(i >= 200) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return -1;
		}
		i++;
		usleep(10000);
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

int test_set(thread_arg_t *targ) {
	int ret = 0;
	int i;

	for(i = 0; i < MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE; i++) {//slot
		int j;

		int pid_slot = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 16 : 1;
		int tx_size = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 188 * 2 : ((i >= MONITOR_SIZE) && (i <  MONITOR_SIZE + REPLACER_SIZE)) ? 188 : 0;
		bool slot_enable;

		switch(i) {
			case 0:
				slot_enable = true;
				break;
			case 1:
				slot_enable = true;
				break;
			case 2:
				slot_enable = true;
				break;
			case 9:
				slot_enable = true;
				break;
			default:
				slot_enable = false;
				break;
		}

		select_slot(targ, i);
		for(j = 0; j < pid_slot; j++) {//pid_slot
			uint32_t pid;
			bool pid_enable;
			bool pid_change;

			switch(j) {
				case 0:
					switch(i) {
						case 0:
							pid = 0x0000;
							pid_enable = true;
							pid_change = true;
							break;
						case 1:
							pid = 0x0533;
							pid_enable = true;
							pid_change = true;
							break;
						case 2:
							pid = 0x0534;
							pid_enable = true;
							pid_change = true;
							break;
						case 9:
							pid = 0x0000;
							pid_enable = true;
							pid_change = true;
							break;
						default:
							pid = 0x0000;
							pid_enable = false;
							pid_change = false;
							break;
					}
					break;
				case 9:
					pid = 0x0001;
					pid_enable = true;
					pid_change = true;
					break;
				default:
					pid = 0x0000;
					pid_enable = false;
					pid_change = false;
					break;
			}
			select_pid_slot(targ, j);
			set_pid(targ, pid, pid_enable, pid_change);
			set_pts(targ, 0x12345678, 0x12345678);
		}

		write_ts_data(targ, tx_size, i);

		set_slot_enable(targ, slot_enable);
	}

	return ret;
}

int test_get(thread_arg_t *targ) {
	int ret = 0;
	int i;

	for(i = 0; i < MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE; i++) {//slot
		int j;

		int pid_slot = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 16 : 1;
		int rx_size = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 188 * 2 : ((i >= 0) && (i <  MONITOR_SIZE + REPLACER_SIZE)) ? 188 : 0;
		bool slot_enable;
		uint32_t matched_count;

		switch(i) {
			default:
				break;
		}

		select_slot(targ, i);
		printf("slot %d:\n", i);

		for(j = 0; j < pid_slot; j++) {//pid_slot
			uint32_t pid;
			bool pid_enable;
			bool pid_change;
			uint32_t pts_low, pts_high;

			switch(j) {
				default:
					break;
			}
			select_pid_slot(targ, j);
			get_pid(targ, &pid, &pid_enable, &pid_change);
			get_pts(targ, &pts_low, &pts_high);
			printf("pid: %08x; pid_enable: %s; pid_change: %s; pts:%08x%08x\n", pid, pid_enable ? "true" : "false", pid_change ? "true" : "false", pts_high, pts_low);
		}

		get_matched_count(targ, &matched_count);
		get_slot_enable(targ, &slot_enable);
		printf("slot_enable %s; matched_count: %08x(%d)\n", slot_enable ? "true" : "false", matched_count, matched_count);

		read_ts_data(targ, rx_size);
	}

	return ret;
}

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("main_proc: ");

	while(stop == 0) {
		test_set(targ);
		test_get(targ);
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

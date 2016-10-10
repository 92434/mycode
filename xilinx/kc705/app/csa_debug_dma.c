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
#include <sys/time.h>
#include <time.h>
#include "kc705.h"

//40bits in, 48bits out
//5byte in, 3 words out(use lower 16 bits)

//uint32_t raw_data[] = {
//	1, 0x00000001, 0x00000000, 50000, 0,
//	2, 0x00000002, 0x00000000, 50000, 0,
//	3, 0x00000003, 0x00000000, 50000, 0,
//	4, 0x00000004, 0x00000000, 50000, 0,
//	5, 0x00000005, 0x00000000, 50000, 0,
//	6, 0x00000006, 0x00000000, 50000, 0,
//	7, 0x00000007, 0x00000000, 50000, 0,
//	8, 0x00000008, 0x00000000, 50000, 0,
//	9, 0x00000009, 0x00000000, 50000, 0,
//	10, 0x0000000a, 0x00000000, 50000, 0,
//};
//#define RAW_DATA_SIZE (sizeof(raw_data) / sizeof(char))
uint32_t *raw_data = NULL;

#define CALC_IN_SIZE (5 * 4)
#define CALC_OUT_SIZE (7 * 4)
#define BULKNUM 36
#define BULKSIZE (CALC_OUT_SIZE * BULKNUM)

#define RAW_DATA_SIZE (CALC_IN_SIZE * BULKNUM)
#define BUFSIZE (CALC_OUT_SIZE * BULKNUM)

typedef enum {
	ADDR_CHANNEL_INDEX = 0,
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
	ADDR_OUT_DATA_3,
	ADDR_OUT_DATA_4,
	ADDR_OUT_DATA_5,
	ADDR_OUT_DATA_6,
	ADDR_DATA_CATCH_COUNT_INDEX,
	ADDR_DATA_CATCH_COUNT_LOW,
	ADDR_DATA_CATCH_COUNT_HIGH,
	ADDR_MASK_LOW,
	ADDR_MASK_HIGH,
	ADDR_VALUE_LOW,
	ADDR_VALUE_HIGH,
	ADDR_DEVICE_IDLE,
	ADDR_DEVICE_READY,
	ADDR_RESET,
	TOTAL_REGS,
} addr_t;

char *reg_name[] = {
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
	"ADDR_OUT_DATA_3",
	"ADDR_OUT_DATA_4",
	"ADDR_OUT_DATA_5",
	"ADDR_OUT_DATA_6",
	"ADDR_DATA_CATCH_COUNT_INDEX",
	"ADDR_DATA_CATCH_COUNT_LOW",
	"ADDR_DATA_CATCH_COUNT_HIGH",
	"ADDR_MASK_LOW",
	"ADDR_MASK_HIGH",
	"ADDR_VALUE_LOW",
	"ADDR_VALUE_HIGH",
	"ADDR_DEVICE_IDLE",
	"ADDR_DEVICE_READY",
	"ADDR_RESET",
};

#define ADDR_OFFSET(addr) (addr * 4)

static int stop = 0;

static void default_sig_action(int signo, siginfo_t *info, void *context)
{
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);

	if(stop == 0) {
		stop = 1;
	} else {
		printf("xiaofei:force stop!\n");
	}

	return;
}

void printids(const char *s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int) pid, (unsigned int) tid, (unsigned int) tid);
}

typedef struct {
	int dma_fd;
	int reg_fd;
	unsigned char *buffer;
} thread_arg_t;

void get_status(thread_arg_t *targ, int *idle, int *ready)
{
	int nread;
	uint32_t *data = (uint32_t *)targ->buffer;
	uint32_t status_size = sizeof(uint32_t) * 2;

	lseek(targ->reg_fd, ADDR_OFFSET(ADDR_DEVICE_IDLE), SEEK_SET);
	nread = read(targ->reg_fd, targ->buffer, status_size);

	if(nread == status_size) {
		*idle = data[0];
		*ready = data[1];
	} else {
		*idle = 0;
		*ready = 0;
	}
}

int init_raw_data(int start)
{
	int i;
	uint32_t *data = raw_data;
	int use_start = start;

	for(i = 0; i < RAW_DATA_SIZE / sizeof(uint32_t); i += 5) {
		data[i + 0] = use_start | 0x00000000;
		data[i + 1] = use_start;
		data[i + 2] = 0;
		data[i + 3] = 50000;//(use_start % 50000) ? (use_start % 50000) : 50000;
		data[i + 4] = 0;

		use_start++;
	}

	return use_start;
}

#if 0
void *read_fn(void *arg)
{
	thread_arg_t *targ = (thread_arg_t *)arg;
	struct timeval tv;
	fd_set rfds;
	fd_set efds;
	int nread;

	//printids("read_fn: ");

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int idle_count = 0;

	while(stop == 0) {
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		FD_SET(targ->dma_fd, &rfds);
		FD_SET(targ->dma_fd, &efds);

		if(select(targ->dma_fd + 1, &rfds, NULL, &efds, &tv) > 0) {
			if(FD_ISSET(targ->dma_fd, &rfds)) {
				//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
				while(nread > 0) {
					nread = read(targ->dma_fd, targ->buffer, BULKSIZE);

					if(nread < 0) {
						printf("%s\n", strerror(errno));
						stop = 1;
						break;
					} else {
						int i;
						uint32_t *data = (uint32_t *)targ->buffer;

						//printf("read %d!\n", nread);
						if(nread == 0) {
							continue;
						}

						for(i = 0; i < nread / sizeof(uint32_t); i += 7) {
							printf("block:<%01x>%10d in:0x%08x%08x times:%10d times_start:%10d out:0x%08x%08x\n",
								   (data[i + 0] & 0xc0000000) >> 30,//block
								   data[i + 0] & 0x3fffffff,//block
								   data[i + 2],//in(high)
								   data[i + 1],//in(low)
								   data[i + 3],//times
								   data[i + 4],//times delay
								   data[i + 6],//out(high)
								   data[i + 5]//out(low)
								  );
						}

						//printf("\n");
					}
				}

			}

			if(FD_ISSET(targ->dma_fd, &efds)) {
				//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
			}
		}

		//return NULL;
	}

	return NULL;
}
#else
void *read_fn(void *arg)
{
	thread_arg_t *targ = (thread_arg_t *)arg;
	int nread;

	//printids("read_fn: ");

	while(stop == 0) {
		int idle, ready;
		get_status(targ, &idle, &ready);

		if(ready == 1) {
			nread = read(targ->dma_fd, targ->buffer, BULKSIZE);

			if(nread < 0) {
				printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				stop = 1;
				break;
			} else {
				int i;
				uint32_t *data = (uint32_t *)targ->buffer;

				//printf("read %d!\n", nread);
				if(nread == 0) {
					printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
					stop = 1;
				}

				for(i = 0; i < nread / sizeof(uint32_t); i += 7) {
					printf("block:<%01x>%10d in:0x%08x%08x times:%10d times_start:%10d out:0x%08x%08x\n",
						   (data[i + 0] & 0xc0000000) >> 30,//block
						   data[i + 0] & 0x3fffffff,//block
						   data[i + 2],//in(high)
						   data[i + 1],//in(low)
						   data[i + 3],//times
						   data[i + 4],//times delay
						   data[i + 6],//out(high)
						   data[i + 5]//out(low)
						  );
				}

				//printf("\n");
			}
		} else {
			//printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			usleep(1);
		}

		//stop = 1;
	}

	return NULL;
}
#endif //if 0

void *write_fn(void *arg)
{
	thread_arg_t *targ = (thread_arg_t *)arg;
	int nwrite = RAW_DATA_SIZE;
	int count;
	int start = 1;

	int mask_low = 0x00000000;
	int value_low = 0x00000000;

	//printids("write_fn: ");

	struct timeval tv0, tv1;
	struct timezone tz0, tz1;

	gettimeofday(&tv0, &tz0);

	lseek(targ->reg_fd, ADDR_OFFSET(ADDR_MASK_LOW), SEEK_SET);
	nwrite = write(targ->reg_fd, &mask_low, sizeof(int));
	lseek(targ->reg_fd, ADDR_OFFSET(ADDR_VALUE_LOW), SEEK_SET);
	nwrite = write(targ->reg_fd, &value_low, sizeof(int));

	while(stop == 0) {
		int idle, ready;
		get_status(targ, &idle, &ready);

		if(start < BULKNUM) {
			start = init_raw_data(start);

			//printf("start %d! count:%d!\n", start, count);
		} else {
			//printf("nwrite %d! count:%d!\n", nwrite, count);
		}

		if(idle == 1) {
			printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			nwrite = write(targ->dma_fd, raw_data, RAW_DATA_SIZE);

			if(nwrite < 0) {
				printf("%s\n", strerror(errno));
				stop = 1;
			} else {
				//printf("write %d!\n", nwrite);
			}
		} else {
			printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			usleep(1);
		}

		//stop = 1;
		return NULL;
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

	return NULL;
}

void main_proc(thread_arg_t *arg)
{
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("main_proc: ");
	int nwrite = RAW_DATA_SIZE;
	int nread = CALC_OUT_SIZE;
	int count;
	int start = 1;

	unsigned long long total_count = 0;
	int usec0 = 0;
	int usec1 = 0;
	unsigned long long usec_duration = 0;
	int speed = 0;
	int delay_count = 0;

	int mask_low = 0x00000000;
	int value_low = 0x00000000;

	//printids("write_fn: ");

	struct timeval tv0, tv1;
	struct timezone tz0, tz1;

	gettimeofday(&tv0, &tz0);

	lseek(targ->reg_fd, ADDR_OFFSET(ADDR_MASK_LOW), SEEK_SET);
	nwrite = write(targ->reg_fd, &mask_low, sizeof(int));
	lseek(targ->reg_fd, ADDR_OFFSET(ADDR_VALUE_LOW), SEEK_SET);
	nwrite = write(targ->reg_fd, &value_low, sizeof(int));

	while(stop == 0) {
		int idle, ready;
		get_status(targ, &idle, &ready);

		if(idle == 1) {
			if((1 == 1) || (start < BULKNUM)) {
				start = init_raw_data(start);

				//printf("start %d! count:%d!\n", start, count);

				nwrite = write(targ->dma_fd, raw_data, RAW_DATA_SIZE);

				if(nwrite < 0) {
					printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
					stop = 1;
				} else {
					//printf("write %d!\n", nwrite);
				}
			}
		} else {
			//printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		}

		if(ready == 1) {
			delay_count = 0;
			nread = read(targ->dma_fd, targ->buffer, BULKSIZE);

			if(nread < 0) {
				printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				stop = 1;
				break;
			} else {
				int i;
				uint32_t *data = (uint32_t *)targ->buffer;

				//printf("read %d!\n", nread);
				if(nread == 0) {
					printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
					stop = 1;
				}

				for(i = 0; i < nread / sizeof(uint32_t); i += 7) {
					if(0 == 1) {
						printf("block:<%01x>%10d in:0x%08x%08x times:%10d times_start:%10d out:0x%08x%08x\n",
							   (data[i + 0] & 0xc0000000) >> 30,//block
							   data[i + 0] & 0x3fffffff,//block
							   data[i + 2],//in(high)
							   data[i + 1],//in(low)
							   data[i + 3],//times
							   data[i + 4],//times delay
							   data[i + 6],//out(high)
							   data[i + 5]//out(low)
							  );
					}

					//total_count = (data[i + 0] & 0x3fffffff);
					total_count++;
				}

				//printf("\n");
			}
		} else {
			//printf("xiaofei: %s:%d: [%s]-wait!\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			//usleep(1);
			delay_count++;

			if(delay_count == 1000) {
				stop = 1;
			}
		}

		//stop = 1;
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

	usec0 = tv0.tv_sec * 1000000 + tv0.tv_usec;
	usec1 = tv1.tv_sec * 1000000 + tv1.tv_usec;
	usec_duration = usec1 - usec0;
	printf("total_count:%llu\n", total_count);
	printf("usec_duration:%llu usec\n", usec_duration);
	printf("speed:%d/s\n", (int)(total_count * 1000000 / usec_duration));
}

void *delay_thread(void *arg)
{
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("write_fn: ");

	int delay_count = 0;

	while(stop == 0) {
		sleep(1);

		delay_count++;

		if(delay_count >= 60) {
			stop = 1;
		}
	}
}

int read_write(thread_arg_t *targ)
{
	int err;
	pthread_t rtid;
	pthread_t wtid;
	pthread_t dtid;

	//err = pthread_create(&rtid, NULL, read_fn, targ);

	//if (err != 0) {
	//	printf("can't create thread: %s\n", strerror(err));
	//}

	//err = pthread_create(&wtid, NULL, write_fn, targ);

	//if (err != 0) {
	//	printf("can't create thread: %s\n", strerror(err));
	//}

	err = pthread_create(&dtid, NULL, delay_thread, targ);

	if (err != 0) {
		printf("can't create thread: %s\n", strerror(err));
	}

	main_proc(targ);

	//pthread_join(rtid, NULL);
	//pthread_join(wtid, NULL);
	pthread_join(dtid, NULL);

	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	int ret = 0;

	char *reg_dev = argv[1];
	char *dma_dev = argv[2];

	thread_arg_t targ;
	int flags;

	int mmap_size;
	//char *mmap_memory;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((targ.reg_fd = open(reg_dev, O_RDWR)) < 0) {
		printf("err: can't open device(%s)!(%s)\n", reg_dev, strerror(errno));
		ret = -1;
		return ret;
	}

	if ((targ.dma_fd = open(dma_dev, O_RDWR)) < 0) {
		printf("err: can't open device(%s)!(%s)\n", dma_dev, strerror(errno));
		ret = -1;
		return ret;
	}

	//flags = fcntl(targ.dma_fd, F_GETFL, 0);
	////用以下方法将socket设置为非阻塞方式
	//fcntl(targ.dma_fd, F_SETFL, flags | O_NONBLOCK);

	//ret = ioctl(targ.dma_fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	//if (ret != 0) {
	//	printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
	//	return ret;
	//}
	//printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	//mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, targ.dma_fd, (off_t)0);
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

	raw_data = (uint32_t *)malloc(RAW_DATA_SIZE);
	targ.buffer = (unsigned char *)malloc(BUFSIZE);

	if(targ.buffer == 0) {
		printf("err: no memory for targ.buffer!\n");
		ret = -1;
		return ret;
	}

	read_write(&targ);

	free(targ.buffer);

	close(targ.dma_fd);
	close(targ.reg_fd);

	return ret;
}

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
#define RAW_DATA_SIZE (5 * 4 * 1500)

#define BUFSIZE (7 * 4 * 1500)

static int stop = 0;
static int force_stop = 0;

static void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);

	if(stop == 0) {
		stop = 1;
	} else {
		force_stop = 1;
		printf("xiaofei:force stop!\n");
	}

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
	//struct timeval tv;
	//fd_set rfds;
	//fd_set efds;
	//int nread;

	//printids("read_fn: ");

	//tv.tv_sec=1;
	//tv.tv_usec=0;

	while(stop == 0) {
		//FD_ZERO(&rfds);
		//FD_ZERO(&efds);
		//FD_SET(targ->fd, &rfds);
		//FD_SET(targ->fd, &efds);

		//if(select(targ->fd + 1, &rfds, NULL, &efds, &tv) > 0) {
		//	if(FD_ISSET(targ->fd, &rfds)) {
		//		//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		//		nread = read(targ->fd, targ->buffer, BUFSIZE);
		//		if(nread < 0) {
		//			printf("%s\n", strerror(errno));
		//		} else {
		//			int i;
		//			uint32_t *data = (uint32_t *)targ->buffer;

		//			printf("read %d!\n", nread);
		//			for(i = 0; i < nread / sizeof(uint32_t); i++) {
		//				if((i != 0) && (i % 12 == 0)) {
		//					printf("\n");
		//				}
		//				printf("%04x ", data[i] & 0xffff);
		//			}
		//			printf("\n");

		//			//return NULL;
		//		}
		//	}
		//	if(FD_ISSET(targ->fd, &efds)) {
		//		//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		//	}
		//}

		return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	//int nwrite;

	//printids("write_fn: ");

	while(stop == 0) {
		//nwrite = write(targ->fd, raw_data, RAW_DATA_SIZE);
		//if(nwrite < 0) {
		//	printf("%s\n", strerror(errno));
		//} else {
		//	//printf("write %d!\n", nwrite);
		//}
		return NULL;
	}
	return NULL;
}

int init_raw_data(int start) {
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

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	int nwrite = RAW_DATA_SIZE;
	int nread = BUFSIZE;
	int count;
	int start = 1;

	//printids("main_proc: ");
	
        struct timeval tv0, tv1;
        struct timezone tz0, tz1;

        gettimeofday(&tv0, &tz0);

	while(stop == 0) {
		for(count = 0; count < 1;) {
			if(nwrite == RAW_DATA_SIZE) {
				start = init_raw_data(start);
				if(start >= 50000) {
					//stop = 1;
				}
				//printf("start %d! count:%d!\n", start, count);
			} else {
				//printf("nwrite %d! count:%d!\n", nwrite, count);
			}

			nwrite = write(targ->fd, raw_data, RAW_DATA_SIZE);
			if(nwrite < 0) {
				printf("%s\n", strerror(errno));
			} else {
				//printf("write %d!\n", nwrite);
				if(nwrite == RAW_DATA_SIZE) {
					count++;
				} else if(nwrite != 0) {
					break;
				} else {
					if(force_stop == 1) {
						break;
					}
				}
			}
		}

		//usleep(100000);

		for(count = 0; count < 1;) {
			nread = read(targ->fd, targ->buffer, BUFSIZE);
			if(nread < 0) {
				printf("%s\n", strerror(errno));
			} else {
				int i;
				uint32_t *data = (uint32_t *)targ->buffer;

				//printf("read %d!\n", nread);
				if(nread == BUFSIZE) {
					count++;
				} else if(nread != 0) {
					break;
				} else {
					if(force_stop == 1) {
						break;
					}
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

		//return;
	}
        gettimeofday(&tv1, &tz1);

        printf("tv0.tv_sec:%d\n", (int)tv0.tv_sec);
        printf("tv0.tv_usec:%d\n", (int)tv0.tv_usec);
        printf("tz0.tz_minuteswest:%d\n", (int)tz0.tz_minuteswest);
        printf("tz0.tz_dsttime:%d\n", (int)tz0.tz_dsttime);

        printf("tv1.tv_sec:%d\n", (int)tv1.tv_sec);
        printf("tv1.tv_usec:%d\n", (int)tv1.tv_usec);
        printf("tz1.tz_minuteswest:%d\n", (int)tz1.tz_minuteswest);
        printf("tz1.tz_dsttime:%d\n", (int)tz1.tz_dsttime);
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

	//ret = ioctl(targ.fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	//if (ret != 0) {
	//	printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
	//	return ret;
	//}
	//printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

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

	raw_data = (uint32_t *)malloc(RAW_DATA_SIZE);
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

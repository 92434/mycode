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


unsigned int raw_data[] = {
};
#define RAW_DATA_SIZE (sizeof(raw_data) / sizeof(char))

#define BUFSIZE (87 * 4 * 1)

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


unsigned int sz_constant[] = {
	0x0b77, 0xa1dd, 0x4240, 0x2f84, 0x2b03, 0x8143, 0x4339, 0xfa53, 0x2fe6, 0x0c1f, 0x344d, 0x7205, 0x5609, 0xfc64, 0x8f03, 0x679c,
	0x3aa1, 0xee1f, 0x6c51, 0xa601, 0x8e45, 0x50a6, 0x0700, 0x8f01, 0x3a3f, 0x0031, 0xa47e, 0xefc3, 0x004f, 0x1e06, 0xe475, 0xaa42,
	0xe580, 0xcb23, 0xb18d, 0x3325, 0x203c, 0xd37d, 0x7c09, 0x5c4a, 0xe6c2, 0xaa69, 0x807a, 0xba12, 0x186e, 0x13e4, 0x0e85, 0xdfb8,
	0x2c4d, 0x59da, 0xd6b1, 0x006e, 0x857a, 0x1866, 0x30ec, 0x262e, 0x4e33, 0x4b38, 0xc1ce, 0x513d, 0x203c, 0x0a65, 0x7589, 0x9267,
	0xae94, 0x34b7, 0x4044, 0x276b, 0xc054, 0xdf24, 0x97c1, 0x48b3, 0x9560, 0x90d6, 0x5c6a, 0x12a5, 0x0053, 0x8bb1, 0x7e00, 0x8924,
	0x451c, 0xc16d, 0xdb13, 0xe6b5, 0x935e, 0xd6b0, 0xd7a5
};

void *read_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;
	struct timeval tv;
	fd_set rfds;
	fd_set efds;
	int nread = 0;
	int toread = BUFSIZE;

	//printids("read_fn: ");

	tv.tv_sec=1;
	tv.tv_usec=0;

	while(stop == 0) {
		if((toread > 0) && (nread == 0)) {
			FD_ZERO(&rfds);
			FD_ZERO(&efds);
			FD_SET(targ->fd, &rfds);
			FD_SET(targ->fd, &efds);

			if(select(targ->fd + 1, &rfds, NULL, &efds, &tv) > 0) {
				if(FD_ISSET(targ->fd, &rfds)) {
					//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
					nread = read(targ->fd, targ->buffer + (BUFSIZE - toread), toread);
					if(nread < 0) {
						printf("%s\n", strerror(errno));
						return NULL;
					}

					toread -= nread;

				}
				if(FD_ISSET(targ->fd, &efds)) {
					//printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
				}
			}
		} else {
			int i;
			uint32_t *data = (uint32_t *)targ->buffer;
			static uint16_t last_data = 0;
			static unsigned char last_byte = 0;

			toread = BUFSIZE;

			//printf("read %d!\n", nread);
			if(nread > 0) {
				if(
					//(1)
					((data[0] & 0xffff) == 0x0b77)
					//&& ((data[1] & 0xffff) == 0xa1dd)
					//&& ((data[2] & 0xffff) == 0x4240)
					//&& ((data[3] & 0xffff) == 0x2f84)
					//&& ((data[4] & 0xffff) == 0x2b03)
					//&& ((data[5] & 0xffff) == 0x8143)
					//&& ((data[6] & 0xffff) == 0x4339)
					//&& ((data[7] & 0xffff) == 0xfa53)
					//&& (last_data != (data[86] & 0xffff))
					//&& (last_byte != (data[79] & 0xff))
					//&& (0x01 == ((data[78] >> 8) & 0xff))
					//&& (0x01 == ((data[79]) & 0xff))
				) {
					char *msg_start = "";
					char *msg_end = "";
					int nread_reg = 0;
					for(i = 0; i < nread / sizeof(uint32_t); i++) {
						if((sz_constant[i] & 0xffff) != (data[i] & 0xffff)) {
							if(
								(i == 78)
							) {
							} else {
								nread_reg = nread;
							}
						} else {
						}
					}
					//nread = nread_reg;
					for(i = 0; i < nread / sizeof(uint32_t); i++) {
						if((i != 0) && (i % 16 == 0)) {
							printf("\n");
						}
						//printf("%02d%02d%04x ", ((data[i] >> 24) & 0xff), ((data[i] >> 16) & 0xff), (data[i] & 0xffff));
						if((sz_constant[i] & 0xffff) != (data[i] & 0xffff)) {
							if(
								(i == 78)
							) {
								msg_start = "";
								msg_end = "";
							} else {
								msg_start = "\033[31m";
								msg_end = "\033[0m";
							}
						} else {
							msg_start = "";
							msg_end = "";
						}
						printf("%s%02x %02x %s", msg_start, ((data[i] >> 8) & 0xff), ((data[i] >> 0) & 0xff), msg_end);
						//printf("(%d)[%02x %02x]%02x %02x ",i, ((data[i] >> 24) & 0xff), ((data[i] >> 16) & 0xff),
						//	((data[i] >> 8) & 0xff), ((data[i] >> 0) & 0xff));
					}
					{
        					struct timeval tv0;
        					struct timezone tz0;
						static int count = 0;

						gettimeofday(&tv0, &tz0);
						if(nread != 0) {
							count = 0;
						} else {
							count += 1;
						}
						printf("\n[tv0.tv_sec:%d, tv0.tv_usec:%d]%d\n", (int)tv0.tv_sec, (int)tv0.tv_usec, count);
					}

					//if((data[86] & 0xffff) != last_data + 1) {
					//	printf("last:%04x, now:%04x\n", last_data, (data[86] & 0xffff));
					//}

					last_byte = (data[79] & 0xff);

					last_data = (data[86] & 0xffff);
				} else {
				}
			}

			nread = 0;
		}
		//return NULL;
	}
	return NULL;
}

void *write_fn(void *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("write_fn: ");

	while(stop == 0) {
		return NULL;
	}
	return NULL;
}

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("main_proc: ");

	while(stop == 0) {

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

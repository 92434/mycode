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

#define BUFSIZE 0x1000

#include "kc705.h"

typedef void (*sig_action_t)(int, siginfo_t *, void *);

static int stop = 0;
static char *mmap_memory = NULL;
static int mmap_size = 0;


void default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called!\n", __func__);
	/* Ensure it's our signal */
	if(stop == 0) {
		stop = 1;
	} else {
		exit(0);
	}

	return;
}

int catch_signal(sig_action_t sig_action) {
	int ret = 0;
	struct sigaction sig_act;

	if(sig_action == 0) {
		sig_action = default_sig_action;
	}

	/* Set up the signal handler */
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_SIGINFO;
	sig_act.sa_sigaction = sig_action;

	/* Map the Signal to the Signal Handler */
	if((ret = sigaction(SIGINT, &sig_act, NULL)) == -1) {
		perror("sigaction");
		return ret;
	}

	return ret;
}

int check_buffer(unsigned int *pdata, int count, unsigned int *pre_value) {
	int ret = 0;
	int i;
	unsigned int ui0, ui1;

	ui0 = ui1 = *pre_value;

	for(i = 0; i < count; i++) {
		ui1 = pdata[i] & 0xffffff;
		
		if(ui1 == ui0 + 1) {
		} else if(ui1 == 0) {
		} else if(ui0 == 0) {
		} else {
			ret = -1;
			printf("!!!failed!!!(%d)\n", i);
			printf("ui0:%010d ui1:%010d\n", ui0, ui1);
		}

		ui0 = ui1;
	}

	*pre_value = ui0;
	return ret;
}

static struct sockaddr_in sock_addr;

static int init_udp_client(unsigned char *ip_addr, unsigned int port) {
	int ret = 0;
	memset(&sock_addr, 0, sizeof(sock_addr));

	//协议地址
	sock_addr.sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr.sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr.sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	return ret;
}

static int init_tcp_client(unsigned char *ip_addr, unsigned int port) {
	int ret = 0;
	memset(&sock_addr, 0, sizeof(sock_addr));

	//协议地址
	sock_addr.sin_family = AF_INET;

	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
	if((ret = inet_pton(AF_INET, ip_addr, &sock_addr.sin_addr)) <= 0){
		printf("inet_pton error for %s\n", ip_addr);
		exit(0);
	}

	sock_addr.sin_port = htons(port);

	if((ret = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if(connect(ret, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0){
		//连接请求
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	}

	return ret;
}

static int udp_send_data(int fd, unsigned char *buffer, unsigned int len) {
	int ret = 0;
	unsigned int step = 256;

	while(len != 0) {
		ret = sendto(fd, buffer, step, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
		if(ret <= 0) {
			printf("err: para error!:%s\n", strerror(errno));
		} else {
			//printf("send data len %d!\n", ret);
		}

		len -= step;
		buffer += step;
	}
	return ret;
}

static int tcp_send_data(int fd, unsigned char *buffer, unsigned int len) {
	int ret = 0;
	unsigned int step = 1024;

	while(len != 0) {
		ret = send(fd, buffer, step, 0);
		if(ret <= 0) {
			printf("err: para error!:%s\n", strerror(errno));
		} else {
			//printf("send data len %d!\n", ret);
		}

		len -= step;
		buffer += step;
	}
	return ret;
}

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

int write_tr(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *tx_data, int tx_size, int tx_dest_offset) {
	int ret = 0;
	pseudo_dma_tr->tx_data = 0;
	pseudo_dma_tr->rx_data = 0;
	pseudo_dma_tr->tx_size = 0;
	pseudo_dma_tr->rx_size = 0;
	pseudo_dma_tr->tx_dest_offset = 0;
	pseudo_dma_tr->rx_src_offset = 0;

	pseudo_dma_tr->tx_data = tx_data;
	pseudo_dma_tr->tx_size = tx_size;
	pseudo_dma_tr->tx_dest_offset = tx_dest_offset;

	ret = ioctl(fd, PCIE_DEVICE_IOCTL_PSEUDO_DMA_TR, pseudo_dma_tr);

	return ret;
}

int read_tr(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *rx_data, int rx_size, int rx_src_offset) {
	int ret = 0;
	pseudo_dma_tr->tx_data = 0;
	pseudo_dma_tr->rx_data = 0;
	pseudo_dma_tr->tx_size = 0;
	pseudo_dma_tr->rx_size = 0;
	pseudo_dma_tr->tx_dest_offset = 0;
	pseudo_dma_tr->rx_src_offset = 0;

	pseudo_dma_tr->rx_data = rx_data;
	pseudo_dma_tr->rx_size = rx_size;
	pseudo_dma_tr->rx_src_offset = rx_src_offset;

	ret = ioctl(fd, PCIE_DEVICE_IOCTL_PSEUDO_DMA_TR, pseudo_dma_tr);

	return ret;
}

int write_ts_pack(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *tx_data, uint8_t *rx_data, int slot) {
	int ret = 0;
	uint32_t *wdata = (uint32_t *)tx_data;
	int i;
	static int loc = 0;

	int tx_size = (slot == 18) ? 188 * 2 : (slot >= 2 && slot < 18) ? 188 : 0;

	*wdata = slot;//replacer #17
	ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_INDEX));
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	for(i = 0; i < tx_size; i++) {
		tx_data[i] = loc + i;
	}
	if(tx_size != 0) {
		loc++;
	}
	ret = write_tr(fd, pseudo_dma_tr, tx_data, tx_size, ADDR_OFFSET(ADDR_TS_DATA_BASE));
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int read_ts_pack(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *tx_data, uint8_t *rx_data, int slot) {
	int ret = 0;
	uint32_t *wdata = (uint32_t *)tx_data;
	uint32_t *rdata = (uint32_t *)rx_data;
	int i;

	int rx_size = (slot == 18) ? 188 * 2 : (slot >= 0 && slot < 18) ? 188 : 0;

	*wdata = 0;//any value
	ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_READ_REQUEST));
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*rdata = 0;
	while(*rdata == 0) {
		ret = read_tr(fd, pseudo_dma_tr, rx_data, 4, ADDR_OFFSET(ADDR_READ_REQUEST));
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
	}

	ret = read_tr(fd, pseudo_dma_tr, rx_data, rx_size, ADDR_OFFSET(ADDR_TS_DATA_BASE));//will valid after 3 clock
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	printf("\n");
	for(i = 0; i < rx_size; i++) {
		if((i != 0) && (i % 16 == 0)) {
			printf("\n");
		}
		printf("%02x ", rx_data[i]);
	}
	printf("\n");

	return ret;
}

int test_ts_pack_write_read(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *tx_data, uint8_t *rx_data) {
	int ret = 0;
	uint32_t *wdata = (uint32_t *)tx_data;
	uint32_t *rdata = (uint32_t *)rx_data;
	int i;

	for(i = 0; i < 19; i++) {
		ret = write_ts_pack(fd, pseudo_dma_tr, tx_data, rx_data, i);
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		ret = read_ts_pack(fd, pseudo_dma_tr, tx_data, rx_data, i);
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
	}

	return ret;
}

int test_pid_op(int fd, pseudo_dma_tr_t *pseudo_dma_tr, uint8_t *tx_data, uint8_t *rx_data) {
	int ret = 0;
	uint32_t *wdata = (uint32_t *)tx_data;
	uint32_t *rdata = (uint32_t *)rx_data;
	int i;
	//int pid = 0x1570;
	int pid = 0x0;

	for(i = 0; i < 2 + 16 + 1; i ++) {
		int j;
		int pid_slot = (i == 18) ? 16 : 1;

		*wdata = i;//
		ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_INDEX));
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		
		for(j = 0; j < pid_slot; j++) {
			*wdata = j;
			ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_PID_INDEX));
			if (ret != 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

			*wdata = PID_INFO(1, pid);
			//pid++;
			ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_PID));
			if (ret != 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

		}

		//*wdata = i % 2;
		*wdata = (i == 0 || i == 1) ? 1 : 0;//
		ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_MATCH_ENABLE));
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

	}

	for(i = 0; i < 2 + 16 + 1; i++) {
		int j;
		int pid_slot = (i == 18) ? 16 : 1;

		*wdata = i;//
		ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_INDEX));
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}

		for(j = 0; j < pid_slot; j++) {
			*wdata = j;//
			ret = write_tr(fd, pseudo_dma_tr, tx_data, 4, ADDR_OFFSET(ADDR_PID_INDEX));
			if (ret != 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}

			ret = read_tr(fd, pseudo_dma_tr, rx_data, 4, ADDR_OFFSET(ADDR_PID));
			if (ret != 0) {
				printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
				return ret;
			}
			printf("slot %d pid_index %d pid:%08x\n", i, j, *rdata);
		}

		ret = read_tr(fd, pseudo_dma_tr, rx_data, 4, ADDR_OFFSET(ADDR_MATCH_ENABLE));
		if (ret != 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
		printf("slot %d match enable:%s\n", i, *rdata == 1 ? "true" : *rdata == 0 ? "false" : "other");

	}

	return ret;
}

int tr_test(int fd) {
	int ret;
	int i;
	int tx_size = 1024;
	int rx_size = 1024;
	uint8_t *tx_data = NULL;
	uint8_t *rx_data = NULL;
	int tx_dest_offset = 0;
	int rx_src_offset = 0;
	pseudo_dma_tr_t *pseudo_dma_tr = NULL;

	tx_data = (uint8_t *)malloc(tx_size);
	if(tx_data == 0) {
		printf("err: no memory for pseudo_dma_tr!\n");
		ret = -1;
		stop = 1;
	} else {
		for(i = 0; i < tx_size; i++) {
			tx_data[i] = i;
		}
	}

	rx_data = (uint8_t *)malloc(rx_size);
	if(rx_data == 0) {
		printf("err: no memory for pseudo_dma_tr!\n");
		ret = -1;
		stop = 1;
	} else {
		for(i = 0; i < rx_size; i++) {
			rx_data[i] = 0;
		}
	}

	pseudo_dma_tr = (pseudo_dma_tr_t *)malloc(sizeof(pseudo_dma_tr_t));
	if(pseudo_dma_tr == 0) {
		printf("err: no memory for pseudo_dma_tr!\n");
		ret = -1;
		stop = 1;
	}

	while(stop == 0) {

		test_pid_op(fd, pseudo_dma_tr, tx_data, rx_data);
		test_ts_pack_write_read(fd, pseudo_dma_tr, tx_data, rx_data);

		break;

		usleep(1000);
	}

	if(pseudo_dma_tr != NULL) {
		free(pseudo_dma_tr);
	}
	if(rx_data != NULL) {
		free(rx_data);
	}
	if(tx_data != NULL) {
		free(tx_data);
	}

	return ret;
}

int main(int argc, char **argv) {
	int ret = 0;
	int fd, sock_fd;
	unsigned char *read_buf;
	char *dev = argv[1];
	int flags;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((fd = open(dev, O_RDONLY))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev, strerror(errno));
		ret = -1;
		return ret;
	}

	flags = fcntl(fd, F_GETFL, 0);
	//用以下方法将socket设置为非阻塞方式
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	ret = ioctl(fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	if (ret != 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	//mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
	mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ, MAP_SHARED, fd, (off_t)0);
	if(mmap_memory == (void *)-1) {
		printf("xiaofei: %s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		return ret;
	}

	if(catch_signal(0) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	tr_test(fd);

	close(fd);

	return ret;
}


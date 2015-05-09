#include <stdio.h>     
#include <unistd.h>    
#include <signal.h>
#include <stdlib.h>   
#include <errno.h>    
#include <string.h>
#include <fcntl.h>    

#define BUFSIZE 4096

static int stop = 0;

typedef void (*sig_action_t)(int, siginfo_t *, void *);

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

int check_buffer(unsigned int *pdata, int count, unsigned char *pre_value) {
	int rtn = 0;
	int i;
	unsigned char c0, c1;
	
	c0 = c1 = *pre_value;

	for(i = 0; i < count; i++) {
		c1 = (pdata[i] >> 8) & 0xff;
		switch(c0) {
			case 0x12:
				if(c1 != 0x34) {
					goto failed;
				}
				break;
			case 0x34:
				if(c1 != 0x56) {
					goto failed;
				}
				break;
			case 0x56:
				if(c1 != 0x78) {
					goto failed;
				}
				break;
			case 0x78:
				if(c1 != 0x90) {
					goto failed;
				}
				break;
			case 0x90:
				if(c1 != 0x12) {
					goto failed;
				}
				break;
			default:
				break;
		}
		c0 = c1;
	}

	*pre_value = c0;
	//printf("!!!success!!!\n");
	return rtn;
failed:
	printf("!!!failed!!!\n");
	rtn = -1;
	return rtn;
}

int main(int argc, char **argv) {
	int ret = 0;
	int fd;
	unsigned char *read_buf;
	struct timeval tv;
	char *dev = argv[1];
	fd_set fds;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((fd = open(dev, O_RDONLY))<0) {
		printf("err: can't open serial port!(%s)\n", strerror(errno));
		ret = -1;
		return ret;
	}

	int flags = fcntl(fd, F_GETFL, 0);

	//用以下方法将socket设置为非阻塞方式
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	if(catch_signal(0) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	read_buf = (unsigned char *)malloc(BUFSIZE);
	if(read_buf == 0) {
		printf("err: no memory for read_buf!\n");
		ret = -1;
		return ret;
	}


	tv.tv_sec=30;
	tv.tv_usec=0;
	while(stop == 0) {
		//printf("wait...\n");
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		if(select(fd + 1, &fds, NULL, NULL, &tv) > 0) {
			if(FD_ISSET(fd, &fds)) {
				int nread;
				unsigned int *pdata = (unsigned int *)read_buf;
				int i;
				static unsigned char pre_value = 0;

				nread = read(fd, read_buf, BUFSIZE);
				//printf("nread:%d\n", nread);
				if(nread <= 0) {
					continue;
				}
				//for(i = 0; i < nread / sizeof(unsigned int); i++) {
				//	printf("%02x ", (pdata[i] >> 8) & 0xff);
				//}
				//printf("\n");
				check_buffer(pdata, nread / sizeof(unsigned int), &pre_value);
			}
		}
	}

	free(read_buf);

	close(fd);

	return ret;
}

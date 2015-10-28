#include <stdio.h>     
#include <stdlib.h>   
#include <unistd.h>    
#include <sys/types.h>
#include <sys/stat.h>  
#include <fcntl.h>    
#include <termios.h>  
#include <errno.h>    
#include <string.h>
#include <signal.h>

#define BUFSIZE 512
#define AUTO_DEBUG

static int stop = 0;
typedef void (*sig_action_t)(int, siginfo_t *, void *);

//初始化串口选项：  
void set_termios(struct termios * pnewtio, int baudrate)
{
	bzero(pnewtio, sizeof(struct termios));
	//8N1
	pnewtio->c_cflag = baudrate | CS8 | CREAD | CLOCAL;
	pnewtio->c_iflag = IGNPAR;
	pnewtio->c_oflag = 0;
	pnewtio->c_lflag = 0; //non ICANON

	pnewtio->c_cc[VINTR] = 0;
	pnewtio->c_cc[VQUIT] = 0;
	pnewtio->c_cc[VERASE] = 0;
	pnewtio->c_cc[VKILL] = 0;
	pnewtio->c_cc[VEOF] = 4;
	pnewtio->c_cc[VTIME] = 5;
	pnewtio->c_cc[VMIN] = 0;
	pnewtio->c_cc[VSWTC] = 0;
	pnewtio->c_cc[VSTART] = 0;
	pnewtio->c_cc[VSTOP] = 0;
	pnewtio->c_cc[VSUSP] = 0;
	pnewtio->c_cc[VEOL] = 0;
	pnewtio->c_cc[VREPRINT] = 0;
	pnewtio->c_cc[VDISCARD] = 0;
	pnewtio->c_cc[VWERASE] = 0;
	pnewtio->c_cc[VLNEXT] = 0;
	pnewtio->c_cc[VEOL2] = 0;
}


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

int main(int argc, char **argv) {
	int ret = 0;
	int fd;
	int nread, nwrite, writed;
	unsigned char *read_buf, *write_buf;
	struct termios oldtio, newtio;
	struct timeval tv;
	char *dev = argv[1];
	fd_set fds;
	int i;

	if(argc < 2) {
		printf("err: para error!:%s\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if ((fd = open(dev, O_RDWR | O_NOCTTY))<0) {
		printf("err: can't open serial port!(%s)\n", strerror(errno));
		ret = -1;
		return ret;
	}

	if(catch_signal(0) == -1) {
		printf("err: can't catch sigio!\n");
		ret = -1;
		return ret;
	}

	read_buf = malloc(BUFSIZE);
	if(read_buf == 0) {
		printf("err: no memory for read_buf!\n");
		ret = -1;
		return ret;
	}

	write_buf = malloc(BUFSIZE);
	if(write_buf == 0) {
		printf("err: no memory for write_buf!\n");
		ret = -1;
		return ret;
	}

	tcgetattr(fd, &oldtio);
	set_termios(&newtio, B115200);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);


#ifdef AUTO_DEBUG
	nwrite = 256;
	for(i = 0; i < nwrite; i++) {
		write_buf[i] = i;
	}

	while(nwrite) {
		writed = write(fd, write_buf, nwrite);
		nwrite -= writed;
	}
#endif

	tv.tv_sec=30;
	tv.tv_usec=0;
	while(stop == 0) {
		//printf("wait...\n");
		FD_ZERO(&fds);
		FD_SET(0, &fds);
		FD_SET(fd, &fds);
		if(select(fd + 1, &fds, NULL, NULL, &tv) > 0) {
#ifdef AUTO_DEBUG
			if(FD_ISSET(fd, &fds)) {
				nread = read(fd, read_buf, BUFSIZE - 1);
				for(i = 0; i < nread; i++) {
					printf("%02x ", read_buf[i]);
				}
				printf("\n");

				nwrite = 256;
				for(i = 0; i < nwrite; i++) {
					write_buf[i] = i;
				}

				while(nwrite) {
					writed = write(fd, write_buf, nwrite);
					nwrite -= writed;
				}
				sleep(2);
			}
#else
			if(FD_ISSET(fd, &fds)) {
				nread = read(fd, read_buf, BUFSIZE - 1);
				read_buf[nread]='\0';
				printf("%s", read_buf);
				printf("\n");
			} else if(FD_ISSET(0, &fds)) {
				fgets(write_buf, BUFSIZE - 1, stdin);
				nwrite = strlen(write_buf);
				//nwrite = read(0, write_buf, BUFSIZE - 1);
				while(nwrite) {
					writed = write(fd, write_buf, nwrite);
					nwrite -= writed;
				}
			}
#endif
		}
	}
	tcsetattr(fd, TCSANOW, &oldtio);

	free(write_buf);
	free(read_buf);

	close(fd);

	return ret;
}

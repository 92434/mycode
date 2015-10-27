#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>

#define TEXT_SIZE 512

struct message {
	int mtype ;
	int status ;
	char time[20] ;
	char mtext[TEXT_SIZE] ;
} ;

char *getxtsj() { 
	time_t tv ;
	struct tm *tmp ;
	static char buf[20] ;
	tv = time(0) ;
	tmp = localtime(&tv) ;
	sprintf(buf,"%02d:%02d:%02d",tmp->tm_hour , tmp->tm_min,tmp->tm_sec);
	return buf ;
}

int main(int argc, char **argv) {
	int msqid ;
	struct msqid_ds info ;
	struct message buf ;
	struct message buf1 ;
	int flag ;
	int sendlength, recvlength ;
	int key ;

	unlink("msg.tmp");  

	system("touch msg.tmp");

	key = ftok("msg.tmp", 0x01) ;

	if (key < 0) {
		perror("ftok key error") ;
		return -1 ;
	}

	msqid = msgget(key, 0600|IPC_CREAT) ;

	if (msqid < 0) {
		perror("create message queue error") ;
		return -1 ;
	}

	buf.mtype = 1 ;

	buf.status = 9 ;

	strcpy(buf.time, getxtsj()) ;

	strcpy(buf.mtext, "happy new year!") ;

	sendlength = sizeof(struct message) - sizeof(long) ;

	flag = msgsnd(msqid, &buf, sendlength , 0) ;

	if (flag < 0) {
		perror("send message error") ;
		return -1 ;
	}

	buf.mtype = 3 ;

	buf.status = 9 ;

	strcpy(buf.time, getxtsj()) ;

	strcpy(buf.mtext, "good bye!") ;

	sendlength = sizeof(struct message) - sizeof(long) ;

	flag = msgsnd(msqid, &buf, sendlength , 0) ;

	if (flag < 0) {
		perror("send message error") ;
		return -1 ;
	}

	system("ipcs -q") ;

	return 0 ;

}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define TEXT_SIZE 512

struct message {
	int mtype ;
	int status ;
	char time[20] ;
	char mtext[TEXT_SIZE] ;
} ;

int main(int argc, char **argv) {
	int msqid ;
	struct msqid_ds info ;
	struct message buf1 ;
	int flag ;
	int recvlength ;
	int key ;
	int mtype ;

	key = ftok("msg.tmp", 0x01) ;
	if (key < 0) {
		perror("ftok key error") ;
		return -1 ;
	}

	msqid = msgget(key, 0) ;
	if (msqid < 0) {
		perror("get ipc_id error") ;
		return -1 ;
	}

	recvlength = sizeof(struct message) - sizeof(long) ;

	memset(&buf1, 0x00, sizeof(struct message)) ;

	mtype = 1 ;

	flag = msgrcv(msqid, &buf1, recvlength ,mtype,0) ;
	if (flag < 0) {
		perror("recv message error\n") ;
		return -1 ;
	}

	printf("type=%d,time=%s, message=%s\n", buf1.mtype, buf1.time, buf1.mtext) ;
	system("ipcs -q") ;

	unlink("msg.tmp");  
	return 0 ;
}

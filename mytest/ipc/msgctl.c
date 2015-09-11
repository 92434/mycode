#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <error.h>
#include <stdlib.h>

#define TEXT_SIZE  512

struct message {
	int mtype;
	char mtext[TEXT_SIZE] ;
};

int main(int argc, char **argv) {
	int msqid ;
	struct msqid_ds info ;
	struct message buf ;
	struct message buf1 ;
	int flag ;
	int sendlength, recvlength ;

	msqid = msgget(IPC_PRIVATE, 0666) ;
	if (msqid < 0) {
		perror("get ipc_id error") ;
		return -1 ;
	}

	printf("msqid:%d\n", msqid);

	buf.mtype = 1 ;

	strcpy(buf.mtext, "happy new year!") ;

	sendlength = sizeof(struct message) - sizeof(long) ;

	flag = msgsnd(msqid, &buf, sendlength , 0) ;
	if (flag < 0) {
		perror("send message error") ;
		return -1 ;
	}

	buf.mtype = 3 ;

	strcpy(buf.mtext, "good bye!") ;

	sendlength = sizeof(struct message) - sizeof(long) ;

	flag = msgsnd(msqid, &buf, sendlength , 0) ;

	if (flag < 0) {
		perror("send message error") ;
		return -1 ;
	}

	flag = msgctl(msqid, IPC_STAT, &info) ;

	if (flag < 0) {
		perror("get message status error") ;
		return -1 ;
	}

	printf("uid:%d, gid = %d, cuid = %d, cgid= %d\n" , info.msg_perm.uid,  info.msg_perm.gid,  info.msg_perm.cuid,  info.msg_perm.cgid ) ;

	printf("read-write:%03o, cbytes = %lu, qnum = %lu, qbytes= %lu\n" , info.msg_perm.mode&0777, info.msg_cbytes, info.msg_qnum, info.msg_qbytes) ;

	system("ipcs -q") ;

	recvlength = sizeof(struct message) - sizeof(long) ;

	memset(&buf1, 0x00, sizeof(struct message)) ;

	flag = msgrcv(msqid, &buf1, recvlength, 3, 0) ;

	if (flag < 0) {
		perror("recv message error") ;
		return -1 ;
	}

	printf("type=%d, message=%s\n", buf1.mtype, buf1.mtext) ;

	flag = msgctl(msqid, IPC_RMID, NULL) ;

	if (flag < 0) {
		perror("rm message queue error") ;
		return -1 ;
	}

	system("ipcs -q") ;

	return 0 ;

}

#include    <sys/types.h>  
#include    <sys/ipc.h>  
#include    <sys/shm.h>  
#include    <stdio.h>  
#include    <error.h>  
#include    <stdlib.h>  

#define SHM_SIZE    4096  
#define SHM_MODE    (SHM_R | SHM_W) /* user read/write */  

int main(void)  
{  
	int     shmid;  
	char    *shmptr;  
	int key;
	struct shmid_ds shmid_ds;

	key = ftok("shm.tmp", 0x03) ;
	if (key < 0) {
		perror("ftok key error") ;
		return -1 ;
	}

	printf("key:%x\n", key);
	if ( (shmid = shmget(key, SHM_SIZE, SHM_MODE)) < 0) {
	//if ( (shmid = shmget(0x44, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");  
		return -1;
	}

	if ( (shmptr = (char *)shmat(shmid, 0, 0)) == (void *) -1) {
		perror("shmat");  
		return -1;
	}

	/* 从共享内存读数据 */  
	printf("%s\n", shmptr);  

	//解除共享内存映射
	if((shmdt(shmptr)) < 0) {
		perror("shmdt");
		return -1;
	}

	if((shmctl(shmid, IPC_RMID, &shmid_ds)) < 0) {
		perror("shmctl");
		return -1;
	}

	exit(0);  
}  

#include    <sys/types.h>  
#include    <sys/ipc.h>  
#include    <sys/shm.h>  
#include    <stdio.h>  
#include    <error.h>  
#include    <stdlib.h>  
#include    <time.h>  

#define SHM_SIZE    4096  
#define SHM_MODE    (SHM_R | SHM_W | IPC_CREAT) /* user read/write */  

int main(void)  
{  
	int     shmid;  
	char    *shmptr;  
	int key;
	struct shmid_ds shmid_ds;
	struct tm *tmp ;

	system("touch shm.tmp");
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


	/* 往共享内存写数据 */  
	sprintf(shmptr, "%s", "hello, world");  

	//解除共享内存映射
	if((shmdt(shmptr)) < 0) {
		perror("shmdt");
		return -1;
	}

	if((shmctl(shmid, SHM_STAT, &shmid_ds)) < 0) {
		perror("shmctl");
		return -1;
	}

	tmp = localtime(&shmid_ds.shm_atime) ;
	printf("attach:%02d:%02d:%02d\n",tmp->tm_hour , tmp->tm_min,tmp->tm_sec);
	tmp = localtime(&shmid_ds.shm_dtime) ;
	printf("detach:%02d:%02d:%02d\n",tmp->tm_hour , tmp->tm_min,tmp->tm_sec);
	tmp = localtime(&shmid_ds.shm_ctime) ;
	printf("change:%02d:%02d:%02d\n",tmp->tm_hour , tmp->tm_min,tmp->tm_sec);
	return 0;
}  

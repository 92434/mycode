
/*
 * shmem.c
 *
 *  Created on: 2012-7-20
 *      Author: liwei.cai
 */
/**
 * 该实例说明：如何使用基本的共享内存函数。
 * 功能：首先创建一个共享内存区，之后创建子进程，在父子进程中将共享内存
 * 分别映射到各自的进程地址空间中。
 * 	父进程先等待用户输入，然后将用户输入的字符串写入到共享内存，之
 * 后往共享内存的头部写入“WROTE”字符串表示父进程已成功写入数据。子进程
 * 一直等待共享内存的头部字符串是“WROTE”，然后将共享内存的有效数据打印
 * 出来，之后父子进程分别解除与共享内存的映射。
 * 	最后在子进程中删除共享内存。
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

int main()
{
	pid_t pid;
	int shmid;
	char flag[] = "WROTE";
	char buff[BUFFER_SIZE];

	//创建共享内存
	if ((shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}
	else
	{
		printf("Create shared-memory: %d\n", shmid);
	}

	//显示共享内存的情况
	system("ipcs -m");

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0) //子进程
	{
		char *shm_addr;

		// 映射共享内存
		if ((shm_addr = (char *)shmat(shmid, 0, 0)) == (char *)-1)
		{
			perror("Child: shmat");
			exit(1);
		}
		else
		{
			printf("Child:Attach shared-memory: %p\n", shm_addr);
		}
		system("ipcs -m");

		//通过检查在共享内存的头部是否标志字符串“WROTE”来确认
		//父进程已经此案够共享内存写入有效数据
		while(strncmp(shm_addr, flag, strlen(flag)))
		{
			printf("Child: wait for enable data...\n");
			sleep(5);
		}
		//获取共享内存的有效数据并显示
		strcpy(buff, shm_addr + strlen(flag));
		printf("Child: Shared-memory: %s\n", buff);

		//解除共享内存映射
		if((shmdt(shm_addr)) < 0)
		{
			perror("shmdt");
			exit(1);
		}
		else
		{
			printf("Child: Deattach shared-memory\n");
		}
		system("ipcs -m");

		//删除共享内存
		if(shmctl(shmid, IPC_RMID, NULL) == -1)
		{
			perror("Child: shmctl(IPC_RMID)\n");
			exit(1);
		}
		else
		{
			printf("Delete shared-memory\n");
		}
		system("ipcs -m");
	}
	else	//父进程
	{
		char *shm_addr;

		//映射共享内存
		if ((shm_addr = (char *)shmat(shmid, 0, 0)) == (char *)-1)
		{
			perror("Parent: shmat");
			exit(1);
		}
		else
		{
			printf("Parent:Attach shared-memory: %p\n", shm_addr);
		}
		sleep(1);
		printf("\n Input some string: \n");
		fgets(buff, BUFFER_SIZE,stdin);
		strncpy(shm_addr + strlen(flag), buff, strlen(buff));
		strncpy(shm_addr, flag, strlen(flag));

		//解除共享内存映射
		if((shmdt(shm_addr)) < 0)
		{
			perror("shmdt");
			exit(1); }
		else
		{
			printf("Parent: Deattach shared-memory\n");
		}
		system("ipcs -m");

		waitpid(pid, NULL, 0);
		printf("Finished\n");
	}
	exit(0);
}

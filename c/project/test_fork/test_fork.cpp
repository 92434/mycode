

/*================================================================
 *   Copyright (C) 2017年08月05日 肖飞 All rights reserved
 *
 *   文件名称：test_fork.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月05日 星期六 23时08分38秒
 *   修改日期：2017年08月06日 星期日 10时35分44秒
 *   描    述：
 *
 *================================================================*/
#include <set>
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "sys/wait.h"

int main(int argc, char **argv)
{
	int ret = 0;
	std::set<int> pid_list;

	int i;

	for(i = 0; i < 10; i++) {
		int pid = fork();

		if(pid == -1) {
			printf("error %s\n", strerror(errno));
		} else if(pid == 0) {
			sleep(6 + i * 1);
			exit(0);
		} else {
			printf("start pid %d\n", pid);
			pid_list.insert(pid);
		}
	}

	sleep(15);

	while(pid_list.size() > 0) {
		int status;
		int w;

		w = wait(&status);

		if (w == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		} else {
			printf("pid %d ", w);
			pid_list.erase(w);
		}

		if (WIFEXITED(status)) {
			printf("exited, status=%d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("killed by signal %d\n", WTERMSIG(status));
		} else if (WIFSTOPPED(status)) {
			printf("stopped by signal %d\n", WSTOPSIG(status));
		} else if (WIFCONTINUED(status)) {
			printf("continued\n");
		}
		sleep(1);
	}

	return ret;
}

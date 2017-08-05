

/*================================================================
 *   Copyright (C) 2017年08月05日 肖飞 All rights reserved
 *
 *   文件名称：test_fork.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年08月05日 星期六 23时08分38秒
 *   修改日期：2017年08月05日 星期六 23时54分49秒
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
			sleep(3 + i * 2);
			exit(0);
		} else {
			pid_list.insert(pid);
		}
	}

	while(pid_list.size() > 0) {
		int status;
		int w;

		w = wait(&status);

		if (w == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		} else {
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

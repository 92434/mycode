

/*================================================================
 *   Copyright (C) 2017年09月08日 肖飞 All rights reserved
 *
 *   文件名称：test_message_queue.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年09月08日 星期五 12时39分51秒
 *   修改日期：2017年09月08日 星期五 13时07分35秒
 *   描    述：
 *
 *================================================================*/
//#include "test_message_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "message_queue.h"

typedef struct {
	int argc;
	char **argv;
} thread_arg_t;

static int stop = 0;

static pthread_mutex_t update_lock;

static void *queue_thread(void *args)
{
	int ret = 0;
	static unsigned int count = 0;

	while(stop == 0) {
		message_t message;

		usleep(200000);

		pthread_mutex_lock(&update_lock);
		message.type = 0;
		message.id = count;
		count++;
		pthread_mutex_unlock(&update_lock);

		ret = queue_message(&message);

		if(ret != 0) {
			printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		} else {

			printf("%s:%s:%d:id:%d\n", __FILE__, __func__, __LINE__, message.id);
		}
	}

	return NULL;
}

static void *dequeue_thread(void *args)
{
	int ret = 0;

	while(stop == 0) {
		message_t *messages = NULL;
		unsigned int capacity = 0;

		usleep(200000);
		ret = dequeue_messages(&messages, &capacity);

		if(ret != 0) {
			printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		} else {

			printf("%s:%s:%d:messages:%p, capacity:%d\n", __FILE__, __func__, __LINE__, messages, capacity);
		}

		if(messages != NULL) {
			free(messages);
		}
	}

	return NULL;
}

int main_proc(void *args)
{
	int ret = 0;
	sleep(1);
	return ret;
}

static int test_message_queue(thread_arg_t *args)
{
#define MAX_QUEUE_TIDS 50
	int ret = 0;
	pthread_t tid_queues[MAX_QUEUE_TIDS];
	pthread_t tid_dequeue;
	int i;

	ret = pthread_mutex_destroy(&update_lock);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	ret = pthread_mutex_init(&update_lock, NULL);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	for(i = 0; i < MAX_QUEUE_TIDS; i++) {
		ret = pthread_create(&tid_queues[i], NULL, queue_thread, args);

		if (ret != 0) {
			printf("can't create thread: %s\n", strerror(errno));
		}
	}


	ret = pthread_create(&tid_dequeue, NULL, dequeue_thread, args);

	if (ret != 0) {
		printf("can't create thread: %s\n", strerror(errno));
	}

	main_proc(args);

	for(i = 0; i < MAX_QUEUE_TIDS; i++) {
		pthread_join(tid_queues[i], NULL);
	}

	pthread_join(tid_dequeue, NULL);

	return EXIT_SUCCESS;
}

typedef void (*sig_action_t)(int, siginfo_t *, void *);

static void default_sig_action(int signo, siginfo_t *info, void *context)
{
	printf("xiaofei:%s called! info->si_signo:%d\n", __func__, info->si_signo);

	if(stop == 0) {
		stop = 1;
	} else {
		printf("xiaofei:force stop!\n");
	}

	return;
}

int catch_signal(sig_action_t sig_action)
{
	int ret = 0;
	struct sigaction sig_act;

	if(sig_action == NULL) {
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

int main(int argc, char **argv)
{
	int ret = 0;

	thread_arg_t args;

	args.argc = argc;
	args.argv = argv;

	catch_signal(NULL);

	ret = message_queue_init();

	printf("message_queue_init:%d\n", ret);

	test_message_queue(&args);

	ret = message_queue_uninit();
	printf("message_queue_uninit:%d\n", ret);

	return ret;
}



/*================================================================
 *   Copyright (C) 2017年09月07日 肖飞 All rights reserved
 *
 *   文件名称：message_queue.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年09月07日 星期四 22时30分13秒
 *   修改日期：2017年09月07日 星期四 23时07分15秒
 *   描    述：
 *
 *================================================================*/
#include "message_queue.h"

static message_queue_t *queue = NULL;

int message_queue_init()
{
	int ret = 0;

	if(queue == NULL) {
		queue = (message_queue_t *)malloc(sizeof(message_queue_t));
	}

	if (queue == NULL) {
		ret = -1;
		goto exit_1;
	}

	ret = sem_init(&sem, 0, 0);

	if(ret != 0) {
		goto exit_2;
	}

	ret = pthread_mutex_init(&lock, NULL);

	if(ret != 0) {
		goto exit_3;
	}

exit_3:
	pthread_mutex_destroy(&queue->lock);
exit_2:
	sem_destroy(&queue->sem);

	free(queue);
	queue = NULL;
exit_1:
	return ret;
}

int message_queue_uninit()
{
	int ret = 0;

	if(queue == NULL) {
		return ret;
	}

	ret = pthread_mutex_destroy(&queue->lock);
	ret = sem_destroy(&queue->sem);

	free(queue);
	queue = NULL;

	return ret;
};

int main(int argc, char **argv)
{
	int ret = 0;
	return ret;
}

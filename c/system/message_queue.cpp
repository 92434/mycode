

/*================================================================
 *   Copyright (C) 2017年09月07日 肖飞 All rights reserved
 *
 *   文件名称：message_queue.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年09月07日 星期四 22时30分13秒
 *   修改日期：2017年09月08日 星期五 13时39分03秒
 *   描    述：
 *
 *================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "message_queue.h"

static message_queue_t *message_queue = NULL;
static pthread_mutex_t queue_lock;

int message_queue_init()
{
	int ret = 0;

	message_queue_t *queue = NULL;

	ret = pthread_mutex_destroy(&queue_lock);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	ret = pthread_mutex_init(&queue_lock, NULL);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	ret = pthread_mutex_lock(&queue_lock);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_1;
	}

	queue = message_queue;


	if(queue != NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_2;
	}

	queue = (message_queue_t *)malloc(sizeof(message_queue_t));

	if (queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_3;
	}

	ret = sem_destroy(&queue->sem);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	ret = sem_init(&queue->sem, 0, 0);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto exit_4;
	}

	INIT_LIST_HEAD(&queue->head);

	message_queue = queue;
	ret = pthread_mutex_unlock(&queue_lock);

	return ret;

exit_4:
	free(queue);
	queue = NULL;
exit_3:
exit_2:
	pthread_mutex_unlock(&queue_lock);
exit_1:
	return ret;
}

int message_queue_uninit()
{
	int ret = 0;

	message_queue_t *queue = NULL;

	ret = pthread_mutex_lock(&queue_lock);

	queue = message_queue;
	message_queue = NULL;

	if(queue != NULL) {
		free(queue);
	}

	ret = pthread_mutex_unlock(&queue_lock);

	return ret;
};

int queue_message(message_t *message)
{
	int ret = 0;
	message_queue_t *queue = NULL;
	message_node_t *node = NULL;

	ret = pthread_mutex_lock(&queue_lock);

	if(message == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_1;
	}

	queue = message_queue;

	if(queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_2;
	}

	node = (message_node_t *)malloc(sizeof(message_node_t));

	if(node == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_3;
	}

	node->type = message->type;
	node->id = message->id;

	list_add_tail(&node->list, &queue->head);

	ret = sem_post(&queue->sem);

	if(ret != 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	}

	ret = pthread_mutex_unlock(&queue_lock);

	return ret;
exit_3:
exit_2:
exit_1:
	pthread_mutex_unlock(&queue_lock);
	return ret;
}

static int _dequeue_message(message_t *message)
{
	int ret = 0;
	message_queue_t *queue = NULL;
	message_node_t *node = NULL;

	if(message == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_1;
	}

	queue = message_queue;

	if(queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_2;
	}

	if(list_empty(&queue->head)) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_3;
	}

	node = list_first_entry(&queue->head, message_node_t, list);
	list_del(&node->list);

	message->type = node->type;
	message->id = node->id;

	free(node);

exit_3:
exit_2:
exit_1:
	return ret;
}

static int _message_queue_wait()
{
	int ret = 0;
	message_queue_t *queue = message_queue;

	if(queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		return ret;
	}

	sem_wait(&queue->sem);
	return ret;
}

static int _message_queue_clean_up()
{
	int ret = 0;
	message_queue_t *queue = message_queue;

	if(queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		return ret;
	}

	while(ret == 0) {
		ret = sem_trywait(&queue->sem);
	}

	ret = 0;

	return ret;
}

int dequeue_message(message_t *message)
{
	int ret = 0;

	_message_queue_wait();
	pthread_mutex_lock(&queue_lock);
	ret = _dequeue_message(message);
	pthread_mutex_unlock(&queue_lock);
	return ret;
}

static unsigned int _queue_capacity()
{
	int ret = 0;
	unsigned int capacity = 0;
	message_queue_t *queue = message_queue;

	struct list_head *list;

	queue = message_queue;

	if(queue == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return capacity;
	}

	if(list_empty(&queue->head)) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		return capacity;
	}

	list_for_each(list, &queue->head) {
		capacity += 1;
	}
	return capacity;
}

int dequeue_messages(message_t **messages, unsigned int *capacity)
{
	int ret = 0;
	message_node_t *node = NULL;
	int i;
	message_t *messages_list = NULL;

	_message_queue_wait();

	ret = pthread_mutex_lock(&queue_lock);

	if(messages == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_1;
	}

	if(capacity == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_2;
	}

	*capacity = _queue_capacity();

	if(*capacity <= 0) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_3;
	}

	messages_list = (message_t *)calloc(*capacity, sizeof(message_t));

	if(messages_list == NULL) {
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		ret = -1;
		goto exit_4;
	}

	for(i = 0; i < *capacity; i++) {
		message_t message;
		ret = _dequeue_message(&message);

		if(ret != 0) {
			printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
			goto exit_5;
		} else {
			messages_list[i] = message;
		}
	}

	*messages = messages_list;

	_message_queue_clean_up();

	ret = pthread_mutex_unlock(&queue_lock);

	return ret;

exit_5:
	free(messages_list);
exit_4:
exit_3:
	*messages = NULL;
	*capacity = 0;
exit_2:
exit_1:
	pthread_mutex_unlock(&queue_lock);
	return ret;
}

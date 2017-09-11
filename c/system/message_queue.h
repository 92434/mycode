

/*================================================================
 *   Copyright (C) 2017年09月07日 肖飞 All rights reserved
 *   
 *   文件名称：message_queue.h
 *   创 建 者：肖飞
 *   创建日期：2017年09月07日 星期四 22时32分01秒
 *   修改日期：2017年09月11日 星期一 22时43分44秒
 *   描    述：
 *
 *================================================================*/
#ifndef _MESSAGE_QUEUE_H
#define _MESSAGE_QUEUE_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <pthread.h>
#include <semaphore.h>
#include "list.h"
typedef struct _message_queue {
	struct list_head head;
	sem_t sem;
} message_queue_t;

typedef struct _message {
	int type;
	int id;
} message_t;

typedef struct _message_node {
	struct list_head list;
	message_t message;
} message_node_t;

int message_queue_init();
int queue_message(message_t *message);
int dequeue_message(message_t *message);
int dequeue_messages(message_t **messages, unsigned int *capacity);
int message_queue_uninit();
#endif //_MESSAGE_QUEUE_H

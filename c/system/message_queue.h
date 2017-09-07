

/*================================================================
 *   Copyright (C) 2017年09月07日 肖飞 All rights reserved
 *   
 *   文件名称：message_queue.h
 *   创 建 者：肖飞
 *   创建日期：2017年09月07日 星期四 22时32分01秒
 *   修改日期：2017年09月07日 星期四 22时41分41秒
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

#include <semaphore.h>
#include "list.h"
#include <pthread.h>
typedef struct _message_queue {
	struct list_head head;
	sem_t sem;
	pthread_mutex_t lock;
} message_queue_t;
typedef struct _message {
	int type;
	int id;
} message_t;
#endif //_MESSAGE_QUEUE_H

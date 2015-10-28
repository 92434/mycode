#ifndef _XIAOFEI_KTHREAD_H
#define _XIAOFEI_KTHREAD_H
#include <linux/kthread.h>

typedef int (*thread_func_t)(void *ppara);
struct task_struct *alloc_work_thread(thread_func_t func, void *ppara, char *format, ...);
int free_work_thread(struct task_struct *thread);

#endif//#ifndef _XIAOFEI_KTHREAD_H

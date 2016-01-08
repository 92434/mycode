#ifndef _XIAOFEI_TIMER_H
#define _XIAOFEI_TIMER_H
#include <linux/timer.h>

typedef void (*timer_func_t)(unsigned long);
typedef struct {
	struct timer_list *tl;
	unsigned long ms;
	timer_func_t func;
	struct work_struct *timer_work;
} timer_data_t;

timer_data_t *alloc_timer(unsigned long ms, timer_func_t func, struct work_struct *timer_work);
void free_timer(void *pdata);

#endif//#ifndef _XIAOFEI_TIMER_H

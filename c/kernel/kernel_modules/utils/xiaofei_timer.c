#include <linux/module.h>
#include <linux/vmalloc.h>
#include "xiaofei_timer.h"
#include "xiaofei_debug.h"

static int start_timer(timer_data_t *pdata) {
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;
	timer_func_t func = pdata->func;

	init_timer(tl);
	tl->expires = jiffies + tmo;
	tl->data = (unsigned long)pdata;
	tl->function = func;
	add_timer(tl);

	return 0;
}

static void default_timer_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	myprintf("!\n");
	tl->expires = jiffies + tmo;
	add_timer(tl);
}

timer_data_t *alloc_timer(unsigned long ms, timer_func_t func, struct work_struct *timer_work) {
	timer_data_t *pdata = NULL;
	struct timer_list *tl = NULL;

	if(func == NULL) {
		func = default_timer_func;
	}

	pdata = (timer_data_t *)vzalloc(sizeof(timer_data_t));
	if(pdata == NULL) {
		mydebug("no memory for timer_data!\n");
		goto alloc_timer_data_failed;
	}

	tl = (struct timer_list *)vzalloc(sizeof(struct timer_list));
	if(tl == NULL) {
		mydebug("no memory for timer_list!\n");
		goto alloc_timer_list_failed;
	}

	pdata->tl = tl;
	pdata->ms = ms;
	pdata->func = func;
	pdata->timer_work = timer_work;

	start_timer(pdata);

	return pdata;

	vfree(tl);
alloc_timer_list_failed:
	vfree(pdata);
alloc_timer_data_failed:
	return NULL;
}

void free_timer(void *pdata) {

	struct timer_list *tl;

	if(pdata == NULL) {
		mydebug("no timer_data!\n");
		return;
	}

	tl = ((timer_data_t *)pdata)->tl;
	if(tl == NULL) {
		mydebug("no timer_list!\n");
		goto free_timer_data;
	}

	del_timer(tl);
	vfree(tl);

free_timer_data:
	vfree(pdata);
}


//timer_data_t *pdata = alloc_timer(1000, default_timer_func, timer_work);

//free_timer(pdata);

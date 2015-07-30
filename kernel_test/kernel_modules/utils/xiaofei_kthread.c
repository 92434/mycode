#include "xiaofei_kthread.h"

static int default_worker_thread(void *ppara) {
	int rtn = 0;
	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(1*HZ); 
	}

	return rtn;
}

struct task_struct *start_thread(thread_func_t func, void *ppara, char *format, ...) {
	va_list args;
	struct task_struct *thread;

	va_start(args, format);

	if(func == NULL) {
		func = default_worker_thread;
	}

	thread = kthread_run(i2s_receiver_worker_thread, ppara, format, args);

	va_end(args);

	return thread;
}

int stop_thread(struct task_struct *thread) {
	return kthread_stop(thread);
}


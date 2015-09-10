#include "xiaofei_kthread.h"
#include "xiaofei_debug.h"

static int default_worker_thread(void *ppara) {
	int ret = 0;
	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(1*HZ); 
		mydebug("\n");
	}

	return ret;
}

struct task_struct *alloc_work_thread(thread_func_t func, void *ppara, char *format, ...) {
	va_list args;
	struct task_struct *thread;
	char name[16];

	va_start(args, format);
	vsnprintf(name, sizeof(name), format, args);
	va_end(args);

	if(func == NULL) {
		func = default_worker_thread;
	}

	thread = kthread_run(func, ppara, name);


	return thread;
}

int free_work_thread(struct task_struct *thread) {
	return kthread_stop(thread);
}


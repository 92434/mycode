#include <linux/module.h>
#include <linux/version.h>

#include "utils/xiaofei_debug.h"

#define MODULE_NAME "test_module"
#define DRIVER_DESCRIPTION "test_module"
#define DRIVER_VERSION "1.0"

/** Driver Module information */
MODULE_AUTHOR("xiaofei");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
MODULE_ALIAS(MODULE_NAME);

static unsigned long count = 0;

#ifdef test_buffer_list
#include "utils/xiaofei_list_buffer.h"
#define BUFFER_COUNT 1
#define BUFFER_SIZE 24
static char *buffers[BUFFER_COUNT];

static void start_test_buffer_list(void) {
	int i;
	list_buffer_t *list;
	char *data_buffer;
	char *data;
	int loop;
	buffer_node_t read, write;

	for(i = 0; i < BUFFER_COUNT; i++) {
		buffers[i] = (char *)vzalloc(BUFFER_SIZE);
		if(buffers[i] == 0) {
			mydebug("\n");
			goto exit1;
		}
	}

	list = init_list_buffer();
	if(list == NULL) {
		mydebug("\n");
		goto exit1;
	}

	for(i = 0; i < BUFFER_COUNT; i++) {
		add_list_buffer_item(buffers[i], 0, BUFFER_SIZE, list);
	}

	//write large data continuely
	data_buffer = (char *)vzalloc(BUFFER_COUNT * BUFFER_SIZE);
	for(i = 1; i <= BUFFER_COUNT * BUFFER_SIZE; i++) {
		data_buffer[i] = i;
	}

	i = BUFFER_COUNT * BUFFER_SIZE;
	data = data_buffer;
	loop = 0;
	while(i > 0) {
		int n;
		get_buffer_node_info(&write, &read, list);
		n = write_buffer(data, 1, list);
		data += n;
		i -= n;
	}
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer + 0, 2, list);
	//write_buffer(data_buffer + 8, 2, list);
	//write_buffer(data_buffer + 10, 2, list);
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer + 2, 2, list);
	//write_buffer(data_buffer + 12, 2, list);
	//write_buffer(data_buffer + 14, 2, list);
	//write large data continuely

	//read large data continuely
	i = BUFFER_COUNT * BUFFER_SIZE - 4;
	data = data_buffer + 4;
	loop = 0;
	while(i > 0) {
		int n;
		get_buffer_node_info(&write, &read, list);
		n = read_buffer(data, 1, list);
		data += n;
		i -= n;
	}

	get_buffer_node_info(&write, &read, list);
	write_buffer(data_buffer + 10, 2, list);
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer, 2, list);

	printk("\n");
	for(i = 0; i < BUFFER_COUNT * BUFFER_SIZE; i++) {
		if((i != 0) && (i % 8 == 0)) {
			printk("\n");
		}
		printk("%02x ", data_buffer[i]);
	}
	printk("\n");
	//read large data continuely

	vfree(data_buffer);

	uninit_list_buffer(list);
exit1:
	for(i = 0; i < BUFFER_COUNT; i++) {
		if(buffers[i] != 0) {
			vfree(buffers[i]);
		}
	}

}
#endif

#ifdef test_kthread
#include "utils/xiaofei_timer.h"
#include "utils/xiaofei_kthread.h"
timer_data_t *pticker = NULL;
timer_data_t *pdata = NULL;
struct task_struct *thread = NULL;
static int default_worker_thread(void *ppara) {
	int ret = 0;
	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		//schedule();
		schedule_timeout(1); 
		count++;
	}

	return ret;
}

static void default_timer_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	myprintf("count:%lu!\n", count);
	count = 0;
	tl->expires = jiffies + tmo;
	add_timer(tl);
}

static void ticker_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	wake_up_process(thread);
	tl->expires = jiffies + tmo;
	add_timer(tl);
}
#endif


static int __init test_init(void) {

#ifdef test_buffer_list
	start_test_buffer_list();
#endif

#ifdef test_kthread
	thread = alloc_work_thread(default_worker_thread, NULL, "%s", "test");
	pdata = alloc_timer(1000, default_timer_func);
	//pticker = alloc_timer(10, ticker_func);
#endif
	return 0;
}

static void __exit test_exit(void) {
#ifdef test_kthread
	free_timer(pdata);
	//free_timer(pticker);
	free_work_thread(thread);
#endif
}

module_init(test_init);
module_exit(test_exit);

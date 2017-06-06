#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdarg.h>
#include <string.h>

typedef struct _log_item {
	//const char *file;
	//const char *func;
	//int line;
	const char *enter_label;
	unsigned long long timestamp_by_ms;
} log_item_t;

#define MAX_LOG_LEVEL 16
#define MAX_BUFFER_SIZE 255

typedef struct _log_stack {
	int pos;
	pid_t pid;
	pthread_t tid;
	log_item_t items[MAX_LOG_LEVEL];
} log_stack_t;

static log_stack_t log_stack;

static int init_log_stack()
{
	log_stack.pos = -1;
	log_stack.pid = getpid();
	log_stack.tid = pthread_self();

	//printf("%s:%s:%d: log_stack.pos:%d, log_stack.pid:%d, log_stack.tid:%d\n", __FILE__, __func__, __LINE__, log_stack.pos, log_stack.pid, (int)log_stack.tid);
}

static unsigned long long get_current_ms()
{
	unsigned long long ret = 0;
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	ret = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	return ret;
}

static int log_enter(const char *enter_label/*, const char *file, const char *func, int line*/)
{
	int ret = 0;
	pid_t pid;
	pthread_t tid;
	log_item_t *log_item;

	pid = getpid();

	if(pid != log_stack.pid) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	tid = pthread_self();

	if(tid != log_stack.tid) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	if((log_stack.pos + 1) >= MAX_LOG_LEVEL) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	log_stack.pos += 1;

	log_item = log_stack.items + log_stack.pos;
	log_item->enter_label = enter_label;
	//log_item->file = file;
	//log_item->func = func;
	//log_item->line = line;
	log_item->timestamp_by_ms = get_current_ms();

out:
	return ret;

}

static int log_out(const char *out_label/*, const char *file, const char *func, int line*/, const char *fmt, ...)
{
	int ret = 0;
	pid_t pid;
	pthread_t tid;
	log_item_t *log_item;
	unsigned long long timestamp_by_ms;
	char buffer[MAX_BUFFER_SIZE];


	int size;
	int capacity = MAX_BUFFER_SIZE - 1;
	va_list ap;
	int offset = 0;

	pid = getpid();

	if(pid != log_stack.pid) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	tid = pthread_self();

	if(tid != log_stack.tid) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	if((log_stack.pos < 0) || (log_stack.pos >= MAX_LOG_LEVEL)) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	log_item = log_stack.items + log_stack.pos;
	timestamp_by_ms = get_current_ms();

	if(strcmp(log_item->enter_label, out_label)) {
		size = snprintf(buffer + offset, capacity, "%02d|%-*s<%s...%s>: duration = %llu ", log_stack.pos, log_stack.pos * 8, "", log_item->enter_label, out_label, timestamp_by_ms - log_item->timestamp_by_ms);
	} else {
		size = snprintf(buffer + offset, capacity, "%02d|%-*s<%s>: duration = %llu ", log_stack.pos, log_stack.pos * 8, "", out_label, timestamp_by_ms - log_item->timestamp_by_ms);
	}

	if(size <= 0) {
		ret = -1;
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		goto out;
	}

	//printf("size:%d\n", size);

	offset += size;
	capacity -= size;

	va_start(ap, fmt);
	size = vsnprintf(buffer + offset, capacity, fmt, ap);
	//printf("size:%d\n", size);
	va_end(ap);

	offset += size;
	capacity -= size;

	buffer[offset] = 0;
	log_stack.pos -= 1;

	printf("%s", buffer);

out:
	return ret;
}

void func3()
{
	usleep(10000);
}

void func2()
{
	usleep(10000);
	log_enter("func3-start");
	func3();
	log_out("func3-end", "%s\n", "---this is test infomation!");
}

void func1()
{
	usleep(10000);
	log_enter("func2");
	func2();
	log_out("func2", "%s\n", "---this is test infomation!");
	log_enter("func2");
	func2();
	log_out("func2", "%s\n", "---this is test infomation!");
	log_enter("func2");
	func2();
	log_out("func2", "%s\n", "---this is test infomation!");
}

int main()
{
	int ret = 0;
	int i;

	//asm("nop");

	init_log_stack();

	log_enter("main");

	for(i = 0; i < 1; i++) {
		log_enter("func1-start");
		func1();
		log_out("func1-end", "%s\n", "---this is test infomation!");

		log_enter("func1-start");
		func1();
		log_out("func1-end", "%s\n", "---this is test infomation!");

		log_enter("func1-start");
		func1();
		log_out("func1-end", "%s\n", "---this is test infomation!");

		log_enter("mytest");

		log_enter("func1-start");
		func1();
		log_out("func1-end", "%s\n", "---this is test infomation!");

		log_out("mytest", "%s\n", "---this is test infomation!");
	}

	log_out("main", "%s\n", "---this is test infomation!");

	return ret;
}

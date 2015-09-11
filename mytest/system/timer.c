#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define TIMER_THREAD_NUM 2

pthread_attr_t attr[TIMER_THREAD_NUM];
struct sigevent sigenv[TIMER_THREAD_NUM];
timer_t timer[TIMER_THREAD_NUM] = {0};
int sleep_duration[TIMER_THREAD_NUM] = {1,1};

static void timeout(union sigval v) {
	time_t t;
	char p[32];
	timer_t *ptimer;
	struct itimerspec ts;
	int ret;

	time(&t);
	strftime(p, sizeof(p), "%T", localtime(&t));
	ptimer = (timer_t *)(v.sival_ptr);
	printf("%s ptimer:%p, thread %x, val = %x, signal captured.\n", p, ptimer, (unsigned int)pthread_self(), v.sival_int);

	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = (*ptimer == timer[0]) ? sleep_duration[0] :
				(*ptimer == timer[1]) ? sleep_duration[1] : 0;
	ts.it_value.tv_nsec = 0;

	ret = timer_settime(*ptimer, CLOCK_REALTIME, &ts, NULL);
	if (ret != 0) {
		printf("settime err(%d)!\n", ret);
	}
}

static void my_pthread_attr_destroy(pthread_attr_t *pattr) {
	pthread_attr_destroy(pattr);
}

static int my_pthread_attr_init(pthread_attr_t *pattr) {
	int ret;

	if ((ret = pthread_attr_init(pattr) != 0)) {
		goto err;
	}
	if ((ret = pthread_attr_setdetachstate(pattr, PTHREAD_CREATE_DETACHED)) != 0) {
		my_pthread_attr_destroy(pattr);
		goto err;
	}
	/* 设置线程的栈大小,失败则用系统默认值 */
	pthread_attr_setstacksize(pattr, 128 * 1024);

	return 0;
err:
	printf("set ptread attr failed(ret:%d)!\n", ret);
	return -1;
}

int main(void) {
	struct itimerspec ts;
	int ret;
	int i;

	for(i = 0; i < TIMER_THREAD_NUM; i++) {
		ret = my_pthread_attr_init(&attr[i]);
		if (ret != 0) {
			printf("init pthread attributes fail(%d)!\n", ret);
			exit(-1);
		}

		memset(&sigenv[i], 0, sizeof(struct sigevent));
		sigenv[i].sigev_value.sival_ptr = &timer[i];
		sigenv[i].sigev_notify = SIGEV_THREAD;
		sigenv[i].sigev_notify_function = timeout;
		sigenv[i].sigev_notify_attributes = &attr[i];
		printf("attr: %p\n", &attr[i]);

		ret = timer_create(CLOCK_REALTIME, &sigenv[i], &timer[i]);
		if(ret != 0) {
			perror("timer_create fail !");
			exit(-1);
		}

		ts.it_interval.tv_sec = 0;
		ts.it_interval.tv_nsec = 0;
		ts.it_value.tv_sec = sleep_duration[i];
		ts.it_value.tv_nsec = 0;

		ret = timer_settime(timer[i], CLOCK_REALTIME, &ts, NULL);
		if(ret != 0) {
			perror("timer_settime fail !");
			exit(-1);
		}
	}

	while(1) {
		sleep(1);
		printf("thread %x.\n", (unsigned int)pthread_self());
	}

error:
	for(i = 0; i < TIMER_THREAD_NUM; i++) {
		if(timer[i] != 0) {
			timer_delete(timer[i]);
		}
		my_pthread_attr_destroy(&attr[i]);
	}

	return 0;
}

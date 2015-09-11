#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#if 1
pthread_attr_t attr;
timer_t hard_timer, software_timer;
struct sigevent hard_evp, software_evp;

static void watchdog_hard_timeout(union sigval v)
{
	time_t t;
	char p[32];
	timer_t *q;
	struct itimerspec ts;
	int ret;

	time(&t);
	strftime(p, sizeof(p), "%T", localtime(&t));
	printf("watchdog hard timeout!\n");
	printf("%s thread %d, val = %u, signal captured.\n", p, (unsigned int)pthread_self(), v.sival_int);

	q = (timer_t *)(v.sival_ptr);
	printf("hard timer_t:%d add:%p, q:%p!\n", (int)hard_timer, &hard_timer, q);
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;

	ret = timer_settime(*q, CLOCK_REALTIME, &ts, NULL);
	if (ret != 0) {
		printf("settime err(%d)!\n", ret);
	}
}

static void watchdog_software_timeout(union sigval v)
{
	time_t t;
	char p[32];
	timer_t *q;
	struct itimerspec ts;
	int ret;

	time(&t);
	strftime(p, sizeof(p), "%T", localtime(&t));
	printf("watchdog software timeout!\n");
	printf("%s thread %d, val = %u, signal captured.\n", p, (unsigned int)pthread_self(), v.sival_int);

	q = (timer_t *)(v.sival_ptr);
	printf("hard timer_t:%d add:%p, q:%p!\n", (int)hard_timer, &hard_timer, q); 
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 2;
	ts.it_value.tv_nsec = 0;

	ret = timer_settime(*q, CLOCK_REALTIME, &ts, NULL);
	if (ret != 0) {
		printf("settime err(%d)!\n", ret);
	} 
}

static void dcmi_sol_pthread_attr_destroy(pthread_attr_t *attr)
{
	pthread_attr_destroy(attr);
}

static int dcmi_sol_pthread_attr_init(pthread_attr_t *attr)
{
	int ret;

	if ((ret = pthread_attr_init(attr) != 0)) {
		goto err;
	}
	if ((ret = pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED)) != 0) {
		dcmi_sol_pthread_attr_destroy(attr);
		goto err;
	}
	/* 设置线程的栈大小,失败则用系统默认值 */
	pthread_attr_setstacksize(attr, 128 * 1024);

	return 0;
err:
	printf("set ptread attr failed(ret:%d)!\n", ret);
	return -1;
}

int main(void)
{
	struct itimerspec ts;
	int ret;

	ret = dcmi_sol_pthread_attr_init(&attr);
	if (ret != 0) {
		printf("init pthread attributes fail(%d)!\n", ret);
		exit(-1);
	}

	memset(&hard_evp, 0, sizeof(struct sigevent));
	hard_evp.sigev_value.sival_ptr = &hard_timer;
	hard_evp.sigev_notify = SIGEV_THREAD;
	hard_evp.sigev_notify_function = watchdog_hard_timeout;
	hard_evp.sigev_notify_attributes = NULL;//&attr;

	memset(&software_evp, 0, sizeof(struct sigevent));
	software_evp.sigev_value.sival_ptr = &software_timer;
	software_evp.sigev_notify = SIGEV_THREAD;
	software_evp.sigev_notify_function = watchdog_software_timeout;
	//software_evp.sigev_notify_attributes = NULL;//&attr;
	software_evp.sigev_notify_attributes = &attr;

	ret = timer_create(CLOCK_REALTIME, &hard_evp, &hard_timer);
	if(ret != 0) {
		perror("hard timer_create fail!");
		exit(-1);
	}
	ret = timer_create(CLOCK_REALTIME, &software_evp, &software_timer);
	if (ret != 0) {
		timer_delete(hard_timer);
		perror("software timer_create fail!");
		exit(-1);
	}

	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;

	ret = timer_settime(hard_timer, CLOCK_REALTIME, &ts, NULL);
	if(ret != 0) {
		perror("hard timer_settime fail!");
		timer_delete(hard_timer);
		timer_delete(software_timer);
		exit(-1);
	}

	ts.it_value.tv_sec = 2;
	ret = timer_settime(software_timer, CLOCK_REALTIME, &ts, NULL);
	if(ret != 0) {
		perror("hard timer_settime fail!");
		timer_delete(hard_timer);
		timer_delete(software_timer);
		exit(-1);
	}

	while(1) {
		printf("main ready sleep!\n");
		sleep(3);
		printf("main sleep finish!\n");
	}

	return 0;
}
#endif

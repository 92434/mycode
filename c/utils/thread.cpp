#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "thread.h"

void Thread::thread_completed(void *ptr)
{
	Thread *p = (Thread*) ptr;
	p->m_alive = 0;

		/* recover state in case thread was cancelled before calling hasStarted */
	if (!p->m_started)
		p->hasStarted();

	p->thread_finished();
}

void *Thread::wrapper(void *ptr)
{
	Thread *p = (Thread*)ptr;
	pthread_cleanup_push(thread_completed, (void*)p);
	p->thread();
	pthread_exit(0);
	pthread_cleanup_pop(1);
	return 0;
}

Thread::Thread()
	: the_thread(0), m_alive(0)
{
}

static const int default_stack_size = 1024*1024;

int Thread::runAsync(int prio, int policy)
{
		/* the thread might already run. */
	if (sync())
		return -1;

	assert(m_state.value() == 1); /* sync postconditions */
	assert(!m_alive);
	m_state.down();
	assert(m_state.value() == 0);

	m_alive = 1;
	m_started = 0;

		/* start thread. */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (pthread_attr_setstacksize(&attr, default_stack_size) != 0)
		printf("[Thread] pthread_attr_setstacksize failed\n");

	if (prio || policy)
	{
		struct sched_param p;
		p.__sched_priority=prio;
		pthread_attr_setschedpolicy(&attr, policy);
		pthread_attr_setschedparam(&attr, &p);
	}

	if (the_thread) {
		printf("old thread joined %d\n", pthread_join(the_thread, 0));
		the_thread = 0;
	}

	if (pthread_create(&the_thread, &attr, wrapper, this))
	{
		pthread_attr_destroy(&attr);
		m_alive = 0;
		printf("couldn't create new thread\n");
		return -1;
	}

	pthread_attr_destroy(&attr);
	return 0;
}

int Thread::run(int prio, int policy)
{
	if (runAsync(prio, policy))
		return -1;
	sync();
	return 0;
}

Thread::~Thread()
{
	if (the_thread)
	{
		/* Warn about this class' design being borked */
		printf("Destroyed thread without joining it, this usually means your thread is now running with a halfway destroyed object\n");
		kill();
	}
}

int Thread::sync(void)
{
	int res;
	int debug_val_before = m_state.value();
	m_state.down(); /* this might block */
	res = m_alive;
	if (m_state.value() != 0)
		printf("Thread::sync: m_state.value() == %d - was %d before\n", m_state.value(), debug_val_before);
	assert(m_state.value() == 0);
	m_state.up();
	return res; /* 0: thread is guaranteed not to run. 1: state unknown. */
}

int Thread::sendSignal(int sig)
{
	if (m_alive)
		return pthread_kill(the_thread, sig);
	else
		printf("send signal to non running thread\n");
	return -1;
}

void Thread::kill(bool sendcancel)
{
	if (!the_thread) /* already joined */
		return;

	if (sync() && sendcancel)
	{
		printf("send cancel to thread\n");
		pthread_cancel(the_thread);
	}
	
	int ret = pthread_join(the_thread, NULL);
	the_thread = 0;
	if (ret)
		printf("pthread_join failed, code: %d\n", ret);
}

void Thread::hasStarted()
{
	assert(!m_state.value());
	m_started = 1;
	m_state.up();
}

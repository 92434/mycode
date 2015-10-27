#include <unistd.h>
#include "lock.h"

void Lock::lock(int res)
{
	if (res>max)
		res=max;
	pthread_mutex_lock(&mutex);
	while ((counter+res)>max)
		pthread_cond_wait(&cond, &mutex);
	counter+=res;
	pthread_mutex_unlock(&mutex);
}

void Lock::unlock(int res)
{
	if (res>max)
		res=max;
	pthread_mutex_lock(&mutex);
	counter-=res;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
}

Lock::Lock(int max): max(max)
{
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&cond, 0);
	counter=0;
}

Lock::~Lock()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

Locker::Locker(Lock &lock, int res): lock(lock), res(res)
{
	lock.lock(res);
}

Locker::~Locker()
{
	lock.unlock(res);
}

Semaphore::Semaphore()
{
	v=1;
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&cond, 0);
}

Semaphore::~Semaphore()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

int Semaphore::down()
{
	int value_after_op;
	pthread_mutex_lock(&mutex);
	while (v<=0)
		pthread_cond_wait(&cond, &mutex);
	v--;
	value_after_op=v;
	pthread_mutex_unlock(&mutex);
	return value_after_op;
}

int Semaphore::decrement()
{
	int value_after_op;
	pthread_mutex_lock(&mutex);
	v--;
	value_after_op=v;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return value_after_op;
}

int Semaphore::up()
{
	int value_after_op;
	pthread_mutex_lock(&mutex);
	v++;
	value_after_op=v;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return value_after_op;
}

int Semaphore::value()
{
	int value_after_op;
	pthread_mutex_lock(&mutex);
	value_after_op=v;
	pthread_mutex_unlock(&mutex);
	return value_after_op;
}


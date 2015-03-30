#ifndef __LOCK_H
#define __LOCK_H

#include <pthread.h>

class singlLock
{
	pthread_mutex_t &lock;
public:
	singlLock(pthread_mutex_t &m )
		:lock(m)
	{
		pthread_mutex_lock(&lock);
	}
	~singlLock()
	{
		pthread_mutex_unlock(&lock);
	}
};

class RdWrLock
{
	friend class RdLocker;
	friend class WrLocker;
	pthread_rwlock_t m_lock;
	RdWrLock(RdWrLock &);
public:
	RdWrLock()
	{
		pthread_rwlock_init(&m_lock, 0);
	}
	~RdWrLock()
	{
		pthread_rwlock_destroy(&m_lock);
	}
	void RdLock()
	{
		pthread_rwlock_rdlock(&m_lock);
	}
	void WrLock()
	{
		pthread_rwlock_wrlock(&m_lock);
	}
	void Unlock()
	{
		pthread_rwlock_unlock(&m_lock);
	}
};

class RdLocker
{
	RdWrLock &m_lock;
public:
	RdLocker(RdWrLock &m)
		: m_lock(m)
	{
		pthread_rwlock_rdlock(&m_lock.m_lock);
	}
	~RdLocker()
	{
		pthread_rwlock_unlock(&m_lock.m_lock);
	}
};

class WrLocker
{
	RdWrLock &m_lock;
public:
	WrLocker(RdWrLock &m)
		: m_lock(m)
	{
		pthread_rwlock_wrlock(&m_lock.m_lock);
	}
	~WrLocker()
	{
		pthread_rwlock_unlock(&m_lock.m_lock);
	}
};

class SinglLock
{
protected:
	pthread_mutex_t m_lock;
private:
	SinglLock(const SinglLock &);
public:
	SinglLock()
	{
		pthread_mutex_init(&m_lock, 0);
	}
	~SinglLock()
	{
		pthread_mutex_destroy(&m_lock);
	}
	void lock()
	{
		pthread_mutex_lock(&m_lock);
	}
	void unlock()
	{
		pthread_mutex_unlock(&m_lock);
	}
	operator pthread_mutex_t&() { return m_lock; }
};

class Condition
{
private:
	Condition(const Condition&);
protected:
	pthread_cond_t m_cond;
public:
	Condition()
	{
		pthread_cond_init(&m_cond, 0);
	}
	~Condition()
	{
		pthread_cond_destroy(&m_cond);
	}
	void signal()
	{
		pthread_cond_signal(&m_cond);
	}
	void wait(pthread_mutex_t& mutex)
	{
		pthread_cond_wait(&m_cond, &mutex);
	}
	operator pthread_cond_t&() { return m_cond; }
};

class SinglLocker
{
protected:
	SinglLock &m_lock;
public:
	SinglLocker(SinglLock &m)
		: m_lock(m)
	{
		m_lock.lock();
	}
	~SinglLocker()
	{
		m_lock.unlock();
	}
};

class Lock
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	int counter, max;
public:
	void lock(int res=100);
	void unlock(int res=100);

	Lock(int max=100);
	~Lock();
};

class Locker
{
	Lock &lock;
	int res;
public:
	Locker(Lock &lock, int res=100);
	~Locker();
};

class Semaphore
{
	int v;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
public:
	Semaphore();
	~Semaphore();

	int down();
	int decrement();
	int up();
	int value();
};

#endif

#ifndef __POLL_H
#define __POLL_H
#include "thread.h"

#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>//realloc
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>

#define POLL_FDS_MAX_COUNT 256
#define POLL_FDS_MAX_COUNT_EACH_THREAD 32
#define DEFAULT_POLL_FD (-1)

class thread_wapper;
class poll_wapper {
protected:
	static pthread_mutex_t map_poll_fds_lock;
	static pthread_mutex_t vector_poll_thread_lock;
	std::map<int, unsigned int> map_poll_fds;
	std::vector<thread_wapper *> vector_poll_thread;

	virtual thread_wapper *allocate_thread(poll_wapper *p) = 0;
public:
	poll_wapper() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		singlLock s0(map_poll_fds_lock);
		singlLock s1(vector_poll_thread_lock);
		map_poll_fds.clear();
		vector_poll_thread.clear();
	}

	~poll_wapper() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		remove_all_fds();
	}

	int add_fd(int fd, unsigned int events);
	int remove_fd(int fd = DEFAULT_POLL_FD);
	int remove_all_fds();

	virtual int process_fd(int fd, unsigned int events);
};

class thread_wapper : public Thread {
protected:
	poll_wapper *ppoll_wapper;
	static pthread_mutex_t map_poll_thread_fds_lock;
	std::map<int, unsigned int> map_poll_thread_fds;

	void thread();

	virtual int add_poll_fd_ext(int fd, unsigned int events) = 0;

	virtual int remove_poll_fd_ext(int fd) = 0;

	virtual int process() = 0;
public:
	thread_wapper(poll_wapper *p) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		singlLock s(map_poll_thread_fds_lock);
		map_poll_thread_fds.clear();

		ppoll_wapper = p;
	}

	~thread_wapper() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		singlLock s(map_poll_thread_fds_lock);
		map_poll_thread_fds.clear();
	}

	void start();

	void stop();

	int add_poll_fd(int fd, unsigned int events);

	int remove_poll_fd(int fd);

	bool could_release();

};

class select_poll : public poll_wapper {
public:
	select_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	~select_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	thread_wapper *allocate_thread(poll_wapper *p);
};

class select_thread : public thread_wapper {
	fd_set fds;
	struct timeval tv;

public:
	select_thread(poll_wapper *p) : thread_wapper(p) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		set_timeout();
	}

	~select_thread() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		stop();
	}

	int set_timeout(int tv_sec = 3, int tv_usec = 0);

	int add_poll_fd_ext(int fd, unsigned int events) {return 0;}

	int remove_poll_fd_ext(int fd){return 0;}

	int process();
};

class poll_poll : public poll_wapper {
public:
	poll_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	~poll_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	thread_wapper *allocate_thread(poll_wapper *p);
};

class poll_thread : public thread_wapper {
	struct pollfd *fds;
	unsigned int timeout_ms;
	int fds_count;
public:
	poll_thread(poll_wapper *p) : thread_wapper(p) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		fds = (struct pollfd *)realloc(NULL, sizeof(struct pollfd) * POLL_FDS_MAX_COUNT_EACH_THREAD);
		//printf("fds:%p, size:%d\n", fds, sizeof(struct pollfd) * POLL_FDS_MAX_COUNT_EACH_THREAD);
		fds_count = 0;
		set_timeout();
	}

	~poll_thread() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		stop();
		fds = (struct pollfd *)realloc(fds, sizeof(struct pollfd) * 0);
		//printf("fds:%p, size:%d\n", fds, sizeof(struct pollfd) * 0);
	}

	int set_timeout(int timeout = 3 * 1000);

	int update_poll_fds_buffer();

	int add_poll_fd_ext(int fd, unsigned int events);

	int remove_poll_fd_ext(int fd);

	int process();
};

class epoll_poll : public poll_wapper {
public:
	epoll_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	~epoll_poll() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
	}

	thread_wapper *allocate_thread(poll_wapper *p);
};

static const int EPOLL_SIZE_HINT = 8;
static const int EPOLL_MAX_EVENTS = 16;

class epoll_thread : public thread_wapper {
	struct epoll_event *epoll_events;
	int m_epoll_fd;
	unsigned int timeout_ms;
public:
	epoll_thread(poll_wapper *p) : thread_wapper(p) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		epoll_events = (struct epoll_event *)realloc(epoll_events, sizeof(struct epoll_event) * POLL_FDS_MAX_COUNT_EACH_THREAD);
		//printf("epoll_events:%p, size:%d\n", epoll_events, sizeof(struct epoll_event) * POLL_FDS_MAX_COUNT_EACH_THREAD);
		m_epoll_fd = -1;
		set_timeout();
	}

	~epoll_thread() {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n");
		stop();
		epoll_events = (struct epoll_event *)realloc(epoll_events, sizeof(struct epoll_event) * 0);
		//printf("epoll_events:%p, size:%d\n", epoll_events, sizeof(struct epoll_event) * 0);
	}

	int set_timeout(int timeout = 3 * 1000);

	int update_poll_fds_buffer();

	int add_poll_fd_ext(int fd, unsigned int events);

	int remove_poll_fd_ext(int fd);

	int process();
};
#endif//#ifndef __POLL_H

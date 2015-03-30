#ifndef __POLL_THREAD_H
#define __POLL_THREAD_H
#include "thread.h"
#include "my_type.h"
#include <vector>
#include <map>
#include <algorithm>

#ifdef LOOPER_USES_EPOLL
// Hint for number of file descriptors to be associated with the epoll instance.
static const int EPOLL_SIZE_HINT = 8;

// Maximum number of file descriptors for which to retrieve poll events each iteration.
static const int EPOLL_MAX_EVENTS = 16;
#endif

class poll_thread : public Thread {
	static poll_thread *instance;
	static pthread_mutex_t poll_lock;
	bool m_thread_status;

	std::map<int, int> fds_notifiers_map;
#ifdef LOOPER_USES_EPOLL
	int m_epoll_fd;
	struct epoll_event *epoll_events;
#else
	struct pollfd *fds;
	std::map<int, unsigned int> fds_all;
#endif

	poll_thread();
	~poll_thread();
	void thread();
	void before_thread();
	void after_thread();
	int process_fds_add(int poll_fd, unsigned int events, int notifier_fd);
	int process_fds_rm(int poll_fd);
	int cleanup_notified();
	int update_fds_cache();
	int poll_once();
	bool get_thread_status();
	void set_thread_status(bool status);
public:
	void start();
	void stop();
	void wait_for_thread();
	int add_poll_fd(int poll_fd, unsigned int events, int notifier_fd);
	int rm_poll_fd(int poll_fd);
	static poll_thread *get_instance();
	static void del_instance();
};
#endif

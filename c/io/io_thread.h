#ifndef __IO_THREAD_H
#define __IO_THREAD_H
#include "thread.h"
#include "poll_thread.h"
#include "my_type.h"
#include <vector>

class io_thread : public Thread {
	static pthread_mutex_t io_fds_lock;
	poll_thread *m_poll_thread_instance;
	bool m_thread_status;
	int wakeup_pipe[2];
	std::map<int, unsigned int> map_io_fds;
	void thread();
	void before_thread();
	void after_thread();
	int cleanup_fds();
	void rw_once();
	void process_notifier(notify_data_t notifier);
	virtual void io_read(int io_fd);
	virtual void io_write(int io_fd);
	virtual void io_err(int io_fd);
	bool get_thread_status();
	void set_thread_status(bool status);
public:
	io_thread();
	~io_thread();
	int add_io_fd(int io_fd, unsigned int events);
	int rm_io_fd(int io_fd);
	void start();
	void stop();
	void wait_for_thread();
};
#endif

#include "io_thread.h"
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

pthread_mutex_t io_thread::io_fds_lock;

void io_thread::io_read(int io_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}
void io_thread::io_write(int io_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}
void io_thread::io_err(int io_fd) {
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

void io_thread::process_notifier(notify_data_t notifier) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	unsigned int flags;

#ifdef LOOPER_USES_EPOLL
	flags = EPOLLMSG | EPOLLERR | EPOLLHUP | EPOLLRDHUP | EPOLLONESHOT | EPOLLET;
	if(notifier.events & flags) {
		io_err(notifier.io_fd);
		return;
	}

	flags = EPOLLIN | EPOLLPRI | EPOLLRDNORM | EPOLLRDBAND;
	if(notifier.events & flags) {
		io_read(notifier.io_fd);
	}
	
	flags = EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND;
	if(notifier.events & flags) {
		io_write(notifier.io_fd);
	}
#else
	flags = POLLMSG | POLLREMOVE | POLLRDHUP | POLLERR | POLLHUP | POLLNVAL;
	if(notifier.events & flags) {
		io_err(notifier.io_fd);
		return;
	}

	flags = POLLIN | POLLPRI | POLLRDNORM | POLLRDBAND;
	if(notifier.events & flags) {
		io_read(notifier.io_fd);
	}
	
	flags = POLLOUT | POLLWRNORM | POLLWRBAND;
	if(notifier.events & flags) {
		io_write(notifier.io_fd);
	}
#endif
}

void io_thread::rw_once() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	int size;
	notify_data_t notifier;
	size = read(wakeup_pipe[0], &notifier, sizeof(notify_data_t));
	if(size <= 0) {
		return;
	}
	
	process_notifier(notifier);
}

void io_thread::before_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

void io_thread::after_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	cleanup_fds();
	close(wakeup_pipe[0]);// make sure close the pipe after threads
	close(wakeup_pipe[1]);
}

void io_thread::thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	hasStarted();

	before_thread();
	while(get_thread_status()) {
		rw_once();
	}
	after_thread();
}

int io_thread::add_io_fd(int io_fd, unsigned int events) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	singlLock s(io_fds_lock);
	unsigned int flags;
#ifdef LOOPER_USES_EPOLL
	flags = /*EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLRDNORM | EPOLLRDBAND | EPOLLWRNORM | EPOLLWRBAND | */EPOLLMSG | EPOLLERR | EPOLLHUP | EPOLLRDHUP | EPOLLONESHOT | EPOLLET;
#else
	flags = /*POLLIN | POLLPRI | POLLOUT | POLLRDNORM | POLLRDBAND | POLLWRNORM | POLLWRBAND | */POLLMSG | POLLREMOVE | POLLRDHUP | POLLERR | POLLHUP | POLLNVAL;
#endif
	events |= flags;

	map_io_fds[io_fd] = events;

	m_poll_thread_instance = poll_thread::get_instance();
	if(m_poll_thread_instance == NULL) {
		return -1;
	}
	return m_poll_thread_instance->add_poll_fd(io_fd, events, wakeup_pipe[1]);
}

int io_thread::rm_io_fd(int io_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	singlLock s(io_fds_lock);
	map_io_fds.erase(io_fd);

	m_poll_thread_instance = poll_thread::get_instance();
	if(m_poll_thread_instance == NULL) {
		return -1;
	}
	return m_poll_thread_instance->rm_poll_fd(io_fd);//need lock
}

int io_thread::cleanup_fds() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	singlLock s(io_fds_lock);

	m_poll_thread_instance = poll_thread::get_instance();
	if(m_poll_thread_instance == NULL) {
		return -1;
	}

	std::map<int, unsigned int>::iterator it = map_io_fds.begin();
	for(it; it != map_io_fds.end(); it++) {
		m_poll_thread_instance->rm_poll_fd(it->first);
	}
	map_io_fds.clear();

	return 0;
}

void io_thread::start() {
	singlLock s(io_fds_lock);
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	
	map_io_fds.clear();

	if (pipe(wakeup_pipe) == -1) {//make sure create pipe before add fds
		printf("pipe create.  errno=%d:%m\n", errno);
		exit(-1);
	}
	int flags;
	flags = fcntl(wakeup_pipe[0],F_GETFL,0);
	flags |= O_NONBLOCK;
	flags = fcntl(wakeup_pipe[0],F_SETFL,flags);

	set_thread_status(true);
	run();
}

void io_thread::stop() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	set_thread_status(false);
}

io_thread::io_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

io_thread::~io_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

void io_thread::wait_for_thread() {
	kill();
}

void io_thread::set_thread_status(bool status) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	m_thread_status = status;
	//printf("%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, m_thread_status ? "true" : "false");
}

bool io_thread::get_thread_status() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	return m_thread_status;
}

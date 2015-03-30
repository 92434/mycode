#include "poll_thread.h"
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif
#include <stdio.h>//printf
#include <stdlib.h>//realloc
#include <errno.h>//errno
#include <unistd.h>//write
#include <string.h>//memset
#include <fcntl.h>//fcntl

pthread_mutex_t poll_thread::poll_lock;

poll_thread *poll_thread::instance = NULL;

void poll_thread::before_thread() {
	fds_notifiers_map.clear();
#ifdef LOOPER_USES_EPOLL
	epoll_events = NULL;
	m_epoll_fd = epoll_create(EPOLL_SIZE_HINT);
	if(m_epoll_fd < 0) {
		printf("Could not create epoll instance.  errno=%d:%m\n", errno);
		exit(-1);
	}
#else
	fds = NULL;
	fds_all.clear();
#endif
}

void poll_thread::after_thread() {
	fds_notifiers_map.clear();

#ifdef LOOPER_USES_EPOLL
	if(epoll_events != NULL) {
		free(epoll_events);
	}
#else
	fds_all.clear();
	if(fds != NULL) {
		free(fds);
	}
#endif

	close(m_epoll_fd);
}

void poll_thread::wait_for_thread() {
	stop();
	kill();
}

void poll_thread::thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	hasStarted();

	before_thread();
	while(get_thread_status()) {
		poll_once();
	}
	after_thread();
}

void poll_thread::start() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	set_thread_status(true);
	run();
}

void poll_thread::stop() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	//kill(true); /* Kill means join actually */
	set_thread_status(false);
}

poll_thread::poll_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	instance = this;
}

poll_thread::~poll_thread() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	wait_for_thread();
	instance = NULL;
}

int poll_thread::process_fds_add(int poll_fd, unsigned int events, int notifier_fd) {
	//printf("%s:%s:%d:%d\n", __FILE__, __func__, __LINE__, poll_fd);
	
	int result = 0;
	bool add = false;

	if(fds_notifiers_map.find(poll_fd) == fds_notifiers_map.end()) {
		add = true;
	}

	int flags;
	flags = fcntl(notifier_fd,F_GETFL,0);
	flags |= O_NONBLOCK;
	fcntl(notifier_fd,F_SETFL,flags);

	fds_notifiers_map[poll_fd] = notifier_fd;

#ifdef LOOPER_USES_EPOLL
	struct epoll_event eventItem;
	memset(& eventItem, 0, sizeof(epoll_event)); // zero out unused members of data field union
	eventItem.events = events;
	eventItem.data.fd = poll_fd;
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	
	if(add != 0) {
		result = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, poll_fd, &eventItem);
	} else {
		result = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, poll_fd, &eventItem);
	}
	if(result != 0) {
		printf("%s failed.  errno=%d:%m\n", add ? "EPOLL_CTL_ADD" : "EPOLL_CTL_MOD", errno);
	}
#else
	fds_all[poll_fd] = events;
#endif

	if(add) {
		update_fds_cache();
	}

	return result;
}

int poll_thread::process_fds_rm(int poll_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	int result = 0;

	if(fds_notifiers_map.find(poll_fd) == fds_notifiers_map.end()) {
		result = -1;
		return result;
	}

	fds_notifiers_map.erase(poll_fd);

#ifdef LOOPER_USES_EPOLL
	struct epoll_event eventItem;
	memset(& eventItem, 0, sizeof(epoll_event)); // zero out unused members of data field union
	eventItem.events = 0;
	eventItem.data.fd = poll_fd;
	result = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, poll_fd, &eventItem);
	if(result != 0) {
		printf("EPOLL_CTL_DEL for %d failed.  errno=%d:%m\n", poll_fd, errno);
	}
#else
	fds_all.erase(poll_fd);
#endif
	update_fds_cache();

	return result;
}

int poll_thread::update_fds_cache() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
#ifdef LOOPER_USES_EPOLL
	//printf("epoll_events:%p, size:%d\n", epoll_events, sizeof(struct epoll_event) * fds_notifiers_map.size());
	epoll_events = (struct epoll_event *)realloc(epoll_events, sizeof(struct epoll_event) * fds_notifiers_map.size());
	if(epoll_events == NULL && fds_notifiers_map.size() != 0) {
		printf("Can not get epoll_events.  errno=%d:%m\n", errno);
		exit(-1);
	}
#else
	printf("fds:%p, size:%d\n", fds, sizeof(struct pollfd) * fds_all.size());
	fds = (struct pollfd *)realloc(fds, sizeof(struct pollfd) * fds_all.size());
	if(fds == NULL && fds_all.size() != 0) {
		printf("Can not get fds.  errno=%d:%m\n", errno);
		exit(-1);
	}

	std::map<int, unsigned int>::iterator it = fds_all.begin();
	int i = 0;
	for(it, i; it != fds_all.end(); it++, i++) {
		fds[i].fd = it->first;
		fds[i].events = it->second;
	}
#endif
	return 0;
}


int poll_thread::add_poll_fd(int poll_fd, unsigned int events, int notifier_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	//printf("%s:%s:%d:%d\n", __FILE__, __func__, __LINE__, poll_fd);
	singlLock s(poll_lock);

	return process_fds_add(poll_fd, events, notifier_fd);
}

int poll_thread::rm_poll_fd(int poll_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	//printf("%s:%s:%d:%d\n", __FILE__, __func__, __LINE__, poll_fd);
	singlLock s(poll_lock);

	return process_fds_rm(poll_fd);
}

int poll_thread::poll_once() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	singlLock s(poll_lock);

	int eventCount;
	//int timeoutMillis = -1;
	int timeoutMillis = 0;

	if(fds_notifiers_map.size() == 0) {
		return 0;
	}

#ifdef LOOPER_USES_EPOLL
	eventCount = epoll_wait(m_epoll_fd, epoll_events, fds_notifiers_map.size() ? fds_notifiers_map.size() : EPOLL_MAX_EVENTS, timeoutMillis);

	for (int i = 0; i < eventCount; i++) {
		int fd = epoll_events[i].data.fd;
		if(fds_notifiers_map.find(fd) == fds_notifiers_map.end()) {
			continue;
		}
		unsigned int events = epoll_events[i].events;
		int notifier_fd = fds_notifiers_map[fd];

		process_fds_rm(fd);

		printf("epoll events[%s%s%s%s%s%s%s%s%s%s%s%s%s] for %d\n",
			events & EPOLLIN ? "EPOLLIN " : " ",
			events & EPOLLPRI ? "EPOLLPRI " : " ",
			events & EPOLLOUT ? "EPOLLOUT " : " ",
			events & EPOLLRDNORM ? "EPOLLRDNORM " : " ",
			events & EPOLLRDBAND ? "EPOLLRDBAND " : " ",
			events & EPOLLWRNORM ? "EPOLLWRNORM " : " ",
			events & EPOLLWRBAND ? "EPOLLWRBAND " : " ",
			events & EPOLLMSG ? "EPOLLMSG " : " ",
			events & EPOLLERR ? "EPOLLERR " : " ",
			events & EPOLLHUP ? "EPOLLHUP " : " ",
			events & EPOLLRDHUP ? "EPOLLRDHUP " : " ",
			events & EPOLLONESHOT ? "EPOLLONESHOT " : " ",
			events & EPOLLET ? "EPOLLET " : " ",
			fd);

		notify_data_t notifier = {
			events : events,
			io_fd : fd,
		};
		write(notifier_fd, &notifier, sizeof(notify_data_t));
	}
#else
	eventCount = poll(fds, fds_all.size(), timeoutMillis);
	for (int i = 0; eventCount != 0 && i < fds_all.size(); i++) {
		int fd = fds[i].fd;
		unsigned int events = fds[i].events;
		int notifier_fd = fds_notifiers_map[fd];

		process_fds_rm(fd);

		if(fds_notifiers_map.find(fd) == fds_notifiers_map.end()) {
			continue;
		}

		printf("epoll events[%s%s%s%s%s%s%s%s%s%s%s%s%s] for %d error: %m(errno: %d)\n",
			events & POLLIN ? "POLLIN " : " ",
			events & POLLPRI ? "POLLPRI " : " ",
			events & POLLOUT ? "POLLOUT " : " ",
			events & POLLRDNORM ? "POLLRDNORM " : " ",
			events & POLLRDBAND ? "POLLRDBAND " : " ",
			events & POLLWRNORM ? "POLLWRNORM " : " ",
			events & POLLWRBAND ? "POLLWRBAND " : " ",
			events & POLLMSG ? "POLLMSG " : " ",
			events & POLLREMOVE ? "POLLREMOVE " : " ",
			events & POLLRDHUP ? "POLLRDHUP " : " ",
			events & POLLERR ? "POLLERR " : " ",
			events & POLLHUP ? "POLLHUP " : " ",
			events & POLLNVAL ? "POLLNVAL " : " ",
			fd, errno);


		notify_data_t notifier = {
			events : events,
			io_fd : fd,
		};
		write(notifier_fd, &notifier, sizeof(notify_data_t));
	}
#endif

	return 0;
}

poll_thread *poll_thread::get_instance() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if(instance != NULL) {
		return instance;
	}

	instance = new poll_thread();
}

void poll_thread::del_instance() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if(instance == NULL) {
		return;
	}

	delete instance;
}

void poll_thread::set_thread_status(bool status) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	m_thread_status = status;
}

bool poll_thread::get_thread_status() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	return m_thread_status;
}

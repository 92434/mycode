#include "poll.h"

#include <errno.h>//errno
#include <string.h>//memset
#include <unistd.h>//memset


pthread_mutex_t poll_wapper::map_poll_fds_lock;
pthread_mutex_t poll_wapper::vector_poll_thread_lock;

int poll_wapper::add_fd(int fd, unsigned int events) {
	int ret = 0;

	singlLock s0(map_poll_fds_lock);
	singlLock s1(vector_poll_thread_lock); 

	if(map_poll_fds.size() >= POLL_FDS_MAX_COUNT) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("open too many files!\n");
		return ret;
	}

	map_poll_fds[fd] = events;

	while(true) {
		std::vector<thread_wapper *>::iterator it_vector_poll_thread;
		thread_wapper *pt;

		for(
				it_vector_poll_thread = vector_poll_thread.begin();
				it_vector_poll_thread != vector_poll_thread.end();
				it_vector_poll_thread++
		   ) {
			pt = *it_vector_poll_thread;
			if(pt->add_poll_fd(fd, events) == 0) {
				return ret;
			}
		}

		pt = allocate_thread(this);
		if(pt == NULL) {
			ret = -1;
			return ret;
		}
		vector_poll_thread.push_back(pt);
	}

	return ret;
}

int poll_wapper::remove_fd(int fd) {
	int ret = 0;

	singlLock s0(map_poll_fds_lock);
	singlLock s1(vector_poll_thread_lock); 

	std::vector<thread_wapper *>::iterator it_vector_poll_thread;

	if(fd == DEFAULT_POLL_FD) {
		fd = map_poll_fds.begin()->first;
	}

	//printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//printf("fd:%x(%p)\n", fd, map_poll_fds.find(fd));
	ret = map_poll_fds.erase(fd);
	//printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//printf("fd:%x(%p)\n", fd, map_poll_fds.find(fd));

	thread_wapper *pt;

	for(
			it_vector_poll_thread = vector_poll_thread.begin();
			it_vector_poll_thread != vector_poll_thread.end();
			it_vector_poll_thread++
	   ) {
		pt = *it_vector_poll_thread;
		if(pt->remove_poll_fd(fd) == 0) {
			break;
		}
	}

	if(it_vector_poll_thread == vector_poll_thread.end()) {
		ret = -1;
		return ret;
	}

	if(pt->could_release() == true) {
		vector_poll_thread.erase(it_vector_poll_thread);
		delete pt;
	}

	return ret;
}

int poll_wapper::remove_all_fds() {
	int ret = 0;
	while(map_poll_fds.size() != 0) {
		remove_fd();
	}
	return ret;
}

int poll_wapper::process_fd(int fd, unsigned int events) {
	int ret = 0;
	//printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	return ret;
}

thread_wapper *select_poll::allocate_thread(poll_wapper *p) {
	return new select_thread(p);
}

pthread_mutex_t thread_wapper::map_poll_thread_fds_lock;
void thread_wapper::thread() {
	hasStarted();
	process();
}

void thread_wapper::start() {
	run();
}
void thread_wapper::stop() {
	kill(true);
}

int thread_wapper::add_poll_fd(int fd, unsigned int events) {
	int ret = 0;

	singlLock s(map_poll_thread_fds_lock);
	if(map_poll_thread_fds.size() >= POLL_FDS_MAX_COUNT_EACH_THREAD) {
		ret = -1;
		return ret;
	}
	map_poll_thread_fds[fd] = events;
	add_poll_fd_ext(fd, events);

	if(map_poll_thread_fds.size() == 1) {
		start();
	}

	return ret;
}

int thread_wapper::remove_poll_fd(int fd) {
	int ret = 0;

	singlLock s(map_poll_thread_fds_lock);

	if(map_poll_thread_fds.find(fd) == map_poll_thread_fds.end()) {
		ret = -1;
		return ret;
	}

	//printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//printf("fd:%x(%p)\n", fd, map_poll_thread_fds.find(fd));
	map_poll_thread_fds.erase(fd);
	//printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	//printf("fd:%x(%p)\n", fd, map_poll_thread_fds.find(fd));

	remove_poll_fd_ext(fd);

	return ret;
}

bool thread_wapper::could_release() {
	singlLock s(map_poll_thread_fds_lock);

	return (map_poll_thread_fds.size() == 0);
}

int select_thread::set_timeout(int tv_sec, int tv_usec) {
	int ret = 0;

	tv.tv_sec = tv_sec;
	tv.tv_usec = tv_usec;

	return ret;
}

int select_thread::process() {
	int ret = 0;
	std::map<int, unsigned int>::iterator it_map_poll_thread_fds;

	while(map_poll_thread_fds.size() != 0) {
		int max_fd = 0;
		{
			singlLock s(map_poll_thread_fds_lock);

			FD_ZERO(&fds);
			for(
					it_map_poll_thread_fds = map_poll_thread_fds.begin();
					it_map_poll_thread_fds != map_poll_thread_fds.end();
					it_map_poll_thread_fds++
			   ) {
				if(it_map_poll_thread_fds->first > max_fd) {
					max_fd = it_map_poll_thread_fds->first;
				}
				FD_SET(it_map_poll_thread_fds->first, &fds);
			}
		}

		if(select(max_fd + 1, &fds, NULL, NULL, &tv) > 0) {
			for(
					it_map_poll_thread_fds = map_poll_thread_fds.begin();
					it_map_poll_thread_fds != map_poll_thread_fds.end();
					it_map_poll_thread_fds++
			   ) {
				if(FD_ISSET(it_map_poll_thread_fds->first, &fds)) {
					if(ppoll_wapper != NULL) {
						ppoll_wapper->process_fd(it_map_poll_thread_fds->first, 0);
					}
				}
			}
		}
	}
	return ret;
}

thread_wapper *poll_poll::allocate_thread(poll_wapper *p) {
	return new poll_thread(p);
}

int poll_thread::set_timeout(int timeout) {
	int ret = 0;
	timeout_ms = timeout;
	return ret;
}

int poll_thread::update_poll_fds_buffer() {
	int ret = 0;
	if(fds == NULL && map_poll_thread_fds.size() != 0) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s\n", strerror(errno));
		ret = -1;
		exit(ret);
	}
	std::map<int, unsigned int>::iterator it = map_poll_thread_fds.begin();
	int i = 0;
	for(; it != map_poll_thread_fds.end(); it++, i++) {
		fds[i].fd = it->first;
		fds[i].events = it->second;
	}
	fds_count = i;

	return ret;
}

int poll_thread::add_poll_fd_ext(int fd, unsigned int events) {
	int ret = 0;
	update_poll_fds_buffer();
	return ret;
}

int poll_thread::remove_poll_fd_ext(int fd) {
	int ret = 0;
	update_poll_fds_buffer();
	return ret;
}

int poll_thread::process() {
	int ret = 0;
	while(map_poll_thread_fds.size() != 0) {
		int eventCount = poll(fds, fds_count, timeout_ms);
		for (int i = 0; eventCount != 0 && i < fds_count; i++) {
			int fd = fds[i].fd;
			unsigned int events = fds[i].events;

			//printf("epoll events[%s%s%s%s%s%s%s%s%s%s%s%s%s] for %d error: %m(errno: %d)\n",
			//		events & POLLIN ? "POLLIN " : " ",
			//		events & POLLPRI ? "POLLPRI " : " ",
			//		events & POLLOUT ? "POLLOUT " : " ",
			//		events & POLLRDNORM ? "POLLRDNORM " : " ",
			//		events & POLLRDBAND ? "POLLRDBAND " : " ",
			//		events & POLLWRNORM ? "POLLWRNORM " : " ",
			//		events & POLLWRBAND ? "POLLWRBAND " : " ",
			//		events & POLLMSG ? "POLLMSG " : " ",
			//		events & POLLREMOVE ? "POLLREMOVE " : " ",
			//		events & POLLRDHUP ? "POLLRDHUP " : " ",
			//		events & POLLERR ? "POLLERR " : " ",
			//		events & POLLHUP ? "POLLHUP " : " ",
			//		events & POLLNVAL ? "POLLNVAL " : " ",
			//		fd, errno);
			if(ppoll_wapper != NULL) {
				ppoll_wapper->process_fd(fd, events);
			}
		}
	}
	return ret;
}

thread_wapper *epoll_poll::allocate_thread(poll_wapper *p) {
	return new epoll_thread(p);
}

int epoll_thread::set_timeout(int timeout) {
	int ret = 0;
	timeout_ms = timeout;
	return ret;
}

int epoll_thread::update_poll_fds_buffer() {
	int ret = 0;
	if(epoll_events == NULL && map_poll_thread_fds.size() != 0) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s\n", strerror(errno));
		ret = -1;
		exit(ret);
	}
	return ret;
}

int epoll_thread::add_poll_fd_ext(int fd, unsigned int events) {
	int ret = 0;
	struct epoll_event eventItem;

	if(m_epoll_fd == -1) {
		m_epoll_fd = epoll_create(EPOLL_SIZE_HINT);
		if(m_epoll_fd < 0) {
			printf("Could not create epoll instance.  errno=%d:%m\n", errno);
			ret = -1;
			exit(ret);
		}
	}

	memset(&eventItem, 0, sizeof(epoll_event)); // zero out unused members of data field union
	eventItem.events = events;
	eventItem.data.fd = fd;
	ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &eventItem);
	if(ret != EEXIST) {
		ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &eventItem);
	} else {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s\n", strerror(errno));
		ret = -1;
	}

	update_poll_fds_buffer();

	return ret;
}

int epoll_thread::remove_poll_fd_ext(int fd) {
	int ret = 0;
	struct epoll_event eventItem;

	memset(&eventItem, 0, sizeof(epoll_event)); // zero out unused members of data field union
	eventItem.events = 0;
	eventItem.data.fd = fd;
	ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &eventItem);
	if(ret != 0) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("%s\n", strerror(errno));
		ret = -1;
	}
	update_poll_fds_buffer();

	if(map_poll_thread_fds.size() == 0) {
		close(m_epoll_fd);
	}
	return ret;
}

int epoll_thread::process() {
	int ret = 0;
	while(map_poll_thread_fds.size() != 0) {
		int eventCount = epoll_wait(m_epoll_fd, epoll_events, map_poll_thread_fds.size() ? map_poll_thread_fds.size() : EPOLL_MAX_EVENTS, timeout_ms);

		for (int i = 0; i < eventCount; i++) {
			int fd = epoll_events[i].data.fd;
			unsigned int events = epoll_events[i].events;

			//printf("epoll events[%s%s%s%s%s%s%s%s%s%s%s%s%s] for %d\n",
			//		events & EPOLLIN ? "EPOLLIN " : " ",
			//		events & EPOLLPRI ? "EPOLLPRI " : " ",
			//		events & EPOLLOUT ? "EPOLLOUT " : " ",
			//		events & EPOLLRDNORM ? "EPOLLRDNORM " : " ",
			//		events & EPOLLRDBAND ? "EPOLLRDBAND " : " ",
			//		events & EPOLLWRNORM ? "EPOLLWRNORM " : " ",
			//		events & EPOLLWRBAND ? "EPOLLWRBAND " : " ",
			//		events & EPOLLMSG ? "EPOLLMSG " : " ",
			//		events & EPOLLERR ? "EPOLLERR " : " ",
			//		events & EPOLLHUP ? "EPOLLHUP " : " ",
			//		events & EPOLLRDHUP ? "EPOLLRDHUP " : " ",
			//		events & EPOLLONESHOT ? "EPOLLONESHOT " : " ",
			//		events & EPOLLET ? "EPOLLET " : " ",
			//		fd);
			if(ppoll_wapper != NULL) {
				ppoll_wapper->process_fd(fd, events);
			}
		}
	}
	return ret;
}

#include "net_server.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif
#include <fcntl.h>

void net_server::io_err(int io_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	close(io_fd);
}

void net_server::io_read(int io_fd) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	int size;
	char buff[MAXLINE] = {0};

	size = recv(io_fd, buff, MAXLINE, 0);
	if(size <= 0) {
		printf("read %d error: %m(errno: %d)\n", io_fd, errno);
		//close(io_fd);
	} else {
		int status;
#ifdef LOOPER_USES_EPOLL
		status = add_io_fd(io_fd, EPOLLIN | EPOLLPRI);
#else
		status = add_io_fd(io_fd, POLLIN | POLLPRI);
#endif
		if(status != 0) {
			printf("add_io_fd error: %m(errno: %d)", errno);
			rm_io_fd(io_fd);
			close(io_fd);
		}
	}

	buff[size] = '\0';
	printf("recv msg from client(%d): %s\n", io_fd, buff);

}

void net_server::init_server_socket(int domain,
				int type,
				int protocol,
				unsigned int in_addr,
				unsigned int server_port) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	m_domain = domain;
	m_type = type;
	m_protocol = protocol;
	m_in_addr = in_addr;
	m_server_port = server_port;
}

void net_server::start_net_server() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if((m_listenfd = socket(m_domain, m_type, m_protocol)) < 0) {
		printf("create socket error: %m(errno: %d)\n", errno);
		exit(0);
	}

	memset(&m_servaddr, 0, sizeof(m_servaddr));
	//协议地址
	m_servaddr.sin_family = m_domain;
	m_servaddr.sin_addr.s_addr = htonl(m_in_addr);
	m_servaddr.sin_port = htons(m_server_port);
	//bind把一个地址族中的特定地址赋给socket
	if(bind(m_listenfd, (struct sockaddr*)&m_servaddr, sizeof(m_servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if(listen(m_listenfd, 10) == -1) {
		//listen 监听
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	start();

	printf("======waiting for client's request======\n");

	while(true) {
		if((m_connfd = accept(m_listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
			continue;
		}
		printf("%s:%s:%d:accept %d\n", __FILE__, __func__, __LINE__, m_connfd);

		int status;
#ifdef LOOPER_USES_EPOLL
		status = add_io_fd(m_connfd, EPOLLIN | EPOLLPRI);
#else
		status = add_io_fd(m_connfd, POLLIN | POLLPRI);
#endif
		if(status != 0) {
			printf("add_io_fd error: %m(errno: %d)", errno);
			rm_io_fd(m_connfd);
			close(m_connfd);
		}
	}
}

#ifndef __NET_SERVER_H
#define __NET_SERVER_H
#include "io_thread.h"
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define MAXLINE 4096

class net_server : public io_thread {
	int m_domain;
	int m_type;
	int m_protocol;
	unsigned int m_in_addr;
	unsigned int m_server_port;
	struct sockaddr_in m_servaddr;
	int m_listenfd, m_connfd;
	void io_err(int io_fd);
	void io_read(int io_fd);
public:
	net_server() {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		m_listenfd = -1;
	}

	~net_server() {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		wait_for_thread();
		if(m_listenfd >= 0) {
			close(m_listenfd);
		}
	}

	void init_server_socket(int domain = AF_INET,
					int type = SOCK_STREAM,
					int protocol = 0,
					unsigned int in_addr = INADDR_ANY,
					unsigned int server_port = 6666);

	void start_net_server();
};

#endif

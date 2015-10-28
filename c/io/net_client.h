#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H

#define MAXLINE 4096

class net_client {
	int m_domain;
	int m_type;
	int m_protocol;
	char *m_in_ip_addr;
	unsigned int m_server_port;
	struct sockaddr_in m_client_addr;
	int m_connfd;

public:
	net_client() {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		m_connfd = -1;
	}

	~net_client() {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		wait_for_thread();
		if(m_connfd >= 0) {
			close(m_connfd);
		}
	}

	void init_client_socket(int domain = AF_INET,
					int type = SOCK_STREAM,
					int protocol = 0,
					char *in_ip_addr = "127.0.0.1",
					unsigned int server_port = 6666);

	void create_net_client();
};

#endif

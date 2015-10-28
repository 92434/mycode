#include "net_client.h"
#include "my_type.h"//notify_data_t
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif
#include <fcntl.h>

void net_client::init_client_socket(int domain,
				int type,
				int protocol,
				char *in_ip_addr,
				unsigned int server_port) {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	m_domain = domain;
	m_type = type;
	m_protocol = protocol;
	m_in_ip_addr = in_ip_addr;
	m_server_port = server_port;
}

void net_client::create_net_client() {
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	if((m_connfd = socket(m_domain, m_type, m_protocol)) < 0) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	memset(&m_client_addr, 0, sizeof(m_client_addr));
	//协议地址
	m_client_addr.sin_family = m_domain;
	if(inet_pton(AF_INET, m_in_ip_addr, &m_client_addr.sin_addr) <= 0){
	//Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换 
		printf("inet_pton error for %s\n",argv[1]);
		exit(0);
	}
	m_client_addr.sin_port = htons(m_server_port);
	if(connect(sockfd, (struct sockaddr*)&m_client_addr, sizeof(m_client_addr)) < 0){
	//连接请求
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	}

	printf("send msg to server: \n");
	fgets(sendline, 4096, stdin);
	if(send(sockfd, sendline, strlen(sendline), 0) < 0)
	{
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
}

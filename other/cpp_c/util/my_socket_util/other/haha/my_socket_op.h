#ifndef __MY_SOCKET_OP_H_INCLUDE
#define __MY_SOCKET_OP_H_INCLUDE
#include <stdio.h>
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")

class my_socket_op
{
public:
	my_socket_op(void);
	bool init_env( void );
	int uninit_env( void );

	bool open_socket(IN int af, IN int type, IN int protocol);
	bool bind_host_port( const char *cp_server_ip, u_short us_server_port );
	bool listen_host_port();
	void accept_client( void );

	bool connect_server( const char *cp_server_ip, u_short us_server_port );
protected:
	SOCKET socket_host;
	SOCKET socket_accept;
	sockaddr_in addr_client;
private:
};
#endif//#ifndef __MY_SOCKET_OP_H_INCLUDE

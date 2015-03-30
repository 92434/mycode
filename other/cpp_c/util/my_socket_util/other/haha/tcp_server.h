#ifndef __TCP_SERVER_H_INCLUDE
#define __TCP_SERVER_H_INCLUDE
#include <stdio.h>
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")

#include <list>
using namespace std;

typedef struct _client_info
{
	SOCKET socket_connect;
	sockaddr_in addr_client;
} client_info_t;

class tcp_server
{
public:
	tcp_server();
	bool init_env( void );
	int uninit_env( void );
private:
	static bool create_pfd_pool_enent( char *cp_event_name = "lst_pfd_pool_event" );
	static bool insert_pfd_pool( client_info_t *p_fd );
	static bool get_fd_from_pfd_pool( HANDLE& h_lst_pfd_pool_event, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set );
	static bool remove_fd_from_fd_set( client_info_t *p_fd, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set );
	static DWORD WINAPI thread_proc_tcp_recv( LPVOID socket_connect );
	static DWORD WINAPI thread_proc_tcp_send( LPVOID lpParam );
public:
	bool open_socket();
	bool bind_host_port( const char *cp_server_ip = "0.0.0.0", u_short us_server_port = 27015 );
	bool listen_host_port();
	void accept_client( void );
private:
	SOCKET socket_server;
	SOCKET socket_accept;

	static HANDLE h_lst_pfd_pool_event;
	static list<client_info_t *> lst_pfd_pool;
};
#endif//#ifndef __TCP_SERVER_H_INCLUDE
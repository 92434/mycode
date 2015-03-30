#ifndef __MY_TCP_SOCKET_OP_H_INCLUDE
#define __MY_TCP_SOCKET_OP_H_INCLUDE
#include "my_socket_op.h"

#include <list>
using namespace std;

typedef struct _client_info
{
	SOCKET socket_client;
	sockaddr_in addr_client;
} client_info_t;

class my_tcp_socket_op : public my_socket_op
{
public:
	static bool insert_pfd_pool( client_info_t *p_fd );
	static bool get_fd_from_pfd_pool( list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set );
	static bool remove_fd_from_fd_set( client_info_t *p_fd, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set );
	
	static DWORD WINAPI thread_proc_tcp_server_recv( LPVOID lp_para );
	static DWORD WINAPI thread_proc_tcp_server_send( LPVOID lp_para );
	bool create_tcp_server(const char *cp_server_ip = "0.0.0.0", u_short us_server_port = 0);
	bool accept_tcp_client();

	static DWORD WINAPI thread_proc_tcp_client_recv( LPVOID lp_para );
	static DWORD WINAPI thread_proc_tcp_client_send( LPVOID lp_para );
	bool create_tcp_client(const char *cp_server_ip = "0.0.0.0", u_short us_server_port = 0);
protected:
private:
	static list<client_info_t *> lst_pfd_pool;
	static HANDLE h_pfd_pool_lock;
};
#endif//#ifndef __MY_TCP_SOCKET_OP_H_INCLUDE

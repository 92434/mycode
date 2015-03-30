#include "my_tcp_socket_op.h"
#include "my_lock_op.h"
#include "my_thread.h"

list<client_info_t *> my_tcp_socket_op::lst_pfd_pool;
HANDLE my_tcp_socket_op::h_pfd_pool_lock = INVALID_HANDLE_VALUE;

bool my_tcp_socket_op::insert_pfd_pool( client_info_t *p_client_info )
{
	bool b_rtn = false;

	if (my_lock_op::get_lock(h_pfd_pool_lock))
	{
		lst_pfd_pool.push_back(p_client_info);
		
		my_lock_op::release_lock(h_pfd_pool_lock);
		b_rtn = true;
	}

	return b_rtn;
}

bool my_tcp_socket_op::get_fd_from_pfd_pool( list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set )
{
	bool b_rtn = false;

	if (my_lock_op::get_lock(h_pfd_pool_lock))
	{
		while (lst_pfd_pool.size())
		{
			if (lst_pfd_set_pool.size() < FD_SETSIZE)
			{
				client_info_t *p_client_info = lst_pfd_pool.front();
				printf("get fd %d from pool!\n", p_client_info->socket_client);
				
				lst_pfd_set_pool.push_back(p_client_info);
				lst_pfd_pool.pop_front();

				FD_SET(p_client_info->socket_client, p_fd_set);
			}

		}

		my_lock_op::release_lock(h_pfd_pool_lock);
		b_rtn = true;
	}
	return b_rtn;
}

bool my_tcp_socket_op::remove_fd_from_fd_set( client_info_t *p_client_info, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set )
{
	bool b_rtn = FALSE;

	FD_CLR(p_client_info->socket_client, p_fd_set);

	list<client_info_t *>::iterator it_lst;

	for (it_lst = lst_pfd_set_pool.begin(); it_lst != lst_pfd_set_pool.end(); it_lst++)
	{
		client_info_t *it_p_fd = *it_lst;

		if (p_client_info == it_p_fd)
		{
			printf("disconnect from %s on socket %d at address 0x%08x!\n", inet_ntoa(p_client_info->addr_client.sin_addr), p_client_info->socket_client, p_client_info);
			if(closesocket(p_client_info->socket_client) != 0)
			{
				printf( "Error at closesocket(): %ld\n", WSAGetLastError() );
			}
			free(p_client_info);
			
			lst_pfd_set_pool.remove(p_client_info);
			b_rtn = TRUE;
			break;
		}
	}

	return b_rtn;
}


bool my_tcp_socket_op::create_tcp_server( const char *cp_server_ip /*= "0.0.0.0"*/, u_short us_server_port /*= 0*/ )
{
	bool b_rtn = true;

	if (b_rtn && !open_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	{
		b_rtn = false;
	}

	if (b_rtn && !bind_host_port(cp_server_ip, us_server_port))
	{
		b_rtn = false;
	}

	if (b_rtn && !listen_host_port())
	{
		b_rtn = false;
	}

	return b_rtn;
}

DWORD WINAPI my_tcp_socket_op::thread_proc_tcp_server_recv( LPVOID lp_para )
{
	list<client_info_t *> lst_pfd_set_pool;

	fd_set fd_connect;

	FD_ZERO(&fd_connect);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 300;

	lst_pfd_set_pool.clear();

	// Send and receive data.

	while (1)
	{
		get_fd_from_pfd_pool(lst_pfd_set_pool, &fd_connect);

		fd_set fd_connect_bak = fd_connect;
		timeval tv_bak = tv;

		//printf("slave before select\n");
		int iErr = select(fd_connect_bak.fd_count, &fd_connect_bak, NULL, NULL, &tv_bak);
		//printf("iErr %d\n", iErr);
		//printf("slave after select\n");
		if (iErr == 0)  //0：等待超时，没有可读写或错误的文件
		{
			continue;
		}

		list<client_info_t *>::iterator it_lst;
		for (it_lst = lst_pfd_set_pool.begin(); it_lst != lst_pfd_set_pool.end(); it_lst++)
		{
			client_info_t *p_client_info = *it_lst;

			if(FD_ISSET(p_client_info->socket_client, &fd_connect_bak))
			{
				char csz_revc_buffer[4096] = "";
				memset(csz_revc_buffer, 0, sizeof(csz_revc_buffer));
				int i_bytes_recv = recv( p_client_info->socket_client, csz_revc_buffer, sizeof(csz_revc_buffer), 0 );

				if (i_bytes_recv > 0)
				{
					printf( "%d:%s (%ld)\n", p_client_info->socket_client, csz_revc_buffer, i_bytes_recv );
				}
				else
				{
					printf("exit %d(%d)\n", p_client_info->socket_client, i_bytes_recv);
					remove_fd_from_fd_set(p_client_info, lst_pfd_set_pool, &fd_connect);
					break;
				}
			}
		}

		if (!lst_pfd_set_pool.size())
		{
			break;
		}
	}

	printf("exit a server thread!\n");
	return 0;
}

DWORD WINAPI my_tcp_socket_op::thread_proc_tcp_server_send( LPVOID lp_para )
{
	SOCKET socket_connect = SOCKET_ERROR;

	while (1)
	{
		int bytesSent;
		char sendbuf[32] = "";

		char *cpStop;

		memset(sendbuf, 0, 32);
		scanf("%s", sendbuf);

		if(sendbuf[0] == ':')
		{
			char *cpStart = sendbuf + 1;
			socket_connect = (SOCKET)strtol(cpStart, &cpStop, 10);
			printf("change to client %d\n", socket_connect);
		}
		else if (socket_connect != SOCKET_ERROR)
		{
			printf("%s\t>%d\n", sendbuf, socket_connect);
			bytesSent = send( socket_connect, sendbuf, strlen(sendbuf), 0 );
			printf( "Bytes Sent: %ld\n", bytesSent );
		}
		else
		{
			printf("input a vilad client socket number!\n");
		}
	}

	return 0;
}

bool my_tcp_socket_op::accept_tcp_client()
{
	bool b_rtn = false;

	my_thread::create_thread(thread_proc_tcp_server_send, 0);

	if (h_pfd_pool_lock == INVALID_HANDLE_VALUE)
	{
		h_pfd_pool_lock = my_lock_op::create_lock("lst_pfd_pool_event");

		if (h_pfd_pool_lock == NULL)
		{
			return b_rtn;
		}
	}

	lst_pfd_pool.clear();

	printf("-----------Server-----------\n");

	while(1)
	{
		fd_set fd_server;

		FD_ZERO(&fd_server);
		FD_SET(socket_host,&fd_server);

		timeval tv;
		tv.tv_sec = 30;
		tv.tv_usec = 0;

		//printf( "Waiting for a client to connect...\n" );
		printf("server before select\n");
		int iErr = select(fd_server.fd_count, &fd_server, NULL, NULL, &tv);//不活跃的描述符对应位置0
		printf("server after select\n");

		if(iErr == 0)  //0：等待超时，没有可读写或错误的文件
		{
			continue;
		}

		if(FD_ISSET(socket_host, &fd_server))//经过select()后的read_fds中对应位为1则FD_ISSET()返回1
		{
			accept_client();

			client_info_t *p_client_info = (client_info_t *)malloc(sizeof(client_info_t));

			if(p_client_info)
			{
				memset(p_client_info, 0, sizeof(client_info_t));

				p_client_info->socket_client = socket_accept;
				p_client_info->addr_client = addr_client;
	
				insert_pfd_pool(p_client_info);

				Sleep(300);

				if (my_lock_op::get_lock(h_pfd_pool_lock))
				{
					if (lst_pfd_pool.size())
					{
						printf("create a recv thread!\n");
						my_thread::create_thread(thread_proc_tcp_server_recv, 0);
					}
					my_lock_op::release_lock(h_pfd_pool_lock);
				}
			}
		}
	}

	b_rtn = true;
	return b_rtn;
}

DWORD WINAPI my_tcp_socket_op::thread_proc_tcp_client_recv( LPVOID lp_para )
{
	SOCKET *p_socket_host = (SOCKET *)lp_para;

	// Send and receive data.

	while(*p_socket_host != SOCKET_ERROR)
	{
		fd_set fd_revc;
		FD_ZERO(&fd_revc);
		FD_SET(*p_socket_host, &fd_revc);

		timeval select_timeout;
		select_timeout.tv_sec = 30;
		select_timeout.tv_usec = 0;

		printf("slave before select\n");
		int i_select_status = select(fd_revc.fd_count, &fd_revc, NULL, NULL, &select_timeout);
		printf("slave after select\n");
		printf("slave select result:0x%x\n", i_select_status);
		if (i_select_status == 0)  //0：等待超时，没有可读写或错误的文件
		{
			continue;
		}

		if(FD_ISSET(*p_socket_host, &fd_revc))
		{
			char csz_recv_buffer[4096];
			memset(csz_recv_buffer, 0, sizeof(csz_recv_buffer));

			int i_bytes_recv = SOCKET_ERROR;
			i_bytes_recv = recv( *p_socket_host, csz_recv_buffer, sizeof(csz_recv_buffer), 0 );
			if(i_bytes_recv > 0)
			{
				printf( "%d:%s (%ld)\n", *p_socket_host, csz_recv_buffer, i_bytes_recv );
			}
			else
			{
				printf("exit %d(%d)\n", *p_socket_host, i_bytes_recv);
				*p_socket_host = SOCKET_ERROR;
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI my_tcp_socket_op::thread_proc_tcp_client_send( LPVOID lp_para )
{
	SOCKET *p_socket_host = (SOCKET *)lp_para;

	// Send and receive data.
	while (*p_socket_host != SOCKET_ERROR)
	{
		char csz_send_buffer[4096];

		memset(csz_send_buffer, 0, sizeof(csz_send_buffer));
		scanf("%s", csz_send_buffer);
		printf("%s>%d\n", csz_send_buffer, *p_socket_host);

		int i_bytes_send = send( *p_socket_host, csz_send_buffer, strlen(csz_send_buffer), 0 );

		printf( "Bytes Sent: %ld\n", i_bytes_send );
	}

	return 0;
}

bool my_tcp_socket_op::create_tcp_client( const char *cp_server_ip /*= "0.0.0.0"*/, u_short us_server_port /*= 0*/ )
{
	bool b_rtn = true;

	if (b_rtn && !open_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	{
		b_rtn = false;
	}

	if (b_rtn && !connect_server(cp_server_ip, us_server_port))
	{
		b_rtn = false;
	}

	if (b_rtn)
	{
		my_thread::create_thread(thread_proc_tcp_client_send, &socket_host);
		my_thread::create_thread(thread_proc_tcp_client_recv, &socket_host);

		while(socket_host != SOCKET_ERROR)
		{
			Sleep(100);
		}
	}

	return b_rtn;
}

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


HANDLE h_lst_pfd_pool_event;
list<client_info_t *> lst_pfd_pool;

bool insert_pfd_pool(client_info_t *p_fd)
{
	bool b_rtn = false;

    DWORD dwWaitResult;
    HANDLE hEvents[1]; 

    hEvents[0] = h_lst_pfd_pool_event;  // thread's read event

    dwWaitResult = WaitForMultipleObjects( 
        1,            // number of handles in array
        hEvents,      // array of event handles
        TRUE,         // wait till all are signaled
        INFINITE);    // indefinite wait

    switch (dwWaitResult) 
    {
        // Both event objects were signaled.
        case WAIT_OBJECT_0: 
			printf("insert a fd to pool!\n");
			lst_pfd_pool.push_back(p_fd);
			b_rtn = true;
            break; 

        // An error occurred.
        default: 
            printf("Wait error: %d\n", GetLastError()); 
    }

    // Set the read event to signaled.

    if (! SetEvent(hEvents[0]) ) 
    { 
        printf("SetEvent failed (%d)\n", GetLastError());
    }

	return b_rtn;
}

bool get_fd_from_pfd_pool(HANDLE& h_lst_pfd_pool_event, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set)
{
	bool b_rtn = false;

    DWORD dwWaitResult;
    HANDLE hEvents[1]; 

    hEvents[0] = h_lst_pfd_pool_event;  // thread's read event

    dwWaitResult = WaitForMultipleObjects( 
        1,            // number of handles in array
        hEvents,      // array of event handles
        TRUE,         // wait till all are signaled
        INFINITE);    // indefinite wait

    switch (dwWaitResult) 
    {
        // Both event objects were signaled.
        case WAIT_OBJECT_0: 
			while(lst_pfd_pool.size())
			{
				if (lst_pfd_set_pool.size() < FD_SETSIZE)
				{
					printf("get fds from pool!\n");
					lst_pfd_set_pool.push_back(lst_pfd_pool.front());
					lst_pfd_pool.pop_front();

					FD_SET(lst_pfd_set_pool.back()->socket_connect, p_fd_set);
				}
				else
				{
					break;
				}
			}
			b_rtn = true;
            break; 

        // An error occurred.
        default: 
            printf("Wait error: %d\n", GetLastError()); 
    }

    // Set the read event to signaled.

    if (! SetEvent(hEvents[0]) ) 
    { 
        printf("SetEvent failed (%d)\n", GetLastError());
    }

	return b_rtn;
}

bool remove_fd_from_fd_set(client_info_t *p_fd, list<client_info_t *>& lst_pfd_set_pool, fd_set *p_fd_set)
{
	bool b_rtn = FALSE;

	FD_CLR(p_fd->socket_connect, p_fd_set);

	list<client_info_t *>::iterator it_lst;

	for (it_lst = lst_pfd_set_pool.begin(); it_lst != lst_pfd_set_pool.end(); it_lst++)
	{
		client_info_t *it_p_fd = *it_lst;

		if (p_fd == it_p_fd)
		{
			printf("disconnect from %s on socket %d at address 0x%08x!\n", inet_ntoa(p_fd->addr_client.sin_addr), p_fd->socket_connect, p_fd);
			if(closesocket(p_fd->socket_connect) != 0)
			{
		        printf( "Error at closesocket(): %ld\n", WSAGetLastError() );
			}
			free(p_fd);
			lst_pfd_set_pool.remove(p_fd);
			b_rtn = TRUE;
			break;
		}
	}

	return b_rtn;
}

DWORD WINAPI thread_proc_tcp_recv( LPVOID socket_connect )
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
		get_fd_from_pfd_pool(h_lst_pfd_pool_event, lst_pfd_set_pool, &fd_connect);

		fd_set fd_connect_bak = fd_connect;
		timeval tv_bak = tv;


        //printf("slave before select\n");
        int iErr = select(fd_connect_bak.fd_count, &fd_connect_bak, NULL, NULL, &tv_bak);
        //printf("slave after select\n");
        if (iErr == 0)  //0：等待超时，没有可读写或错误的文件
        {
            continue;
        }

		list<client_info_t *>::iterator it_lst;
		for (it_lst = lst_pfd_set_pool.begin(); it_lst != lst_pfd_set_pool.end(); it_lst++)
		{
			client_info_t *p_fd = *it_lst;


			if(FD_ISSET(p_fd->socket_connect, &fd_connect_bak))
			{
				char csz_revc_buffer[4096] = "";
				memset(csz_revc_buffer, 0, sizeof(csz_revc_buffer));
				int i_bytes_recv = recv( p_fd->socket_connect, csz_revc_buffer, sizeof(csz_revc_buffer), 0 );

				if (i_bytes_recv > 0)
				{
					printf( "%d:%s (%ld)\n", p_fd->socket_connect, csz_revc_buffer, i_bytes_recv );
				}
				else
				{
					printf("exit %d(%d)\n", p_fd->socket_connect, i_bytes_recv);
					remove_fd_from_fd_set(p_fd, lst_pfd_set_pool, &fd_connect);
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


DWORD WINAPI thread_proc_tcp_send( LPVOID lpParam )
{
    // Send and receive data.

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

    return 1;
}



DWORD WINAPI thread_proc_tcp_server( LPVOID lpParam )
{
	static bool sb_create_send_thread = true;
    DWORD d_thread_id;

    CreateThread(NULL, 0, thread_proc_tcp_recv, lpParam, 0, &d_thread_id);

	if(sb_create_send_thread)
	{
		sb_create_send_thread = false;
	    CreateThread(NULL, 0, thread_proc_tcp_send, lpParam, 0, &d_thread_id);
	}
    return 0;
}


int main()
{

    // Initialize Winsock.
    WSADATA wsa_data;
    int i_wsa_status = WSAStartup( MAKEWORD(2,2), &wsa_data );
    if ( i_wsa_status != NO_ERROR )
    {
        printf("Error at WSAStartup()\n");
        WSACleanup();
        return 1;
    }

    // Create a socket.
    SOCKET socket_server = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( socket_server == INVALID_SOCKET )
    {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return 1;
    }
	else
	{
        printf( "server socket %ld\n", socket_server );
	}

    // Bind the socket.
    sockaddr_in addr_server;

    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = inet_addr( "0.0.0.0" );
    addr_server.sin_port = htons( 27015 );

    if ( bind( socket_server, (SOCKADDR*) &addr_server, sizeof(addr_server) ) == SOCKET_ERROR )
    {
        printf( "bind() failed.\n" );
        goto Exit;
    }

    // Listen on the socket.
    if ( listen( socket_server, 1 ) == SOCKET_ERROR )
    {
        printf( "Error listening on socket.\n");
        goto Exit;
    }


    // Accept connections.
    SOCKET socket_accept = SOCKET_ERROR;

    printf("-----------Server-----------\n");

	h_lst_pfd_pool_event = CreateEvent( 
        NULL,         // default security attributes
        FALSE,         // auto-reset event
        TRUE,         // initial state is signaled
        "lst_pfd_pool_event"  // object name
        );
	if(h_lst_pfd_pool_event == NULL)
	{
        printf("CreateEvent failed (%d)\n", GetLastError());
        goto Exit;
	}
	else
	{
		lst_pfd_pool.clear();
	}

    while(1)
    {
        fd_set fd_server;

        FD_ZERO(&fd_server);
        FD_SET(socket_server,&fd_server);

        timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        //printf( "Waiting for a client to connect...\n" );
        printf("server before select\n");
        int iErr = select(fd_server.fd_count, &fd_server, NULL, NULL, &tv);//不活跃的描述符对应位置0
        printf("server after select\n");
        printf("select result:%d\n", iErr);

        if(iErr == 0)  //0：等待超时，没有可读写或错误的文件
        {
            continue;
        }

        if(FD_ISSET(socket_server, &fd_server))//经过select()后的read_fds中对应位为1则FD_ISSET()返回1
        {
            sockaddr_in addr_client;

            memset(&addr_client, 0, sizeof(addr_client));

            int addrlen = sizeof(addr_client);

            while ( socket_accept == SOCKET_ERROR )
            {
                socket_accept = accept( socket_server, (struct sockaddr *)&addr_client, &addrlen );

                printf("New connection from %s(%d) on socket %d\n",inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port), socket_accept);
            }

            printf( "Client Connected.\n");

            client_info_t *p_client_info = (client_info_t *)malloc(sizeof(client_info_t));
            if(p_client_info)
            {
                memset(p_client_info, 0, sizeof(client_info_t));

                p_client_info->socket_connect = socket_accept;
                p_client_info->addr_client = addr_client;
                socket_accept = SOCKET_ERROR;

				lst_pfd_pool.push_back(p_client_info);

				Sleep(300);

				if (lst_pfd_pool.size())
				{
					DWORD d_thread_id;
					printf("create a server thread!\n");
					CreateThread(NULL, 0, thread_proc_tcp_server, 0, 0, &d_thread_id);
				}
            }
        }
    }

Exit:
	if(closesocket(socket_server) != 0)
	{
        printf( "Error at closesocket(): %ld\n", WSAGetLastError() );
	}
    return 0;
}



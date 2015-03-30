#include <stdio.h>
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")

bool b_exit = false;

// Connect to a server.
sockaddr_in addr_client;


DWORD WINAPI thread_proc_tcp_receive( LPVOID lp_para )
{
    SOCKET *p_socket = (SOCKET *)lp_para;
    SOCKET socket_accept = *p_socket;

    // Send and receive data.

    while (!b_exit)
    {
        fd_set fd_revc;
        FD_ZERO(&fd_revc);
        FD_SET(socket_accept, &fd_revc);

        timeval select_timeout;
        select_timeout.tv_sec = 30;
        select_timeout.tv_usec = 0;

        printf("slave before select\n");
        int i_select_status = select(socket_accept + 1, &fd_revc, NULL, NULL, &select_timeout);
        printf("slave after select\n");
        printf("slave select result:0x%x\n", i_select_status);
        if (i_select_status == 0)  //0：等待超时，没有可读写或错误的文件
        {
            continue;
        }

        if(FD_ISSET(socket_accept, &fd_revc))
        {
            char csz_recv_buffer[4096];
            memset(csz_recv_buffer, 0, sizeof(csz_recv_buffer));

            int i_bytes_recv = SOCKET_ERROR;
            i_bytes_recv = recv( socket_accept, csz_recv_buffer, sizeof(csz_recv_buffer), 0 );
            if(i_bytes_recv > 0)
            {
                printf( "%d:%s (%ld)\n", socket_accept, csz_recv_buffer, i_bytes_recv );
            }
            else
            {
                printf("exit %d(%d)\n", socket_accept, i_bytes_recv);
                b_exit = true;
                return 0;
            }
        }
    }
}


DWORD WINAPI thread_proc_tcp_send( LPVOID lp_para )
{
    SOCKET *p_socket = (SOCKET *)lp_para;
    SOCKET socket_client = *p_socket;

    // Send and receive data.
    while (!b_exit)
    {
        char csz_send_buffer[4096];

        memset(csz_send_buffer, 0, sizeof(csz_send_buffer));
        scanf("%s", csz_send_buffer);
        printf("%s>%d\n", csz_send_buffer, socket_client);

        int i_bytes_send = send( socket_client, csz_send_buffer, strlen(csz_send_buffer), 0 );

        printf( "Bytes Sent: %ld\n", i_bytes_send );
    }

    return 0;
}



DWORD WINAPI thread_proc_tcp_client( LPVOID lp_para )
{
    DWORD dwThreadId;

    CreateThread(NULL, 0, thread_proc_tcp_receive, lp_para, 0, &dwThreadId);
    CreateThread(NULL, 0, thread_proc_tcp_send, lp_para, 0, &dwThreadId);
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
    SOCKET socket_client;
    socket_client = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( socket_client == INVALID_SOCKET )
    {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return 1;
    }

    char csz_client_addr[4096];

    printf("please input client addr:\n");
    memset(csz_client_addr, 0, sizeof(csz_client_addr));
    //scanf("%s", csz_client_addr);
    strcpy(csz_client_addr, "127.0.0.1");

    memset(&addr_client, 0, sizeof(sockaddr_in));

    addr_client.sin_family = AF_INET;
    addr_client.sin_addr.s_addr = inet_addr( csz_client_addr );
    addr_client.sin_port = htons( 27015 );

    if ( connect( socket_client, (SOCKADDR*) &addr_client, sizeof(addr_client) ) == SOCKET_ERROR)
    {
        printf( "Failed to connect.\n" );
        WSACleanup();
        return 1;
    }
	else
	{
        printf("New connection from %s(%d) on socket %d\n",inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port), socket_client);
	}

    printf("-----------Client-----------\n");

    DWORD dwThreadId;
    CreateThread(NULL, 0, thread_proc_tcp_client, &socket_client, 0, &dwThreadId);

    while (!b_exit)
    {
        Sleep(1000);
    }

    return 0;
}


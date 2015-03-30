#include <WinSock2.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")

bool b_exit = false;

DWORD WINAPI thread_proc_udp_receive( LPVOID lp_para )
{
    SOCKET secket_receive;
    secket_receive = socket(AF_INET, SOCK_DGRAM, 0);
    if(secket_receive == INVALID_SOCKET)
    {
        printf("receive socket Error!");
        b_exit = true;
        return 1;
    }

    unsigned long ul_cmd = 1;
    int i_fionbio_status = ioctlsocket(secket_receive, FIONBIO, &ul_cmd);
    if(i_fionbio_status == SOCKET_ERROR)
    {
        printf("receive 设置非阻塞模式失败！\n");
        b_exit = true;
        return 1;
    }

    sockaddr_in addr_server;
    memset(&addr_server, 0, sizeof(sockaddr_in));
    addr_server.sin_addr.S_un.S_addr = (INADDR_ANY);
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = ntohs(5000);

    int i_bind_status = bind(secket_receive, (sockaddr*)&addr_server, sizeof(addr_server));
    if (i_bind_status != 0)
    {
        printf("receive bind failed!");
        b_exit = true;
        return 1;
    }

    // Send and receive data.
    while (!b_exit)
    {
        fd_set fd_server_recv;
        timeval select_timeout;

        FD_ZERO(&fd_server_recv);
        FD_SET(secket_receive, &fd_server_recv);

        select_timeout.tv_sec = 30;
        select_timeout.tv_usec = 0;

        printf("receive before select\n");
        int iErr = select(secket_receive + 1, &fd_server_recv, NULL, NULL, &select_timeout);
        printf("receive after select\n");
        printf("receive result:0x%x\n", iErr);
        if(iErr == 0)  //0：等待超时，没有可读写或错误的文件
        {
            continue;
        }

        if(FD_ISSET(secket_receive, &fd_server_recv))
        {
            char csz_recv_buffer[4096];
            int i_bytesRecv = SOCKET_ERROR;

            sockaddr_in addr_recv;
            int i_addr_len = sizeof(sockaddr);

            memset(&addr_recv, 0, sizeof(sockaddr_in));
            memset(csz_recv_buffer, 0, sizeof(csz_recv_buffer));

            i_bytesRecv = recvfrom(secket_receive, csz_recv_buffer, sizeof(csz_recv_buffer), 0, (sockaddr*)&addr_recv, &i_addr_len);

            printf("%s:%s\n",inet_ntoa(addr_recv.sin_addr), csz_recv_buffer);
            printf("Bytes received %d\n", i_bytesRecv);
        }
    }
}


DWORD WINAPI thread_proc_udp_send( LPVOID lp_para )
{
    // Send and receive data.

    SOCKET socket_send = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_send == INVALID_SOCKET)
    {
        printf("send socket Error!");
        b_exit = true;
        return 1;
    }

    unsigned long ul_cmd = 1;
    int i_fionbio_status = ioctlsocket(socket_send, FIONBIO, &ul_cmd);
    if(i_fionbio_status == SOCKET_ERROR)
    {
        printf("send 设置非阻塞模式失败！\n");
        b_exit = true;
        return 1;
    }

    sockaddr_in addr_send;

    memset(&addr_send, 0, sizeof(sockaddr_in));

    while (!b_exit)
    {
        int i_bytes_send;
        char csz_send_buffer[4096];

        memset(csz_send_buffer, 0, sizeof(csz_send_buffer));
        scanf("%s", csz_send_buffer);

        if(csz_send_buffer[0] == ':')
        {
            char *cp_start = csz_send_buffer + 1;

            memset(&addr_send, 0, sizeof(sockaddr_in));
            addr_send.sin_addr.S_un.S_addr = inet_addr(cp_start);
            addr_send.sin_family = AF_INET;
            addr_send.sin_port = ntohs(5001);
            printf("change to client %s\n", cp_start);
        }
        else if(addr_send.sin_family)
        {
            printf("%s>%s\n", inet_ntoa(addr_send.sin_addr), csz_send_buffer);
            i_bytes_send = sendto(socket_send, csz_send_buffer, strlen(csz_send_buffer), 0, (sockaddr*)&addr_send, sizeof(addr_send));
            printf( "Bytes Sent: %ld\n", i_bytes_send );
        }
        else
        {
            printf("select a client!\n");
        }
    }
}



DWORD WINAPI thread_proc_server( LPVOID lp_para )
{
    DWORD d_thread_id;

    CreateThread(NULL, 0, thread_proc_udp_receive, lp_para, 0, &d_thread_id);
    CreateThread(NULL, 0, thread_proc_udp_send, lp_para, 0, &d_thread_id);
    return 0;
}

int main()
{
    WSADATA wsa_data;


    int Ret = WSAStartup(MAKEWORD(2,2),&wsa_data);
    printf("......Server......\n");
    if(Ret != 0)
    {
        printf("无法初始化winsock.\n");
        WSACleanup();
        return 1;
    }
    else
    {

        printf("初始化winsock成功\n");
    }

    DWORD d_thread_id;
    CreateThread(NULL, 0, thread_proc_server, 0, 0, &d_thread_id);

    while (!b_exit)
    {
        Sleep(100);
    }

    //closesocket(secket_receive);
    WSACleanup();
    printf("exit\n");
    return 0;
}

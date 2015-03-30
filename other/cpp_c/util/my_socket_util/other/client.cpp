#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>
#include <arpa/inet.h>

#define RET_OK 0
#define RET_ERROR -1
#define BUFFER_SIZE 1024
#define ECHO_PORT 8080

int main(int argc,char **argv)
{
    int sockfd;
    int fdmax;
    int ret_select;
    int i;
    int len;
    
    char buffer[BUFFER_SIZE];

    fd_set read_fds;

    fd_set copy_fds;

    struct sockaddr_in seraddr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    int opt;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
    
    memset(&seraddr,0,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(ECHO_PORT);
    seraddr.sin_addr.s_addr = inet_addr("192.168.1.142");

    connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
        
    FD_ZERO(&read_fds);
    FD_SET(0,&read_fds);
    FD_SET(sockfd,&read_fds);
    fdmax = sockfd;

    while(1)
    {
        copy_fds = read_fds;

        ret_select = select(fdmax+1,&copy_fds,NULL,NULL,NULL);//返回活跃的字符数
        if(ret_select == 0)     
        {
            continue;
        }
        else
        {
            if(FD_ISSET(sockfd,&copy_fds)) //套接字中有信息，接收信息
            {
                memset(buffer,0,BUFFER_SIZE);
                len = recv(sockfd,buffer,sizeof(buffer),0);

                if(len == 0)//收到信息为空，即server端退出
                {
                    FD_CLR(sockfd,&copy_fds);
                    fdmax = 0;
                }
                else
                {
                    printf("Receive message:  %s/n",buffer);
                }
            }

            if(FD_ISSET(0,&copy_fds)) //从标准输入输入信息，发送到server端
            {
                fgets(buffer,sizeof(buffer),stdin);
                len=send(sockfd,buffer,strlen(buffer),0);
            }
        }
    }
    close(sockfd);
    return 0;
} 



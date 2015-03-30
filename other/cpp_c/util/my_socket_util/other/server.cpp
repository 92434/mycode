#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define ECHO_PORT 8080

int main()
{
    fd_set master;
    fd_set read_fds;
    char buffer[BUFFER_SIZE];


    int sockfd;
    int fdmax;
    int addrlen;
    int newfd;
    int nbytes;
    int ret_select;
    int i;
    int j;

    struct sockaddr_in seraddr;
    struct sockaddr_in cliaddr;

    FD_ZERO(&master);//置空套接字描述符集
    FD_ZERO(&read_fds);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    

    //解决端口冲突问题
    int opt;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(ECHO_PORT);
    seraddr.sin_addr.s_addr=INADDR_ANY;

    //绑定套接字和地址结构

    bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    

    //监听
    listen(sockfd,10);
    
    //将套接字描述符加到描述符集里

    FD_SET(sockfd,&master);
    fdmax=sockfd;

    while(1)
    {
        read_fds = master;//复制描述符集，为了保存原有描述符集
        ret_select = select(fdmax+1,&read_fds,NULL,NULL,NULL);//不活跃的描述符对应位置0
        
        for(i=0;i<=fdmax;i++)
        {
            if(FD_ISSET(i,&read_fds))//经过select()后的read_fds中对应位为1则FD_ISSET()返回1
            {
                if(i==sockfd)//判断是否为server中的第一个套接字
                {
                    addrlen=sizeof(cliaddr);
                    
                    newfd=accept(sockfd,(struct sockaddr *)&cliaddr,&addrlen);//创建新的套接字
                    FD_SET(newfd,&master);//将新的套接字写入描述符集中
                    fdmax=newfd;
                    printf("new connection from %s on socket %d/n",inet_ntoa(cliaddr.sin_addr),newfd);
                }
                else
                {
                    memset(buffer,0,BUFFER_SIZE);//置空buffer
                    nbytes = recv(i,buffer,sizeof(buffer),0);//接收信息存于buffer中
                    printf("from client: %s/n",buffer);
                    
                    if(nbytes==0)   //没收到信息，即client端退出
                    {
                        FD_CLR(i,&master); //将该client对应的套接字从描述符集中清除
                    }

                    for(j=0;j<=fdmax;j++) //循环群发收到的信息
                    {
                        if(FD_ISSET(j,&master))
                        {
                            if(j!=sockfd && j!=i)//群发收到的信息但排除本身和来源端
                            send(j,buffer,strlen(buffer),0);    
                        }   
                    }   
                }
            }
        }
    }
}



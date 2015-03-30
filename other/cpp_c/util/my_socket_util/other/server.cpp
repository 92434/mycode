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

    FD_ZERO(&master);//�ÿ��׽�����������
    FD_ZERO(&read_fds);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    

    //����˿ڳ�ͻ����
    int opt;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(ECHO_PORT);
    seraddr.sin_addr.s_addr=INADDR_ANY;

    //���׽��ֺ͵�ַ�ṹ

    bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    

    //����
    listen(sockfd,10);
    
    //���׽����������ӵ�����������

    FD_SET(sockfd,&master);
    fdmax=sockfd;

    while(1)
    {
        read_fds = master;//��������������Ϊ�˱���ԭ����������
        ret_select = select(fdmax+1,&read_fds,NULL,NULL,NULL);//����Ծ����������Ӧλ��0
        
        for(i=0;i<=fdmax;i++)
        {
            if(FD_ISSET(i,&read_fds))//����select()���read_fds�ж�ӦλΪ1��FD_ISSET()����1
            {
                if(i==sockfd)//�ж��Ƿ�Ϊserver�еĵ�һ���׽���
                {
                    addrlen=sizeof(cliaddr);
                    
                    newfd=accept(sockfd,(struct sockaddr *)&cliaddr,&addrlen);//�����µ��׽���
                    FD_SET(newfd,&master);//���µ��׽���д������������
                    fdmax=newfd;
                    printf("new connection from %s on socket %d/n",inet_ntoa(cliaddr.sin_addr),newfd);
                }
                else
                {
                    memset(buffer,0,BUFFER_SIZE);//�ÿ�buffer
                    nbytes = recv(i,buffer,sizeof(buffer),0);//������Ϣ����buffer��
                    printf("from client: %s/n",buffer);
                    
                    if(nbytes==0)   //û�յ���Ϣ����client���˳�
                    {
                        FD_CLR(i,&master); //����client��Ӧ���׽��ִ��������������
                    }

                    for(j=0;j<=fdmax;j++) //ѭ��Ⱥ���յ�����Ϣ
                    {
                        if(FD_ISSET(j,&master))
                        {
                            if(j!=sockfd && j!=i)//Ⱥ���յ�����Ϣ���ų��������Դ��
                            send(j,buffer,strlen(buffer),0);    
                        }   
                    }   
                }
            }
        }
    }
}



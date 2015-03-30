#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <string>
#include <iostream>
using namespace std;

 

//#define HOSTSTR "www.webxml.com.cn"
//#define PHPSTR "/webservices/qqOnlineWebService.asmx/qqCheckOnline"
//#define REQUESTSTR "qqCode=474497857"
//#define HOSTSTR "w3school.com.cn"
//#define PHPSTR "/tiy/t.asp"
//#define REQUESTSTR "f=ajax_get"
//#define HOSTSTR "192.168.1.37"
//#define PHPSTR "/web/getservices"
//#define REQUESTSTR "sRef=1:7:1:0:0:0:0:0:0:0:(type%20==%201)%20||%20(type%20==%2017)%20||%20(type%20==%20195)%20||%20(type%20==%2025)%20ORDER%20BY%20name"

#define PORT 8001

#define BUFSIZE 1024


void get_http_request_content(string str_url, string &str_host, string &str_php, string &str_request){
	int i_start_pos = str_url.find("?",0);
	if(i_start_pos != string::npos){
		str_request = str_url.substr(i_start_pos + 1, string::npos);
		str_url = str_url.substr(0, i_start_pos);
	}

	i_start_pos = str_url.find("http://", 0);
	if(i_start_pos != string::npos){
		i_start_pos += string("http://").length();
		str_url = str_url.substr(i_start_pos, string::npos);
	}
	
	i_start_pos = str_url.find("/", 0);
	
	str_host = str_url.substr(0, i_start_pos);

	if(i_start_pos != string::npos){
		str_php = str_url.substr(i_start_pos, string::npos);
	}
	
	
}
 

int main(int argc, char **argv)

{
	
        int sockfd, ret, i, h;

        struct sockaddr_in servaddr;

        char cpHttpReq[4096], cpContentReq[4096], buf[BUFSIZE], *cpContentLen;

        socklen_t tContentLen;

        fd_set   t_set1;

        struct timeval  tv;

	struct hostent *host;

	string str_host, str_php, str_request;

	if(argc >= 2){
		get_http_request_content(argv[1], str_host, str_php, str_request);
	}else{
		fprintf(stderr,"没有参数\n");
		exit(1);
	}

	if((host=gethostbyname("192.168.1.37"))==NULL)
	{
		fprintf(stderr,"不能得到IP\n");
		exit(1);
	}
	printf("HostName :%s\n",host->h_name);
	printf("IP Address :%s\n",inet_ntoa(*((struct in_addr *)host->h_addr)));

 

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {

                printf("创建网络连接失败,本线程即将终止---socket error!\n");

                exit(0);

        };

 

        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;

        servaddr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, inet_ntoa(*((struct in_addr *)host->h_addr)), &servaddr.sin_addr) <= 0 ){

                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");

                exit(0);

        };

 

        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){

                printf("连接到服务器失败,connect error!\n");

                exit(0);

        }

        printf("与远端建立了连接\n");

	int flags = fcntl(sockfd, F_GETFL, 0);

	//用以下方法将socket设置为非阻塞方式
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	 
	//将非阻塞的设置回阻塞可以用
	//fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK);


 

        //发送数据

        memset(cpContentReq, 0, 4096);

        strcat(cpContentReq, str_request.c_str());


 

        memset(cpHttpReq, 0, 4096);

        //strcat(cpHttpReq, "GET ");
        strcat(cpHttpReq, "POST ");
        strcat(cpHttpReq, str_php.c_str());
        //strcat(cpHttpReq, " HTTP/1.1\n");
        strcat(cpHttpReq, " HTTP/1.0\r\n");

        strcat(cpHttpReq, "User-Agent: Wget/1.12 (linux-gnu)\r\n");
        strcat(cpHttpReq, "Accept: */*\r\n");

        strcat(cpHttpReq, "Host: ");
        strcat(cpHttpReq, str_host.c_str());
        strcat(cpHttpReq, "\r\n");


        //strcat(cpHttpReq, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:20.0) Gecko/20100101 Firefox/20.0\r\n");

        //strcat(cpHttpReq, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");

        //strcat(cpHttpReq, "Accept-Language: en-US,en;q=0.5\n");

        //strcat(cpHttpReq, "Accept-Encoding: gzip, deflate\n");

        strcat(cpHttpReq, "Connection: keep-alive\n\r\n");
        //strcat(cpHttpReq, "HTTP/1.0 200 OK\r\n");

        //strcat(cpHttpReq, "Cache-Control: max-age=0\r\n");

        //strcat(cpHttpReq, "X-Requested-With: XMLHttpRequest\r\n");

        //strcat(cpHttpReq, "X-Prototype-Version: 1.6.1\r\n");

	if(tContentLen = strlen(cpContentReq)){
		cpContentLen=(char *)malloc(128);

		sprintf(cpContentLen, "%d", tContentLen);

		strcat(cpHttpReq, "Content-Type: application/x-www-form-urlencoded\r\n");

		//strcat(cpHttpReq, "Referer: http://");
		//strcat(cpHttpReq, str_host.c_str());
		//strcat(cpHttpReq, "/\n");
	
		strcat(cpHttpReq, "Content-Length: ");

		strcat(cpHttpReq, cpContentLen);

		strcat(cpHttpReq, "\r\n\r\n");

		strcat(cpHttpReq, cpContentReq);

		free(cpContentLen);
		
	}



        printf("%s\n",cpHttpReq);


        ret = write(sockfd, cpHttpReq, strlen(cpHttpReq));

        if (ret < 0) {

                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));

                exit(0);

        }else{

                printf("消息发送成功，共发送了%d个字节！\n\n", ret);

        }

 
 

        while(1){

                sleep(2);

		FD_ZERO(&t_set1);

		FD_SET(sockfd, &t_set1);


                //tv.tv_sec= 0;
                tv.tv_sec= 0;

                tv.tv_usec= 500;

                h= 0;

                printf("--------------->1\n");

                h= select(sockfd +1, &t_set1, NULL, NULL, &tv);

                printf("--------------->2\n");

 

                //if (h == 0) continue;

                if (h < 0) {

                        close(sockfd);

                        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");

                        return -1;

                };

 

                if (h > 0){

                        memset(buf, 0, 4096);

                        i= read(sockfd, buf, 4095);

                        if (i==0){

                                close(sockfd);

                                printf("读取数据报文时发现远端关闭，该线程终止！\n");

                                return -1;

                        }

 

                        //wprintf(L"%s\n", buf);
                        printf("%s\n", buf);

                }

        }

        close(sockfd);

 

 

        return 0;

}

#ifndef _COMMON_FUC_H_
#define  _COMMON_FUC_H_
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>   
#include <sys/wait.h>  
#include <signal.h>   
#include <syslog.h>  
#include <sys/epoll.h>
#include <ctype.h>
#include <sys/syscall.h>
#include <sys/stat.h>
//#include <resource.h>
#if 1
#define TRACE printf
#else
#define TRACE(a, ...) do{ } while(0) 
#endif
#define SEED_LENGTH 16
#define MAX_CLI_LIMIT 		2000
#define MAXDATASIZE 		4096
#define ASSERTNOERR(x, msg) do{\
        if ((x) == -1){perror(msg); exit(1);}}while(0)
			
void dump_packet(char *string,unsigned char *data, int len);
void dump_packet_line(char *string,unsigned char *data, int len);
void dump_packet_oneline(char *string,unsigned char *data, int len);
unsigned long long get_random ();
int getpeerport(int fd);
const char * tcp_getpeername(int fd);
int getpeeraddr(int fd,struct sockaddr_in  *addr_output);
int init_tcplisten(char *ip4, int port, int max_link);
const char * getpeernameByAddr(struct sockaddr *saddr);
void maximize_recvbuf(const int sfd);
int get_bin_pathname( char *pathname );
int IsExistProgram(unsigned short degree,unsigned short freq,unsigned short sid);
int strToIntVal_c(char *value, int defaultvalue);
int x_or_cw(unsigned char *data,int l);
void cs_sleepms(unsigned int msec);
//int SetRLimit();
int setnonblocking(int sock);
int check_domain(const char * addr);
int create_socket(const char * addr, uint16_t port,const char * localaddr,uint16_t localport,int pro_type);
char *trim(char*p_str);
char *alltrim(char *str);
void reconf(int sck);
int Read(int fd,void *buffer,int length) ;
int Write(int fd,void *buffer,int length) ;
int CreateThread(void *(*start_routine)(void *), void *arg, pthread_t *thread, pthread_attr_t *pAttr);
unsigned short cmd_xor(unsigned char *srcData,int len);
int cmd_check(unsigned char *srcData,int len);
int load_addr_seg(void);
void showfpos(fpos_t * x);
#endif
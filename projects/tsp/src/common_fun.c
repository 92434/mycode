#include <linux/tcp.h>

#include "common_fun.h"
unsigned long int c_tab[256] = { /* CRC polynomial 0xedb88320 */
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


int x_or_cw(unsigned char *data,int l)
{
	int i;
	if(l<0)
		return -1;
	for(i=0;i<l;i++)
	{
		*(data+i) ^= (unsigned char)((c_tab[i%256]>>(2*(i%4)))&0xFF);
	}

	return 1;
}
void dump_packet(char *string,unsigned char *data, int len)
{
    int i;
    TRACE("----------%s------%d------------\n",string,len);
    for(i=0; i<len; i++) {
        TRACE("%02x ", (unsigned char)(data[i] & 0xff));
        if((i+1)%16==0)  {
            TRACE("\n");
        }
    }
    TRACE("\n");
}
void dump_packet_line(char *string,unsigned char *data, int len)
{
    int i;
    TRACE("--%s(len=%d):",string,len);
    for(i=0; i<len; i++) {
        TRACE("%02x ", (unsigned char)(data[i] & 0xff));
        //if ((0x0f&i) == 0x0f) {
            //TRACE("\n");
        //}
    }
    TRACE("\n");
}

void dump_packet_oneline(char *string,unsigned char *data, int len)
{
    int i;
    TRACE("--%s(len=%d):",string,len);
    for(i=0; i<len; i++) {
        TRACE("%02x ", (unsigned char)(data[i] & 0xff));
        //if ((0x0f&i) == 0x0f) {
            //TRACE("\n");
        //}
    }
}

unsigned long long get_random ()    //用于生成随机数种子
{
	int fd;
	unsigned long long r;
	fd = open ("/dev/urandom", O_RDONLY);  //用urandom的方法输出数据数

	if (fd > 0)
    {
		while(1)
		{
		  read(fd, &r, sizeof (r));
		  if(r>0)
		  	break;
		  //printf("r:%0llx\n",r);
		}
		close(fd);
    }
	return r;
}

int getpeerport(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int n;

    if(fd<0) return 0;
   n = getpeername(fd,(struct sockaddr *)&addr,&len);
     //n = getsockname(fd,(struct sockaddr *)&addr,&len);
    if(n<0) return 0;
    else return addr.sin_port;
}
const char * tcp_getpeername(int fd)
{
        static  char tmp[128];
        char * p = &tmp[0];
        char tt[15];
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int n;
        memset(&addr,0,len);
        sprintf(p,"NA");
        if(fd<0) return p;
        n = getpeername(fd,(struct sockaddr *)&addr,&len);
        if(n<0) return p;
        if(inet_ntop(addr.sin_family,&addr.sin_addr,p,128)==NULL){
                sprintf(p,"NA");
                return p;
        }
        strcat(p,":");
        snprintf(tt,15,"%d",ntohs(addr.sin_port));
        strcat(p,tt);
        return p;
}

const char * getpeernameByAddr(struct sockaddr *saddr)
{
        static  char tmp[128];
        char * p = &tmp[0];
        char tt[15];
        struct sockaddr_in addr;;
        memcpy(&addr,(struct sockaddr_in *)saddr,sizeof(struct sockaddr_in));
        if(inet_ntop(addr.sin_family,&addr.sin_addr,p,128)==NULL){
                sprintf(p,"NA");
                return p;
        }
        strcat(p,":");
        snprintf(tt,15,"%d",ntohs(addr.sin_port));
        strcat(p,tt);
        return p;
}


int getpeeraddr(int fd,struct sockaddr_in  *addr_output)
{

	int n;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	memset(&addr,0,len);
	if(fd==-1) return -1;
	n = getpeername(fd,(struct sockaddr *)&addr,&len);
	//printf("getpeername ret:%d,addr:%x\n",n,(int)addr);
	if(n<0) return -1;

	memcpy(addr_output,&addr,sizeof(addr));
	return 0;
}

char * tcp_getip(int fd)
{
        static  char tmp[128];
        char * p = &tmp[0];
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int n;
        memset(&addr,0,len);
        if(fd<0) return NULL;
        n = getpeername(fd,(struct sockaddr *)&addr,&len);
        if(n<0) return NULL;
        if(inet_ntop(addr.sin_family,&addr.sin_addr,p,128)==NULL){
                return NULL;
        }
        return p;
}


int init_tcplisten(char *ip4, int port, int max_link)
{
    //临时变量
    int            sock_listen4;
    struct sockaddr_in    addr4;
    unsigned int        optval;
    struct linger        optval1;

    //初始化数据结构
    bzero(&addr4, sizeof(addr4));
	if(ip4==NULL)
		addr4.sin_addr.s_addr = INADDR_ANY;
	else
   		inet_pton(AF_INET, ip4, &(addr4.sin_addr));
    //addr4.sin_addr.s_addr = INADDR_ANY;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(port);

    //创建SOCKET
    sock_listen4 = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sock_listen4) return(-1);

    //设置SO_REUSEADDR选项(服务器快速重起)
    optval = 0x1;
    setsockopt(sock_listen4, SOL_SOCKET, SO_REUSEADDR, &optval, 4);

    //设置SO_LINGER选项(防范CLOSE_WAIT挂住所有套接字)
    optval1.l_onoff = 1;
    optval1.l_linger = 5;
    setsockopt(sock_listen4, SOL_SOCKET, SO_LINGER, &optval1, sizeof(struct linger));
	int keepalive = 1; // 开启keepalive属性
	setsockopt(sock_listen4, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    if (0 > bind(sock_listen4, (struct sockaddr *)&addr4, sizeof(addr4))) {
        close(sock_listen4);
        return(-1);
    }

    if (0 > listen(sock_listen4, max_link)) {
        close(sock_listen4);
        return(-1);
    }

    return(sock_listen4);
}

/*
 * Sets a socket's send buffer size to the maximum allowed by the system.
 */
void maximize_recvbuf(const int sfd) {
    socklen_t intsize = sizeof(int);
    int min, max, avg;
    int old_size;
	#define MAXRECVBUF_SIZE (256 * 1024 * 1024)

    /* Start with the default size. */
    if (getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, &old_size, &intsize) != 0) {
        return;
    }

    /* Binary-search for the real maximum. */
    min = old_size;
    max = MAXRECVBUF_SIZE;

    while (min <= max) {
        avg = ((unsigned int)(min + max)) / 2;
        if (setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, (void *)&avg, intsize) == 0) {
            min = avg + 1;
        } else {
            max = avg - 1;
        }
    }
}


void cs_sleepms(unsigned int msec)
{
	//does not interfere with signals like sleep and usleep do
	struct timespec req_ts;
	req_ts.tv_sec = msec/1000;
	req_ts.tv_nsec = (msec % 1000) * 1000000L;
	int32_t olderrno = errno;		// Some OS (especially MacOSX) seem to set errno to ETIMEDOUT when sleeping
	nanosleep (&req_ts, NULL);
	errno = olderrno;
}
/*
int SetRLimit()
{
	struct rlimit rlim;
	rlim.rlim_cur = MAX_CLI_LIMIT;
	rlim.rlim_max = MAX_CLI_LIMIT;
	if (setrlimit(RLIMIT_NOFILE, &rlim) != 0)
	{
		perror("setrlimit");
	}
	else
	{
		printf("setrlimit ok\n");
	}
	return 0;
}
*/
int get_bin_pathname( char *pathname )
{
    FILE *fp = NULL;
    char _filename[1024] = {0};

    int _pid = getpid();

    sprintf(_filename, "dirname `ls -n /proc/%d/exe | awk '{if ($8==\"->\") {print $19} else {if ($9==\"->\") {print $10} else {print $11}}}'`", _pid);
    fp = popen(_filename, "r");
    fgets(_filename, sizeof(_filename), fp);
    fclose(fp);

    _filename[strlen(_filename)-1] = '\0';
    strcpy( pathname, _filename );

    return 0;
}




int strToIntVal_c(char *value, int defaultvalue){
	int i;
	if (strlen(value) == 0) return defaultvalue;
	if(strncasecmp(value,"0x",2))
		i = strtol(value, NULL, 10);
	else
		i = strtol(value, NULL, 16);
	return i;

}

int setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
	{
		return -1;
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		return -1;
	}
	return 0;
}

int check_domain(const char * addr){
	int len=0,i;
	if(addr==NULL)
		return -1;
	len=strlen(addr);
	if(addr[len-1]=='\0')
		len--;
	if(len>15)
		return 1;
	for(i=0;i<len;i++){
		if(addr[i]=='.')
			continue;
		if(addr[i]>0x39)
			return 1;
	}
	return 0;
}

//pro_type 0:tcp 1:udp
int create_socket(const char * addr, uint16_t port,const char * localaddr,uint16_t localport,int pro_type)
{
    struct sockaddr_in saddr,slocaladdr;
    int r;
    int fd;
    int len = sizeof(saddr);
    int on = 1;
    memset(&saddr,0,sizeof(saddr));
    memset(&slocaladdr,0,sizeof(slocaladdr));
	if(pro_type==0)
		fd= socket(AF_INET,SOCK_STREAM,0);
	else
		fd= socket(AF_INET,SOCK_DGRAM,0);

	if (fd < 0) {
		TRACE("create_socket: fd socket failed:%s",strerror(errno));
		fd=-1;
		return -1;
	}
	r = setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(r<0){
            TRACE("create_socket:setsockopt failed:%s",strerror(errno));
			close(fd);
            return -1;
    }
    if(NULL!=localaddr||0!=localport){
                slocaladdr.sin_family = AF_INET;
                slocaladdr.sin_port = htons(localport);
		if(localaddr!=NULL){
			inet_pton(AF_INET,localaddr ,&slocaladdr.sin_addr);
			if(r<0){
				TRACE("create_socket: inet_pton(%s:%d) return %d,error:%s",localaddr,localport,r,strerror(errno));
				close(fd);
				return -1;
			}
		} else {
			slocaladdr.sin_addr.s_addr = INADDR_ANY;
		}
		r = bind(fd,(struct sockaddr*)&slocaladdr,sizeof(slocaladdr));
		if (r < 0) {
			TRACE("create_socket: bind failed:%s",strerror(errno));
			close(fd);
			return -1;
		}
	}
	if(NULL!=addr)
	{
		int isDomain=0;
		struct hostent  *host = NULL;
		saddr.sin_family = AF_INET;
		isDomain=check_domain(addr);
		if(isDomain==0){
			saddr.sin_addr.s_addr = inet_addr(addr);
		}
		else if(isDomain==1){
			host = gethostbyname(addr);
			if(host){
				char ipaddrs[32];
				snprintf(ipaddrs,32, "%s", inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
				saddr.sin_addr.s_addr = inet_addr(ipaddrs);
		     }
			else {
				return -1;
			}
		}
		saddr.sin_port=htons(port);
		if(saddr.sin_addr.s_addr<=0){
			TRACE("[FEEDIKS]create_socket: inet_pton(%s:%d),error:%s",addr,port,strerror(errno));
			return -2;
		}
		r = connect(fd,(struct sockaddr*)&saddr,len);
		if (r<0) {
			TRACE("create_socket:fd connect failed:%s",strerror(errno));
			close(fd);
			return -3;
		}
	}
	return fd;
}

/****************************************************************
 * remove blank char before str *
 ****************************************************************/
char *ltrim(char*p_str)
{
    char *p_tmp=p_str;
    while(isspace(*p_tmp)!=0)
    {
        p_tmp++;
    }
    return p_tmp;
}
/****************************************************************
 * remove blank char after str *
 ****************************************************************/
void rtrim(char*p_str)
{
    char *p_tmp=p_str+strlen(p_str)-1;
    while(isspace(*p_tmp)!=0)
    {
        *p_tmp='\0';
        p_tmp--;
    }
}
/****************************************************************
 * remove blank char after and before str *
 ****************************************************************/
char *trim(char*p_str)
{
    rtrim(p_str);
    return ltrim(p_str);
}

char *alltrim(char *str)
{
	char *p=str;
	while(*p!=0)	//没有结束，循环
	{
		if(*p<=32)//遇到空格字符处理
		{
			char *q=p;	//从空格处开始
			while(*q!=0) //直到末尾的所有字符
			{
				*q=*(q+1); //逐次前移
				q++;	  //每移一个字符，指针加1，准备移下一个字符
			}
		}
		else		//当前字符不是空格
		{
			p++;  //指针后移，指向待检查的新字符
		}
	}
	return str;
}


// a example of using the udpserver.
// it is a conference room.
//

void reconf(int sck)
{
	int r = 0; long o = 1; socklen_t ol = sizeof(long);
	r = setsockopt(sck, IPPROTO_TCP, TCP_NODELAY, (char*)&o, ol);
	if (r == -1)
		printf("setsockopt(nodelay)");
	//else
		//printf("socket shorten delay ok.\n");
	//end if
}//end reconf

int Read(int fd,void *buffer,int length)
{
	int n = recv(fd, buffer, length, MSG_NOSIGNAL);
	if(n<=0){
		if(errno==EINTR) /* 中断错误 我们继续写*/
	    	n = recv(fd, buffer, length, MSG_NOSIGNAL);
		else             /* 其他错误 没有办法,只好撤退了*/
	    	return(-1);
		}

	return n;
}


int Write(int fd,void *buffer,int length)
{
	if(fd<0)
		return -1;
	int n = send(fd, buffer, length, MSG_NOSIGNAL);
	if(n<=0){
		if(errno==EINTR) /* 中断错误 我们继续写*/
	    	n = send(fd, buffer, length, MSG_NOSIGNAL);
		else             /* 其他错误 没有办法,只好撤退了*/
	    	return(-1);
		}

	return n;
}

int CreateThread(void *(*start_routine)(void *), void *arg, pthread_t *thread, pthread_attr_t *pAttr)
{
	pthread_attr_t thr_attr;
	if(pAttr == NULL)
	{
		pAttr = &thr_attr;
		pthread_attr_init(pAttr);
		pthread_attr_setstacksize(pAttr, 1024 * 1024); // 1 M的堆栈
		pthread_attr_setdetachstate(pAttr, PTHREAD_CREATE_DETACHED);
	}
	pthread_t tid;
	if(thread == NULL)
	{
		thread = &tid;
	}

	int r = pthread_create(thread, pAttr, start_routine, arg);
	pthread_attr_destroy(pAttr);
	return r;
}


typedef struct{
	char start[17];
	char end[17];
}_ip_seg;
typedef struct{
	unsigned int start;
	unsigned int end;
}_in_addr_seg;

#define SEG_COUNT 41
static _ip_seg chi_ip_seg[SEG_COUNT]={
	//移动
	{"117.136.12.0","117.136.255.255"},
	{"120.196.0.0","120.196.255.255"},
	{"203.81.16.0","203.81.31.255"},
	{"211.136.192.0","211.136.253.179"},
	{"211.139.128.0","211.139.255.255"},
	{"218.204.160.0","218.204.167.255"},
	{"221.179.0.0","221.179.127.255"},
	//联通
	{"27.40.0.0","27.47.255.255"},
	{"27.36.0.0","27.39.255.255"},
	{"112.88.0.0","112.97.255.255"},
	{"120.80.0.0","120.87.255.255"},
	{"122.13.0.0","122.13.255.255"},
	{"163.0.0.0","163.204.255.255"},
	{"210.12.0.0","210.21.208.134"},
	{"218.104.160.0","218.104.199.25"},
	{"221.4.0.0","221.5.127.255"},
	//电信
	{"1.0.32.0","1.4.15.255"},
	{"101.99.127.255","101.101.104.0"},
	{"103.22.40.0","103.22.43.255"},
	{"106.0.16.0","106.127.255.255"},
	{"110.93.63.255","110.173.64.0"},
	{"113.64.0.0","113.212.191.255"},
	{"114.79.127.255","114.141.64.0"},
	{"115.166.64.0","116.50.15.255"},
	{"117.74.95.255","117.103.40.0"},
	{"118.102.32.0","118.127.159.255"},
	{"119.82.223.255","119.120.0.0"},
	{"120.143.128.0","124.40.223.255"},
	{"125.88.0.0","125.95.255.255"},
	{"14.0.0.0","14.223.255.255"},
	{"180.148.152.0","183.91.143.255"},
	{"202.96.191.255","202.103.128.0"},
	{"203.0.104.0","203.95.255.255"},
	{"218.14.0.0","219.143.255.255"},
	{"27.54.79.255","27.121.120.0"},
	{"36.0.16.0","36.37.63.255"},
	{"39.0.15.255","39.0.128.0"},
	{"42.0.16.0","42.99.111.255"},
	{"58.60.0.0","58.65.239.255"},
	{"59.32.0.0","59.42.255.255"},
	{"61.4.88.0","61.147.255.255"},
};

static _in_addr_seg in_addr_seg[SEG_COUNT];

int load_addr_seg(void){
	int i;
	unsigned int seg0,seg1,seg2,seg3;
	memset(in_addr_seg,0,sizeof(_in_addr_seg)*SEG_COUNT);
	for(i=0;i<SEG_COUNT;i++){
		sscanf(chi_ip_seg[i].start, "%d.%d.%d.%d", &seg0, &seg1, &seg2, &seg3);
		in_addr_seg[i].start=(seg0<<24)+(seg1<<16)+(seg2<<8)+(seg3);
		sscanf(chi_ip_seg[i].end, "%d.%d.%d.%d", &seg0, &seg1, &seg2, &seg3);
		in_addr_seg[i].end=(seg0<<24)+(seg1<<16)+(seg2<<8)+(seg3);
		//TRACE("[%d]start:0x%0x,end:0x%0x\n",i,in_addr_seg[i].start,in_addr_seg[i].end);
	}
	return 0;
}
int IsInChi_ip(char *ip){
	int i;
	unsigned int ip_int=0;
	unsigned int seg0,seg1,seg2,seg3;
	if(ip==NULL||strlen(ip)>17)
		return -1;
	sscanf(ip, "%d.%d.%d.%d", &seg0, &seg1, &seg2, &seg3);
	ip_int=(seg0<<24)+(seg1<<16)+(seg2<<8)+(seg3);
	for(i=0;i<SEG_COUNT;i++){
		if(ip_int>=in_addr_seg[i].start&&ip_int<=in_addr_seg[i].end)
			return 1;
	}
	return 0;
}

void showfpos(fpos_t * x)
{
	if (sizeof(fpos_t) == sizeof(char)) {
		unsigned char y;
		memmove(&y, x, sizeof(char));
		printf("fpos_t %u:", y);
		return;
	}
	if (sizeof(fpos_t) == sizeof(int)) {
		unsigned int y;
		memmove(&y, x, sizeof(int));
		printf("fpos_t %u:", y);
		return;
	}
	if (sizeof(fpos_t) == sizeof(long)) {
		unsigned long y;
		memmove(&y, x, sizeof(long));
		printf("fpos_t %lu:", y);
		return;
	}
	if (sizeof(fpos_t) == sizeof(long long)) {
		unsigned long long y;
		memmove(&y, x, sizeof(long long));
		printf("fpos_t %llu:", y);
		return;
	}
	printf("fpos_t ?:");
}



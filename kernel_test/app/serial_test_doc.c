//串口简介
//串行口是计算机一种常用的接口，具有连接线少，通讯简单，得到广泛的使用。常用的串口是 RS-232-C 接口（又称 EIA RS-232-C）它是在 1970 年由美国电子工业协会（EIA）联合贝尔系统、 调制解调器厂家及计算机终端生产厂家共同制定的用于串行通讯的标准。它的全名是"数据终端设备（DTE）和数据通讯设备（DCE）之间串行二进制数据交换 接口技术标准"该标准规定采用一个 25 个脚的 DB25 连接器，对连接器的每个引脚的信号内容加以规定，还对各种信号的电平加以规定。传输距离在码元畸变小于 4% 的情况下，传输电缆长度应为 50 英尺。
//Linux 操作系统从一开始就对串行口提供了很好的支持
//计算机串口的引脚说明
//序号 信号名称 符号 流向   功能
//2 发送数据 TXD DTE→DCE DTE发送串行数据
//3 接收数据 RXD DTE←DCE DTE 接收串行数据
//4 请求发送 RTS DTE→DCE DTE 请求 DCE 将线路切换到发送方式
//5 允许发送 CTS DTE←DCE DCE 告诉 DTE 线路已接通可以发送数据
//6 数据设备准备好 DSR DTE←DCE DCE 准备好
//7 信号地 　 　 　　　   信号公共地
//8 载波检测 DCD DTE←DCE 表示 DCE 接收到远程载波
//20 数据终端准备好 DTR DTE→DCE DTE 准备好
//22 振铃指示 RI DTE←DCE 表示 DCE 与线路接通，出现振铃
//
//串口操作
//串口操作需要的头文件
#include     <stdio.h>     
#include     <stdlib.h>    
#include     <unistd.h>    
#include     <sys/types.h>
#include     <sys/stat.h>  
#include     <fcntl.h>     
#include     <termios.h>   
#include     <errno.h>     

//打开串口
//在 Linux 下串口文件是位于 /dev 下的
//串口一 为 /dev/ttyS0
//串口二 为 /dev/ttyS1
//打开串口是通过使用标准的文件打开函数操作：
int fd;

fd = open( "/dev/ttyS0", O_RDWR);
if (-1 == fd){

	perror(" 提示错误！");
}

//设置串口
//最基本的设置串口包括波特率设置，效验位和停止位设置。
//串口的设置主要是设置 struct termios 结构体的各成员值。
struct termio
{   unsigned short c_iflag;
	unsigned short c_oflag;    
	unsigned short c_cflag;       
	unsigned short c_lflag;       
	unsigned char    c_line;          
	unsigned char    c_cc[NCC];   
};

//设置这个结构体很复杂，我这里就只说说常见的一些设置：
//波特率设置
//下面是修改波特率的代码：
struct termios Opt;
tcgetattr(fd, &Opt);
cfsetispeed(&Opt,B19200);
cfsetospeed(&Opt,B19200);  
tcsetattr(fd,TCANOW,&Opt);

//设置波特率的例子函数：

int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
	B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300, 38400,
	19200, 9600, 4800, 2400, 1200, 300, };
void set_speed(int fd, int speed){
	int   i;
	int   status;
	struct termios   Opt;
	tcgetattr(fd, &Opt);
	for ( i= 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {

			tcflush(fd, TCIOFLUSH);//设置前flush    
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);  
			//通过tcsetattr函数把新的属性设置到串口上。
			//tcsetattr(串口描述符，立即使用或者其他标示，指向termios的指针)
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0)
			{       
				perror("tcsetattr fd1");
				return;    
			}   
			tcflush(fd,TCIOFLUSH); //设置后flush
		}
	}
}

//效验位和停止位的设置：
//无效验 8位
Option.c_cflag &= ~PARENB;
Option.c_cflag &= ~CSTOPB;
Option.c_cflag &= ~CSIZE;
Option.c_cflag |= ~CS8;
//奇效验(Odd) 7位
Option.c_cflag |= ~PARENB;
Option.c_cflag &= ~PARODD;
Option.c_cflag &= ~CSTOPB;
Option.c_cflag &= ~CSIZE;
Option.c_cflag |= ~CS7;
//偶效验(Even) 7位
Option.c_cflag &= ~PARENB;
Option.c_cflag |= ~PARODD;
Option.c_cflag &= ~CSTOPB;
Option.c_cflag &= ~CSIZE;
Option.c_cflag |= ~CS7;
//Space效验 7位
Option.c_cflag &= ~PARENB;
Option.c_cflag &= ~CSTOPB;
Option.c_cflag &= &~CSIZE;
Option.c_cflag |= CS8;
//设置效验的函数：

int set_Parity(int fd, int databits, int stopbits, int parity)
{
	struct termios options;
	if (tcgetattr(fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return (FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits)

	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);
	}
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;
			options.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;
			break;
		case 'S':
		case 's':
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
	}

	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported stop bits\n");
			return (FALSE);
	}

	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 150;
	options.c_cc[VMIN] = 0;
	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}

//需要注意的是:
//如果不是开发终端之类的，只是串口传输数据，而不需要串口来处理，那么使用原始模式(Raw Mode)方式来通讯，设置方式如下：
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
options.c_oflag &= ~OPOST;  

//读写串口
//设置好串口之后，读写串口就很容易了，把串口当作文件读写就是。
//发送数据
char buffer[1024];
int    Length;
int    nByte;
nByte = write(fd, buffer ,Length)

	//读取串口数据
	//使用文件操作read函数读取，如果设置为原始模式(Raw Mode)传输数据，那么read函数返回的字符数是实际串口收到的字符数。
	//可以使用操作文件的函数来实现异步读取，如fcntl，或者select等来操作。
	char buff[1024];
	int    Len;
	int readByte = read(fd,buff,Len);

	//关闭串口
	//关闭串口就是关闭文件。
	close(fd);

	//例子
	//下面是一个简单的读取串口数据的例子，使用了上面定义的一些函数和头文件

#define FALSE -1
#define TRUE   0

int OpenDev(char *Dev)
{
	//Dev 就是设备，设备就是文件，就是给出该设备文件的路径
	int fd = open(Dev, O_RDWR ); //| O_NOCTTY | O_NDELAY
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		return fd;
}
int main(int argc, char **argv)
{
	int fd;
	int nread;
	char buff[512];
	char *dev = "/dev/ttyS1"; //串口二
	fd = OpenDev(dev);
	set_speed(fd, 19200);
	if (set_Parity(fd, 8, 1, 'N') == FALSE)
	{
		printf("Set Parity Error\n");
		exit (0);
	}
	while (1) //循环读取数据
	{
		while ((nread = read(fd, buff, 512))>0)
		{
			printf("\nLen %d\n", nread);
			buff[nread+1] = '\0';
			printf("\n%s", buff);
		}
	}
	//close(fd);
	// exit (0);
}
//1、虚拟机下使用串口的方法
//      使用vmwave，默认串口设备是没有添加的，通过vmwave将设备加入即可正常使用串口。虚拟机串口打开后，可能会占用windows下的串口。另外，虚拟机的串口收发比正常的速度的确要慢许多。
//2、消除Linux串口收发的一些规则
//Linux 串口收发有许多模式，如：
//（1） 接收返回模式： 如果串口没有接收到数据，read()函数不返回。
//（2） 数据接收\n才返回接收的数据，否则read()函数返回0
//（3） 特殊字符解析问题，部分特殊字符接收/发送时，会被屏蔽或者转义。如发送0x0A 接收变为0x0A 0x0A ，0x0D被屏蔽等。
//（4） 接收反馈：如串口接收到数据，立即将该数据发送出去。
//（上面是我遇到的一些问题，可能表述不很清楚，呵呵。如果用于收发txt文件，一般不大注意。）
//3、解决问题的方法是，消除这些默认规则，关键是struct termios 的参数影响。
struct termios {
	tcflag_t c_iflag;              
	tcflag_t c_oflag;              
	tcflag_t c_cflag;              
	tcflag_t c_lflag;              
	cc_t c_line;                   
	cc_t c_cc[NCCS];          
};
//由于研究不深，如果要消除所有上面的规则，我是如下处理的
struct termios options;
//串口打开方式：
open ("dev/ttyS0" , O_RDWR|O_NOCTTY| O_NDELAY );
//消除收发模式规则：
options.c_lflag        = 0;
options.c_oflag        = 0;
options.c_iflag        = 0;

//消除字符屏蔽规则：
options.c_cc[VINTR]    = 0;      
options.c_cc[VQUIT]     = 0;  
options.c_cc[VERASE]    = 0;  
options.c_cc[VKILL]    = 0;  
options.c_cc[VEOF]     = 0;  
options.c_cc[VTIME]    = 1;  
options.c_cc[VMIN]     = 0;  
options.c_cc[VSWTC]    = 0;  
options.c_cc[VSTART]   = 0;  
options.c_cc[VSTOP]    = 0;  
options.c_cc[VSUSP]    = 0;  
options.c_cc[VEOL]     = 0;  
options.c_cc[VREPRINT] = 0;  
options.c_cc[VDISCARD] = 0;  
options.c_cc[VWERASE] = 0;  
options.c_cc[VLNEXT]   = 0;  
options.c_cc[VEOL2]    = 0;  
//以上设置，在其它参数串口设置前执行，如果你需要保留部分参数，请参阅http://blog.chinaunix.net/article.php?articleId=15964&blogId=60
//在RedHat Feroda 4 下编译通过
//
//= = = = = = = = = = = 非阻塞read= = = = = = = = = = =
//Q：在调用串口read(fd,   buff,   len);时,如果串口没有数据,会停在read处,请问有没有办法让这个read动作中止?
//A：使用非阻塞方式select函数（I/O多工机制）或者open的时候加O_NONBLOCK参数。
//int select(int n,fd_set * readfds,fd_set * writefds,fd_set * exceptfds,struct timeval * timeout);关于这个函数的使用我会在下篇blog中整理。
//= = = = = = = = = = = 串口收发源码= = = = = = = = = = =
//       一下代码已经经过我测试，没有问题。开发环境Redhat9，运行环境s3c2410
//= = = = = = receive.c= = = = = =
#include   <stdio.h>     
#include   <stdlib.h>   
#include   <unistd.h>    
#include   <sys/types.h>
#include   <sys/stat.h>  
#include   <fcntl.h>    
#include   <termios.h>  
#include   <errno.h>    
#include   <string.h>
#define TRUE 1
//初始化串口选项：  
void setTermios(struct termios * pNewtio, int uBaudRate)
{
	bzero(pNewtio, sizeof(struct termios));
	//8N1
	pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL;
	pNewtio->c_iflag = IGNPAR;
	pNewtio->c_oflag = 0;
	pNewtio->c_lflag = 0; //non ICANON

	pNewtio->c_cc[VINTR] = 0;
	pNewtio->c_cc[VQUIT] = 0;
	pNewtio->c_cc[VERASE] = 0;
	pNewtio->c_cc[VKILL] = 0;
	pNewtio->c_cc[VEOF] = 4;
	pNewtio->c_cc[VTIME] = 5;
	pNewtio->c_cc[VMIN] = 0;
	pNewtio->c_cc[VSWTC] = 0;
	pNewtio->c_cc[VSTART] = 0;
	pNewtio->c_cc[VSTOP] = 0;
	pNewtio->c_cc[VSUSP] = 0;
	pNewtio->c_cc[VEOL] = 0;
	pNewtio->c_cc[VREPRINT] = 0;
	pNewtio->c_cc[VDISCARD] = 0;
	pNewtio->c_cc[VWERASE] = 0;
	pNewtio->c_cc[VLNEXT] = 0;
	pNewtio->c_cc[VEOL2] = 0;
}
#define BUFSIZE 512
int main(int argc, char **argv)
{
	int fd;
	int nread;
	char buff[BUFSIZE];
	struct termios oldtio, newtio;
	struct timeval tv;
	char *dev ="/dev/ttyS1";
	fd_set rfds;
	if ((fd = open(dev, O_RDWR | O_NOCTTY))<0)
	{
		printf("err: can't open serial port!\n");
		return -1;
	}
	tcgetattr(fd, &oldtio);
	setTermios(&newtio, B115200);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	tv.tv_sec=30;
	tv.tv_usec=0;
	while (TRUE)
	{
		printf("wait...\n");
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		if (select(1+fd, &rfds, NULL, NULL, &tv)>0)
		{
			if (FD_ISSET(fd, &rfds))
			{
				nread=read(fd, buff, BUFSIZE);
				printf("readlength=%d\n", nread);
				buff[nread]='\0';
				printf("%s\n", buff);
			}
		}
	}
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
}
//= = = = = send.c= = = = = =
#include   <stdio.h>     
#include   <stdlib.h>   
#include   <unistd.h>    
#include   <sys/types.h>
#include   <sys/stat.h>  
#include   <fcntl.h>    
#include   <termios.h>  
#include   <errno.h>    
#include   <string.h>
//初始化串口选项：  
void setTermios(struct termios * pNewtio, int uBaudRate)
{
	bzero(pNewtio, sizeof(struct termios));
	//8N1
	pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL;
	pNewtio->c_iflag = IGNPAR;
	pNewtio->c_oflag = 0;
	pNewtio->c_lflag = 0; //non ICANON

	pNewtio->c_cc[VINTR] = 0;
	pNewtio->c_cc[VQUIT] = 0;
	pNewtio->c_cc[VERASE] = 0;
	pNewtio->c_cc[VKILL] = 0;
	pNewtio->c_cc[VEOF] = 4;
	pNewtio->c_cc[VTIME] = 5;
	pNewtio->c_cc[VMIN] = 0;
	pNewtio->c_cc[VSWTC] = 0;
	pNewtio->c_cc[VSTART] = 0;
	pNewtio->c_cc[VSTOP] = 0;
	pNewtio->c_cc[VSUSP] = 0;
	pNewtio->c_cc[VEOL] = 0;
	pNewtio->c_cc[VREPRINT] = 0;
	pNewtio->c_cc[VDISCARD] = 0;
	pNewtio->c_cc[VWERASE] = 0;
	pNewtio->c_cc[VLNEXT] = 0;
	pNewtio->c_cc[VEOL2] = 0;
}
int main(int argc, char **argv)
{
	int fd;
	int nCount, nTotal, i;
	struct termios oldtio, newtio;
	char *dev ="/dev/ttyS1";
	if ((argc!=3) || (sscanf(argv[1], "%d", &nTotal) != 1))
	{
		printf("err: need tow arg!\n");
		return -1;
	}
	if ((fd = open(dev, O_RDWR | O_NOCTTY))<0)
	{
		printf("err: can't open serial port!\n");
		return -1;
	}
	tcgetattr(fd, &oldtio);
	setTermios(&newtio, B115200);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	for (i=0; i<nTotal; i++)
	{
		nCount=write(fd, argv[2], strlen(argv[2]));
		printf("send data\n");
		sleep(1);
	}
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
	return 0;
}
//= = = = = =.makefile= = = = = =
CC = /usr/local/arm/2.95.3/bin/arm-linux-gcc
all:receive send
receive: receive.c
$(CC) receive.c -o receive
send: send.c
$(CC) send.c -o send
clean:
-rm -rf testCOM receive send

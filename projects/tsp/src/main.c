
// For clarity,error checking has been omitted.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>   
#include <sys/types.h>   
#include <sys/wait.h>  
#include <syslog.h>  
#include <pthread.h>
#include "common_fun.h"
#include "csa.h"
#include "tsp_regs.h"
#include "main.h"


//static int work_mode=0;// 1:create chain header 2:calc chain tail 3:sort rainbow table
static FILE  *record_file_fd=NULL;
static FILE  *table_fd=NULL;
static long chain_table_pos=0;
static long chain_header_pos=0;
static int calc_table_pos=0;
static long calc_chain_pos=0;
static long sort_chain_pos=0;


#define sw16(x) \
   ((short)(\
      (((short)(x)&(short)0x00ffU)<< 8) |\
      (((short)(x)&(short)0xff00U)>> 8)))
#define Swap64(ll) (((ll) >> 56) |(((ll) & 0x00ff000000000000) >> 40)  |(((ll) & 0x0000ff0000000000) >> 24)|(((ll) & 0x000000ff00000000) >> 8)|(((ll) & 0x00000000ff000000) << 8)|(((ll) & 0x0000000000ff0000) << 24)|(((ll) & 0x000000000000ff00) << 40)|(((ll) << 56))) 
#define Swap48(ll) (((ll) >> 56) |(((ll) & 0x00ff000000000000) >> 40)  |(((ll) & 0x0000ff0000000000) >> 24)|(((ll) & 0x000000ff00000000) >> 8)|(((ll) & 0x00000000ff000000) << 8)|(((ll) & 0x0000000000ff0000) << 24)|(((ll) & 0x000000000000ff00) << 40)|(((ll) << 56))) 
#define IDX_ITEM_LEN 8
#define IDX_HEADER_BITS 24//no more than 32
typedef struct{
	unsigned long long idx_header:IDX_HEADER_BITS;
	unsigned long long file_pos:40;
}_idx_item_stru;
typedef union{
	unsigned long long ll_value;
	_idx_item_stru _idx_value;
}_union_idx;
static _idx_item_stru latest_idx;
static int idx_item_cnt=0;
static _idx_item_stru *index_array=NULL;
static unsigned long long table_file_size=0;
static unsigned char * index_buf=NULL;
static unsigned char *table_buf=NULL;

typedef struct{
	unsigned char header[CHAIN_HEADER_LENGTH];
	unsigned char tail[CHAIN_TAIL_LENGTH];
	short match;
	unsigned char cw[8];
}calc_tail_stru;

////////////////////////////////////////////////////////////////////////////
///////////////////////////thread pool process/////////////////////////////////
#define MAX_QUERY_TIMES 50000
#define THREAD_MAX 8
static unsigned int s_thread_para[THREAD_MAX][8];
static pthread_t s_tid[THREAD_MAX];
static pthread_mutex_t s_mutex[THREAD_MAX];

static calc_tail_stru calc_tail[MAX_QUERY_TIMES];//CALC_QUEUE_LEN
typedef struct{
	unsigned int idx:29;
	unsigned int isok:2;	//0:not begin 1:during querying 2:query finished
	unsigned int query_answer:1;
	unsigned long long tail;
}_chain_item;

unsigned long long StrToLong_6B(unsigned char *buf){
	unsigned long long a=0;
	a=((unsigned long long)buf[0]<<40)|
		((unsigned long long)buf[1]<<32)|
		((unsigned long long)buf[2]<<24)|
		((unsigned long long)buf[3]<<16)|
		((unsigned long long)buf[4]<<8)|
		buf[5];
	return a;
}

int restore_create_chain_pos(void){
	char string[256]={0};
	record_file_fd=fopen("chain.cfg","w+");//写的时候如果文件存在，会被清空，从头开始写
	if(record_file_fd==NULL){
		TRACE("chain.cfg open fail[B],pls check(%s)\n",strerror(errno));
		return -1;
	}
	snprintf(string,128,"chain_table_pos:%ld\r\n",chain_table_pos);
	fputs(string,record_file_fd);
	
	memset(string,0,128);
	snprintf(string,128,"create_chain_pos:%ld\r\n",chain_header_pos);
	fputs(string,record_file_fd);

	memset(string,0,128);
	snprintf(string,128,"calc_table_pos:%d\r\n",calc_table_pos);
	fputs(string,record_file_fd);
	
	memset(string,0,128);
	snprintf(string,128,"calc_chain_pos:%ld\r\n",calc_chain_pos);
	fputs(string,record_file_fd);

	memset(string,0,128);
	snprintf(string,128,"sort_chain_pos:%ld\r\n",sort_chain_pos);
	fputs(string,record_file_fd);
	
	if(record_file_fd){
		fclose(record_file_fd);
		record_file_fd=NULL;
	}
	return 0;
}

void signal_handler(int signo)   
{   
    signal(signo, signal_handler);  
    switch(signo)   
    {   
		case SIGHUP:
		case SIGINT:  
		case SIGTERM:
		case SIGQUIT:
		{
			TRACE("exit msg\n");
			if(table_fd){
				fclose(table_fd);
				table_fd=NULL;
			}
			restore_create_chain_pos();
             		exit(0);   
        	}
             break;   
        	case SIGALRM:   
             {   
	             syslog(LOG_NOTICE, "Process recieve SIGALRM");   
	             break;   
             }   
		case SIGSEGV:{
			int loopflag = 1;
			while(loopflag){        
			    sleep(1);
				TRACE("\n=============================================\n");
			}
			break;
		
		}/**/
		
        case SIGCHLD:   
             {   
			int *pidstat=NULL;   
			pid_t pid;   
			pid = waitpid(0, pidstat, WNOHANG);   
			if (pid>0)   
				syslog(LOG_WARNING, "child %d terminated\n", pid);   
			syslog(LOG_NOTICE, "exit handle child process\n");   
			break;   
		}   
        default:   
             syslog(LOG_WARNING, "%d signal unregister\n", signo);   
             break;   
    }   
}

int init_signal(void){
	signal(SIGHUP, &signal_handler);   
	signal(SIGSEGV, &signal_handler);   
	signal(SIGQUIT, &signal_handler);   
	signal(SIGINT,  &signal_handler);   
	signal(SIGTERM, &signal_handler);   
	signal(SIGALRM, &signal_handler);   
	signal(SIGCHLD, &signal_handler);  
	signal(SIGPIPE, &signal_handler);
	return 0;
}



unsigned int getHeader(unsigned char *buf){
	unsigned int a=0;
	int i;
	for(i=0;i<IDX_HEADER_BITS/8;i++){
		a+=(unsigned int)buf[i]<<(IDX_HEADER_BITS-8*(i+1));
	}
	return a;
}

/*
table_file:	table file name
idx_item_len:	length of each index item,such as front 4B
*/

int create_dic_index(char* table_file){
	#define READ_DIC_BUFSIZE (CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*102400
	unsigned char buf[READ_DIC_BUFSIZE]={0};
	int n,i;
	int tmp_header=0;
	//int seg_doidx_finished=0;
	int chain_item_count=0;
	//_union_idx tmp_idx_value;
	unsigned int idx_header=0;
	unsigned long long tmp_idx_pos=0;//,tmp_header_BE=0;
	_idx_item_stru idx_struct;
	FILE  *idx_file_fd=NULL;
	idx_file_fd=fopen("index","ab");//写的时候如果文件存在，会被清空，从头开始写
	if(idx_file_fd==NULL){
		TRACE("index file open fail,pls check(%s)\n",strerror(errno));
		return -1;
	}
	/*n=fseek(idx_file_fd,-1L*sizeof(_idx_item_stru),SEEK_END);
	if(n<0){
		memset(&latest_idx,0,sizeof(_idx_item_stru));
		tmp_idx_pos=0;
	}
	else{
		n=fread((char*)&latest_idx,1,sizeof(_idx_item_stru),idx_file_fd);
		if(n<=0){
			TRACE("read index file err(%s)\n",strerror(errno));
			return -2;
		}
		tmp_idx_pos=latest_idx.file_pos;
	}
	*/
	latest_idx.idx_header=0xFFFFFF;
	if(table_fd==NULL)
		table_fd=fopen(table_file,"r");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		return -1;
	}
	
	n=fseek(table_fd ,(tmp_idx_pos==0)?0:(tmp_idx_pos+(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)), SEEK_SET);
	if(n!=0){
		TRACE("table file seek fail:%s \n",strerror(errno));
		goto create_idx_end;
	}
	//tmp_header_BE=0;
create_idx_start:
	//chain_item_count=0;
	memset(buf,0,READ_DIC_BUFSIZE);
	//find the first empty chain item(without chain_tail)
	n=fread(buf,1,READ_DIC_BUFSIZE,table_fd);
	if(n<(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)||n%(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)!=0){
		TRACE("table file read fail or reach the file end,err:%s,n:%d \n",strerror(errno),n);
		goto create_idx_end;
	}
	chain_item_count=n/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	//tmp_header_BE=htonl(latest_idx.idx_header);
	for(i=0;i<chain_item_count;i++){
		idx_header=getHeader(buf+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		if(latest_idx.idx_header==idx_header)
			continue;
		else{
			latest_idx.idx_header=idx_header;
			
			tmp_header=idx_header;
			idx_struct.idx_header=((tmp_header&0xFF)<<16)|(tmp_header&0xFF00)|((tmp_header&0xFF0000)>>16);
			idx_struct.file_pos=tmp_idx_pos+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
			fwrite((unsigned char*)&idx_struct,1,sizeof(_idx_item_stru),idx_file_fd);
			//TRACE("tmp_idx_value:%llx,tmp_header:%llx,idx_header:%llx,pos:%llx\n",tmp_idx_value.ll_value,tmp_header,idx_struct.idx_header,idx_struct.file_pos);
			//dump_packet_line("", (unsigned char*)&idx_struct, 8);
		}
	}
	tmp_idx_pos+=n;
	goto create_idx_start;
create_idx_end:	
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}
	if(idx_file_fd>0){
		fclose(idx_file_fd);
		idx_file_fd=NULL;
	}
	TRACE("create index end time:%d,idx_pos:%lld\n",(int)time((time_t *) 0),tmp_idx_pos);
	return 0;
}

_idx_item_stru * load_dic_index(char* index_file){
	unsigned long long file_size=0,n,i;
	int j;
	unsigned char temp=0,*buf=NULL;
	FILE  *idx_file_fd=NULL;
	idx_file_fd=fopen(index_file,"rb+");
	if(idx_file_fd==NULL){
		TRACE("index file open fail,pls check(%s)\n",strerror(errno));
		return NULL;
	}
	n=fseek(idx_file_fd,0,SEEK_END);
	if(n<0){
		goto load_dic_err; 
	}
	else{
		file_size = ftell(idx_file_fd);
		TRACE("dic_index file(%s) size:%lld\n",index_file,file_size);
	}
	index_buf=(unsigned char *)malloc(file_size);
	if(index_buf==NULL){
		TRACE("MALLOC memory(%lldB) for index file err:%s\n",file_size,strerror(errno));
		goto load_dic_err; 
	}
	else
		TRACE("MALLOC memory(%lldB) for index file ok\n",file_size);
	
	fseek(idx_file_fd,0,SEEK_SET);
	n=fread(index_buf,1,file_size,idx_file_fd);
	if(n!=file_size){
		TRACE("read index file err:%s(read %lld B)",strerror(errno),file_size);
		goto load_dic_err; 
	}
	else
		TRACE("read index file finished.");
	idx_item_cnt=file_size/IDX_ITEM_LEN;
	
	for(i=0;i<idx_item_cnt;i++){
		buf=index_buf+i*IDX_ITEM_LEN;
		for(j=0;j<(IDX_HEADER_BITS/8)/2;j++){
			temp=buf[(IDX_HEADER_BITS/8)-j-1];
			buf[(IDX_HEADER_BITS/8)-j-1]=buf[j];
			buf[j]=temp;
		}
	}
		
	return (_idx_item_stru *)index_buf;
load_dic_err:
	if(index_buf){
		free(index_buf);
		index_buf=NULL;
	}
	if(idx_file_fd>0){
		fclose(idx_file_fd);
		idx_file_fd=NULL;
	}
	return NULL;
}

/*create chain header 
chain_header_length:
item_account:the total account of record in each table
table_idx:create chain header for current table
table_file:the rainbow table bin file path
*/
int create_chain_header_func(char* table_file){
	#define CREATE_UNIT_NO 1024
	#define WRITE_FILE_BUFSIZE (CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*CREATE_UNIT_NO
	int i,ii,n;
	long j,record_temp_counter=0;
	int start_table=chain_table_pos;
	unsigned char buf[WRITE_FILE_BUFSIZE]={0};
	unsigned char rand_head[5]={0};//[TABLE_COUNT][]={{'0','1','2'},{'3','4','5'},{'6','7','8'},{'9','A','B'},{'C','D','E','F'}};
	int fd = open("/dev/urandom", O_RDONLY);  //用urandom的方法输出数据数
	
	//open(table_file,O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	
	//create tables
	for(i=start_table;i<TABLE_COUNT;i++){
		long loop_pos=0;
		int item_each_loop_wide=0,loop_idx=0;
		int rand_loop_cnt=0;
		long start_header_intable=chain_header_pos;
		int buf_pos=0;
		if(table_fd==NULL){
			char file_name[32]={0};
			snprintf(file_name,32,"%s_%d",table_file,i);
			table_fd=fopen(file_name,"ab+");
			if(table_fd==NULL){
				TRACE("open table file:%s fail\n",file_name);
				return -1;
			}
			else
				TRACE("create %s\n",file_name);
		}
		switch(i){
			case 0:
				rand_loop_cnt=3;rand_head[0]=0x00,rand_head[1]=0x10,rand_head[2]=0x20;
				break;
			case 1:
				rand_loop_cnt=3;rand_head[0]=0x30,rand_head[1]=0x40,rand_head[2]=0x50;
				break;
			case 2:
				rand_loop_cnt=3;rand_head[0]=0x60,rand_head[1]=0x70,rand_head[2]=0x80;
				break;
			case 3:
				rand_loop_cnt=3;rand_head[0]=0x90,rand_head[1]=0xA0,rand_head[2]=0xB0;
				break;
			case 4:
				rand_loop_cnt=4;rand_head[0]=0xC0,rand_head[1]=0xD0,rand_head[2]=0xE0,rand_head[3]=0xF0;
				break;
		}
		if(ITEM_COUNT_INTABLE<rand_loop_cnt)
			item_each_loop_wide=rand_loop_cnt;
		else
			item_each_loop_wide=ITEM_COUNT_INTABLE/rand_loop_cnt;//每次的小区间长度，
		loop_pos=chain_header_pos%item_each_loop_wide;
		loop_idx=chain_header_pos/item_each_loop_wide;
		record_temp_counter=0;
		memset(buf,0,WRITE_FILE_BUFSIZE);
		TRACE("table idx:%d,the %ld chain,loop_wide:%d,loop_pos:%ld,loop_idx:%d,loop_header_char:%0x\n",
			i,start_header_intable,item_each_loop_wide,loop_pos,loop_idx,rand_head[loop_idx]);
		for(j=start_header_intable;j<ITEM_COUNT_INTABLE;j++){
			buf_pos=record_temp_counter*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
			while((n=read(fd, buf+buf_pos, (CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)))==(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)){
				break;
			}
			
			*(buf+buf_pos)&=0x0F;
			*(buf+buf_pos)|=rand_head[loop_idx];
			loop_pos++;
			if(loop_pos>=item_each_loop_wide){
				loop_pos=0;
				loop_idx++;
			}
			chain_header_pos++;
			record_temp_counter++;
			if(record_temp_counter>=CREATE_UNIT_NO){
				n=fwrite(buf,WRITE_FILE_BUFSIZE,1,table_fd);
				if(n<=0)
					TRACE("write table file error,a:%s\n",strerror(errno));
				record_temp_counter=0;
				memset(buf,0,WRITE_FILE_BUFSIZE);
			}
			#if 0
			{
				unsigned char ppp[10]={0};
				memcpy(ppp,buf+buf_pos, CHAIN_HEADER_LENGTH);
				//TRACE("chain_header_pos:%ld,ppp:%s\n",chain_header_pos,ppp);
				TRACE("%0x ",rand_head[loop_idx]);
				dump_packet_line("ppp",ppp,9);
			}
			#endif
		}
		
		if(record_temp_counter>0){
			n=fwrite(buf,(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*record_temp_counter,1,table_fd);
			if(n<=0){
				TRACE("write table file error,b:%s\n",strerror(errno));
			}
		}

		//verify tables
		unsigned char zero[6]={0};
		int verify_result=0,verify_times=0;
		n=fseek(table_fd ,0, SEEK_SET);
		if(n!=0){
			TRACE("lseek the start of the table fail:%s\n",strerror(errno));
			goto create_end;
		}
		while(1){
			n=fread(buf,1,WRITE_FILE_BUFSIZE,table_fd);
			if(n<=0){
				TRACE("Verify break or reach the file end, exit:%s\n",strerror(errno));
				break;
			}
			record_temp_counter=n/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
			for(ii=0;ii<record_temp_counter;ii++){
				if(memcmp(buf+ii*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),zero,CHAIN_HEADER_LENGTH)==0||
					memcmp(buf+ii*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH,zero,CHAIN_TAIL_LENGTH)!=0){
					//fpos_t filepos;
					verify_result=-1;
					//fgetpos(table_fd, &filepos);
					//TRACE("table file broken at :");
					//showfpos(&filepos);
					//TRACE("\n");
					dump_packet((char*)"dump file",buf,WRITE_FILE_BUFSIZE);
					break;
				}
				verify_times++;
			}
		}
		if(verify_result==0){
			TRACE("table_%ld verify ok,verify_times:%d\n",chain_table_pos,verify_times);
		}
		//end
	create_end:
		if(table_fd){
			fclose(table_fd);
			table_fd=NULL;
		}
		
		chain_header_pos=0;
		chain_table_pos++;
		break;
	}
	if(fd){
		close(fd);
	}
	return 0;
}


//非递归算法
int binary( int *a, int key, int n )
{
	int left = 0, right = n - 1, mid = 0;
	mid = ( left + right ) / 2;
	while( left < right && a[mid] != key )
	{
		if( a[mid] < key )
			left = mid + 1;
		else if( a[mid] > key )
			right = mid - 1;
		mid = ( left + right ) / 2;
	}
	if( a[mid] == key )
		return mid;
	return -1;
}
unsigned long  long query_index(unsigned int value, unsigned long* pos){//can 
	if(pos==NULL)
		return -1;
	long long left = 0, right = idx_item_cnt - 1, mid = 0;
	mid = ( left + right ) / 2;
	while( left < right && index_array[mid].idx_header != value )
	{
		if(  index_array[mid].idx_header < value )
			left = mid + 1;
		else if(  index_array[mid].idx_header > value )
			right = mid - 1;
		mid = ( left + right ) / 2;
	}
	if(  index_array[mid].idx_header == value ){
		*pos=index_array[mid].file_pos;
		return mid;
	}
	return -1;
}

long  long  query_table(unsigned long  long pos,unsigned char* key_input, unsigned long long read_size){
	int n;
	unsigned char *buf=NULL;
	unsigned long  long tmp=0;
	unsigned long long key=StrToLong_6B(key_input);
	
	
	if(pos<0||read_size<(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH))
		return -1;
	n=read_size/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	int left = 0, right = n - 1, mid = 0;
	mid = ( left + right ) / 2;
	buf=table_buf+pos;
	
	do{
		tmp=StrToLong_6B(buf+mid*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		if( tmp < key )
			left = mid + 1;
		else if( tmp > key )
			right = mid - 1;
		mid = ( left + right ) / 2;
	}
	while( left < right && tmp!= key );

	tmp=StrToLong_6B(buf+mid*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
	if(tmp == key )
		return (unsigned long  long)(pos+mid*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH));

	return -1;
}


unsigned long long find_header_in_table(int tail_idx){
	unsigned int index_header=getHeader(calc_tail[tail_idx].tail);
	unsigned long begin_pos=0,read_len=0;
	long index_pos=-1;
	long long ret=0;
	if(table_fd==NULL){
		TRACE("table_fd null\n");
		return 0;
	}
	//index_header=((index_header&0xFF)<<16)|(index_header&0xFF00)|((index_header&0xFF0000)>>16);
	index_pos=query_index(index_header,&begin_pos);
	//TRACE("tail:%016llx,index_pos:0x%06x,begin_pos:%ld\n",tail,index_pos,begin_pos);
	if(index_pos==-1){
		//TRACE("index_pos err\n");
		return 0;
	}
	
	
	if(index_pos+1<idx_item_cnt)
		read_len=index_array[index_pos+1].file_pos-begin_pos;
	else
		read_len=table_file_size-begin_pos;
	ret=query_table(begin_pos,calc_tail[tail_idx].tail, read_len);
	if(ret>=0){
		TRACE("query_table query tail ret:%lld\n",ret);
		calc_tail[tail_idx].match=1;
		memcpy(calc_tail[tail_idx].header,table_buf+ret,CHAIN_HEADER_LENGTH);
	}
	
	return 0;
}




int find_header_in_table_multithread(int thread, int tail_idx){
	unsigned int index_header=getHeader(calc_tail[tail_idx].tail);
	unsigned long begin_pos=0,read_len=0;
	int index_pos=-1,ret=-1;
	
	//index_header=((index_header&0xFF)<<16)|(index_header&0xFF00)|((index_header&0xFF0000)>>16);
	index_pos=query_index(index_header,&begin_pos);
	//TRACE("tail:%016llx,index_pos:0x%06x,begin_pos:%ld\n",tail,index_pos,begin_pos);
	if(index_pos==-1){
		//TRACE("index_pos err\n");
		return -1;
	}
	
	
	if(index_pos+1<idx_item_cnt)
		read_len=index_array[index_pos+1].file_pos-begin_pos;
	else
		read_len=table_file_size-begin_pos;
	ret=query_table(begin_pos,calc_tail[tail_idx].tail, read_len);
	if(ret>=0){
		TRACE("query_table query tail read_len:%ld,begin_pos:%lx,ret:%d\n",read_len,begin_pos,ret);
		calc_tail[tail_idx].match=1;
		memcpy(calc_tail[tail_idx].header,table_buf+ret,CHAIN_HEADER_LENGTH);
	}
	return calc_tail[tail_idx].match;
}
void * calc_task(unsigned int thread_para[])
{
	int        pool_index,r;   
	pthread_detach(pthread_self());
	pool_index = thread_para[7];
	
wait_unlock:
	pthread_mutex_lock(s_mutex + pool_index);
	while(1){
		r=find_header_in_table_multithread(thread_para[7],thread_para[1]*THREAD_MAX+pool_index);
		thread_para[1]++;
		if(0){
			TRACE("[%d]answer:%d,thread_para[1]:%05d ",pool_index,r,thread_para[1]);
			dump_packet_line("tail",calc_tail[thread_para[1]].tail,CHAIN_TAIL_LENGTH);
		}
		if(thread_para[1]>=(MAX_QUERY_TIMES/THREAD_MAX)){
			thread_para[0]=2;
			//TRACE("[%d]answer:%d,thread_para[1]:%d\n",pool_index,r,thread_para[1]);
			break;
		}
	}
	goto wait_unlock;

	pthread_exit(NULL);
}

static int init_thread_pool(void)
{
    int    i, rc;

    for(i = 0; i < THREAD_MAX; i++) {
        s_thread_para[i][0] = 0;
        s_thread_para[i][7] = i;
        pthread_mutex_lock(s_mutex + i);
    }

    for(i = 0; i < THREAD_MAX; i++) {
        rc = pthread_create(s_tid + i, 0, (void *)calc_task, (void *)(s_thread_para[i]));
        if (0 != rc) {
            fprintf(stderr, "pthread_create test_server4 err(%d)\n",i);
            return(-1);
        }
    }

    return(0);
}


int test_hdd_speed_multithread(char* table_file){
	int j;
	long n,table_size=0;
	if(table_fd==NULL)
		table_fd=fopen(table_file,"r");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		goto query_end;
	}
	n=fseek(table_fd,0,SEEK_END);
	if(n<0){
		goto query_end; 
	}
	else{
		table_size = ftell(table_fd);
		table_file_size=table_size;
		TRACE("table_size file(%s) size:%ld\n",table_file,table_size);
	}
	
	table_buf=(unsigned char *)malloc(table_size);
	if(table_buf==NULL){
		TRACE("malloc memory for table_buf fail,size:%ld,err:%s\n",table_size,strerror(errno));
		goto query_end;
	}
	fseek(table_fd,0,SEEK_SET);
	n=fread(table_buf,1,table_size,table_fd);
	if(n!=table_size){
		TRACE("read table file err:%s(table_size:%ld,read %ld B)\n",strerror(errno),table_size,n);
		goto query_end; 
	}
	else
		TRACE("load table file finished.\n");
	
	
	index_array=load_dic_index("index");
	if(index_array==NULL)
		goto query_end;
	init_thread_pool();
	TRACE("thread numbers:%d,MAX query times:%d\n",THREAD_MAX,MAX_QUERY_TIMES);

	
	for(j=0;j<THREAD_MAX;j++){
		s_thread_para[j][0] = 1;
		pthread_mutex_unlock(s_mutex + j);
	}
	TRACE("query begin time:%d\n",(int)time((time_t *) 0));
	int loop_break=1;
	while(1){
		loop_break=1;
		for(j=0;j<THREAD_MAX;j++){
			if(s_thread_para[j][0] != 2){
				loop_break=0;
			}
		}
		if(loop_break)
			break;
		cs_sleepms(200);
	}
	
query_end:
	if(index_buf){
		free(index_buf);
		index_buf=NULL;
	}
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}
	if(table_buf){
		free(table_buf);
		table_buf=NULL;
	}
	TRACE("calc end time:%d\n",(int)time((time_t *) 0));
	return 0;
}

int test_hdd_speed(char* table_file){
	//unsigned long long answer=0;
	long n,table_size;
	int i=0;
	if(table_fd==NULL)
		table_fd=fopen(table_file,"r");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		goto test_exit;
	}
	n=fseek(table_fd,0,SEEK_END);
	if(n<0){
		goto test_exit; 
	}
	else{
		table_size = ftell(table_fd);
		table_file_size=table_size;
		TRACE("table_size file(%s) size:%ld\n",table_file,table_size);
	}
	table_buf=(unsigned char *)malloc(table_size);
	if(table_buf==NULL){
		TRACE("malloc memory for table_buf fail,size:%ld,err:%s\n",table_size,strerror(errno));
		goto test_exit;
	}
	fseek(table_fd,0,SEEK_SET);
	n=fread(table_buf,1,table_size,table_fd);
	if(n!=table_size){
		TRACE("read table file err:%s(table_size:%ld,read %ld B)\n",strerror(errno),table_size,n);
		goto test_exit; 
	}
	else
		TRACE("load table file finished.\n");
	
	index_array=load_dic_index("index");
	if(index_array==NULL)
		goto test_exit;
	TRACE("query begin time:%d\n",(int)time((time_t *) 0));
	
	find_header_in_table(0);
	TRACE("match:%d\n",calc_tail[0].match);
	
	test_exit:
	TRACE("[query %d tails]query end time:%d\n",i,(int)time((time_t *) 0));
	if(table_buf){
		free(table_buf);
		table_buf=NULL;
	}
	if(index_buf){
		free(index_buf);
		index_buf=NULL;
	}
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}
	return 0;
}


void swapIfFirstIsGreater(unsigned char *a, unsigned char *b)
{
	unsigned long aa=((unsigned long)a[CHAIN_HEADER_LENGTH+0]<<40)|((unsigned long)a[CHAIN_HEADER_LENGTH+1]<<32)|((unsigned long)a[CHAIN_HEADER_LENGTH+2]<<24)|((unsigned long)a[CHAIN_HEADER_LENGTH+3]<<16)|((unsigned long)a[CHAIN_HEADER_LENGTH+4]<<8)|a[CHAIN_HEADER_LENGTH+5];
	unsigned long bb=((unsigned long)b[CHAIN_HEADER_LENGTH+0]<<40)|((unsigned long)b[CHAIN_HEADER_LENGTH+1]<<32)|((unsigned long)b[CHAIN_HEADER_LENGTH+2]<<24)|((unsigned long)b[CHAIN_HEADER_LENGTH+3]<<16)|((unsigned long)b[CHAIN_HEADER_LENGTH+4]<<8)|b[CHAIN_HEADER_LENGTH+5];
	if(aa > bb)
	{
		unsigned char temp[CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH]={0};
		memcpy(temp,a,CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);	
		memcpy(a,b,CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
		memcpy(b,temp,CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	}
}

void bitonicSort_6B(
    unsigned char * input,
    const unsigned long length,
    const int sortIncreasing)
{
    const unsigned long halfLength = length/2;

    unsigned long i;
    for(i = 2; i <= length; i *= 2)
    {
        unsigned long j;
        for(j = i; j > 1; j /= 2)
        {
            int increasing = sortIncreasing;
            const unsigned long half_j = j/2;

            unsigned long k;
            for(k = 0; k < length; k += j)
            {
                const unsigned long k_plus_half_j = k + half_j;
                unsigned long l;

                if(i < length)
                {
                    if((k == i) || (((k % i) == 0) && (k != halfLength)))
                    {
                        increasing = !increasing;
                    }
                }

                for(l = k; l < k_plus_half_j; ++l)
                {
                    if(increasing)
                    {
                        swapIfFirstIsGreater(input+(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*l, 
					input+(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*(l + half_j));
                    }
                    else
                    {
                        swapIfFirstIsGreater(input+(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*(l + half_j), 
					input+(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*l);
                    }
                }
            }
        }
    }
}
int sort_seg_func(char* table_file){
	#define SORT_QUEUE_LEN 32768//268435456L//4294967296
	#define SORT_BUF_SIZE ((CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*SORT_QUEUE_LEN)
	unsigned char *buf=NULL;
	unsigned long readByte=0;
	unsigned long long file_size=0,n;
	int file_idx=0;
	buf = (unsigned char *)malloc(SORT_BUF_SIZE);
	if(buf==NULL){
		TRACE("malloc memory fail(%s)\n",strerror(errno));
		return -1;
	}
	
	if(table_fd==NULL)
		table_fd=fopen(table_file,"rb+");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		goto sort_end;
	}
	else{
		n=fseek(table_fd,0,SEEK_END);
		if(n<0){
			goto sort_end;
		}
		else{
			file_size = ftell(table_fd);
			if(file_size<=0){
				TRACE("table file size err:%lld\n",file_size);
				goto sort_end;
			}
			TRACE("table filesize:%lld\n",file_size);
		}
	}

	
	TRACE("sort begin time:%d\n",(int)time((time_t *) 0));
	TRACE("sort begin sort_chain_pos:%ld\n",sort_chain_pos);
start_sort_seek:
	{
		n=fseek(table_fd , sort_chain_pos*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH) , SEEK_SET);
		if(n!=0){
			TRACE("table file seek fail:%s \n",strerror(errno));
			goto sort_end;
		}
	
		memset(buf,0,SORT_BUF_SIZE);
		//find the first empty chain item(without chain_tail)
		readByte=fread(buf,1,SORT_BUF_SIZE,table_fd);
		TRACE("read %ld B,tick:%d\n",readByte,(int)time((time_t *) 0));
		//dump_packet("ori data",buf,SORT_BUF_SIZE);
		if(readByte<(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)||readByte%(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)!=0){
			TRACE("table file read fail or reach the file end,err:%s,n:%ld \n",strerror(errno),readByte);
			goto sort_end;
		}
		long get_item_cnt=readByte/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
		if((get_item_cnt&(get_item_cnt-1))!=0){
			TRACE("table file read %ldB,get_item_cnt:%ld not Power of 2 \n",readByte,get_item_cnt);
			goto sort_end;
		}
		TRACE("get_item_cnt:%ld\n",get_item_cnt);
		bitonicSort_6B(buf,get_item_cnt,1);
		TRACE("bitonicSort_6B finished\n");
		/*for(int i=0;i<get_item_cnt;i++){
			dump_packet_line("", buf+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH));
		}*/
		sort_chain_pos+=get_item_cnt;
		{
			FILE  *seg_file_fd=NULL;
			char f_name[64]={0};
			//snprintf(f_name,64,"%ld_%d",sort_chain_pos,SORT_QUEUE_LEN);
			file_idx=sort_chain_pos/get_item_cnt-1;
			snprintf(f_name,64,"file%lld_no%02d.bin",file_size/SORT_BUF_SIZE,file_idx);
			seg_file_fd=fopen(f_name,"wb");//写的时候如果文件存在，会被清空，从头开始写
			if(seg_file_fd==NULL){
				TRACE("f_name open fail,pls check(%s)\n",strerror(errno));
				goto sort_end;
			}
			fwrite(buf,1,get_item_cnt*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),seg_file_fd);
			fclose(seg_file_fd);
			TRACE("write file:%s,chain_table_pos:%ld,write_size:%ld,tick:%d\n",f_name,sort_chain_pos,
				get_item_cnt*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),(int)time((time_t *) 0));
			restore_create_chain_pos();
		}
		goto start_sort_seek;
	}

sort_end:
	if(buf){
		free(buf);
	}
	
	TRACE("sort end time:%d\n",(int)time((time_t *) 0));
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}

	
	return 0;
}

int merge_sort(char *fileA, char *fileB, char * output_file){
	#define MERGE_MEM_LEN 16777216
	#define MERGE_BUF_SIZE (CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*MERGE_MEM_LEN

	FILE  *fd_A=NULL,*fd_B=NULL,*merge_file_fd=NULL;
	unsigned char *buf_A=NULL,*buf_B=NULL,*buf_Merge=NULL;

	int i,readByte=0;
	unsigned long long bufA_cur=0,bufA_fin=0,bufA_left=0;
	unsigned long long bufB_cur=0,bufB_fin=0,bufB_left=0;
	unsigned long long bufM_cur=0;//A_left:1 B_left:0
	unsigned long long Amax=0,Bmax=0,Max=0,TempA=0,TempB=0;
	int AB_flag=1;//1: A 0: B
	unsigned long long merge_file_size=0;
	
	if(fileA==NULL||fileB==NULL||output_file==NULL)
		return -1;
	fd_A=fopen(fileA,"rb+");
	if(fd_A==NULL){
		TRACE("open table fd_A:%s fail,err:%s\n",fileA,strerror(errno));
		goto merge_end;
	}
	fd_B=fopen(fileB,"rb+");
	if(fd_B==NULL){
		TRACE("open table fd_A:%s fail,err:%s\n",fileB,strerror(errno));
		goto merge_end;
	}
	//char f_name[64]="merge_file.bin";
	//snprintf(f_name,64,"%ld_%d",sort_chain_pos,SORT_QUEUE_LEN);
	
	merge_file_fd=fopen(output_file,"wb");//写的时候如果文件存在，会被清空，从头开始写
	if(merge_file_fd==NULL){
		TRACE("f_name open fail,pls check(%s)\n",strerror(errno));
		goto merge_end;
	}
	
	buf_A = (unsigned char *)malloc(MERGE_BUF_SIZE);
	if(buf_A==NULL){
		TRACE("malloc buf_A memory fail(%s)\n",strerror(errno));
		goto merge_end;
	}
	buf_B = (unsigned char *)malloc(MERGE_BUF_SIZE);
	if(buf_B==NULL){
		TRACE("malloc buf_B memory fail(%s)\n",strerror(errno));
		goto merge_end;
	}
	buf_Merge = (unsigned char *)malloc(MERGE_BUF_SIZE*2);
	if(buf_Merge==NULL){
		TRACE("malloc buf_Merge memory fail(%s)\n",strerror(errno));
		goto merge_end;
	}
	TRACE("merge begin time:%d\n",(int)time((time_t *) 0));
	
merge_start:
	readByte=fread(buf_A+bufA_left,1,MERGE_BUF_SIZE-bufA_left,fd_A);
	bufA_fin=(bufA_left+readByte);///(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	bufA_cur=0;
	Amax=StrToLong_6B(buf_A+bufA_fin-CHAIN_TAIL_LENGTH);
	
	readByte=fread(buf_B+bufB_left,1,MERGE_BUF_SIZE-bufB_left,fd_B);
	bufB_fin=(bufB_left+readByte);///(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	bufB_cur=0;
	Bmax=StrToLong_6B(buf_B+bufB_fin-CHAIN_TAIL_LENGTH);
	
	memset(buf_Merge,0,MERGE_BUF_SIZE*2);
	bufM_cur=0;
	if(Amax<Bmax){
		Max=Amax;
		//AorB_left=0;
	}
	else{
		Max=Bmax;
		//AorB_left=1;
	}
	if(bufA_fin==0){
		if(bufB_fin!=0){
			fwrite(buf_B,1,bufB_fin,merge_file_fd);
			merge_file_size+=bufB_fin;
			printf("[B]write file :%llxB,total size:%llx\n",bufB_fin,merge_file_size);
		}
		goto merge_end;
	}
	if(bufB_fin==0){
		if(bufA_fin!=0){
			fwrite(buf_A,1,bufA_fin,merge_file_fd);
			merge_file_size+=bufA_fin;
			printf("[C]write file :%llxB,total size:%llx\n",bufA_fin,merge_file_size);
		}
		goto merge_end;
	}
//merge_loop:
	while(1){
		TempA=TempB=0;
		if(AB_flag){//A
			TempA=StrToLong_6B(buf_A+bufA_cur+CHAIN_HEADER_LENGTH);
			for(i=bufB_cur;i<bufB_fin;i+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH){
				TempB=StrToLong_6B(buf_B+i+CHAIN_HEADER_LENGTH);
				if(TempB>TempA)
					break;
			}
			if(i==bufB_cur){
				memcpy(buf_Merge+bufM_cur,buf_A+bufA_cur,CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
				bufM_cur+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH;
				bufA_cur+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH;
				//memcpy(buf_Merge+bufM_cur,buf_B+bufB_cur,16);
				//bufM_cur+=16;
				//bufB_cur+=16;
			}
			else if(i>bufB_cur){
				memcpy(buf_Merge+bufM_cur,buf_B+bufB_cur,i-bufB_cur);
				bufM_cur+=(i-bufB_cur);
				bufB_cur+=(i-bufB_cur);
			}
			//printf("[AB_flag:%d,bufB_cur:%d,i:%d,%llx] ",AB_flag,bufB_cur,i,TempA);
		}
		else{//B
			TempB=StrToLong_6B(buf_B+bufB_cur+CHAIN_HEADER_LENGTH);
			for(i=bufA_cur;i<bufA_fin;i+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH){
				TempA=StrToLong_6B(buf_A+i+CHAIN_HEADER_LENGTH);
				if(TempA>TempB)
					break;
			}
			if(i==bufA_cur){
				memcpy(buf_Merge+bufM_cur,buf_B+bufB_cur,CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
				bufM_cur+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH;
				bufB_cur+=CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH;
				//memcpy(buf_Merge+bufM_cur,buf_A+bufA_cur,16);
				//bufM_cur+=16;
				//bufA_cur+=16;
			}
			else if(i>bufA_cur){
				memcpy(buf_Merge+bufM_cur,buf_A+bufA_cur,i-bufA_cur);
				bufM_cur+=(i-bufA_cur);
				bufA_cur+=(i-bufA_cur);
			}
			//printf("[AB_flag:%d,bufA_cur:%d,i:%d,%llx] ",AB_flag,bufA_cur,i,TempB);
		}
		AB_flag=!AB_flag;
		//printf("bufA_fin:%d,bufA_cur:%d,bufB_fin:%d,bufB_cur:%d\n",bufA_fin,bufA_cur,bufB_fin,bufB_cur);
		if(TempA>=Max||TempB>=Max){//one loop end
			fwrite(buf_Merge,1,bufM_cur,merge_file_fd);
			bufA_left=bufA_fin-bufA_cur;
			memmove(buf_A,buf_A+bufA_cur,bufA_left);
			bufB_left=bufB_fin-bufB_cur;
			memmove(buf_B,buf_B+bufB_cur,bufB_left);
			merge_file_size+=bufM_cur;
			printf("[A]write file :%llxB,total size:%llx,bufA_left:%llx,bufB_left:%llx\n",bufM_cur,merge_file_size,bufA_left,bufB_left);
			goto merge_start;
		}
	}
			
merge_end:
	TRACE("merge_end,bufA_fin:%lld,bufB_fin:%lld\n",bufA_fin,bufB_fin);
	if(fd_A>0){
		fclose(fd_A);
		fd_A=NULL;
	}
	if(fd_B>0){
		fclose(fd_B);
		fd_B=NULL;
	}
	if(merge_file_fd>0){
		fclose(merge_file_fd);
		merge_file_fd=NULL;
	}
	if(buf_A){
		free(buf_A);
	}
	if(buf_B){
		free(buf_B);
	}
	TRACE("merge end time:%d\n",(int)time((time_t *) 0));
	return 0;
}

void merge_seg_func(int file_num){
	int i,j;
	char fileA[64]={0},fileB[64]={0},fileO[64]={0};
	for(i=file_num/2;i>0;i=(i>>1)){
		for(j=0;j<i;j++){
			snprintf(fileA,64,"file%d_no%02d.bin",2*i,2*j);
			snprintf(fileB,64,"file%d_no%02d.bin",2*i,2*j+1);
			snprintf(fileO,64,"file%d_no%02d.bin",i,j);
			printf("step:%d,pass:%d,fileA:%s,fileB:%s,fileO:%s\n",i,j,fileA,fileB,fileO);
			merge_sort(fileA,fileB,fileO);
			if(0){//2*i!=file_num){
				if(remove(fileA)==0)
					TRACE("remove %s OK\n",fileA);
				else
					TRACE("remove %s fail\n",fileA);
				if(remove(fileB)==0)
					TRACE("remove %s OK\n",fileB);
				else
					TRACE("remove %s fail\n",fileB);
			}
		}
		//return;
	}
}

void check_sort_result(char* table_file){
	#define CHECK_BUFSIZE (CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)*102400
	unsigned char buf[CHECK_BUFSIZE]={0};
	int chain_item_count=0;
	unsigned long long TempA=0,TempB=0,i,n,file_pos=0;
	int ret=0;
	if(table_fd==NULL)
		table_fd=fopen(table_file,"r");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		return;
	}
	
check_start:
	memset(buf,0,CHECK_BUFSIZE);
	n=fread(buf,1,CHECK_BUFSIZE,table_fd);
	if(n<(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)||n%(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)!=0){
		TRACE("table file read fail or reach the file end,err:%s,n:%lld \n",strerror(errno),n);
		goto check_end;
	}
	chain_item_count=n/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	for(i=0;i<chain_item_count-1;i++){
		TempA=StrToLong_6B(buf+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		TempB=StrToLong_6B(buf+(i+1)*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		if(TempA>=TempB){
			TRACE("sort err,file_pos:%lld,offset:%lld\n",file_pos,i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH));
			ret=-1;
			goto check_end;
		}
	}
	file_pos+=n;
	goto check_start;
	
check_end:	
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}
	if(ret==0)
		TRACE("check sort result passed!\n");
	return;
}

void remove_repeate_item(char* table_file){
	#define READ_BUFSIZE 2952790016L
	unsigned char *buf=NULL,*buf_copy=NULL;
	FILE  *copyfile_fd=NULL;
	unsigned long long TempA=0,TempB=0,pos_begin=0,pos_end=0,i,j,n,max=0,file_pos=0,chain_item_count=0;
	if(table_fd==NULL)
		table_fd=fopen(table_file,"r");
	if(table_fd==NULL){
		TRACE("open table file:%s fail,err:%s\n",table_file,strerror(errno));
		return;
	}

	if(copyfile_fd==NULL)
		copyfile_fd=fopen("new_table","w+");
	if(copyfile_fd==NULL){
		TRACE("open copy file fail,err:%s\n",strerror(errno));
		goto remove_end;
	}

	buf=(unsigned char *)malloc(READ_BUFSIZE);
	if(buf==NULL){
		TRACE("malloc memory for buf fail,err:%s\n",strerror(errno));
		goto remove_end;
	}
	buf_copy=(unsigned char *)malloc(READ_BUFSIZE);
	if(buf_copy==NULL){
		TRACE("malloc memory for buf_copy fail,err:%s\n",strerror(errno));
		goto remove_end;
	}
remove_start:
	memset(buf,0,READ_BUFSIZE);
	memset(buf_copy,0,READ_BUFSIZE);
	n=fread(buf,1,READ_BUFSIZE,table_fd);
	if(n<(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)||n%(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)!=0){
		TRACE("table file read fail or reach the file end,err:%s,n:%lld \n",strerror(errno),n);
		goto remove_end;
	}
	
	TRACE("read table file %lld B,file_pos:%lld,pos_begin:%lld,max:%llx #",n,file_pos,pos_begin,max);
	file_pos+=n;
	
	chain_item_count=n/(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH);
	pos_begin=pos_end=0;
	j=0;
	for(i=0;i<chain_item_count-1;i++){
		TempB=StrToLong_6B(buf+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		if(TempB==max){
			pos_begin++;
		}
		else
			break;
	}
	
	for(;i<chain_item_count-1;i++){
		TempA=StrToLong_6B(buf+(i+1)*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		TempB=StrToLong_6B(buf+i*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
		
		if(TempA==TempB){
			pos_end=i+1;
			if(pos_end>pos_begin){
				memcpy(buf_copy+j*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),
					buf+pos_begin*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),
					(pos_end-pos_begin)*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH));
				j+=(pos_end-pos_begin);
			}
			pos_begin=pos_end+1;
		}
	}
	if(pos_begin<chain_item_count){//处理buf最后的一部分
		memcpy(buf_copy+j*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),
			buf+pos_begin*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),
			(chain_item_count-pos_begin)*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH));
		j+=(chain_item_count-pos_begin);
	}
	n=fwrite(buf_copy,1,j*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH),copyfile_fd);
	max=StrToLong_6B(buf+(chain_item_count-1)*(CHAIN_HEADER_LENGTH+CHAIN_TAIL_LENGTH)+CHAIN_HEADER_LENGTH);
	TRACE("write copy file %lld B!i:%lld,j:%lld,max:%llx\n",n,i,j,max);
	goto remove_start;
remove_end:
	if(buf)
		free(buf);
	if(buf_copy)
		free(buf_copy);
	if(table_fd>0){
		fclose(table_fd);
		table_fd=NULL;
	}

	if(copyfile_fd>0){
		fclose(copyfile_fd);
		copyfile_fd=NULL;
	}
	TRACE("remove_repeate_item finished!\n");
	return;
}


int init_config_file(char* table_file){
	char str[128]={0};
	record_file_fd=fopen(table_file,"rb");
	if(record_file_fd==NULL){
		TRACE("chain.cfg open fail,pls check\n");
		return -1;
	}

	while(!feof(record_file_fd) && fgets(str, 128, record_file_fd)){//
		char *buf=alltrim(str);
		if(memcmp(buf,"chain_table_pos:",16)==0)
		{
			chain_table_pos=strtoul((const char *)buf+16,0,10);
			if(chain_table_pos<0){
				TRACE("chain_table_pos err:%s\n",buf);
				return -1;
			}
			TRACE("read chain_table_pos :%ld\n",chain_table_pos);
		}
		else if(memcmp(buf,"create_chain_pos:",17)==0)
		{
			chain_header_pos=strtoul((const char *)buf+17,0,10);
			if(chain_header_pos<0){
				TRACE("chain_header_pos err:%s\n",buf);
				return -1;
			}
			TRACE("read chain_header_pos :%ld\n",chain_header_pos);
		}
		else if(memcmp(buf,"calc_table_pos:",15)==0)
		{
			calc_table_pos=strtoul((const char *)buf+15,0,10);
			if(calc_table_pos<0){
				TRACE("calc_table_pos err:%s\n",buf);
				return -1;
			}
			TRACE("read calc_table_pos :%d\n",calc_table_pos);
		}
		else if(memcmp(buf,"calc_chain_pos:",15)==0)
		{
			calc_chain_pos=strtoul((const char *)buf+15,0,10);
			if(calc_chain_pos<0){
				TRACE("calc_chain_pos err:%s\n",buf);
				return -1;
			}
			TRACE("read calc_chain_pos :%ld\n",calc_chain_pos);
		}
		else if(memcmp(buf,"sort_chain_pos:",15)==0)
		{
			sort_chain_pos=strtoul((const char *)buf+15,0,10);
			if(sort_chain_pos<0){
				TRACE("sort_chain_pos err:%s\n",buf);
				return -1;
			}
			TRACE("read sort_chain_pos :%ld\n",sort_chain_pos);
		}
	}
	if(record_file_fd){
		fclose(record_file_fd);
		record_file_fd=NULL;
	}
	return 0;
}

void repair_cw(unsigned char *cw){
	int i;
	for(i=0;i<2;i++)
		cw[4*i+3]=(cw[4*i]+cw[4*i+1]+cw[4*i+2])&0xFF;
}
void test(void){
	int i,j;// 
	unsigned char input[15]={0x24,0x45,0x1C,0xC1,0x6D,0xDB,0x13,0xE6,0xB5,0x93,0x5E,0xD6,0xB0,0xD7,0xA5},output[7]={0};
	unsigned char ib[8]={0},cb[8]={0},cw[8]={0x3b,0xba,0xc0,0,0xa0,0xda,0,0};//{0x0A,0x7F,0x4C,0,0x72,0xA8,0,0};//,zero[8]={0}
	TRACE("begin time:%d\n",(int)time((time_t *) 0));
	for(j=0;j<5000;j++){
		repair_cw(cw);
		//memset(ib,0,8);
		//memset(cb,0,8);
		//memset(output,0,7);
		dump_packet_line("cw",cw,8);
		stream_cypher(1, cw, input, ib);
		stream_cypher(0, cw,NULL, cb); 
		for(i=0;i<7; i++)  output[i] = input[i+8] ^ cb[i];//output is tail
		for(i=0;i<3;i++){
			cw[i]=(output[i]+j)%256;
			cw[i+4]=(output[i+3]+j)%256;
		}/**/
		dump_packet_line("cb",cb,8);
		dump_packet_line("output",output,7);	
		//dump_packet_line("cw",cw,8);
		TRACE("============================\n");
	}
	TRACE("end time:%d\n",(int)time((time_t *) 0));
	
	
	return;
}


int get_up_triangle_tail(unsigned char *plaintext_6B){
	int i,j,k;
	unsigned char input[15]={0x24,0x45,0x1C,0xC1,0x6D,0xDB,0x13,0xE6,0xB5,0x93,0x5E,0xD6,0xB0,0xD7,0xA5},output[7]={0};
	unsigned char ib[8]={0},cb[8]={0},cw[8]={0};//{0x0A,0x7F,0x4C,0,0x72,0xA8,0,0};//,zero[8]={0}
	TRACE("begin time:%d\n",(int)time((time_t *) 0));
	dump_packet_line("plaintext_6B", plaintext_6B,6);
	for(i=0;i<MAX_QUERY_TIMES;i++){// //i:column no
		memcpy(output,plaintext_6B,6);
		for(j=i;j<MAX_QUERY_TIMES;j++){
			for(k=0;k<3;k++){
				cw[k]=(output[k]+j)%256;
				cw[k+4]=(output[k+3]+j)%256;
			}
			repair_cw(cw);
			TRACE("i:%05d,loop:%05d ",i,j);
			dump_packet_oneline("cw", cw,8);
			stream_cypher(1, cw, input, ib);
			stream_cypher(0, cw,NULL, cb); 
			dump_packet_oneline("cb", cb,8);
			for(k=0;k<7; k++)  output[k] = input[k+8] ^ cb[k];//output is tail
			dump_packet_line(" output", output,7);
			//if(j==11) break;
		}
		
		memcpy(calc_tail[i].tail,output,CHAIN_TAIL_LENGTH);
		TRACE("i:%05d ",i);
		dump_packet_line("up_triangle", output,CHAIN_TAIL_LENGTH);
		if(i==11) break;// 
	}
	TRACE("end time:%d\n",(int)time((time_t *) 0));
	return 0;
}

int calc_down_triangle(void){
	int i,j,k;
	unsigned char input[15]={0x24,0x45,0x1C,0xC1,0x6D,0xDB,0x13,0xE6,0xB5,0x93,0x5E,0xD6,0xB0,0xD7,0xA5},output[7]={0};
	unsigned char ib[8]={0},cb[8]={0},cw[8]={0};
	for(i=0;i<MAX_QUERY_TIMES;i++){
		if(calc_tail[i].match){
			for(k=0;k<3;k++){
				cw[k]=calc_tail[i].header[k];
			}
			cw[4]=calc_tail[i].header[3];
			cw[5]=calc_tail[i].header[4];
			if(i==0){ 
				repair_cw(cw);
				memcpy(calc_tail[i].cw,cw,8);
			}
			else{
				for(j=0;j<i-1;j++){
					repair_cw(cw);
					printf("j:%d ",j);
					dump_packet_oneline("cw", cw,8);
					stream_cypher(1, cw, input, ib);
					stream_cypher(0, cw,NULL, cb); 
					dump_packet_oneline("cb", cb,8);
					for(k=0;k<7; k++)  output[k] = input[k+8] ^ cb[k];//output is tail
					dump_packet_line("output", output,7);
					for(k=0;k<3;k++){
						cw[k]=(output[k]+j+1)%256;
						cw[k+4]=(output[k+3]+j+1)%256;
					}
				}
				memcpy(calc_tail[i].cw,cw,8);
			}
			dump_packet_line("header", calc_tail[i].header,CHAIN_HEADER_LENGTH);
			TRACE("i:%05d ",i);
			dump_packet_line("cw", cw,8);
		}
	}
	return 0;
}
int main(int argc,char *  argv[])
{
	char *dev = argv[1];
	init_tsp_reg(dev);
	return 0;
	init_signal();
	//init_config_file("chain.cfg");
	#if 1
	int i;
	memset(&calc_tail,0,sizeof(calc_tail_stru)*MAX_QUERY_TIMES);
	//
	//unsigned long long ttt=StrToLong_6B(temp_tail);
	//TRACE("TTT:%llx\n",ttt);
	
	if(1)
	{
		unsigned char plain_text[6]={0x11,0x05,0x82,0x63,0xe8,0xca};//{0x10,0x00,0x00,0x00,0x00,0x00};//
		/*int fd = open("/dev/urandom", O_RDONLY);
		for(i=0;i<MAX_QUERY_TIMES;i++){
			read(fd,calc_tail[i].tail,CHAIN_TAIL_LENGTH);
		}*/
		get_up_triangle_tail(plain_text);
		test_hdd_speed_multithread("file1_no00.bin");
		calc_down_triangle();
	}
	else
	{
		unsigned char temp_tail[CHAIN_TAIL_LENGTH]={0xff,0xff,0xff,0xd7,0x2c,0x0b};//{0xff,0xff,0xff,0x04,0x0a,0x32};
		//calc_tail[0].tail=StrToLong_6B(temp_tail);
		memcpy(calc_tail[0].tail,temp_tail,CHAIN_TAIL_LENGTH);
		test_hdd_speed("file20_no19.bin");
	}
	for(i=0;i<MAX_QUERY_TIMES;i++){
		if(calc_tail[i].match){
			TRACE("calc_tail[%d] match is 1,tail is",i);
			dump_packet_line("", calc_tail[i].tail,CHAIN_TAIL_LENGTH);
		}
	}
	#else
	{
		//char file_name[64]="./xaa";
		//create_chain_header_func(file_name);
		//sort_seg_func(file_name);
		//merge_seg_func(2);
		
		//remove_repeate_item("file1_no00.bin");
		//check_sort_result("file1_no00.bin");
		//create_dic_index("file1_no00.bin");
	}
	#endif
	//restore_create_chain_pos();
	//test_hdd_speed_multithread("file1_no0.bin");
	return 0;
}



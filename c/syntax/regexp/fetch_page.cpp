//export LD_LIBRARY_PATH=libiconv-1.14/__install/lib
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>
#include <iconv.h>
#include <errno.h>

typedef struct
{
	int len; //当前字符串data的长度
	char *data; //返回的字符串
}DATA; //保存libcurl回调函数中返回的数据

typedef struct node
{
	char *url;
	char *topic;

	struct node *next;
}LIST; //一个链表结构，用来缓存第一次扫描贴吧时帖子的地址与标题

size_t get_data(char *ptr,size_t size,size_t nmemb,DATA *data) //libcurl返回数据的回调函数，使用动态字符串的方式保存返回的数据
{
	if(data->len)
	{
		char *temp;

		temp=(char *)malloc(data->len+1);
		snprintf(temp,data->len+1,"%s",data->data);
		free(data->data);

		data->data=(char *)malloc(data->len+nmemb+1);
		snprintf(data->data,data->len+nmemb+1,"%s%s",temp,ptr);
		free(temp);
	}
	else
	{
		data->data=(char *)malloc(nmemb+1);
		snprintf(data->data,nmemb+1,"%s",ptr);
	}

	data->len+=nmemb;
	return nmemb;
}

LIST *list_init(void) //链表的初始化
{
	LIST *list;

	list=(LIST *)malloc(sizeof(LIST));
	list->url=NULL;
	list->topic=NULL;
	list->next=NULL;

	return list;
}

void list_add(LIST *list,char *url,char *topic) //将链表中添加数据
{
	LIST *temp;

	while(list->next)
		list=list->next;

	temp=(LIST *)malloc(sizeof(LIST));
	temp->url=(char *)malloc(strlen(url)+1);
	snprintf(temp->url,strlen(url)+1,"%s",url);
	temp->topic=(char *)malloc(strlen(topic)+1);
	snprintf(temp->topic,strlen(topic)+1,"%s",topic);

	temp->next=NULL;
	list->next=temp;
}

void list_destroy(LIST *list) //销毁链表，释放内存
{
	LIST *temp;

	while(list->next)
	{
		list=list->next;

		temp=list;
		free(list->url);
		free(list->topic);
		free(temp);
	}
}

int gbk_to_utf8(char *in,char *out,size_t out_bytes) //由于百度贴吧使用的是GBK编码，而我本机是使用UTF-8编码的，所以需要进行编码转换
{
	iconv_t cd;
	size_t in_bytes=strlen(in);

	//if((cd=iconv_open("UTF-8//","GBK//IGNORE")) == (iconv_t)-1)
	if((cd=iconv_open("UTF-8","UTF-8")) == (iconv_t)-1)//for my utf-8 encoder source
		return -1;
	if(iconv(cd,&in,&in_bytes,&out,&out_bytes) == -1)
		return -1;

	iconv_close(cd);

	return 0;
}

LIST *topic_list(char *str) //第一次扫描百度贴吧，获取url与标题并缓存在链表中
{
	LIST *list=NULL;
	regex_t reg;
	regmatch_t pmatch[3]; //两个子表达式
	char topic[1024]={0};
	char url[128]={0};
	char temp[1024]={0};

	list=list_init();

	if(regcomp(&reg,"href=\"\\(/p/[0-9]\\{6,10\\}\\)\" title=\"\\(.[^\"]*\\)\" target",0) != 0) //对贴吧中含有帖子与帖子链接的字符进行匹配的正则表达式，并使用子表达式匹配出链接与标题
	{
		perror("regcomp"); //这里只是简单地debug，正确的提取错误的方法是使用regerror
		regfree(&reg);
		return NULL;
	}
	while(regexec(&reg,str,3,pmatch,0) == 0) //循环匹配
	{
		bzero(url,sizeof(url));
		snprintf(url,pmatch[1].rm_eo-pmatch[1].rm_so+23,
				"http://tieba.baidu.com%s",str+pmatch[1].rm_so); //取出第一个子表达式即帖子链接地址
		bzero(temp,sizeof(temp));
		snprintf(temp,pmatch[2].rm_eo-pmatch[2].rm_so+1,
				"%s",str+pmatch[2].rm_so); //取出第二个子表达式结果即帖子的标题

		bzero(topic,sizeof(topic));
		gbk_to_utf8(temp,topic,sizeof(topic)-1); //进行编码转换
		list_add(list,url,topic); //将结果缓存到链表中

		str+=pmatch[0].rm_eo; //移动字符串首地址到前面匹配的结果的未尾
	}
	regfree(&reg);

	return list;
}

void list_print(LIST *list) //打印链表中的内容
{
	while(list->next)
	{
		list=list->next;

		printf("%s %s\n",list->url,list->topic);
	}
}

void print_topic_and_post_time(CURL *curl,LIST *list,char *type) //打印标题与发帖时间的函数
{
	regex_t reg;
	regmatch_t pmatch[1];
	char temp[20];
	DATA data;
	LIST *head;

	if(strcmp(type,"1") == 0)
		head=list_init();
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);
	if(regcomp(&reg,"[0-9]\\{4\\}-[0-9]\\{2\\}-[0-9]\\{2\\} [0-9]\\{2\\}:[0-9]\\{2\\}",0) != 0) //匹配出发帖时间的正则表达式
		return;

	while(list->next) //从缓存链表中取出链接进行访问并匹配出发帖时间
	{
		list=list->next;

		curl_easy_setopt(curl,CURLOPT_URL,list->url);
		data.data=NULL;
		data.len=0;
		curl_easy_perform(curl);

		if(data.len)
		{
			bzero(temp,sizeof(temp));

			if(regexec(&reg,data.data,1,pmatch,0) != 0)
				snprintf(temp,sizeof(temp),"未知发表时间");
			else
				snprintf(temp,pmatch[0].rm_eo-pmatch[0].rm_so+1,"%s",data.data+pmatch[0].rm_so); //取出发匹配结果

			if(strcmp(type,"1") == 0) // 由于获取帖子标题和发帖时间时链接不同，所以要分两步进行访问，第一步访问缓存地址与标题，第二步访问缓存中缓存的链接，所以在时间上可能会很长，百度贴 吧一页的帖子数量在50帖，这里采用两种方法进行输出，一、访问所有帖子链接并缓存到链表中然后一次性打印，二、每访问一个帖子链接便打印一次，显然由于 数量过多，前种方法在等待屏幕的输出方面需要一定的时间
				list_add(head,temp,list->topic);
			else
				printf("%s %s\n",temp,list->topic);

			free(data.data);
		}
	}
	regfree(&reg);

	if(strcmp(type,"1") == 0)
	{
		list_print(head);
		list_destroy(head);
	}
}

int main(int argc,char **argv)
{
	CURL *curl;
	CURLcode code;
	char tieba[128]={0};
	DATA data;
	LIST *list;

	data.len=0;
	data.data=NULL;
	if(argc < 3)
	{
		printf("tieba  [1|an other]\n");
		return -1;
	}
	snprintf(tieba,sizeof(tieba),"http://tieba.baidu.com/f?kw=%s",argv[1]); //贴吧链接
	curl=curl_easy_init();

	curl_easy_setopt(curl,CURLOPT_URL,tieba);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,get_data); //设置回调函数
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data); //设置回调函数的参数
	code=curl_easy_perform(curl);

	if(code != 0)
		return -1;
	if(data.len)
	{
		list=topic_list(data.data); //缓存帖子标题与地址
		free(data.data);
		if(list)
		{
			print_topic_and_post_time(curl,list,argv[2]); //打印帖子标题与发帖时间
			list_destroy(list);
		}
	}

	curl_easy_cleanup(curl);
	return 0;
}

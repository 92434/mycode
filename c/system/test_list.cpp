

/*================================================================
 *   Copyright (C) 2017年08月18日 肖飞 All rights reserved
 *
 *   文件名称：test_list.cpp
 *   创 建 者：肖飞

 *   创建日期：2017年08月18日 星期五 09时23分55秒
 *   修改日期：2017年08月18日 星期五 10时31分54秒
 *   描    述：
 *
 *================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct _mem_info {
	unsigned long long total_count;
	unsigned long long total_size;
	struct list_head head; //need to initilized
} mem_info_t;

typedef struct _node {
	struct list_head list;

	unsigned long long size;
	void *ptr;
} node_t;

mem_info_t mem_info = {0};

int init_mem_info()
{
	int ret = 0;
	mem_info.total_count = 0;
	mem_info.total_size = 0;
	INIT_LIST_HEAD(&mem_info.head);
	printf("mem_info.head:%p\n", &mem_info.head);
	return ret;
}

int account_malloc(int size, void *ptr)
{
	int ret = 0;

	node_t *node = (node_t *)malloc(sizeof(node_t));

	if(node == NULL) {
		ret = -1;
		return ret;
	}

	mem_info.total_count += 1;
	mem_info.total_size += size;

	node->ptr = ptr;
	node->size = size;
	list_add_tail(&node->list, &mem_info.head);

	return ret;
}

int account_free(void *ptr)
{
	int ret = 0;
	node_t *node = NULL;
	bool found = false;

	if(list_empty(&mem_info.head)) {
		printf("meminfo:no meminfo!\n");
		ret = -1;
		return ret;
	}

	list_for_each_entry(node, &mem_info.head, node_t, list) {
		if(node->ptr == ptr) {
			found = true;
			break;
		}
	}

	if(!found) {
		printf("meminfo:not found mem block %p!\n", ptr);
		ret = -1;
		return ret;
	}

	mem_info.total_count -= 1;
	mem_info.total_size -= node->size;

	list_del(&node->list);
	free(node);

	return ret;
}

void *alloc_1(int size)
{
	int ret = 0;
	void *ptr = NULL;

	ptr = malloc(size);

	if(ptr == NULL) {
		return ptr;
	}

	ret = account_malloc(size, ptr);

	if(ret != 0) {
		free(ptr);
		ptr = NULL;
		return ptr;
	}

	return ptr;
}

void free_1(void *ptr)
{
	int ret = 0;
	ret = account_free(ptr);

	if(ret != 0) {
		return;
	}

	free(ptr);
}

int p_mem_info()
{
	int ret = 0;
	printf("meminfo:total_count:%llu, total_size:%llu\n", mem_info.total_count, mem_info.total_size);
	return ret;
}

int main(int argc, char **args)
{
	int ret = 0;
	void *p1, *p2, *p3, *p4, *p5;

	init_mem_info();
	free_1(p5);
	p_mem_info();
	p1 = alloc_1(1);
	free_1(p5);
	p_mem_info();
	p2 = alloc_1(2);
	free_1(p5);
	p_mem_info();
	p3 = alloc_1(3);
	free_1(p5);
	p_mem_info();
	free_1(p5);
	p4 = alloc_1(4);
	p_mem_info();
	free_1(p5);
	free_1(p4);
	p_mem_info();
	free_1(p5);
	free_1(p1);
	p_mem_info();
	free_1(p5);
	free_1(p3);
	p_mem_info();
	free_1(p5);
	free_1(p2);
	p_mem_info();
	free_1(p5);
	free_1(p2);
	p_mem_info();

	return ret;
}

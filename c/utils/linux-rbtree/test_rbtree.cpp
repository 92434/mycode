

/*================================================================
 *   Copyright (C) 2017年09月18日 肖飞 All rights reserved
 *
 *   文件名称：test_rbtree.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年09月18日 星期一 11时59分54秒
 *   修改日期：2017年09月25日 星期一 11时26分56秒
 *   描    述：
 *
 *================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include "test_rbtree.h"

typedef struct _data {
	struct rb_node node;
	int d;
} data_t;

data_t *my_search(struct rb_root *root, int d)
{
	struct rb_node *node = root->rb_node;

	while (node != NULL) {
		data_t *data = container_of(node, data_t, node);
		int result;

		result = d - data->d;

		if (result < 0) {
			node = node->rb_left;
		} else if (result > 0) {
			node = node->rb_right;
		} else {
			printf("find data %p for %d\n", data, d);
			return data;
		}
	}

	return NULL;
}

data_t *my_insert(struct rb_root *root, int d)
{
	struct rb_node **pnode = &(root->rb_node);
	struct rb_node *parent = NULL;

	data_t *new_data = (data_t *)malloc(sizeof(data_t));

	if(new_data == NULL) {
		return NULL;
	}

	printf("alloc new_data %p for %d\n", new_data, d);

	new_data->d = d;

	/* Figure out where to put new node */
	while (*pnode) {
		data_t *data = container_of(*pnode, data_t, node);
		int result = new_data->d - data->d;

		parent = *pnode;

		if (result < 0) {
			pnode = &((*pnode)->rb_left);
		} else if (result > 0) {
			pnode = &((*pnode)->rb_right);
		} else {
			printf("free new_data %p for %d(exist)!\n", new_data, d);
			free(new_data);
			return data;
		}
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&new_data->node, parent, pnode);
	rb_insert_color(&new_data->node, root);

	return new_data;
}

int my_erase(struct rb_root *root, int d)
{
	int ret = 0;
	data_t *data = my_search(root, d);

	if(data == NULL) {
		printf("can not find data for %d!\n", d);
		ret = -1;
		return ret;
	}

	if (data) {
		printf("free data %p for %d!\n", data, d);
		rb_erase(&data->node, root);
		free(data);
	}

	return ret;
}


static struct rb_root mytree = RB_ROOT;

#define TEST_COUNT 500000

int main(int argc, char **argv)
{
	int ret = 0;
	int i;

	for(i = 0; i < TEST_COUNT; i++) {
		printf("insert %d\n", i);
		my_insert(&mytree, i);
	}

	for(i = 0; i < TEST_COUNT; i++) {
		printf("erase %d\n", i);
		my_erase(&mytree, i);
	}

	return ret;
}

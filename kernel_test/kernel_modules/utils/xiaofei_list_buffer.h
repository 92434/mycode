#ifndef _XIAOFEI_LIST_BUFFER_H
#define _XIAOFEI_LIST_BUFFER_H
#include <linux/list.h>

typedef struct _buffer_node {
	struct list_head list;
	int size;
	int read_offset;
	int write_offset;
	char *buffer;
	void *buffer_addr;
	int avail_for_read;
	int avail_for_write;
	int base_addr_of_list_buffer;
} buffer_node_t;

typedef struct _list_buffer {
	int size;
	struct list_head *first;
	struct list_head *read;
	struct list_head *write;
} list_buffer_t;

list_buffer_t *init_list_buffer(void);
buffer_node_t *add_list_buffer_item(char *buffer, void *buffer_addr, int size, list_buffer_t *list);
void uninit_list_buffer(list_buffer_t *list);
int read_buffer(char *buffer, int size, list_buffer_t *list);
int write_buffer(char *buffer, int size, list_buffer_t *list);
int get_buffer_node_info(buffer_node_t *write_node, buffer_node_t *read_node, list_buffer_t *list);
bool read_available(list_buffer_t *list);
void start_test_buffer_list(void);

#endif//#ifndef _XIAOFEI_LIST_BUFFER_H

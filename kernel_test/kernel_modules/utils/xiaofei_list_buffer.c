#include "xiaofei_list_buffer.h"
#include "xiaofei_debug.h"
#include <linux/vmalloc.h>

list_buffer_t *init_list_buffer(void) {
	list_buffer_t *list = (list_buffer_t *)vzalloc(sizeof(list_buffer_t));
	if(list == NULL) {
		mydebug("alloc list_buffer_t failed.\n");
		goto exit;
	}

exit:
	return list;
}


static bool buffer_node_exist(buffer_node_t *node, list_buffer_t *list) {
	bool rtn = false;

	struct list_head *node_list = &(node->list);
	struct list_head *item, *next;
	
	list_for_each_safe(item, next, list->write) {
		if(node_list == item) {
			rtn = true;
		}
	}

	return rtn;
}

buffer_node_t *add_list_buffer_item(char *buffer, void *buffer_addr, int size, list_buffer_t *list) {
	struct list_head *node_list = NULL;
	buffer_node_t *node = NULL;

	if(list == NULL) {
		goto exit;
	}

	node = (buffer_node_t *)vzalloc(sizeof(buffer_node_t));
	if(node == NULL) {
		mydebug("alloc buffer_node_t failed.\n");
		goto exit;
	}

	node->size = size;
	node->buffer = buffer;
	node->buffer_addr = buffer_addr;
	node->read_offset = size;
	node->write_offset = 0;
	node_list = &(node->list);

	INIT_LIST_HEAD(node_list);

	if(list->write == NULL) {
		list->write = node_list;
		list->read = node_list;
	} else if(buffer_node_exist(node, list)) {
		vfree(node);
		node = NULL;
	}else {
		list_add(node_list, list->write);
	}

exit:
	return node;
}

void uninit_list_buffer(list_buffer_t *list) {
	buffer_node_t *node = NULL, *node_next = NULL;

	if(list == NULL) {
		return;
	}

	list_for_each_entry_safe(node, node_next, list->write, list) {
		vfree(node);
	}
	
	if((node != NULL) && (&(node->list) == list->write)) {
		vfree(node);
	}

	vfree(list);
}

int read_buffer(char *buffer, int size, list_buffer_t *list) {
	char *data;
	buffer_node_t *node;
	int end_offset;
	int read_count = 0;

	node = list_entry(list->read, buffer_node_t, list);

	if(node->read_offset == node->write_offset) {
		return read_count;
	}

	end_offset = node->read_offset + size;

	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((node->read_offset < node->write_offset) && (node->write_offset < end_offset)) {
		end_offset = node->write_offset;
	}

	myprintf("node:%p\n", (void *)node);
	myprintf("end_offset:%d\n", end_offset);
	myprintf("node->read_offset:%d\n\n", node->read_offset);

	data = node->buffer + node->read_offset;

	read_count = end_offset - node->read_offset;

	if(buffer != NULL) {
		memcpy(buffer, data, read_count);
	}

	node->read_offset += read_count;
	if(node->read_offset == node->size) {
		list->read = list->read->next;
	}

	return read_count;
}

int write_buffer(char *buffer, int size, list_buffer_t *list) {
	char *data;
	buffer_node_t *node;
	int end_offset;
	int write_count = 0;

	node = list_entry(list->write, buffer_node_t, list);
	
	if(node->write_offset == node->size) {
		node->write_offset = 0;
	}

	end_offset = node->write_offset + size;
	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((node->write_offset <= node->read_offset) && (node->read_offset < end_offset)) {
		myprintf("overwrite from %p!\n", (void *)(node->buffer + node->read_offset));
	}

	if(node->read_offset == node->size) {
		node->read_offset = 0;
	}

	data = node->buffer + node->write_offset;

	myprintf("node:%p\n", (void *)node);
	myprintf("end_offset:%d\n", end_offset);
	myprintf("node->write_offset:%d\n\n", node->write_offset);
	write_count = end_offset - node->write_offset;

	if(buffer != NULL) {
		memcpy(data, buffer, write_count);
	}

	node->write_offset += write_count;
	if(node->write_offset == node->size) {
		list->write = list->write->next;
	}

	return write_count;
}

#define BUFFER_COUNT 6
#define BUFFER_SIZE 4
static char *buffers[BUFFER_COUNT];

void start_test_buffer_list(void) {
	int i;
	list_buffer_t *list;
	char *data_buffer;
	char *data;
	int loop;

	for(i = 0; i < BUFFER_COUNT; i++) {
		buffers[i] = (char *)vzalloc(BUFFER_SIZE);
		if(buffers[i] == 0) {
			mydebug("\n");
			goto exit1;
		}
	}

	list = init_list_buffer();
	if(list == NULL) {
		mydebug("\n");
		goto exit1;
	}

	for(i = 0; i < BUFFER_COUNT; i++) {
		add_list_buffer_item(buffers[i], 0, BUFFER_SIZE, list);
	}

	//write large data continuely
	data_buffer = (char *)vzalloc(BUFFER_COUNT * BUFFER_SIZE);
	for(i = 1; i <= BUFFER_COUNT * BUFFER_SIZE; i++) {
		data_buffer[i] = i;
	}

	i = BUFFER_COUNT * BUFFER_SIZE;
	data = data_buffer;
	loop = 0;
	while(i > 0) {
		int n;
		n = write_buffer(data, 1, list);
		data += n;
		i -= n;
	}
	read_buffer(data_buffer + 0, 2, list);
	write_buffer(data_buffer + 8, 2, list);
	write_buffer(data_buffer + 10, 2, list);
	read_buffer(data_buffer + 2, 2, list);
	write_buffer(data_buffer + 12, 2, list);
	write_buffer(data_buffer + 14, 2, list);
	//write large data continuely

	//read large data continuely
	i = BUFFER_COUNT * BUFFER_SIZE - 4;
	data = data_buffer + 4;
	loop = 0;
	while(i > 0) {
		int n;
		n = read_buffer(data, 1, list);
		data += n;
		i -= n;
	}

	printk("\n");
	for(i = 0; i < BUFFER_COUNT * BUFFER_SIZE; i++) {
		if((i != 0) && (i % 8 == 0)) {
			printk("\n");
		}
		printk("%02x ", data_buffer[i]);
	}
	printk("\n");
	//read large data continuely

	vfree(data_buffer);

	uninit_list_buffer(list);
exit1:
	for(i = 0; i < BUFFER_COUNT; i++) {
		if(buffers[i] != 0) {
			vfree(buffers[i]);
		}
	}

}

#include <linux/vmalloc.h>
#include "xiaofei_debug.h"
#include "xiaofei_list_buffer.h"

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
	int read_offset, write_offset;

	node = list_entry(list->read, buffer_node_t, list);
	read_offset = node->read_offset;
	write_offset = node->write_offset;

	if(read_offset == write_offset) {
		return read_count;
	}

	end_offset = read_offset + size;

	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((read_offset < write_offset) && (write_offset < end_offset)) {
		end_offset = write_offset;
	}

	myprintf("node:%p\n", (void *)node);
	myprintf("end_offset:%d\n", end_offset);
	myprintf("node->read_offset:%d\n\n", node->read_offset);

	data = node->buffer + node->read_offset;

	read_count = end_offset - node->read_offset;

	if(buffer != NULL) {
		memcpy(buffer, data, read_count);
	}

	read_offset += read_count;

	if(read_offset == node->size) {
		list->read = list->read->next;
	}
	
	node->read_offset = read_offset;

	return read_count;
}

int write_buffer(char *buffer, int size, list_buffer_t *list) {
	char *data;
	buffer_node_t *node;
	int end_offset;
	int write_count = 0;
	int read_offset, write_offset;

	node = list_entry(list->write, buffer_node_t, list);

	if(node->write_offset == node->size) {
		node->write_offset = 0;
	}
	read_offset = node->read_offset;
	write_offset = node->write_offset;
	

	end_offset = write_offset + size;
	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((write_offset <= read_offset) && (read_offset < end_offset)) {
		myprintf("overwrite from %p!\n", (void *)(node->buffer + read_offset));
	}

	if(read_offset == node->size) {
		node->read_offset = 0;
	}

	data = node->buffer + write_offset;

	myprintf("node:%p\n", (void *)node);
	myprintf("end_offset:%d\n", end_offset);
	myprintf("write_offset:%d\n\n", write_offset);
	write_count = end_offset - write_offset;

	if(buffer != NULL) {
		memcpy(data, buffer, write_count);
	}

	write_offset += write_count;
	if(write_offset == node->size) {
		list->write = list->write->next;
	}

	node->write_offset = write_offset;

	return write_count;
}

bool list_buffer_empty(list_buffer_t *list) {
	buffer_node_t *node;

	node = list_entry(list->read, buffer_node_t, list);

	return (node->read_offset == node->write_offset);
}

int get_buffer_node_info(buffer_node_t *write_node, buffer_node_t *read_node, list_buffer_t *list) {
	int rtn = 0;
	char *data;
	buffer_node_t *node;
	int end_offset;
	int write_count, read_count;
	int write_offset, read_offset;

	if((write_node == NULL) || (read_node == NULL)) {
		rtn = -1;
		return rtn;
	}

	node = list_entry(list->write, buffer_node_t, list);
	*write_node = *node;

	if(write_node->write_offset == write_node->size) {
		write_node->write_offset = 0;
	}
	write_offset = write_node->write_offset;
	end_offset = write_node->size;
	write_count = end_offset - write_offset;
	write_node->avail_for_write = write_count;
	printk("write_node->avail_for_write:%d\n", write_node->avail_for_write);


	node = list_entry(list->read, buffer_node_t, list);
	*read_node = *node;

	read_offset = read_node->read_offset;
	end_offset = read_node->write_offset;
	if((read_offset > read_node->write_offset)) {
		end_offset = read_node->size;
	}
	read_count = end_offset - read_node->read_offset;

	read_node->avail_for_read = read_count;
	printk("read_node->avail_for_read:%d\n", read_node->avail_for_read);

	return rtn;
}

#define BUFFER_COUNT 1
#define BUFFER_SIZE 24
static char *buffers[BUFFER_COUNT];

void start_test_buffer_list(void) {
	int i;
	list_buffer_t *list;
	char *data_buffer;
	char *data;
	int loop;
	buffer_node_t read, write;

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
		get_buffer_node_info(&write, &read, list);
		n = write_buffer(data, 1, list);
		data += n;
		i -= n;
	}
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer + 0, 2, list);
	//write_buffer(data_buffer + 8, 2, list);
	//write_buffer(data_buffer + 10, 2, list);
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer + 2, 2, list);
	//write_buffer(data_buffer + 12, 2, list);
	//write_buffer(data_buffer + 14, 2, list);
	//write large data continuely

	//read large data continuely
	i = BUFFER_COUNT * BUFFER_SIZE - 4;
	data = data_buffer + 4;
	loop = 0;
	while(i > 0) {
		int n;
		get_buffer_node_info(&write, &read, list);
		n = read_buffer(data, 1, list);
		data += n;
		i -= n;
	}

	get_buffer_node_info(&write, &read, list);
	write_buffer(data_buffer + 10, 2, list);
	get_buffer_node_info(&write, &read, list);
	read_buffer(data_buffer, 2, list);

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

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


static bool buffer_node_exist(char *buffer, list_buffer_t *list) {
	bool ret = false;

	buffer_node_t *node = NULL, *node_next = NULL;
	
	if((list == NULL) || (list->first == NULL)) {
		return ret;
	}

	node = list_prepare_entry(node, list->first, list);
	if(node->buffer == buffer) {
		ret = true;
		return ret;
	}

	list_for_each_entry_safe(node, node_next, list->first, list) {
		if(node->buffer == buffer) {
			ret = true;
			return ret;
		}
	}

	return ret;
}

buffer_node_t *add_list_buffer_item(char *buffer, void *buffer_addr, int size, list_buffer_t *list) {
	struct list_head *node_list = NULL;
	buffer_node_t *node = NULL;

	if(list == NULL) {
		goto exit;
	}

	if(buffer_node_exist(buffer, list)) {
		goto exit;
	}

	node = (buffer_node_t *)vzalloc(sizeof(buffer_node_t));
	if(node == NULL) {
		mydebug("alloc buffer_node_t failed.\n");
		goto exit;
	}

	//printk("alloc node:%p\n", (void *)node);

	node->size = size;
	node->buffer = buffer;
	node->buffer_addr = buffer_addr;
	node->read_offset = size;
	node->write_offset = 0;
	node->base_addr_of_list_buffer = list->size;
	//printk("node->base_addr_of_list_buffer:%d\n", node->base_addr_of_list_buffer);

	list->size += node->size;
	node_list = &(node->list);
	

	INIT_LIST_HEAD(node_list);

	if(list->first == NULL) {
		list->first = node_list;
		list->write = node_list;
		list->read = node_list;
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

	if(list->first == NULL) {
		goto release_list;
	}

	list_for_each_entry_safe(node, node_next, list->first, list) {
		vfree(node);
		//printk("free node:%p\n", (void *)node);
	}

	vfree(node);
	//printk("free node:%p\n", (void *)node);

release_list:
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
		goto exit;
	}

	end_offset = read_offset + size;

	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((read_offset < write_offset) && (write_offset < end_offset)) {
		end_offset = write_offset;
	}

	//myprintf("node:%p\n", (void *)node);
	//myprintf("end_offset:%d\n", end_offset);
	//myprintf("node->read_offset:%d\n\n", node->read_offset);

	data = node->buffer + read_offset;

	read_count = end_offset - read_offset;

	if(buffer != NULL) {
		memcpy(buffer, data, read_count);
	}

	read_offset += read_count;
	node->read_offset = read_offset;

exit:
	if(read_offset == node->size) {
		list->read = list->read->next;
	}

	return read_count;
}

int write_buffer(char *buffer, int size, list_buffer_t *list) {
	char *data;
	buffer_node_t *node;
	int end_offset;
	int write_count = 0;
	int read_offset, write_offset;

	node = list_entry(list->write, buffer_node_t, list);

	read_offset = node->read_offset;

	if(node->write_offset == node->size) {
		node->write_offset = 0;
		if(read_offset == 0) {
			buffer_node_t *node1;

			node1 = list_entry(list->read, buffer_node_t, list);
			myprintf("overwrite from %p!\n", (void *)(node->buffer + read_offset));
			myprintf("overwrite:node->read_offset:%x\n", node->read_offset);
			myprintf("overwrite:node1->buffer:%p\n", node1->buffer);
			myprintf("overwrite:node1->read_offset:%x\n", node1->read_offset);
			myprintf("overwrite:node1->write_offset:%x\n", node1->write_offset);
		}
	}

	write_offset = node->write_offset;
	
	end_offset = write_offset + size;
	if(end_offset > node->size) {
		end_offset = node->size;
	}

	if((write_offset < read_offset) && (read_offset <= end_offset)) {
		buffer_node_t *node1;

		node1 = list_entry(list->read, buffer_node_t, list);
		myprintf("overwrite from %p!\n", (void *)(node->buffer + read_offset));
		myprintf("overwrite:node->read_offset:%x\n", node->read_offset);
		myprintf("overwrite:node1->buffer:%p\n", node1->buffer);
		myprintf("overwrite:node1->read_offset:%x\n", node1->read_offset);
		myprintf("overwrite:node1->write_offset:%x\n", node1->write_offset);
	}

	data = node->buffer + write_offset;

	//myprintf("node:%p\n", (void *)node);
	//myprintf("end_offset:%d\n", end_offset);
	//myprintf("write_offset:%d\n\n", write_offset);

	write_count = end_offset - write_offset;

	if(buffer != NULL) {
		memcpy(data, buffer, write_count);
	}

	write_offset += write_count;
	if(write_offset == node->size) {
		list->write = list->write->next;
	}

	node->write_offset = write_offset;
	if(read_offset == node->size) {
		node->read_offset = 0;
	}

	return write_count;
}

int get_buffer_node_info(buffer_node_t *write_node, buffer_node_t *read_node, list_buffer_t *list) {
	int ret = 0;
	buffer_node_t *node;
	int end_offset;
	int write_count, read_count;
	int write_offset, read_offset;

	if((write_node == NULL) && (read_node == NULL)) {
		ret = -1;
		return ret;
	}

	if(write_node != NULL) {
		node = list_entry(list->write, buffer_node_t, list);
		*write_node = *node;

		if(write_node->write_offset == write_node->size) {
			write_node->write_offset = 0;
		}
		write_offset = write_node->write_offset;

		end_offset = write_node->size;
		write_count = end_offset - write_offset;
		write_node->avail_for_write = write_count;
		//printk("write_node->avail_for_write:%d\n", write_node->avail_for_write);
	}


	if(read_node != NULL) {
		node = list_entry(list->read, buffer_node_t, list);
		*read_node = *node;

		read_offset = read_node->read_offset;
		write_offset = read_node->write_offset;

		end_offset = write_offset;
		if((read_offset > write_offset)) {
			end_offset = read_node->size;
		}
		read_count = end_offset - read_offset;

		read_node->avail_for_read = read_count;
		//printk("read_node->avail_for_read:%d\n", read_node->avail_for_read);
	}

	return ret;
}

bool read_available(list_buffer_t *list) {
	buffer_node_t read;
	bool available = false;
	int ret;

	ret = get_buffer_node_info(NULL, &read, list);
	if(ret != 0) {
		return available;
	}

	if(read.avail_for_read != 0) {
		available = true;
	}

	return available;
}

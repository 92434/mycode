#include <linux/vmalloc.h>

#include "xiaofei_debug.h"
#include "xiaofei_list_buffer.h"

list_buffer_t *init_list_buffer(void) {
	list_buffer_t *list = (list_buffer_t *)vzalloc(sizeof(list_buffer_t));
	if(list == NULL) {
		mydebug("alloc list_buffer_t failed.\n");
		goto exit;
	}

	mutex_init(&list->list_buffer_lock);

exit:
	return list;
}

void disable_list_buffer_overwrite(list_buffer_t *list, bool disable) {
	list->disable_overwrite = disable;
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
	node->read_offset = 0;
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
	buffer_node_t *read_node;
	buffer_node_t local_node;
	int read_count = 0;
	int read_start, read_end;
	int read_offset, write_offset;

	mutex_lock(&list->list_buffer_lock);
	read_node = list_entry(list->read, buffer_node_t, list);
	local_node = *read_node;


	read_offset = local_node.read_offset;
	write_offset = local_node.write_offset;

	read_start = read_offset;
	read_end = read_start + size;

	if(read_end > local_node.size) {
		read_end = local_node.size;
	}

	if((read_start <= write_offset) && (write_offset < read_end)) {
		read_end = write_offset;
	}

	//myprintf("read read_node:%p(%d-%d)\n", (void *)read_node, read_start, read_end);

	data = local_node.buffer + read_start;

	read_count = read_end - read_start;

	if((buffer != NULL) && (read_count != 0)) {
		memcpy(buffer, data, read_count);
	}

	read_node->read_offset = read_end;

	if(read_end == local_node.size) {
		list->read = list->read->next;
		read_node = list_entry(list->read, buffer_node_t, list);
		read_node->read_offset = 0;
	}
	mutex_unlock(&list->list_buffer_lock);

	return read_count;
}

int write_buffer(char *buffer, int size, list_buffer_t *list) {
	char *data;
	buffer_node_t *write_node;
	buffer_node_t *read_node;
	buffer_node_t local_node;
	buffer_node_t local_read_node;
	int write_count = 0;
	int write_start, write_end;
	int read_offset, write_offset;

	mutex_lock(&list->list_buffer_lock);
	write_node = list_entry(list->write, buffer_node_t, list);
	read_node = list_entry(list->read, buffer_node_t, list);
	local_node = *write_node;
	local_read_node = *read_node;

	read_offset = local_node.read_offset;
	write_offset = local_node.write_offset;

	write_start = write_offset;

	write_end = write_start + size;
	if(write_end > local_node.size) {
		write_end = local_node.size;
	}
	
	{

		if(
				(list->read == list->write)
				&& (write_start < read_offset)
				&& (read_offset <= write_end)
				) {
			myprintf("will overwrite at:%p(%d-%d)-%p(%d)!\n", (void *)write_node, write_start, write_end, (void *)read_node, read_offset);
			if(list->disable_overwrite) {
				write_count = -1;
				return write_count;
			}
		} else if(
				(list->read == list->write->next)
				&& (write_end == local_node.size)
				&& (local_read_node.read_offset == 0)
				) {
			myprintf("will overwrite at:%p(%d-%d)-%p(%d)!\n", (void *)write_node, write_start, write_end, (void *)read_node, local_read_node.read_offset);
			if(list->disable_overwrite) {
				write_count = -1;
				return write_count;
			}
		}
	}

	data = local_node.buffer + write_start;

	//myprintf("write write_node:%p(%d-%d)\n", (void *)write_node, write_start, write_end);

	write_count = write_end - write_start;

	if((buffer != NULL) && (write_count != 0)) {
		memcpy(data, buffer, write_count);
	}

	write_node->write_offset = write_end;

	if(write_end == write_node->size) {
		list->write = list->write->next;
		write_node = list_entry(list->write, buffer_node_t, list);
		write_node->write_offset = 0;
	}
	mutex_unlock(&list->list_buffer_lock);

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
		mutex_lock(&list->list_buffer_lock);
		node = list_entry(list->write, buffer_node_t, list);
		*write_node = *node;
		mutex_unlock(&list->list_buffer_lock);

		if(write_node->write_offset == write_node->size) {
			write_node->write_offset = 0;
		}
		write_offset = write_node->write_offset;

		end_offset = write_node->size;
		write_count = end_offset - write_offset;
		write_node->avail_for_write = write_count;
		//myprintf("write_node->avail_for_write:%d\n", write_node->avail_for_write);
	}


	if(read_node != NULL) {
		mutex_lock(&list->list_buffer_lock);
		node = list_entry(list->read, buffer_node_t, list);
		*read_node = *node;
		mutex_unlock(&list->list_buffer_lock);

		read_offset = read_node->read_offset;
		write_offset = read_node->write_offset;

		end_offset = write_offset;
		if((read_offset > write_offset)) {
			end_offset = read_node->size;
		}
		read_count = end_offset - read_offset;

		read_node->avail_for_read = read_count;
		//myprintf("read_node->avail_for_read:%d\n", read_node->avail_for_read);
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

void reset_list_buffer(list_buffer_t *list) {
	buffer_node_t *node = NULL, *node_next = NULL;

	if(list == NULL) {
		return;
	}

	if(list->first == NULL) {
		return;
	}

	mutex_lock(&list->list_buffer_lock);
	list_for_each_entry_safe(node, node_next, list->first, list) {
		node->write_offset = node->read_offset = 0;
	}

	node->write_offset = node->read_offset = 0;

	list->write = list->read = list->first;

	mutex_unlock(&list->list_buffer_lock);
}

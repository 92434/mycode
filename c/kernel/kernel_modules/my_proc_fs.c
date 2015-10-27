//#define DEBUG_FILE_NAME_LEN  60
LIST_HEAD(fb_debug_info);
static struct mutex fb_debug_info_lock;		/* Lock for open/release/ioctl funcs */
typedef struct fb_debug_node{
	struct list_head list_head;
	char *filename;
	char *funcname;
	int line;
	int id;
	int status;
#define LOG_BUFFER_SIZE 64
	char log_info[LOG_BUFFER_SIZE];
	int used;
} fb_debug_node_t;

#define MEM_POOL_SIZE 128
typedef struct debug_mempool{
	fb_debug_node_t nodes[MEM_POOL_SIZE];
	int nospace;
} debug_mempool_t;

static debug_mempool_t mempool = {0};

static fb_debug_node_t *node_get(){
	fb_debug_node_t *node = NULL;
	int i;

	if(mempool.nospace){
		return node;
	}

	for(i = 0; i < MEM_POOL_SIZE; i++){
		if(!mempool.nodes[i].used){
			node = &mempool.nodes[i];
			node->used = 1;
			break;
		}
	}
	
	if(i == MEM_POOL_SIZE){
		mempool.nospace = 1;
	}

	return node;
}

static void node_put(fb_debug_node_t *node){
	node->used = 0;
	if(mempool.nospace){
		mempool.nospace = 0;
	}
}

static int c_show(struct seq_file *m, void *v)
{
	struct list_head *head = &fb_debug_info;
	struct list_head *list_pos = (struct list_head *)v;
	fb_debug_node_t *node = list_entry(list_pos, fb_debug_node_t, list_head);

	//seq_printf(m, "%s:%d", node->func_name, node->line);
	seq_printf(m, "%s:%s:%d:%d", node->filename, node->funcname, node->line, node->id);
	seq_printf(m, ":%s", node->log_info);
	seq_puts(m, "\n");

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	mutex_lock(&fb_debug_info_lock);
	loff_t index = 0;
	struct list_head *head = &fb_debug_info;
	struct list_head *list_pos = &fb_debug_info;

	if(*pos){
		list_for_each(list_pos, head){
			index++;
			if(index == *pos)
			{
				break;
			}
		}
	}

	list_pos = list_pos->next;
	return list_pos != head ? (void *)list_pos : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	struct list_head *head = &fb_debug_info;
	struct list_head *list_pos = (struct list_head *)v;

	++*pos;
	list_pos = list_pos->next;
	return list_pos != head ? list_pos : NULL;
}

static void c_stop(struct seq_file *m, void *v)
{
	mutex_unlock(&fb_debug_info_lock);
}
const struct seq_operations fb_debug_info_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= c_show
};

static int fb_debug_info_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &fb_debug_info_op);
}

static const struct file_operations proc_fb_debug_info_operations = {
	.open		= fb_debug_info_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static void trim_file_name(const char *path, char **name){
	*name = path;
	char *pos = path;

	while(pos = strchr(pos, '/')){
		*name = pos;
		pos += 1;
	}

	if(*name && **name == '/'){
		*name = (*name) + 1;
	}
}

void create_fb_debug_file()
{
	mutex_init(&fb_debug_info_lock);
	proc_create("xiaofei_fb_debug", 0, NULL, &proc_fb_debug_info_operations);
}
EXPORT_SYMBOL(create_fb_debug_file);

void xiaofei_deadlock_detect(const char *str_file, const char *str_func, const int line, int id, int status)
{
	struct list_head *head = &fb_debug_info;
	fb_debug_node_t *debug_node = NULL;

	mutex_lock(&fb_debug_info_lock);
	if(!list_empty(head)){
		struct list_head *list_pos;


		fb_debug_node_t *node;
		list_for_each(list_pos, head){
			node = list_entry(list_pos, fb_debug_node_t, list_head);
			
			if(node->filename != str_file){
				continue;
			}
			if(id != 0 && node->id != id){
				continue;
			}
			debug_node = node;
			break;
		}
	}

	if(debug_node && status == 0){
		list_del(&debug_node->list_head);
		node_put(debug_node);
	}else{
		if(debug_node && status == 1){
			//int printk(const char *fmt, ...);
			//printk("[xiaofei]: <%s:%s:%d>\n", str_file, str_func, line);
			debug_node = NULL;
		}

		if(!debug_node){
			debug_node = node_get();
		}

		if(debug_node){
			debug_node->filename = str_file;
			debug_node->funcname = str_func;
			debug_node->line = line;
			debug_node->id = id;
			debug_node->status = status;
			list_add_tail(&debug_node->list_head, head);
		}
	}
	mutex_unlock(&fb_debug_info_lock);
}
EXPORT_SYMBOL(xiaofei_deadlock_detect);

void xiaofei_trace_collect(const char *str_file, const char *str_func, const int line, const char *fmt, ...)
{
	struct list_head *head = &fb_debug_info;
	fb_debug_node_t *debug_node = NULL;

	mutex_lock(&fb_debug_info_lock);

	debug_node = node_get();
	if(!debug_node){
		if(!list_empty(head)){
			struct list_head *list_pos = head->next;

			list_del(list_pos);
			debug_node = list_entry(list_pos, fb_debug_node_t, list_head);
		}
	}

	if(debug_node){
		va_list args;

		va_start(args, fmt);
		snprintf(debug_node->log_info, LOG_BUFFER_SIZE, fmt, args);
		va_end(args);
		debug_node->filename = str_file;
		debug_node->funcname = str_func;
		debug_node->line = line;
		list_add_tail(&debug_node->list_head, head);
	}

	mutex_unlock(&fb_debug_info_lock);
}
EXPORT_SYMBOL(xiaofei_trace_collect);

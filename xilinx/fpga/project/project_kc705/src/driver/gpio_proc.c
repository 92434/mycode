#include <linux/proc_fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#include "utils/xiaofei_debug.h"

/*
 *test:
 *echo gpio pin_dir >/proc/test_gpio/gpio_direction
 *cat /proc/test_gpio/gpio_direction
 *
 *
 *
 * */

typedef struct {
	struct rb_node node;
	int gpio;
	bool open;
} gpio_proc_t;

#define BASE_DIR "gpio_test"

static struct proc_dir_entry *gpio_proc_dir; /* /proc/sal dir entry */
//static DEFINE_SPINLOCK(data_lock);
//spin_lock(&data_lock);
//spin_unlock(&data_lock);

struct rb_root mytree = RB_ROOT;

gpio_proc_t *my_search(struct rb_root *root, int gpio) {
	struct rb_node *node = root->rb_node;

	while (node) {
		gpio_proc_t *data = container_of(node, gpio_proc_t, node);
		int result;

		result = gpio - data->gpio;

		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, int gpio) {
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	int rtn = 0;

	gpio_proc_t *data = (gpio_proc_t *)vzalloc(sizeof(gpio_proc_t));
	if(data == NULL) {
		rtn = -1;
		mydebug("\n");
		return rtn;
	}

	data->gpio = gpio;

	/* Figure out where to put new node */
	while (*new) {
		gpio_proc_t *this = container_of(*new, gpio_proc_t, node);
		int result = data->gpio - this->gpio;

		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else {
			rtn = -1;
			mydebug("\n");
			return rtn;
		}
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return rtn;
}

int my_eraser(struct rb_root *root, int gpio) {
	gpio_proc_t *data = my_search(root, gpio);
	if(data == NULL) {
		return 0;
	}

	if (data) {
		rb_erase(&data->node, root);
		vfree(data);
	}
	return 0;
}

static int gpio_direction_open(struct inode *inode, struct file *file) {
	gpio_proc_t *data = (gpio_proc_t *)PDE_DATA(inode);
	if(data == NULL) {
		mydebug("failed!\n");
		return -1;
	}

	//mydebug("\n");
	//if(!capable(CAP_SYS_ADMIN))
	//	return -EPERM;

	data->open = 1;

	return 0;
}

static int gpio_direction_release(struct inode *inode, struct file *file) {
	gpio_proc_t *data = (gpio_proc_t *)PDE_DATA(inode);
	if(data == NULL) {
		mydebug("failed!\n");
		return 0;
	}

	//mydebug("\n");
	data->open = 0;
	return 0;
}

static ssize_t gpio_direction_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
	gpio_proc_t *data = (gpio_proc_t *)PDE_DATA(file_inode(file));
	u8 value[8];
	u64 bufsize;

	if(data == NULL) {
		mydebug("failed!\n");
		return count;
	}

	bufsize = snprintf(value, 8, "%d\n", gpio_get_value_cansleep(data->gpio));

	return simple_read_from_buffer(buffer, count, ppos, value, bufsize);
}

static ssize_t gpio_direction_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
	//int i;
	gpio_proc_t *data = (gpio_proc_t *)PDE_DATA(file_inode(file));
	char cmd[32];
	size_t size;
	int value;

	if(data == NULL) {
		mydebug("failed!\n");
		return count;
	}

	size = sizeof(cmd);
	if (count < size)
		size = count;

	if (copy_from_user(cmd, buffer, size))
		return -EFAULT;

	//for(i = 0; i < size; i++) {
	//	if((i != 0) && (i % 16 == 0)) {
	//		printk("\n");
	//	}

	//	printk("%02xh(%c) ", cmd[i], cmd[i]);
	//}
	//printk("\n");
	//
	if (sscanf(cmd, "%d", &value) == 1) {
		//printk("gpio:%d, value:%d\n", data->gpio, value);

		switch(value) {
			case 0:
			case 1:
				{
					gpio_direction_output(data->gpio, value);
				}
				break;
			case 2:
				{
					gpio_direction_input(data->gpio);
				}
				break;
			default:
				break;
		}
	} else {
		mydebug("%s\n", cmd);
	}


	return count;
}

static const struct file_operations gpio_ops = {
	.open    = gpio_direction_open,
	.release = gpio_direction_release,
	.read    = gpio_direction_read,
	.write   = gpio_direction_write,
	.llseek  = default_llseek,
};


static ssize_t gpio_export_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
	char cmd[32];
	size_t size;
	int gpio;

	//mydebug("\n");
	size = sizeof(cmd);
	if (count < size)
		size = count;

	if (copy_from_user(cmd, buffer, size))
		return -EFAULT;

	if (sscanf(cmd, "%d", &gpio) == 1) {
		gpio_proc_t *data;
		//mydebug("export pin:%d\n", gpio);
		if(gpio_request(gpio, NULL) != 0) {
			mydebug("export pin error:%d\n", gpio);
			return count;
		}

		snprintf(cmd, sizeof(cmd), "%d", gpio);
		my_insert(&mytree, gpio);
		data = my_search(&mytree, gpio);
		if(data == NULL) {
			mydebug("\n");
			return count;
		}

		proc_create_data(cmd,
				0666,
				gpio_proc_dir,
				&gpio_ops,
				data);
	}


	return count;
}

static const struct file_operations gpio_export_ops = {
	.write   = gpio_export_write,
};

static ssize_t gpio_unexport_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
	char cmd[32];
	size_t size;
	int gpio;

	//mydebug("\n");
	size = sizeof(cmd);
	if (count < size)
		size = count;

	if (copy_from_user(cmd, buffer, size))
		return -EFAULT;

	if (sscanf(cmd, "%d", &gpio) == 1) {
		//mydebug("unexport pin:%d\n", gpio);
		gpio_free(gpio);

		snprintf(cmd, sizeof(cmd), "%d", gpio);
		if(remove_proc_subtree(cmd, gpio_proc_dir) != 0) {
			mydebug("unexport pin failed!:%d\n", gpio);
		}
		my_eraser(&mytree, gpio);
	}


	return count;
}

static const struct file_operations gpio_unexport_ops = {
	.write   = gpio_unexport_write,
};

static struct proc_dir_entry **items = NULL; /* keeps track of every entry */
static char *names[] = {
	"export",
	"unexport",
};

static const struct file_operations *ops[] = {
	&gpio_export_ops,
	&gpio_unexport_ops,
};

#define NAMES_SIZE (sizeof(names) / sizeof(char *))

int create_gpio_proc_dir(void) {
	int ret = 0;
	int i;

	mydebug("\n");
	gpio_proc_dir = proc_mkdir(BASE_DIR, NULL);
	if(gpio_proc_dir == NULL) {
		ret = -1;
		return ret;
	}

	items = (struct proc_dir_entry **)vzalloc(sizeof(struct proc_dir_entry *) * NAMES_SIZE);

	for (i=0; i < NAMES_SIZE; i++) {
		/* pass the feature bit in question as misc data */
		items[i] = proc_create_data(names[i],
				0666,
				gpio_proc_dir,
				ops[i],
				NULL);
	}
	return ret;
}

void remove_gpio_proc_dir(void) {
	struct rb_node *node;

	for (node = rb_first(&mytree); node;) {
		gpio_proc_t *data = rb_entry(node, gpio_proc_t, node);
		char cmd[32];

		printk("key=%d\n", data->gpio);

		gpio_free(data->gpio);
		snprintf(cmd, sizeof(cmd), "%d", data->gpio);
		if(remove_proc_subtree(cmd, gpio_proc_dir) != 0) {
			mydebug("unexport pin failed!:%d\n", data->gpio);
		}

		node = rb_next(node);

		rb_erase(&data->node, &mytree);
		vfree(data);
	}

	for (node = rb_first(&mytree); node; node = rb_next(node)) {
		gpio_proc_t *data = rb_entry(node, gpio_proc_t, node);
		printk("key=%d\n", data->gpio);
	}

	remove_proc_subtree(BASE_DIR, NULL);
	vfree(items);
	mydebug("\n");
}

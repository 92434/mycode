#include <linux/proc_fs.h>
#include <linux/gpio.h>
//#include <linux/device.h>
//#include <linux/fs.h>
#include <linux/uaccess.h>

#include "utils/xiaofei_debug.h"

typedef enum {
	gpio_direction = 0,
	gpio_item_count,
} gpio_items_t;

typedef struct {
	int gpio;
	bool open;
} gpio_proc_t;

#define BASE_DIR "gpio_test"

static struct proc_dir_entry *gpio_proc_dir; /* /proc/sal dir entry */
static DEFINE_SPINLOCK(data_lock);
static gpio_proc_t gpio = {0};

static int gpio_direction_open(struct inode *inode, struct file *file) {
	gpio_proc_t *gpio = PDE_DATA(inode);

	mydebug("\n");
	//if(!capable(CAP_SYS_ADMIN))
	//	return -EPERM;

	spin_lock(&data_lock);
	if(gpio->open) {
		spin_unlock(&data_lock);
		return -EBUSY;
	}
	gpio->open = 1;
	spin_unlock(&data_lock);

	return 0;
}

static int gpio_direction_release(struct inode *inode, struct file *file) {
	gpio_proc_t *gpio = PDE_DATA(inode);

	mydebug("\n");
	spin_lock(&data_lock);
	gpio->open = 0;
	spin_unlock(&data_lock);
	return 0;
}

static ssize_t gpio_direction_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
	gpio_proc_t *gpio = PDE_DATA(file_inode(file));
	u8 value[8];
	u64 bufsize;

	mydebug("\n");
	bufsize = snprintf(value, 8, "%d\n", gpio_get_value_cansleep(gpio->gpio));

	return simple_read_from_buffer(buffer, count, ppos, value, bufsize);
}

static ssize_t gpio_direction_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
	//int i;
	gpio_proc_t *gpio = PDE_DATA(file_inode(file));
	char cmd[32];
	size_t size;
	int value;

	mydebug("\n");
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
	if (sscanf(cmd, "%d %d", &gpio->gpio, &value) == 2) {
		printk("gpio:%d, value:%d\n", gpio->gpio, value);

		switch(value) {
			case 0:
			case 1:
				{
					gpio_free(gpio->gpio);
					if(gpio_request(gpio->gpio, NULL) != 0) {
						return count;
					}

					gpio_direction_output(gpio->gpio, value);
				}
				break;
			case 2:
				{
					gpio_free(gpio->gpio);
					if(gpio_request(gpio->gpio, NULL) != 0) {
						return count;
					}

					gpio_direction_input(gpio->gpio);
				}
				break;
			default:
				{
					gpio_free(gpio->gpio);
				}
				break;
		}
	}


	return count;
}

static const struct file_operations gpio_direction_ops = {
	.open    = gpio_direction_open,
	.release = gpio_direction_release,
	.read    = gpio_direction_read,
	.write   = gpio_direction_write,
	.llseek  = default_llseek,
};

static const struct file_operations *ops[gpio_item_count] = {
	&gpio_direction_ops,
};
static struct proc_dir_entry *items[gpio_item_count] = {NULL}; /* keeps track of every entry */
static char *names[gpio_item_count] = {
	"gpio_direction",
};


int create_gpio_proc_dir(void) {
	int ret = 0;
	int i;

	mydebug("\n");
	gpio_proc_dir = proc_mkdir(BASE_DIR, NULL);
	if(gpio_proc_dir == NULL) {
		ret = -1;
		return ret;
	}

	for (i=0; i < gpio_item_count; i++) {
		/* pass the feature bit in question as misc data */
		items[i] = proc_create_data(names[i],
					0666,
					gpio_proc_dir,
					ops[i],
					&gpio);
	}
	return ret;
}

void remove_gpio_proc_dir(void) {
	remove_proc_subtree(BASE_DIR, NULL);
	mydebug("\n");
}

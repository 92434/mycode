#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/poll.h>

#include "pcie.h"
#include "pcie_device.h"

#include "utils/xiaofei_debug.h"

#define MAX_KC705_DEV_NUM 16


static int kc705_open(struct inode *i, struct file *filp) {
	pcie_dma_t *dma = container_of(i->i_cdev, pcie_dma_t, cdev);
	filp->private_data = dma;
	init_waitqueue_head(&dma->wq);
	mydebug("\n");
	return 0;
}

static int kc705_close(struct inode *i, struct file *filp) {
	mydebug("\n");
	return 0;
}

static ssize_t kc705_read(struct file *filp, char __user * buf, size_t len, loff_t * off) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;

	//mydebug("\n");

	if(len == 0) {
		return ret;
	}

	while(!read_available(dma->list)) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(dma->wq, (read_available(dma->list)));
	}

	ret = read_buffer(buf, len, dma->list);

	return ret;
}

static ssize_t kc705_write(struct file *filp, const char __user * buf, size_t len, loff_t * off) {
	mydebug("\n");
	return len;
}

static unsigned int kc705_poll(struct file *filp, poll_table *wait) {
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	unsigned int mask =0;


	/* 把等待队列添加到poll_table */
	poll_wait(filp, &dma->wq, wait);

	/*返回掩码*/
	if (read_available(dma->list))
		mask = POLLIN | POLLRDNORM;/*设备可读*/
	return mask;
}

static int kc705_mmap(struct file *filp, struct vm_area_struct *vma) {
	int ret;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	unsigned long requested_size;
	unsigned long vma_start;
	buffer_node_t *node = NULL, *node_next = NULL;
	list_buffer_t *list = dma->list;

	mydebug("\n");

	if((list == NULL) || (list->read == NULL)) {
		return -EAGAIN;
	}

	requested_size = vma->vm_end - vma->vm_start;

	if (requested_size > list->size) {
		return -EAGAIN;
	}

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	vma_start = vma->vm_start;

	node = list_prepare_entry(node, list->first, list);
	ret = remap_pfn_range(vma, vma_start, page_to_pfn(virt_to_page(node->buffer))/*virt_to_pfn(node->buffer)*/, node->size, vma->vm_page_prot);
	if(ret != 0) {
		return -EAGAIN;
	}
	mydebug("node->buffer:%p node->size:%d\n", node->buffer, node->size);
	vma_start += node->size;
	requested_size -= node->size;

	list_for_each_entry_safe(node, node_next, list->first, list) {
		ret = remap_pfn_range(vma, vma_start, page_to_pfn(virt_to_page(node->buffer))/*virt_to_pfn(node->buffer)*/, node->size, vma->vm_page_prot);
		if(ret != 0) {
			return -EAGAIN;
		}
		mydebug("node->buffer:%p node->size:%d\n", node->buffer, node->size);
		vma_start += node->size;
		requested_size -= node->size;
	}
	mydebug("requested_size:%lu\n", requested_size);

	return 0;
}

static long kc705_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	list_buffer_t *list = dma->list;

	mydebug("\n");
	switch (cmd) {
		case PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE:
			{
				if (copy_to_user((int *)arg, &list->size, sizeof(int))) {
					mydebug("\n");
					ret = -EFAULT;
				}
			}
			break;
		case PCIE_DEVICE_IOCTL_GET_NODE_INFO:
			{
				buffer_node_t read;
				node_info_t info;
				ret = get_buffer_node_info(NULL, &read, dma->list);
				if(ret != 0) {
					ret = -EFAULT;
				}
				info.size = read.size;
				info.read_offset = read.read_offset;
				info.avail_for_read = read.avail_for_read;
				info.base_addr_of_list_buffer = read.base_addr_of_list_buffer;
				if (copy_to_user((int *)arg, &info, sizeof(node_info_t))) {
					ret = -EFAULT;
				}
			}
			break;
		default:
			break;
	}

	return ret;
}

static const struct file_operations kc705_fops = {
	.owner = THIS_MODULE,
	.open = kc705_open,
	.release = kc705_close,
	.read = kc705_read,
	.write = kc705_write,
	.mmap = kc705_mmap,
	.unlocked_ioctl = kc705_ioctl,
	.poll = kc705_poll,
};

int setup_kc705_dev(pcie_dma_t *dma, char *namefmt, ...) {
	int status = 0;

	va_list args;

	va_start(args, namefmt);
	vsnprintf(dma->dev_name, sizeof(dma->dev_name), namefmt, args);
	va_end(args);

	if (dma->kc705_dev_id == 0) {
		/* Alloc Major & Minor number for char device */
		status = alloc_chrdev_region(&dma->kc705_dev_id, 0, MAX_KC705_DEV_NUM, dma->dev_name);
		if (status) {
			mydebug("\n");
			goto alloc_chrdev_region_failed;
		}
		printk("MAJOR(dma->kc705_dev_id):%d\n", MAJOR(dma->kc705_dev_id));
	}

	if (dma->kc705_class == NULL) {
		dma->kc705_class = class_create(THIS_MODULE, dma->dev_name);
		if (IS_ERR(dma->kc705_class)) {
			status = PTR_ERR(dma->kc705_class);
			mydebug("\n");
			goto class_create_failed;
		}
	}

	cdev_init(&dma->cdev, &kc705_fops);

	status = cdev_add(&dma->cdev, dma->kc705_dev_id, 1);
	if (status < 0) {
		goto cdev_add_failed;
	}

	/* Add Device node in system */
	dma->device = device_create(dma->kc705_class, NULL, dma->kc705_dev_id, NULL, "%s", dma->dev_name);
	if (IS_ERR(dma->device)) {
		status = PTR_ERR(dma->device);
		goto device_create_failed;
	}

	return status;

	device_destroy(dma->kc705_class, dma->kc705_dev_id);
device_create_failed:
	cdev_del(&dma->cdev);
cdev_add_failed:
	class_destroy(dma->kc705_class);
class_create_failed:
	unregister_chrdev_region(dma->kc705_dev_id, MAX_KC705_DEV_NUM);
alloc_chrdev_region_failed:
	return status;
}

void uninstall_kc705_dev(pcie_dma_t *dma) {
	if(dma->device != NULL) {
		device_destroy(dma->kc705_class, dma->kc705_dev_id);
		cdev_del(&dma->cdev);
		class_destroy(dma->kc705_class);
		unregister_chrdev_region(dma->kc705_dev_id, MAX_KC705_DEV_NUM);
		dma->device = NULL;
	}
}

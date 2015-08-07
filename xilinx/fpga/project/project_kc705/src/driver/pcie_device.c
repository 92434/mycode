#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/highmem.h>

#include "pcie.h"
#include "pcie_device.h"

#include "utils/xiaofei_debug.h"

#define MODULE_NAME "kc705_pcie"

static wait_queue_head_t wq;
static dev_t kc705_dev_id = 0;
static struct class *kc705_class = NULL;
struct device *device = NULL;
#define MAX_KC705_DEV_NUM 16


static int kc705_open(struct inode *i, struct file *filp) {
	kc705_pci_dev_t *kc705_pci_dev = container_of(i->i_cdev, kc705_pci_dev_t, cdev);
	filp->private_data = kc705_pci_dev;
	init_waitqueue_head(&wq);
	return 0;
}

static int kc705_close(struct inode *i, struct file *filp) {

	return 0;
}

static ssize_t kc705_read(struct file *filp, char __user * buf, size_t len, loff_t * off) {
	int rtn = 0;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)filp->private_data;

	if(len == 0) {
		return rtn;
	}

	while(!read_available(kc705_pci_dev->list)) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(wq, (read_available(kc705_pci_dev->list)));
	}

	rtn = read_buffer(buf, len, kc705_pci_dev->list);

	return rtn;
}

static ssize_t kc705_write(struct file *filp, const char __user * buf, size_t len, loff_t * off) {

	return len;
}

static int kc705_mmap(struct file *filp, struct vm_area_struct *vma) {
	int rtn;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)filp->private_data;
	unsigned long requested_size;
	unsigned long vma_start;
	buffer_node_t *node = NULL, *node_next = NULL;
	list_buffer_t *list = kc705_pci_dev->list;

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
	rtn = remap_pfn_range(vma, vma_start, page_to_pfn(virt_to_page(node->buffer))/*virt_to_pfn(node->buffer)*/, node->size, vma->vm_page_prot);
	if(rtn != 0) {
		return -EAGAIN;
	}
	vma_start += node->size;
	requested_size -= node->size;

	list_for_each_entry_safe(node, node_next, list->first, list) {
		rtn = remap_pfn_range(vma, vma_start, page_to_pfn(virt_to_page(node->buffer))/*virt_to_pfn(node->buffer)*/, node->size, vma->vm_page_prot);
		if(rtn != 0) {
			return -EAGAIN;
		}
		vma_start += node->size;
		requested_size -= node->size;
	}

	return 0;
}

static long kc705_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)filp->private_data;
	list_buffer_t *list = kc705_pci_dev->list;

	switch (cmd) {
		case PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE:
			{
				if (copy_to_user((int *)arg, &list->size, sizeof(int))) {
					return -EFAULT;
				}
			}
			break;
		case PCIE_DEVICE_IOCTL_GET_NODE_INFO:
			{
				if (copy_to_user((int *)arg, &list->size, sizeof(int))) {
					return -EFAULT;
				}
			}
			break;
		default:
			break;
	}

	return 0;
}

static const struct file_operations kc705_fops = {
	.owner = THIS_MODULE,
	.open = kc705_open,
	.release = kc705_close,
	.read = kc705_read,
	.write = kc705_write,
	.mmap = kc705_mmap,
	.unlocked_ioctl = kc705_ioctl,
};

int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev) {
	int status = 0;

	if (kc705_dev_id == 0) {
		/* Alloc Major & Minor number for char device */
		status = alloc_chrdev_region(&kc705_dev_id, 0, MAX_KC705_DEV_NUM, MODULE_NAME);
		if (status) {
			mydebug("\n");
			goto alloc_chrdev_region_failed;
		}
		printk("MAJOR(kc705_dev_id):%d\n", MAJOR(kc705_dev_id));
	}

	if (kc705_class == NULL) {
		kc705_class = class_create(THIS_MODULE, MODULE_NAME);
		if (IS_ERR(kc705_class)) {
			status = PTR_ERR(kc705_class);
			mydebug("\n");
			goto class_create_failed;
		}
	}

	cdev_init(&kc705_pci_dev->cdev, &kc705_fops);

	status = cdev_add(&kc705_pci_dev->cdev, kc705_dev_id, 1);
	if (status < 0) {
		goto cdev_add_failed;
	}

	/* Add Device node in system */
	device = device_create(kc705_class, NULL, kc705_dev_id, NULL, "%s", MODULE_NAME);
	if (IS_ERR(device)) {
		status = PTR_ERR(device);
		goto device_create_failed;
	}

	return status;

	device_destroy(kc705_class, kc705_dev_id);
device_create_failed:
	cdev_del(&kc705_pci_dev->cdev);
cdev_add_failed:
	class_destroy(kc705_class);
class_create_failed:
	unregister_chrdev_region(kc705_dev_id, MAX_KC705_DEV_NUM);
alloc_chrdev_region_failed:
	return status;
}

void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev) {
	if(device != NULL) {
		device_destroy(kc705_class, kc705_dev_id);
		cdev_del(&kc705_pci_dev->cdev);
		class_destroy(kc705_class);
		unregister_chrdev_region(kc705_dev_id, MAX_KC705_DEV_NUM);
		device = NULL;
	}
}

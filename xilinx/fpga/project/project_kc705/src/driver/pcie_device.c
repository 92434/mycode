#include <linux/device.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/poll.h>

#include "kc705.h"
#include "pcie.h"
#include "pcie_device.h"

#define MAX_KC705_DEV_NUM 16

void start_dma(void);
void stop_dma(void);

static int kc705_open(struct inode *i, struct file *filp) {
	kc705_pci_dev_t *kc705_pci_dev = container_of(i->i_cdev, kc705_pci_dev_t, cdev);
	filp->private_data = kc705_pci_dev;
	mydebug("\n");
	return 0;
}

static int kc705_close(struct inode *i, struct file *filp) {
	mydebug("\n");
	return 0;
}

static ssize_t kc705_read(struct file *filp, char __user * buf, size_t len, loff_t * off) {
	int ret = 0;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)filp->private_data;
	kc705_pci_dev = kc705_pci_dev;

	mydebug("\n");

	if(len == 0) {
		return ret;
	}

	return ret;
}

static ssize_t kc705_write(struct file *filp, const char __user * buf, size_t len, loff_t * off) {
	mydebug("\n");
	return len;
}

static unsigned int kc705_poll(struct file *filp, poll_table *wait) {
	unsigned int mask =0;


	mask = POLLIN | POLLRDNORM;/*设备可读*/
	return mask;
}

static int kc705_mmap(struct file *filp, struct vm_area_struct *vma) {
	int ret;
	unsigned long requested_size;

	mydebug("\n");

	requested_size = vma->vm_end - vma->vm_start;

	ret =  -EAGAIN;
	return ret;
}

static long kc705_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int ret = 0;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)filp->private_data;
	kc705_pci_dev = kc705_pci_dev;

	switch (cmd) {
		case PCIE_DEVICE_IOCTL_START_DMA_THREAD:
			{
				mydebug("\n");
				start_dma();
			}
			break;
		case PCIE_DEVICE_IOCTL_STOP_DMA_THREAD:
			{
				mydebug("\n");
				stop_dma();
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

int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev, char *namefmt, ...) {
	int status = 0;

	va_list args;

	va_start(args, namefmt);
	vsnprintf(kc705_pci_dev->dev_name, sizeof(kc705_pci_dev->dev_name), namefmt, args);
	va_end(args);

	if (kc705_pci_dev->kc705_dev_id == 0) {
		/* Alloc Major & Minor number for char device */
		status = alloc_chrdev_region(&kc705_pci_dev->kc705_dev_id, 0, MAX_KC705_DEV_NUM, kc705_pci_dev->dev_name);
		if (status) {
			mydebug("\n");
			goto alloc_chrdev_region_failed;
		}
		printk("MAJOR(kc705_pci_dev->kc705_dev_id):%d\n", MAJOR(kc705_pci_dev->kc705_dev_id));
	}

	if (kc705_pci_dev->kc705_class == NULL) {
		kc705_pci_dev->kc705_class = class_create(THIS_MODULE, kc705_pci_dev->dev_name);
		if (IS_ERR(kc705_pci_dev->kc705_class)) {
			status = PTR_ERR(kc705_pci_dev->kc705_class);
			mydebug("\n");
			goto class_create_failed;
		}
	}

	cdev_init(&kc705_pci_dev->cdev, &kc705_fops);

	status = cdev_add(&kc705_pci_dev->cdev, kc705_pci_dev->kc705_dev_id, 1);
	if (status < 0) {
		goto cdev_add_failed;
	}

	/* Add Device node in system */
	kc705_pci_dev->device = device_create(kc705_pci_dev->kc705_class, NULL, kc705_pci_dev->kc705_dev_id, NULL, "%s", kc705_pci_dev->dev_name);
	if (IS_ERR(kc705_pci_dev->device)) {
		status = PTR_ERR(kc705_pci_dev->device);
		goto device_create_failed;
	}

	return status;

	device_destroy(kc705_pci_dev->kc705_class, kc705_pci_dev->kc705_dev_id);
device_create_failed:
	cdev_del(&kc705_pci_dev->cdev);
cdev_add_failed:
	class_destroy(kc705_pci_dev->kc705_class);
class_create_failed:
	unregister_chrdev_region(kc705_pci_dev->kc705_dev_id, MAX_KC705_DEV_NUM);
alloc_chrdev_region_failed:
	return status;
}

void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->device != NULL) {
		device_destroy(kc705_pci_dev->kc705_class, kc705_pci_dev->kc705_dev_id);
		cdev_del(&kc705_pci_dev->cdev);
		class_destroy(kc705_pci_dev->kc705_class);
		unregister_chrdev_region(kc705_pci_dev->kc705_dev_id, MAX_KC705_DEV_NUM);
		kc705_pci_dev->device = NULL;
	}
}

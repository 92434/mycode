#include <linux/fs.h>
#include <linux/poll.h>

#include "pcie.h"
#include "utils/xiaofei_debug.h"
#include "kc705.h"//PCIE_DEVICE_IOCTL_START_DMA_THREAD
//
#define MAX_KC705_DEV_NUM 16

void start_dma(kc705_pci_dev_t *kc705_pci_dev);
void stop_dma(kc705_pci_dev_t *kc705_pci_dev);

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

	switch (cmd) {
		case PCIE_DEVICE_IOCTL_START_DMA_THREAD:
			{
				mydebug("\n");
				start_dma(kc705_pci_dev);
			}
			break;
		case PCIE_DEVICE_IOCTL_STOP_DMA_THREAD:
			{
				mydebug("\n");
				stop_dma(kc705_pci_dev);
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

int alloc_kc705_chrdev(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	dev_t dev = 0;
	unsigned int major = 0;
	unsigned int minor = 0;
	int i;

	ret = alloc_chrdev_region(&dev, 0, MAX_KC705_DEV_NUM, kc705_pci_dev->devname);
	if (ret != 0) {
		mydebug("\n");
		return ret;
	}

	major = MAJOR(dev);
	minor = MINOR(dev);

	mydebug("kc705 major:%d\n", major);

	kc705_pci_dev->dev = MKDEV(major, minor);
	mydebug("%s minor:%d\n", kc705_pci_dev->devname, minor);

	minor++;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		if(i < MAX_KC705_DEV_NUM) {
			dma->dev = MKDEV(major, minor);
			mydebug("%s minor:%d\n", dma->devname, minor);
			minor++;
		}
	}

	return ret;
}

void free_kc705_chrdev(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		dma->dev = 0;
	}

	unregister_chrdev_region(kc705_pci_dev->dev, MAX_KC705_DEV_NUM);
	kc705_pci_dev->dev = 0;
}

int create_kc705_device_class(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;

	if(kc705_pci_dev->kc705_class != NULL) {
		mydebug("\n");
		ret = -1;
		return ret;
	}

	kc705_pci_dev->kc705_class = class_create(THIS_MODULE, kc705_pci_dev->devname);
	if (IS_ERR(kc705_pci_dev->kc705_class)) {
		mydebug("\n");
		ret = PTR_ERR(kc705_pci_dev->kc705_class);
		return ret;
	}

	return ret;
}

void destroy_kc705_device_class(kc705_pci_dev_t *kc705_pci_dev) {

	if(!IS_ERR(kc705_pci_dev->kc705_class)) {
		class_destroy(kc705_pci_dev->kc705_class);
	}

	kc705_pci_dev->kc705_class = NULL;
}

int init_kc705_dev_class(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;

	ret = alloc_kc705_chrdev(kc705_pci_dev);
	if(ret != 0) {
		mydebug("\n");
		goto alloc_kc705_chrdev_failed;
	}

	ret = create_kc705_device_class(kc705_pci_dev);
	if(ret != 0) {
		mydebug("\n");
		goto create_kc705_device_class_failed;
	}

	return ret;

	destroy_kc705_device_class(kc705_pci_dev);
create_kc705_device_class_failed:
	free_kc705_chrdev(kc705_pci_dev);
alloc_kc705_chrdev_failed:
	return ret;
}

void uninit_kc705_dev_class(kc705_pci_dev_t *kc705_pci_dev) {
	destroy_kc705_device_class(kc705_pci_dev);
	free_kc705_chrdev(kc705_pci_dev);
}


int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev) {
	int status = 0;

	if(kc705_pci_dev->devname == NULL) {
		mydebug("\n");
		goto no_device_name_failed;
	}

	cdev_init(&kc705_pci_dev->cdev, &kc705_fops);

	status = cdev_add(&kc705_pci_dev->cdev, kc705_pci_dev->dev, 1);
	if (status < 0) {
		goto cdev_add_failed;
	}

	/* Add Device node in system */
	kc705_pci_dev->device = device_create(kc705_pci_dev->kc705_class, &(kc705_pci_dev->pdev->dev), kc705_pci_dev->dev, NULL, "%s", kc705_pci_dev->devname);
	if (IS_ERR(kc705_pci_dev->device)) {
		status = PTR_ERR(kc705_pci_dev->device);
		goto device_create_failed;
	}

	return status;

	device_destroy(kc705_pci_dev->kc705_class, kc705_pci_dev->dev);
	kc705_pci_dev->device = NULL;
device_create_failed:
	cdev_del(&kc705_pci_dev->cdev);
cdev_add_failed:
no_device_name_failed:
	return status;
}

void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->device != NULL) {
		device_destroy(kc705_pci_dev->kc705_class, kc705_pci_dev->dev);
		kc705_pci_dev->device = NULL;
		cdev_del(&kc705_pci_dev->cdev);
	}
}

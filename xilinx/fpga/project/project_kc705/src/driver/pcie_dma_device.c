#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>

#include "pcie.h"
#include "utils/xiaofei_debug.h"
#include "pcie_tr_thread.h"
#include "kc705.h"//PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE

static int pcie_dma_open(struct inode *i, struct file *filp) {
	pcie_dma_t *dma = container_of(i->i_cdev, pcie_dma_t, cdev);
	filp->private_data = dma;
	//mydebug("\n");
	return 0;
}

static int pcie_dma_close(struct inode *i, struct file *filp) {
	//mydebug("\n");
	return 0;
}

static ssize_t pcie_dma_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	loff_t offset = *off;
	loff_t end = *off + len;
	int idle_count = 0;

	//mydebug("len:%d, off:%lld\n", len, *off);

	if(len == 0) {
		return ret;
	}

	if(!access_ok(VERIFY_WRITE, buf, len)) {
		mydebug("%p\n", current);
		return ret;
	}

	while(ret < len) {
		int c;

		if(end - offset >= dma->receive_bulk_size) {
			c = dma->receive_bulk_size;
		} else {
			c = end - offset;
		}

		if(!dma->is_auto_receive) {
			if(dma->is_ready_for_read != NULL) {
				if(!dma->is_ready_for_read(dma)) {
					c = 0;
				}
			}

			if(c > 0) {
				if(put_pcie_tr(
						dma,
						0,
						offset,
						0,
						c,
						NULL,
						NULL) < 0) {
					mydebug("%p\n", current);
					c = 0;
				}
			} else {
				c = 0;
			}
		}

		if((c > 0) && read_available(dma->list)) {
			c = read_buffer(buf + ret, c, dma->list);
			ret += c;
			offset += c;
			idle_count = 0;
		} else {
			if (filp->f_flags & O_NONBLOCK) {
				return ret;
			}

			if(idle_count++ == 1) {
				return ret;
			}

			//wait_event_interruptible_timeout(dma->wq, true, HZ);
		}
	}

	return ret;
}

static ssize_t pcie_dma_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	loff_t offset = *off;
	loff_t end = *off + len;
	int idle_count = 0;

	//mydebug("len:%d, off:%lld\n", len, *off);

	if(len > PCIe_MAP_BAR_SIZE) {
		ret = -ERANGE;
		return ret;
	}

	if((dma->is_ready_for_write != NULL) && (ret = 0)) {
		if(!dma->is_ready_for_write(dma)) {
			return ret;
		}
	}

	while(ret < len) {
		int c;

		if(end - offset >= dma->send_bulk_size) {
			c = dma->send_bulk_size;
		} else {
			c = end - offset;
		}

		if(c > 0) {
			if(copy_from_user(dma->bar_map_memory[1], buf + ret, c)) {
				mydebug("%p\n", current);
				return ret;
			}

			if(put_pcie_tr(
					dma,
					offset,
					0,
					c,
					0,
					NULL,
					NULL
				) < 0) {
				c = 0;
			}
		} else {
			c = 0;
		}

		if(c > 0) {
			ret += c;
			offset += c;
			idle_count = 0;
		} else {
			if (filp->f_flags & O_NONBLOCK)
				return ret;

			if(idle_count++ == 1) {
				return ret;
			}
		}
	}

	return ret;
}

static unsigned int pcie_dma_poll(struct file *filp, poll_table *wait) {
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	unsigned int mask =0;

	//mydebug("\n");

	/* 把等待队列添加到poll_table */
	poll_wait(filp, &dma->wq, wait);

	/*返回掩码*/
	if (read_available(dma->list))
		mask = POLLIN | POLLRDNORM;/*设备可读*/
	return mask;
}

static int pcie_dma_mmap(struct file *filp, struct vm_area_struct *vma) {
	int ret;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	unsigned long requested_size;
	unsigned long vma_start;
	buffer_node_t *node = NULL, *node_next = NULL;
	list_buffer_t *list = dma->list;

	//mydebug("\n");

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
	//mydebug("node->buffer:%p node->size:%d\n", node->buffer, node->size);
	vma_start += node->size;
	requested_size -= node->size;

	list_for_each_entry_safe(node, node_next, list->first, list) {
		ret = remap_pfn_range(vma, vma_start, page_to_pfn(virt_to_page(node->buffer))/*virt_to_pfn(node->buffer)*/, node->size, vma->vm_page_prot);
		if(ret != 0) {
			return -EAGAIN;
		}
		//mydebug("node->buffer:%p node->size:%d\n", node->buffer, node->size);
		vma_start += node->size;
		requested_size -= node->size;
	}
	//mydebug("requested_size:%lu\n", requested_size);

	return 0;
}

static long pcie_dma_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int ret = 0;
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	list_buffer_t *list = dma->list;

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
		case PCIE_DEVICE_IOCTL_PSEUDO_DMA_TR:
			{
				pseudo_dma_tr_t pseudo_dma_tr;

				if(copy_from_user(&pseudo_dma_tr, (pseudo_dma_tr_t __user *) arg, sizeof(pseudo_dma_tr_t __user))) {
					mydebug("%p\n", current);
					ret = -EFAULT;
					return ret;
				}

				//myprintf("tx_size:%d, rx_size:%d\n", pseudo_dma_tr.tx_size, pseudo_dma_tr.rx_size);
				
				if(pseudo_dma_tr.tx_size != 0) {
					copy_from_user(dma->bar_map_memory[1], pseudo_dma_tr.tx_data, pseudo_dma_tr.tx_size);
				}

				if(pseudo_dma_tr.rx_size != 0) {
					ret = put_pcie_tr(
							dma,
							pseudo_dma_tr.tx_dest_offset,
							pseudo_dma_tr.rx_src_offset,
							pseudo_dma_tr.tx_size,
							pseudo_dma_tr.rx_size,
							NULL,
							NULL);
					if(ret == -1) {
						pseudo_dma_tr.rx_size = 0;
					}
				}

				if(pseudo_dma_tr.rx_size != 0) {
					if(!access_ok(VERIFY_WRITE, pseudo_dma_tr.rx_data, pseudo_dma_tr.rx_size)) {
						mydebug("%p\n", current);
						ret = -EFAULT;
						return ret;
					}

					if(read_buffer(pseudo_dma_tr.rx_data, pseudo_dma_tr.rx_size, dma->list) != pseudo_dma_tr.rx_size) {
						mydebug("%p\n", current);
						ret = -EFAULT;
						return ret;
					}
				}
			}
			break;
		default:
			break;
	}

	return ret;
}

static loff_t pcie_dma_llseek(struct file *filp, loff_t offset, int whence) {
	pcie_dma_t *dma = (pcie_dma_t *)filp->private_data;
	char *whence_str = (whence == SEEK_SET) ? "SEEK_SET" : ((whence == SEEK_CUR) ? "SEEK_CUR" : "other");

	//mydebug("dma->devname:%s, offset:%lld, whence:%s(%d)\n", dma->devname, offset, whence_str, whence);
	
	dma = dma;
	whence_str = whence_str;

	switch (whence) {
		case SEEK_SET:
			return vfs_setpos(filp, offset, PCIe_MAP_BAR_SIZE);
		case SEEK_CUR:
			return vfs_setpos(filp, filp->f_pos + offset, PCIe_MAP_BAR_SIZE);
		default:
			return -ENXIO;
	}
}
static const struct file_operations pcie_dma_fops = {
	.owner = THIS_MODULE,
	.open = pcie_dma_open,
	.release = pcie_dma_close,
	.read = pcie_dma_read,
	.write = pcie_dma_write,
	.mmap = pcie_dma_mmap,
	.unlocked_ioctl = pcie_dma_ioctl,
	.poll = pcie_dma_poll,
	.llseek = pcie_dma_llseek,
};

int setup_pcie_dma_dev(pcie_dma_t *dma) {
	int ret = 0;

	kc705_pci_dev_t *kc705_pci_dev = dma->kc705_pci_dev;

	if(dma->devname == NULL) {
		mydebug("\n");
		goto no_device_name_failed;
	}

	cdev_init(&dma->cdev, &pcie_dma_fops);

	ret = cdev_add(&dma->cdev, dma->dev, 1);
	if (ret < 0) {
		goto cdev_add_failed;
	}

	/* Add Device node in system */
	dma->device = device_create(kc705_pci_dev->kc705_class, &(kc705_pci_dev->pdev->dev), dma->dev, NULL, "%s", dma->devname);
	if (IS_ERR(dma->device)) {
		ret = PTR_ERR(dma->device);
		goto device_create_failed;
	}

	init_waitqueue_head(&dma->wq);

	return ret;

	device_destroy(kc705_pci_dev->kc705_class, dma->dev);
	dma->device = NULL;
device_create_failed:
	cdev_del(&dma->cdev);
cdev_add_failed:
no_device_name_failed:
	return ret;
}

void uninstall_pcie_dma_dev(pcie_dma_t *dma) {
	kc705_pci_dev_t *kc705_pci_dev = dma->kc705_pci_dev;

	if(dma->device != NULL) {
		device_destroy(kc705_pci_dev->kc705_class, dma->dev);
		dma->device = NULL;
		cdev_del(&dma->cdev);
	}
}

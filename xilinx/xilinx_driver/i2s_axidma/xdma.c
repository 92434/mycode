/*
 * Wrapper Driver used to control a two-channel Xilinx DMA Engine
 */
#include <linux/dmaengine.h>
#include "xdma.h"

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>
#include <linux/dma-mapping.h>
//#include <xen/page.h>

#include <linux/slab.h>
#include <linux/amba/xilinx_dma.h>
#include <linux/platform_device.h>

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/poll.h>

#define XILINX_DMA_DEVICE_ID_SHIFT	28

typedef struct _match_info {
	u32 private_value;
	const char *name;
} match_info_t;

char *xdma_addr;
dma_addr_t xdma_handle;

struct xdma_dev *xdma_dev_info[MAX_DEVICES + 1];
u32 num_devices;

static dev_t dev_num;		// Global variable for the device number
static struct class *cl;	// Global variable for the device class
static struct cdev c_dev;	// Global variable for the character device structure

#define RINGBUFFER_SIZE (DMA_LENGTH * 1024)

typedef struct _ringbuffer {
	unsigned char buffer[RINGBUFFER_SIZE];
	unsigned char *pread;
	unsigned char *pwrite;
} ringbuffer_t;

typedef struct _i2s_receiver_dev {
	struct task_struct *i2s_receiver_thread;
	struct xdma_chan_cfg rx_config;
	struct xdma_buf_info rx_buf;
	struct xdma_transfer rx_trans;
	ringbuffer_t *ringbuffer;
	wait_queue_head_t wq;
} i2s_receiver_dev_t;

static i2s_receiver_dev_t i2s_reciever = {0};

int read_dma_buffer_from_ringbuffer(unsigned char *buffer);
bool ringbuffer_empty(void);
void xdma_add_dev_info(struct dma_chan *tx_chan, struct dma_chan *rx_chan, const char *name) {
	struct completion *tx_cmp, *rx_cmp;

	xdma_dev_info[num_devices] = (struct xdma_dev *)kzalloc(sizeof(struct xdma_dev), GFP_KERNEL);
	tx_cmp = (struct completion *)kzalloc(sizeof(struct completion), GFP_KERNEL);
	rx_cmp = (struct completion *)kzalloc(sizeof(struct completion), GFP_KERNEL);

	xdma_dev_info[num_devices]->rx_chan = (u32)rx_chan;
	xdma_dev_info[num_devices]->rx_cmp = (u32)rx_cmp;

	xdma_dev_info[num_devices]->tx_chan = (u32)tx_chan;
	xdma_dev_info[num_devices]->tx_cmp = (u32)tx_cmp;

	strcpy(xdma_dev_info[num_devices]->dma_name, name);

	num_devices++;
}

static bool xdma_filter(struct dma_chan *chan, void *param) {
	match_info_t *pinfo = (match_info_t *)param;

	if (*((int *)chan->private) != pinfo->private_value)
		return false;

	if (strncmp(dma_chan_name(chan), pinfo->name, strlen(pinfo->name)) != 0)
		return false;

	return true;

}

int xdma_probe(void) {
	int rtn = 0;

	dma_cap_mask_t mask;
	match_info_t match_tx, match_rx;
	const char *names[] = {"dma1chan",0};
	struct dma_chan *tx_chan, *rx_chan;
	int i;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE | DMA_PRIVATE, mask);

	for(i = 0; names[i] != 0; i++) {
		match_tx.private_value = ((DMA_MEM_TO_DEV & 0xFF) | XILINX_DMA_IP_DMA);
		match_tx.name = names[i];

		match_rx.private_value = ((DMA_DEV_TO_MEM & 0xFF) | XILINX_DMA_IP_DMA);
		match_rx.name = names[i];

		tx_chan = dma_request_channel(mask, xdma_filter, (void *)&match_tx);
		rx_chan = dma_request_channel(mask, xdma_filter, (void *)&match_rx);

		if (!tx_chan && !rx_chan) {
			printk(KERN_INFO "<%s> probe: channels not found for %s\n", MODULE_NAME, names[i]);
			rtn = -1;
			break;
		} else {
			xdma_add_dev_info(tx_chan, rx_chan, names[i]);
		}
	}

	return rtn;
}


static int xdma_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "<%s> file: open()\n", MODULE_NAME);

	init_waitqueue_head(&i2s_reciever.wq);
	return 0;
}

static int xdma_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "<%s> file: close()\n", MODULE_NAME);

	return 0;
}

int print_dma_buffer(unsigned char *buffer) {
	int rtn = 0;
	int i;
	unsigned int *pdata = (unsigned int *)buffer;
	for(i = 0; i < I2S_DATA_COUNT_EACH_GROUP; i++) {
		printk("%02x\t", (pdata[i] >> 8) & 0xff);
	}
	printk("\n");

	return rtn;
}

int read_dma_buffer(unsigned char *buffer, int n) {
	ssize_t rtn = 0;
	int i = n;

	while(n-- != 0) {
		//printk("%d/%d\n", n + 1, i);
		if(read_dma_buffer_from_ringbuffer(buffer) != 0) {
			break;
		}

		//print_dma_buffer(buffer);

		buffer += DMA_LENGTH;
		rtn += DMA_LENGTH;
	}

	return rtn;
}

static ssize_t xdma_read(struct file *filp, char __user * buf, size_t len, loff_t * off) {
	int rtn = 0;
	//printk(KERN_INFO "<%s> file: read()\n", MODULE_NAME);

	if((len == 0) || (len % DMA_LENGTH) != 0) {
		printk(KERN_INFO "<%s> read: read size is not valid!!!\n", MODULE_NAME, len, *off);
		rtn = -EFAULT;
		return rtn;
	}

	while(ringbuffer_empty()) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(i2s_reciever.wq, (!ringbuffer_empty()));
	}

	rtn = read_dma_buffer(buf, (len / DMA_LENGTH));

	return rtn;
}

static ssize_t xdma_write(struct file *f, const char __user * buf, size_t len, loff_t * off) {
	printk(KERN_INFO "<%s> file: write()\n", MODULE_NAME);

	return len;
}

static int xdma_mmap(struct file *filp, struct vm_area_struct *vma) {
	int result;
	unsigned long requested_size;

	requested_size = vma->vm_end - vma->vm_start;

	printk(KERN_INFO "<%s> file: mmap()\n", MODULE_NAME);
	printk(KERN_INFO "<%s> file: memory size reserved: %d, mmap size requested: %lu\n", MODULE_NAME, DMA_LENGTH, requested_size);

	if (requested_size > DMA_LENGTH) {
		printk(KERN_ERR "<%s> Error: %d reserved != %lu requested)\n", MODULE_NAME, DMA_LENGTH, requested_size);

		return -EAGAIN;
	}

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	result = remap_pfn_range(vma, vma->vm_start, page_to_pfn(virt_to_page(xdma_addr))/*virt_to_pfn(xdma_addr)*/, requested_size, vma->vm_page_prot);

	if (result) {
		printk(KERN_ERR "<%s> Error: in calling remap_pfn_range: returned %d\n", MODULE_NAME, result);

		return -EAGAIN;
	}

	return 0;
}

static void xdma_get_dev_info(char *name, struct xdma_dev *dev) {
	int i;

	for (i = 0; i < MAX_DEVICES; i++) {
		if(strncmp(xdma_dev_info[i]->dma_name, name, strlen(name)) != 0)
			break;
	}
	memcpy(dev, xdma_dev_info[i], sizeof(struct xdma_dev));
}

void xdma_device_control(struct xdma_chan_cfg *chan_cfg) {
	struct dma_chan *chan;
	struct dma_device *chan_dev;
	struct xilinx_dma_config config;

	config.direction = chan_cfg->dir;
	config.coalesc = chan_cfg->coalesc;
	config.delay = chan_cfg->delay;
	config.reset = chan_cfg->reset;

	chan = (struct dma_chan *)chan_cfg->chan;

	if (chan) {
		chan_dev = chan->device;
		chan_dev->device_control(chan, DMA_SLAVE_CONFIG, (unsigned long)&config);
	}
}

static void xdma_sync_callback(void *completion) {
	complete(completion);
}

void xdma_prep_buffer(struct xdma_buf_info *buf_info) {
	struct dma_chan *chan;
	dma_addr_t buf;
	size_t len;
	enum dma_transfer_direction dir;
	enum dma_ctrl_flags flags;
	struct dma_async_tx_descriptor *chan_desc;
	struct completion *cmp;
	dma_cookie_t cookie;

	chan = (struct dma_chan *)buf_info->chan;
	cmp = (struct completion *)buf_info->completion;
	buf = xdma_handle + buf_info->buf_offset;
	len = buf_info->buf_size;
	dir = buf_info->dir;

	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;

	chan_desc = dmaengine_prep_slave_single(chan, buf, len, dir, flags);
	if (!chan_desc) {
		printk(KERN_ERR "<%s> Error: dmaengine_prep_slave_single error\n", MODULE_NAME);
		buf_info->cookie = -EBUSY;
	} else {
		chan_desc->callback = xdma_sync_callback;
		chan_desc->callback_param = cmp;

		// set the prepared descriptor to be executed by the engine
		cookie = chan_desc->tx_submit(chan_desc);
		if (dma_submit_error(cookie)) {
			printk(KERN_ERR "<%s> Error: tx_submit error\n", MODULE_NAME);
		}

		buf_info->cookie = cookie;
	}
}

void xdma_start_transfer(struct xdma_transfer *trans) {
	unsigned long tmo = msecs_to_jiffies(1000 * 60 * 60 * 60);
	enum dma_status status;
	struct dma_chan *chan;
	struct completion *cmp;
	dma_cookie_t cookie;

	chan = (struct dma_chan *)trans->chan;
	cmp = (struct completion *)trans->completion;
	cookie = trans->cookie;

	init_completion(cmp);
	dma_async_issue_pending(chan);

	if (trans->wait) {
		tmo = wait_for_completion_timeout(cmp, tmo);
		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);
		if (0 == tmo) {
			printk(KERN_ERR "<%s> Error: transfer timed out\n", MODULE_NAME);
		} else if (status != DMA_COMPLETE) {
			printk(KERN_INFO "<%s> transfer: returned completion callback status of: \'%s\'\n", MODULE_NAME, status == DMA_ERROR ? "error" : "in progress");
		}
	}
}

void xdma_stop_transfer(struct dma_chan *chan) {
	struct dma_device *chan_dev;

	if (chan) {
		chan_dev = chan->device;
		chan_dev->device_control(chan, DMA_TERMINATE_ALL, (unsigned long)NULL);
	}
}

static long xdma_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	struct xdma_dev xdma_dev;
	struct xdma_chan_cfg chan_cfg;
	struct xdma_buf_info buf_info;
	struct xdma_transfer trans;
	u32 devices;
	u32 chan;

	switch (cmd) {
		case XDMA_GET_NUM_DEVICES:
			printk(KERN_INFO "<%s> ioctl: XDMA_GET_NUM_DEVICES\n", MODULE_NAME);

			devices = num_devices;
			if (copy_to_user((u32 *) arg, &devices, sizeof(u32)))
				return -EFAULT;

			break;
		case XDMA_GET_DEV_INFO:
			printk(KERN_INFO "<%s> ioctl: XDMA_GET_DEV_INFO\n", MODULE_NAME);

			if (copy_from_user((void *)&xdma_dev, (const void __user *)arg, sizeof(struct xdma_dev)))
				return -EFAULT;

			xdma_get_dev_info(xdma_dev.dma_name, &xdma_dev);

			if (copy_to_user((struct xdma_dev *)arg, &xdma_dev, sizeof(struct xdma_dev)))
				return -EFAULT;
			break;
		case XDMA_DEVICE_CONTROL:
			printk(KERN_INFO "<%s> ioctl: XDMA_DEVICE_CONTROL\n", MODULE_NAME);

			if (copy_from_user((void *)&chan_cfg, (const void __user *)arg, sizeof(struct xdma_chan_cfg)))
				return -EFAULT;
			xdma_device_control(&chan_cfg);
			break;
		case XDMA_PREP_BUF:
			printk(KERN_INFO "<%s> ioctl: XDMA_PREP_BUF\n", MODULE_NAME);

			if (copy_from_user((void *)&buf_info, (const void __user *)arg, sizeof(struct xdma_buf_info)))
				return -EFAULT;

			xdma_prep_buffer(&buf_info);

			if (copy_to_user((struct xdma_buf_info *)arg, &buf_info, sizeof(struct xdma_buf_info)))
				return -EFAULT;

			break;
		case XDMA_START_TRANSFER:
			printk(KERN_INFO "<%s> ioctl: XDMA_START_TRANSFER\n", MODULE_NAME);

			if(copy_from_user((void *)&trans, (const void __user *)arg, sizeof(struct xdma_transfer)))
				return -EFAULT;

			xdma_start_transfer(&trans);
			break;
		case XDMA_STOP_TRANSFER:
			printk(KERN_INFO "<%s> ioctl: XDMA_STOP_TRANSFER\n", MODULE_NAME);

			if (copy_from_user((void *)&chan, (const void __user *)arg, sizeof(u32)))
				return -EFAULT;

			xdma_stop_transfer((struct dma_chan *)chan);
			break;
		default:
			break;
	}

	return 0;
}

static unsigned int xdma_poll(struct file *filp, poll_table *wait) {
	struct scull_pipe *dev =filp->private_data;
	unsigned int mask =0;

	dev = dev;
	/* 把等待队列添加到poll_table */
	poll_wait(filp, &i2s_reciever.wq, wait);

	/*返回掩码*/
	if (!ringbuffer_empty())
		mask = POLLIN | POLLRDNORM;/*设备可读*/
	return mask;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = xdma_open,
	.release = xdma_close,
	.read = xdma_read,
	.write = xdma_write,
	.mmap = xdma_mmap,
	.poll = xdma_poll,
	.unlocked_ioctl = xdma_ioctl,
};

void xdma_remove(void) {
	int i;

	for (i = 0; i < MAX_DEVICES; i++) {
		if (xdma_dev_info[i]) {
			if (xdma_dev_info[i]->tx_chan) {
				dma_release_channel((struct dma_chan *) xdma_dev_info[i]->tx_chan);
			}

			if (xdma_dev_info[i]->tx_cmp) {
				kfree((struct completion *) xdma_dev_info[i]->tx_cmp);
				xdma_dev_info[i]->tx_cmp = NULL;
			}

			if (xdma_dev_info[i]->rx_chan) {
				dma_release_channel((struct dma_chan *) xdma_dev_info[i]->rx_chan);
			}

			if (xdma_dev_info[i]->rx_cmp) {
				kfree((struct completion *) xdma_dev_info[i]->rx_cmp);
				xdma_dev_info[i]->rx_cmp = NULL;
			}
		}
	}
}

int init_i2s_ring_buffer(void) {
	int rtn = 0;

	i2s_reciever.ringbuffer = (ringbuffer_t *)kzalloc(sizeof(ringbuffer_t), GFP_KERNEL);
	if(i2s_reciever.ringbuffer == NULL) {
		rtn = -1;
	}

	i2s_reciever.ringbuffer->pwrite = i2s_reciever.ringbuffer->pread = i2s_reciever.ringbuffer->buffer;

	return rtn;
}

int uninit_i2s_ring_buffer(void) {
	int rtn = 0;

	if(i2s_reciever.ringbuffer != NULL) {
		kfree((ringbuffer_t *) i2s_reciever.ringbuffer);
	}

	i2s_reciever.ringbuffer = NULL;

	return rtn;
}

int write_dma_buffer_to_ringbuffer(unsigned char *buffer) {
	int rtn = 0;

	if(i2s_reciever.ringbuffer->pwrite == i2s_reciever.ringbuffer->pread) {
		//printk("<%s> ringbuffer overwrite!!!!!!!!!!!!!!!\n", MODULE_NAME);
	}

	memcpy(i2s_reciever.ringbuffer->pwrite, buffer, DMA_LENGTH);

	if((i2s_reciever.ringbuffer->pwrite + DMA_LENGTH) == (i2s_reciever.ringbuffer->buffer + RINGBUFFER_SIZE)) {
		i2s_reciever.ringbuffer->pwrite = i2s_reciever.ringbuffer->buffer;
	} else {
		i2s_reciever.ringbuffer->pwrite += DMA_LENGTH;
	}

	return rtn;
}

int read_dma_buffer_from_ringbuffer(unsigned char *buffer) {
	int rtn = 0;
	if(i2s_reciever.ringbuffer->pread == i2s_reciever.ringbuffer->pwrite) {
		//printk(KERN_INFO "<%s> ringbuffer nothing!!!!!!!!!!!!!!!\n", MODULE_NAME);
		rtn = -1;
		return rtn;
	}

	copy_to_user(buffer, i2s_reciever.ringbuffer->pread, DMA_LENGTH);

	if((i2s_reciever.ringbuffer->pread + DMA_LENGTH) == (i2s_reciever.ringbuffer->buffer + RINGBUFFER_SIZE)) {
		i2s_reciever.ringbuffer->pread = i2s_reciever.ringbuffer->buffer;
	} else {
		i2s_reciever.ringbuffer->pread += DMA_LENGTH;
	}

	return rtn;
}

bool ringbuffer_empty(void) {
	return (i2s_reciever.ringbuffer->pread == i2s_reciever.ringbuffer->pwrite);
}

void xdma_i2s_receive(void) {
	unsigned int *pdata = xdma_addr;

	static int i;



	static struct timeval ti, tf;


	//memset(xdma_addr, 0xff, DMA_LENGTH);	// fill rx with a value

	//// display contents before transfer:
	//printk(KERN_INFO "<%s> test: rx buffer before transmit:\n", MODULE_NAME);
	//for (i = 0; i < I2S_DATA_COUNT_EACH_GROUP; i++) {
	//	printk("%08x\t", pdata[i]);
	//}
	//printk("\n");

	// measure time:
	do_gettimeofday(&ti);

	i2s_reciever.rx_config.chan = xdma_dev_info[0]->rx_chan;
	i2s_reciever.rx_config.coalesc = 1;
	i2s_reciever.rx_config.delay = 0;
	xdma_device_control(&i2s_reciever.rx_config);

	i2s_reciever.rx_buf.chan = xdma_dev_info[0]->rx_chan;
	i2s_reciever.rx_buf.buf_offset = (u32) 0;
	i2s_reciever.rx_buf.buf_size = (u32) DMA_LENGTH;
	i2s_reciever.rx_buf.dir = DMA_DEV_TO_MEM;
	i2s_reciever.rx_buf.completion = (u32) xdma_dev_info[0]->rx_cmp;
	xdma_prep_buffer(&i2s_reciever.rx_buf);

	//printk(KERN_INFO "<%s> test: xdma_start_transfer rx\n", MODULE_NAME);
	i2s_reciever.rx_trans.chan = xdma_dev_info[0]->rx_chan;
	i2s_reciever.rx_trans.wait = 1;
	i2s_reciever.rx_trans.completion = (u32) xdma_dev_info[0]->rx_cmp;
	i2s_reciever.rx_trans.cookie = i2s_reciever.rx_buf.cookie;

	// measure time to prepare channels:
	do_gettimeofday(&tf);
	//printk(KERN_INFO "<%s> test: time to prepare DMA channels [us]: %ld\n", MODULE_NAME, (tf.tv_usec - ti.tv_usec));
	do_gettimeofday(&ti);	// to read transfer time only

	// start transfer:
	xdma_start_transfer(&i2s_reciever.rx_trans);

	// measure time:
	do_gettimeofday(&tf);
	//printk(KERN_INFO "<%s> test: DMA transfer time [us]: %ld\n", MODULE_NAME, (tf.tv_usec - ti.tv_usec));
	//printk(KERN_INFO "<%s> test: DMA bytes received: %d\n", MODULE_NAME, DMA_LENGTH);
	//printk(KERN_INFO "<%s> test: DMA speed in Mbytes/s: %ld\n", MODULE_NAME, DMA_LENGTH / (tf.tv_usec - ti.tv_usec));

	//// display contents after transfer:
	//printk(KERN_INFO "<%s> test: rx buffer after transmit:\n", MODULE_NAME);

	//for (i = 0; i < I2S_DATA_COUNT_EACH_GROUP; i++) {
	//	printk("%08x\t", pdata[i]);
	//}
	//printk("\n");
	write_dma_buffer_to_ringbuffer(xdma_addr);
}

int i2s_receiver_worker_thread(void *ppara) {
	int rtn = 0;
	while(true) {
		if(kthread_should_stop()) {
			return -1;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		xdma_i2s_receive();
	}

	return rtn;
}

static int start_receiver_thread(void) {
	int rtn = 0;

	i2s_reciever.i2s_receiver_thread = kthread_run(i2s_receiver_worker_thread, NULL, "%s", "i2s_receiver_driver");

	if (IS_ERR_OR_NULL(i2s_reciever.i2s_receiver_thread)) {
		rtn = i2s_reciever.i2s_receiver_thread ? PTR_ERR(i2s_reciever.i2s_receiver_thread) : -EINTR;
	}

	return rtn;
}

static int stop_receiver_thread(void) {
	int rtn = 0;
	rtn = kthread_stop(i2s_reciever.i2s_receiver_thread);
	return rtn;
}

static int __init xdma_init(void)
{
	int rtn = 0;

	/* device constructor */
	printk(KERN_INFO "<%s> init: registered\n", MODULE_NAME);

	/* allocate mmap area */
	xdma_addr = dma_zalloc_coherent(NULL, DMA_LENGTH, &xdma_handle, GFP_KERNEL);
	if (!xdma_addr) {
		rtn = -1;
		printk(KERN_ERR "<%s> Error: allocating dma memory failed\n", MODULE_NAME);
		rtn = -ENOMEM;
		goto dma_zalloc_coherent_failed;
	}

	/* hardware setup */
	if(xdma_probe() != 0) {
		rtn = -1;
		printk(KERN_ERR "<%s> Error: xdma_probe_failed\n", MODULE_NAME);
		goto xdma_probe_failed;
	}

	if ((cl = class_create(THIS_MODULE, MODULE_NAME)) == NULL) {
		rtn = -1;
		printk(KERN_ERR "<%s> Error: class_create_failed\n", MODULE_NAME);
		goto class_create_failed;
	}

	if (alloc_chrdev_region(&dev_num, 0, 1, MODULE_NAME) < 0) {
		rtn = -1;
		printk(KERN_ERR "<%s> Error: alloc_chrdev_region_failed\n", MODULE_NAME);
		goto alloc_chrdev_region_failed;
	}

	cdev_init(&c_dev, &fops);

	if (cdev_add(&c_dev, dev_num, 1) == -1) {
		rtn = -1;
		goto cdev_add_failed;
	}

	if (device_create(cl, NULL, dev_num, NULL, MODULE_NAME) == NULL) {
		rtn = -1;
		printk(KERN_ERR "<%s> Error: device_create_failed\n", MODULE_NAME);
		goto device_create_failed;
	}

	if(init_i2s_ring_buffer() != 0) {
		goto init_i2s_ring_buffer_failed;
	}

	start_receiver_thread();

	return rtn;

init_i2s_ring_buffer_failed:
	device_destroy(cl, dev_num);
device_create_failed:
	cdev_del(&c_dev);
cdev_add_failed:
	unregister_chrdev_region(dev_num, 1);
alloc_chrdev_region_failed:
	class_destroy(cl);
class_create_failed:
	xdma_remove();
xdma_probe_failed:
	dma_free_coherent(NULL, DMA_LENGTH, xdma_addr, xdma_handle);
dma_zalloc_coherent_failed:
	return rtn;
}

static void __exit xdma_exit(void) {
	printk(KERN_INFO "<%s> exit: unregistered\n", MODULE_NAME);

	stop_receiver_thread();

	uninit_i2s_ring_buffer();

	/* device destructor */
	device_destroy(cl, dev_num);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev_num, 1);
	class_destroy(cl);

	/* hardware shutdown */
	xdma_remove();

	/* free mmap area */
	if (xdma_addr) {
		dma_free_coherent(NULL, DMA_LENGTH, xdma_addr, xdma_handle);
	}
}

module_init(xdma_init);
module_exit(xdma_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Wrapper Driver For A Xilinx DMA Engine");

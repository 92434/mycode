/*
 * xvdma.c
 *
 * Xilinx Video DMA Driver
 *
 * Author: Xilinx Inc.
 *
 * 2002-2006 (c)Xilinx Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#include <linux/module.h>
//#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/slab.h>
//#include <linux/fcntl.h>
//#include <linux/sysctl.h>
//#include <linux/fs.h>
//#include <linux/of_address.h>
//#include <linux/of_device.h>
#include <linux/of_platform.h>
//#include <asm/io.h>
#include <asm/uaccess.h>
//#include <linux/dma-mapping.h>
//#include <linux/dmaengine.h>
#include <linux/amba/xilinx_dma.h>
#include <linux/sched.h>
#include <linux/uio_driver.h>
#include "xvdma.h"

#define printk_xiaofei
//#define printk_xiaofei printk

struct xvdma_dev *xvdma_dev_info[MAX_DEVICES + 1];

static u64 dma_mask = 0xFFFFFFFFUL;
u32 num_devices;
struct completion cmp;

static void release(struct device *dev) {
}

struct uio_info myuio_info = {
	.name = DRIVER_NAME,
	.version = "0.1",
	.irq = UIO_IRQ_NONE,
};

static struct resource resource[] = {
	[0] = {
		.start	= 0,
		.end	= 0,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device my_uio_device = {
	.name = "uio_pdrv",
	.id = -1,
	.dev = {
		.platform_data = &myuio_info,
		.release = release,
	},
	.resource = resource,
	.num_resources = ARRAY_SIZE(resource),
};

void xvdma_get_dev_info(char *name, struct xvdma_dev *dev)
{
	int i;

	for(i = 0; i < num_devices; i++) {
		if(strcmp(xvdma_dev_info[i]->dma_name, name) == 0)
			break;
	}
	memcpy(dev, xvdma_dev_info[i], sizeof(struct xvdma_dev));
}

/*
 * This function is called when an application opens handle to the
 * bridge driver.
 */
static int xvdma_open(struct inode *ip, struct file *filp)
{
	return 0;
}

static int xvdma_release(struct inode *ip, struct file *filp)
{
	return 0;
}

void xvdma_device_control(struct xvdma_chan_cfg *chan_cfg)
{
	struct dma_chan *chan;
	struct dma_device *chan_dev;

	chan = (struct dma_chan *)chan_cfg->chan;

	if (chan) {
		chan_dev = chan->device;
		chan_dev->device_control(chan, DMA_SLAVE_CONFIG, (unsigned long)&chan_cfg->config);
	}
}

void vdma_sync_callback(void *completion)
{
	printk_xiaofei("xiaofei: %s:%d\n", __func__, __LINE__);
	complete(completion);
}

void xvdma_prep_slave_sg(struct xvdma_buf_info *buf_info)
{
	static u32 frm_cnt = 0;
	static int buf_size = 0;
	static dma_addr_t dma_srcs[MAX_FRAMES];
	static dma_addr_t dma_dsts[MAX_FRAMES];

	u32 flags = DMA_CTRL_ACK | DMA_COMPL_SKIP_DEST_UNMAP | DMA_PREP_INTERRUPT;
	struct dma_chan *chan = (struct dma_chan *)buf_info->chan;
	struct dma_device *chan_dev = (chan != 0) ? chan->device : 0;
	char *pix_buffer;

	struct scatterlist chansg[MAX_FRAMES];
	struct dma_async_tx_descriptor *chan_desc;
	dma_addr_t *dma_addrs;
	int i;

	if(chan == 0) {
		return;
	}

	if(buf_info->mem_type == DMA_TO_DEVICE) {
		dma_addrs = dma_srcs;
		pix_buffer = myuio_info.mem[0].addr;
	} else if(buf_info->mem_type == DMA_FROM_DEVICE) {
		dma_addrs = dma_dsts;
		pix_buffer = myuio_info.mem[0].addr + IMAGE_SIZE;
	} else {
		return;
	}

	for (i = 0; i < frm_cnt; i++) {
		dma_unmap_single(chan_dev->dev, dma_addrs[i], buf_size, buf_info->mem_type);
	}

	frm_cnt = buf_info->frm_cnt;
	buf_size = buf_info->buf_size;
	if(frm_cnt * buf_size > BUFFER_SIZE) {
		return;
	}

	sg_init_table(chansg, frm_cnt);
	for (i = 0; i < frm_cnt; i++) {
		dma_addrs[i] = dma_map_single(chan_dev->dev, pix_buffer + i * buf_size, buf_size, buf_info->mem_type);
		printk_xiaofei("xiaofei: %s:%d dma_addrs[i]:%x buf_size:%x\n", __func__, __LINE__, dma_addrs[i], buf_size);
		sg_dma_address(&chansg[i]) = dma_srcs[i];
		sg_dma_len(&chansg[i]) = buf_size;
	}

	chan_desc = chan_dev->device_prep_slave_sg(chan, chansg, frm_cnt, buf_info->direction, flags, NULL);
	if(chan_desc == 0) {
		return;
	}

	if (buf_info->callback) {
		chan_desc->callback = vdma_sync_callback;
		chan_desc->callback_param = &cmp;
	}

	chan_desc->tx_submit(chan_desc);
}

int map_xvdma_buffer_addr(xvdma_buffer_addr_t *addr) {
	int ret = 0;

	addr->in_addr = virt_to_phys(myuio_info.mem[0].addr + IMAGE_SIZE);
	addr->out_addr = virt_to_phys(myuio_info.mem[0].addr);
	printk("xiaofei: %s:%d addr->in_addr:%p(%p) addr->out_addr %p(%p)\n", __func__, __LINE__, addr->in_addr, myuio_info.mem[0].addr + IMAGE_SIZE, addr->out_addr, myuio_info.mem[0].addr);
	return ret;
}

struct xdma_regs {
	u32 cr;     /* 0x00 Control Register */
	u32 sr;     /* 0x04 Status Register */
	u32 cdr;    /* 0x08 Current Descriptor Register */
	u32 pad1;
	u32 tdr;    /* 0x10 Tail Descriptor Register */
	u32 pad2;
	u32 src;    /* 0x18 Source Address Register (cdma) */
	u32 pad3;
	u32 dst;    /* 0x20 Destination Address Register (cdma) */
	u32 pad4;
	u32 btt_ref;/* 0x28 Bytes To Transfer (cdma) or park_ref (vdma) */
	u32 version;         /* 0x2c version (vdma) */
};

struct debugfs_regset32 {
	struct debugfs_reg32 *regs;
	int nregs;
	void __iomem *base;
};

/* Per DMA specific operations should be embedded in the channel structure
*/
struct xilinx_dma_chan {
	struct xdma_regs __iomem *regs;   /* Control status registers */
	struct vdma_addr_regs *addr_regs; /* Direct address registers */
	spinlock_t lock;                  /* Descriptor operation lock */
	struct list_head active_list;	  /* Active descriptors */
	struct list_head pending_list;	  /* Descriptors waiting */
	struct list_head removed_list;       /* Descriptors queued for removal */
	struct dma_chan common;           /* DMA common channel */
	struct dma_pool *desc_pool;       /* Descriptors pool */
	struct device *dev;               /* The dma device */
	int    irq;                       /* Channel IRQ */
	int    id;                        /* Channel ID */
	enum dma_transfer_direction direction;/* Transfer direction */
	int    max_len;                   /* Maximum data len per transfer */
	int    num_frms;                  /* Number of frames */
	int    has_SG;                    /* Support scatter transfers */
	int    has_DRE;                   /* Support unaligned transfers */
	int    genlock;                   /* Support genlock mode */
	int    err;                       /* Channel has errors */
	struct tasklet_struct tasklet;    /* Cleanup work after irq */
	u32    feature;                   /* IP feature */
	void   (*start_transfer)(struct xilinx_dma_chan *chan);
	struct xilinx_dma_config config;  /* Device configuration info */

	bool cyclic;
	struct debugfs_regset32 debugfs_regset;
};

#define to_xilinx_chan(chan) container_of(chan, struct xilinx_dma_chan, common)
#define XILINX_DMA_XR_IRQ_IOC_MASK	0x00001000 /* Completion interrupt */
#define DMA_IN(addr)  (ioread32(addr))

void xvdma_stop_transfer(struct dma_chan *chan)
{
	struct dma_device *chan_dev;

	if (chan) {
		chan_dev = chan->device;
		chan_dev->device_control(chan, DMA_TERMINATE_ALL, (unsigned long)1);
	}
}

void xvdma_start_transfer(struct xvdma_transfer *tx_info) {
	unsigned long tmo = msecs_to_jiffies(500);
	unsigned long expire = tmo + jiffies;
	static struct dma_chan *dma_chan[] = {0,0};

	init_completion(&cmp);

	if (tx_info->chan)
		dma_async_issue_pending((struct dma_chan *)tx_info->chan);

	if (tx_info->wait) {
		printk_xiaofei("xiaofei: %s:%d:wait for complete...\n", __func__, __LINE__);
		//tmo = wait_for_completion_timeout(&cmp, tmo);
		struct xilinx_dma_chan *chan = to_xilinx_chan((struct dma_chan *)tx_info->chan);
		while(expire > jiffies) {
			if(XILINX_DMA_XR_IRQ_IOC_MASK & DMA_IN(&chan->regs->sr)) {
				break;
			}
			schedule();
		}

		if(expire <= jiffies) {
			printk("xiaofei: %s:%d:Timeout has occured...\n", __func__, __LINE__);
		} else {
			printk_xiaofei("xiaofei: %s:%d:transmit complete!\n", __func__, __LINE__);
		}
		dma_chan[1] = (struct dma_chan *)tx_info->chan;
		int i;
		for(i = 0; i < 2; i++){
			if(dma_chan[i] == 0) {
				continue;
			}
			xvdma_stop_transfer(dma_chan[i]);
			dma_chan[i] = 0;
		}
	} else {
		dma_chan[0] = (struct dma_chan *)tx_info->chan;
	}

}

static long xvdma_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	u32 devices, chan;
	struct xvdma_dev xvdma_dev;
	struct xvdma_chan_cfg chan_cfg;
	struct xvdma_buf_info buf_info;
	struct xvdma_transfer tx_info;

	switch (cmd) {
		case XVDMA_GET_NUM_DEVICES:
			{
				devices = num_devices;
				if (copy_to_user((u32 *)arg, &devices, sizeof(u32)))
					return -EFAULT;
				break;
			}
		case XVDMA_GET_DEV_INFO:
			{
				if (copy_from_user((void *)&xvdma_dev, (const void __user *)arg, sizeof(struct xvdma_dev)))
					return -EFAULT;

				xvdma_get_dev_info(xvdma_dev.dma_name, &xvdma_dev);

				if (copy_to_user((struct xvdma_dev *)arg, &xvdma_dev, sizeof(struct xvdma_dev)))
					return -EFAULT;
				break;
			}
		case XVDMA_DEVICE_CONTROL:
			{
				if(copy_from_user((void *)&chan_cfg, (const void __user *)arg, sizeof(struct xvdma_chan_cfg)))
					return -EFAULT;

				xvdma_device_control(&chan_cfg);
				break;
			}
		case XVDMA_PREP_BUF:
			{
				if(copy_from_user((void *)&buf_info, (const void __user *)arg, sizeof(struct xvdma_buf_info)))
					return -EFAULT;
				xvdma_prep_slave_sg(&buf_info);
				break;
			}
		case XVDMA_START_TRANSFER:
			{
				if(copy_from_user((void *)&tx_info, (const void __user *)arg, sizeof(struct xvdma_transfer)))
					return -EFAULT;

				xvdma_start_transfer(&tx_info);
				break;
			}
		case XVDMA_STOP_TRANSFER:
			{
				if(copy_from_user((void *)&chan, (const void __user *)arg, sizeof(u32)))
					return -EFAULT;

				xvdma_stop_transfer((struct dma_chan *)chan);
				break;
			}
		case XVDMA_GET_DMA_MEM_ADDR:
		{
			xvdma_buffer_addr_t addr;
			map_xvdma_buffer_addr(&addr);
			if (copy_to_user((u32 *)arg, &addr, sizeof(xvdma_buffer_addr_t))) {
				return -EFAULT;
			}
		}
		break;
		default:
			break;
	}
	return 0;
}

void xvdma_add_dev_info(struct dma_chan *tx_chan, struct dma_chan *rx_chan, const char *name)
{
	xvdma_dev_info[num_devices] = (struct xvdma_dev *)kzalloc(sizeof(struct xvdma_dev), GFP_KERNEL);

	xvdma_dev_info[num_devices]->tx_chan = (u32) tx_chan;
	xvdma_dev_info[num_devices]->rx_chan = (u32) rx_chan;

	strcpy(xvdma_dev_info[num_devices]->dma_name, name);

	num_devices++;
}

typedef struct _match_info {
	u32 private_value;
	const char *name;
} match_info_t;

static bool xvdma_filter(struct dma_chan *chan, void *param)
{
	match_info_t *pinfo = (match_info_t *)param;

	printk_xiaofei("[%s]--match %x:%x\n", __func__, pinfo->private_value, chan->private);
	printk_xiaofei("[%s]--match %s:%s\n", __func__, pinfo->name, dev_name(chan->device->dev));

	if ((int)chan->private != pinfo->private_value)
		return false;

	if (strcmp(dev_name(chan->device->dev), pinfo->name) != 0)
		return false;

	return true;
}

void xvdma_scan_channels(void)
{
	dma_cap_mask_t mask;
	match_info_t match_tx, match_rx;
	const char *names[] = {DMA_SOBEL_NAME,0};
	struct dma_chan *tx_chan, *rx_chan;
	int i;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE | DMA_PRIVATE, mask);

	for(i = 0; names[i] != 0; i++) {
		match_tx.private_value = ((DMA_MEM_TO_DEV & 0xFF) | XILINX_DMA_IP_VDMA);
		match_tx.name = names[i];

		match_rx.private_value = ((DMA_DEV_TO_MEM & 0xFF) | XILINX_DMA_IP_VDMA);
		match_rx.name = names[i];

		tx_chan = dma_request_channel(mask, xvdma_filter, (void *)&match_tx);
		rx_chan = dma_request_channel(mask, xvdma_filter, (void *)&match_rx);

		if (!tx_chan && !rx_chan) {
			printk(KERN_INFO "<%s> probe: channels not found for %s\n", DRIVER_NAME, names[i]);
			break;
		} else {
			xvdma_add_dev_info(tx_chan, rx_chan, names[i]);
		}
	}
}

static const struct file_operations xvdma_fops = {
	.owner = THIS_MODULE,
	.open = xvdma_open,
	.unlocked_ioctl = xvdma_ioctl,
	.release = xvdma_release,
};

struct xvdma_drvdata {
	struct device *dev;
	struct cdev cdev;       /* Char device structure */
	dev_t devt;
};
static struct class *xvdma_class;

static int __devinit xvdma_probe(struct platform_device *pdev)
{
	dev_t devt;
	struct xvdma_drvdata *drvdata = NULL;
	struct device *dev = &pdev->dev;
	int retval;

	int i;
	for(i = 0; i < MAX_DEVICES; i++) {
		xvdma_dev_info[i] = 0;
	}

	devt = MKDEV(XVDMA_MAJOR, XVDMA_MINOR);

	drvdata = kzalloc(sizeof(struct xvdma_drvdata), GFP_KERNEL);
	if (!drvdata) {
		dev_err(dev, "Couldn't allocate device private record\n");
		retval = -ENOMEM;
		goto failed0;
	}

	drvdata->dev = dev;
	drvdata->devt = devt;

	xvdma_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(xvdma_class)) {
		retval = PTR_ERR(xvdma_class);
		goto failed1;
	}

	cdev_init(&drvdata->cdev, &xvdma_fops);
	drvdata->cdev.owner = THIS_MODULE;
	retval = cdev_add(&drvdata->cdev, devt, 1);
	if (retval) {
		dev_err(dev, "cdev_add() failed\n");
		goto failed2;
	}

	struct device *device = NULL;
	device = device_create(xvdma_class, dev, devt, NULL, DRIVER_NAME);
	if (IS_ERR(device)) {
		dev_err(dev, "device_create() failed\n");
		goto failed3;
	}

	dev_set_drvdata(dev, (void *)drvdata);

	xvdma_scan_channels();
	dev_info(dev, "Xilinx VDMA probe successful\n \
			Devices Scanned %d\n", num_devices);
	return 0;
failed3:
	cdev_del(&drvdata->cdev);
failed2:
	class_destroy(xvdma_class);
failed1:
	kfree(drvdata);
failed0:
	return retval;
}

void xvdma_release_channels(void)
{
	int i;
	for(i = 0; i < MAX_DEVICES; i++) {
		if(xvdma_dev_info[i] == 0) {
			continue;
		}

		if (xvdma_dev_info[i]->tx_chan)
			dma_release_channel((struct dma_chan *)xvdma_dev_info[i]->tx_chan);
		if (xvdma_dev_info[i]->rx_chan)
			dma_release_channel((struct dma_chan *)xvdma_dev_info[i]->rx_chan);
	}
}

static int __devexit xvdma_remove(struct platform_device *op)
{
	struct xvdma_drvdata *drvdata;
	struct device *dev = &op->dev;

	drvdata = (struct xvdma_drvdata *)dev_get_drvdata(dev);
	if (!drvdata)
		return 0;

	xvdma_release_channels();
	device_destroy(xvdma_class, MKDEV(XVDMA_MAJOR, XVDMA_MINOR));
	cdev_del(&drvdata->cdev);
	class_destroy(xvdma_class);
	kfree(drvdata);
	dev_set_drvdata(dev, NULL);

	return 0;
}

static struct platform_driver xvdma_driver = {
	.driver = {
		.name = DRIVER_NAME,
	},
	.probe = xvdma_probe,
	.remove = __devexit_p(xvdma_remove),
	.suspend = XVDMA_SUSPEND,
	.resume = XVDMA_RESUME,
};

static struct platform_device xvdma_device = {
	.name = DRIVER_NAME,
	.id = 0,
	.dev = {
		.platform_data = NULL,
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xFFFFFFFF,
		.release = release,
	},
	.resource = NULL,
	.num_resources = 0,
};

static int __init xvdma_init(void)
{
	int ret;

	my_uio_device.resource[0].start = (u32)kmalloc(BUFFER_SIZE, GFP_KERNEL);
	if(my_uio_device.resource[0].start == 0) {
		ret = -ENOMEM;
		goto uio_mem_failed;
	}
	my_uio_device.resource[0].end = my_uio_device.resource[0].start + BUFFER_SIZE - 1;
	printk_xiaofei("xiaofei: %s:%d addr:%x end:%x\n", __func__, __LINE__, my_uio_device.resource[0].start, my_uio_device.resource[0].end);

	ret = platform_device_register(&my_uio_device);
	if(ret != 0) {
		goto uio_register_failed;
	}

	ret = platform_device_register(&xvdma_device);
	if(ret != 0) {
		goto xvdma_device_reigster_failed;
	}

	ret = platform_driver_register(&xvdma_driver);
	if(ret != 0) {
		goto xvdma_driver_reigster_failed;
	}

	return 0;

xvdma_driver_reigster_failed:
	platform_device_unregister(&xvdma_device);
xvdma_device_reigster_failed:
	platform_device_unregister(&my_uio_device);
uio_register_failed:
	kfree(myuio_info.mem[0].addr);
uio_mem_failed:
	return ret;
}

static void __exit xvdma_exit(void)
{
	platform_device_unregister(&xvdma_device);
	platform_driver_unregister(&xvdma_driver);
	platform_device_unregister(&my_uio_device);
	kfree(my_uio_device.resource[0].start);
}

module_init(xvdma_init);
module_exit(xvdma_exit);

MODULE_DESCRIPTION("Xilinx VDMA driver wrap");
MODULE_LICENSE("GPL");

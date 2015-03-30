/*
 * xotsu2.c
 *
 * xotsu2 Driver
 *
 * Author: xiaofei.
 *
 */
#include <linux/module.h>
//#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/slab.h>
//#include <linux/fcntl.h>
//#include <linux/sysctl.h>
//#include <linux/fs.h>
#include <linux/of_address.h>
//#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
//#include <asm/io.h>
#include <asm/uaccess.h>
//#include <linux/dma-mapping.h>
//#include <linux/dmaengine.h>
#include <linux/amba/xilinx_dma.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include "xotsu2.h"
#ifdef KERNEL_DIGILENT
#include <linux/uio_driver.h>
#else
#include <linux/interrupt.h>
#include <linux/fs.h>
#endif

#define XOTSU2_CONTROL_BUS_ADDR_AP_RETURN 0x20
#define XOTSU2_CONTROL_BUS_ADDR_ISR       0x0c

//#define printk_xiaofei
#define printk_xiaofei printk

struct xotsu2_device {
	void __iomem *regs;
	struct device *dev;
	int irq;
	struct tasklet_struct tasklet;    /* Cleanup work after irq */
};

static struct xotsu2_device *xdev;
static struct completion cmp;

xotsu2_buffer_addr_t buffer_addr;
static int create_image_buffer(void) {
	int ret = 0;

	buffer_addr.k_out_addr = (u32)kmalloc(BUFFER_SIZE, GFP_KERNEL | GFP_DMA);
	if(buffer_addr.k_out_addr == 0) {
		ret = -ENOMEM;
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto kmalloc_failed;
	}

	buffer_addr.k_in_addr = buffer_addr.k_out_addr + IMAGE_SIZE;
	buffer_addr.phy_out_addr = virt_to_phys(buffer_addr.k_out_addr);
	buffer_addr.phy_in_addr = virt_to_phys(buffer_addr.k_in_addr);

	printk_xiaofei("xiaofei: %s:%d "
			"in_addr:%p(%p) "
			"out_addr %p(%p)\n", 
			__func__, __LINE__, 
			buffer_addr.k_in_addr, buffer_addr.phy_in_addr, 
			buffer_addr.k_out_addr, buffer_addr.phy_out_addr);

kmalloc_failed:
	return ret;
}

static void release_image_buffer(void) {
	kfree(buffer_addr.k_out_addr);
	memset(&buffer_addr, 0, sizeof(xotsu2_buffer_addr_t));
}

#ifdef KERNEL_DIGILENT
static void uio_release(struct device *dev) {
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
		.release = uio_release,
	},
	.resource = resource,
	.num_resources = ARRAY_SIZE(resource),
};

static int create_uio_device(void) {
	int ret;

	ret = create_image_buffer();
	if(ret != 0) {
		ret = -ENOMEM;
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto create_image_buffer_failed;
	}
	my_uio_device.resource[0].start = buffer_addr.k_out_addr;
	my_uio_device.resource[0].end = buffer_addr.k_out_addr + BUFFER_SIZE - 1;
	printk_xiaofei("xiaofei: %s:%d addr:%x end:%x\n", 
			__func__, __LINE__, 
			my_uio_device.resource[0].start, 
			my_uio_device.resource[0].end);

	ret = platform_device_register(&my_uio_device);
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto uio_register_failed;
	}

	return 0;

uio_register_failed:
	release_image_buffer();
create_image_buffer_failed:
	return -1;
}

static int release_uio_device(void) {
	int ret = 0;

	platform_device_unregister(&my_uio_device);
	release_image_buffer();
	return ret;
}
#endif

static irqreturn_t intr_handler(int irq, void *data)
{
	struct xotsu2_device *xdev = (struct xotsu2_device *)data;

	printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
	iowrite32(1, xdev->regs + XOTSU2_CONTROL_BUS_ADDR_ISR);
	tasklet_schedule(&xdev->tasklet);
	return IRQ_HANDLED;
}


static void do_tasklet(unsigned long data)
{
	//struct xotsu2_device *xdev = (struct xotsu2_device *)data;
	
	complete(&cmp);
}

static int xotsu2_open(struct inode *ip, struct file *filp)
{
	return 0;
}

static int xotsu2_release(struct inode *ip, struct file *filp)
{
	return 0;
}

static int wait_for_xotsu2_intr(xotsu2_result_t *result) {
	int ret = 0;
	unsigned long tmo = msecs_to_jiffies(1000);

	result->is_timeout = false;
	tmo = wait_for_completion_timeout(&cmp, tmo);
	if(tmo == 0) {
		result->is_timeout = true;
	}
	result->value = ioread32(xdev->regs + XOTSU2_CONTROL_BUS_ADDR_AP_RETURN);

	return ret;
}

static long xotsu2_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case XOTSU_GET_DMA_MEM_ADDR:
			{
				if (copy_to_user((u32 *)arg, &buffer_addr, 
							sizeof(xotsu2_buffer_addr_t))) {
					return -EFAULT;
				}
			}
			break;
		case XOTSU_WAIT_FOR_RESULT:
			{
				xotsu2_result_t result;
				wait_for_xotsu2_intr(&result);
				if (copy_to_user((u32 *)arg, &result, 
							sizeof(xotsu2_result_t))) {
					return -EFAULT;
				}
			}
			break;
		default:
			break;
	}
	return 0;
}

static const struct file_operations xotsu2_fops = {
	.owner = THIS_MODULE,
	.open = xotsu2_open,
	.unlocked_ioctl = xotsu2_ioctl,
	.release = xotsu2_release,
};

static struct miscdevice xotsu2_miscdev = {
	.name = DRIVER_NAME,
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &xotsu2_fops,
};

static int create_xotsu2_cdev(void) {
	int ret;

	ret = misc_register(&xotsu2_miscdev);
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
	}

	return ret;
}

static int release_xotsu2_cdev(void) {
	int ret = 0;
	ret = misc_deregister(&xotsu2_miscdev);
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
	}
	return ret;
}

//static int __devinit xotsu2_of_probe(struct platform_device *pdev)
static int xotsu2_of_probe(struct platform_device *pdev)
{
	struct device_node *node;
	int ret;

	xdev = devm_kzalloc(&pdev->dev, sizeof(*xdev), GFP_KERNEL);
	if (!xdev) {
		dev_err(&pdev->dev, "Not enough memory for device\n");
		ret = -ENOMEM;
		goto xotsu2_alloc_failed;
	}

	xdev->dev = &pdev->dev;

	node = pdev->dev.of_node;

	/* iomap registers */
	xdev->regs = of_iomap(node, 0);
	if (!xdev->regs) {
		dev_err(&pdev->dev, "unable to iomap registers\n");
		ret = -ENXIO;
		goto iomap_reg_failed;
	}

	/* find the IRQ line, if it exists in the device tree */
	xdev->irq = irq_of_parse_and_map(node, 0);
	ret = request_irq(xdev->irq, intr_handler, IRQF_SHARED,
			DRIVER_NAME, xdev);
	if (ret) {
		dev_err(xdev->dev, "unable to request IRQ %d\n", ret);
		goto request_irq_failed;
	}
	printk_xiaofei("xiaofei:%s irq is %x(%d)\n", dev_name(xdev->dev), xdev->irq, xdev->irq);


	init_completion(&cmp);
	tasklet_init(&xdev->tasklet, do_tasklet, (unsigned long)xdev);

#ifdef KERNEL_DIGILENT
	ret = create_uio_device();
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto create_uio_device_failed;
	}
#else
	ret = create_image_buffer();
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto create_image_buffer_failed;
	}
#endif

	ret = create_xotsu2_cdev();
	if(ret != 0) {
		printk_xiaofei("xiaofei:%s:%d\n", __func__, __LINE__);
		goto create_xotsu2_cdev_failed;
	}

	platform_set_drvdata(pdev, xdev);

	return ret;

create_xotsu2_cdev_failed:
#ifdef KERNEL_DIGILENT
	release_uio_device();
create_uio_device_failed:
#else
	release_image_buffer();
create_image_buffer_failed:
#endif
	free_irq(xdev->irq, xdev);
request_irq_failed:
	irq_dispose_mapping(xdev->irq);
iomap_reg_failed:
	devm_kfree(&pdev->dev, xdev);
xotsu2_alloc_failed:
	return ret;
}

//static int __devexit xotsu2_remove(struct platform_device *pdev)
static int xotsu2_remove(struct platform_device *pdev)
{
	struct xotsu2_device *xdev = platform_get_drvdata(pdev);

	release_xotsu2_cdev();
#ifdef KERNEL_DIGILENT
	release_uio_device();
#else
	release_image_buffer();
#endif
	free_irq(xdev->irq, xdev);
	irq_dispose_mapping(xdev->irq);
	iounmap(xdev->regs);
	devm_kfree(&pdev->dev, xdev);

	return 0;
}

static const struct of_device_id xotsu2_of_ids[] = {
	{ .compatible = "xlnx,otsu2-1.0" },
	{}
};

static struct platform_driver xotsu2_of_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = xotsu2_of_ids,
	},
	.probe = xotsu2_of_probe,
	//.remove = __devexit_p(xotsu2_remove),
	.remove = xotsu2_remove,
};

static int __init xotsu2_init(void)
{
	return platform_driver_register(&xotsu2_of_driver);
}

static void __exit xotsu2_exit(void)
{
	platform_driver_unregister(&xotsu2_of_driver);
}

module_init(xotsu2_init);
module_exit(xotsu2_exit);

MODULE_DESCRIPTION("xotsu2 driver");
MODULE_LICENSE("GPL");

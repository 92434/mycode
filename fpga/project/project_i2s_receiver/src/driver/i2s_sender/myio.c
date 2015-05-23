/*
 * myio.c
 *
 * myio Driver
 *
 * Author: xiaofei.
 *
 */
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/cdev.h>
#include <linux/slab.h>
//#include <linux/fcntl.h>
//#include <linux/sysctl.h>
#include <linux/fs.h>
#include <linux/of_address.h>
//#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>

#define DRIVER_NAME "myio-dev"
#define PWM_DURATION (0 * 4)
#define PWM_DUTY (1 * 4)
#define CYCLE (3000000)

struct myio_device {
	void __iomem *regs;
	struct device *dev;
};

//static int __devinit myio_of_probe(struct platform_device *pdev)
static int myio_of_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device_node *pnode;
	static struct myio_device *pdevice;
	//struct resource *res;

	pdevice = devm_kzalloc(&pdev->dev, sizeof(*pdevice), GFP_KERNEL);
	if (!pdevice) {
		dev_err(&pdev->dev, "Not enough memory for device\n");
		ret = -ENOMEM;
		goto myio_alloc_failed;
	}

	pdevice->dev = &pdev->dev;

	pnode = pdev->dev.of_node;

	/* iomap registers */
	pdevice->regs = of_iomap(pnode, 0);
	//res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	//pdevice->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(pdevice->regs)) {
		dev_err(&pdev->dev, "unable to iomap registers\n");
		ret = -ENXIO;
		goto iomap_reg_failed;
	}

	writel(100000000 / CYCLE, pdevice->regs + PWM_DURATION);
	writel(((100000000 / CYCLE) / 2) | 0x80000000, pdevice->regs + PWM_DUTY);

	platform_set_drvdata(pdev, pdevice);

	return ret;

iomap_reg_failed:
	devm_kfree(&pdev->dev, pdevice);
myio_alloc_failed:
	return ret;
}

//static int __devexit myio_remove(struct platform_device *pdev)
static int myio_remove(struct platform_device *pdev)
{
	struct myio_device *pdevice = platform_get_drvdata(pdev);

	writel(((100000000 / CYCLE) / 2) & 0x7fffffff, pdevice->regs + PWM_DUTY);
	iounmap(pdevice->regs);
	devm_kfree(&pdev->dev, pdevice);

	return 0;
}

static const struct of_device_id myio_of_ids[] = {
	{ .compatible = "xlnx,myip-io-1.0" },
	{}
};

static struct platform_driver myio_of_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = myio_of_ids,
	},
	.probe = myio_of_probe,
	//.remove = __devexit_p(myio_remove),
	.remove = myio_remove,
};

static int __init myio_init(void)
{
	return platform_driver_register(&myio_of_driver);
}

static void __exit myio_exit(void)
{
	platform_driver_unregister(&myio_of_driver);
}

module_init(myio_init);
module_exit(myio_exit);

MODULE_DESCRIPTION("myio driver");
MODULE_LICENSE("GPL");

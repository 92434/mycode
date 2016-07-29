/**
 *  This is a simple demon of uio driver.
 *  Last modified by 
 09-05-2011   Joseph Yang(Yang Honggang)<ganggexiongqi@gmail.com>
 *
 * Compile:  
 *   Save this file name it simple.c
 *   # echo "obj-m := simple.o" > Makefile
 *   # make -Wall -C /lib/modules/`uname -r`/build M=`pwd` modules
 * Load the module:
 *   #modprobe uio
 *   #insmod simple.ko
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/slab.h> /* kmalloc, kfree */
#define MY_NAME "my_uio"

struct uio_info myuio_info = {
	.name = MY_NAME,
	.version = "0.1",
	.irq = UIO_IRQ_NONE,
};

static int my_uio_dev_probe(struct device *dev);
static int my_uio_dev_remove(struct device *dev);
static struct device_driver my_uio_driver = {
	.name = MY_NAME,
	.bus = &platform_bus_type,
	.probe = my_uio_dev_probe,
	.remove = my_uio_dev_remove,
};

static int my_uio_dev_probe(struct device *dev)
{
	printk("my_uio_dev_probe( %p)\n", dev);
	myuio_info.mem[0].addr = (unsigned long)kmalloc(1024, GFP_KERNEL);

	if(myuio_info.mem[0].addr == 0)
		return -ENOMEM;
	myuio_info.mem[0].memtype = UIO_MEM_LOGICAL;
	myuio_info.mem[0].size = 1024;

	if(uio_register_device(dev, &myuio_info))
		return -ENODEV;
	return 0;
}

static int my_uio_dev_remove(struct device *dev)
{
	uio_unregister_device(&myuio_info);

	return 0;
}

static struct platform_device * my_uio_device;

static int __init my_mod_init(void)
{
	my_uio_device = platform_device_register_simple(MY_NAME, -1, NULL, 0);

	return driver_register(&my_uio_driver);
}

static void __exit my_mod_exit(void)
{
	platform_device_unregister(my_uio_device);
	driver_unregister(&my_uio_driver);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("my UIO driver");

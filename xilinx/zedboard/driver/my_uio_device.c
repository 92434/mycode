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
#define MY_NAME "xiaofei_test_uio"

struct uio_info myuio_info = {
	.name = MY_NAME,
	.version = "0.1",
	.irq = UIO_IRQ_NONE,
};

static struct platform_device my_uio_device = {
	.name = "uio_pdrv",
	.id = -1,
	.dev = {
		.platform_data		= &myuio_info,
	},
};

static int __init my_mod_init(void)
{
	myuio_info.mem[0].addr = (unsigned long)kmalloc(1024, GFP_KERNEL);
	if(myuio_info.mem[0].addr == 0) {
		return -ENOMEM;
	}
	myuio_info.mem[0].memtype = UIO_MEM_LOGICAL;
	myuio_info.mem[0].size = 1024;

	return platform_device_register(&my_uio_device);
}

static void __exit my_mod_exit(void)
{
	platform_device_unregister(&my_uio_device);
	kfree(myuio_info.mem[0].addr);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("my uio driver");

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
#include <linux/slab.h> /* kmalloc, kfree */
#include <linux/spi/spi.h>
#include <linux/spi/spi_gpio.h>
#define MY_NAME "wm8804-spi-gpio"

#define MISO 902
#define MOSI 903
#define SCK 904
#define SPI_BUS_NUM 1

static struct spi_device *spidevice = NULL;

static struct spi_gpio_platform_data spi_gpio_cfg = {
	.sck		= SCK,
	.mosi		= MOSI,
	.miso		= MISO,
	.num_chipselect = 1,
};

static struct platform_device wm8804_device = {
	.name		= "spi_gpio",
	.id		= SPI_BUS_NUM,
	.dev.platform_data = &spi_gpio_cfg,
};

static int __init my_mod_init(void)
{
	int ret = 0;
	printk("%s\n", __PRETTY_FUNCTION__);
	ret = platform_device_register(&wm8804_device);

	return ret;
}

static void __exit my_mod_exit(void) {
	printk("%s\n", __PRETTY_FUNCTION__);
	platform_device_unregister(&wm8804_device);
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("wm8804 spi-gpio platform driver");

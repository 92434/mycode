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

//164 : s0//FMC_LPC_LA22_P
//158 : s1//FMC_LPC_LA19_P
//160 : s2//FMC_LPC_LA19_N

#define MISO 165//FMC_LPC_LA20_N
#define MOSI 172//FMC_LPC_LA23_N
#define SCK 173//FMC_LPC_LA23_P
#define SPI_BUS_NUM 1

static struct spi_gpio_platform_data spi_gpio_cfg = {
	.sck		= SCK,
	.mosi		= MOSI,
	.miso		= MISO,
	.num_chipselect = 1,
};

static void kc705_spi_master_release(struct device *dev) {
	struct spi_gpio_platform_data *pdata = dev->platform_data;
	pdata = pdata;
}

static struct platform_device kc705_spi_master_device = {
	.name		= "spi_gpio",
	.id		= SPI_BUS_NUM,
	.dev.platform_data = &spi_gpio_cfg,
	.dev.release = kc705_spi_master_release,
};

static int __init kc705_spi_master_init(void)
{
	int ret = 0;
	printk("%s\n", __PRETTY_FUNCTION__);
	ret = platform_device_register(&kc705_spi_master_device);

	return ret;
}

static void __exit kc705_spi_master_exit(void) {
	printk("%s\n", __PRETTY_FUNCTION__);
	platform_device_unregister(&kc705_spi_master_device);
}

module_init(kc705_spi_master_init);
module_exit(kc705_spi_master_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("kc705 spi-gpio master");

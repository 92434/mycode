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
#include "interface_conf.h"

#define SPI_BUS_NUM 1
#define SPI_BUS_NUM_1 2
#define SPI_BUS_NUM_2 3

static struct spi_gpio_platform_data spi_gpio_cfg = {
	.sck		= SPI_SCK,
	.mosi		= SPI_MOSI,
	.miso		= SPI_MISO,
	.num_chipselect = 1,
};
#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
static struct spi_gpio_platform_data spi_gpio_cfg_1 = {
	.sck		= SPI_SCK_1,
	.mosi		= SPI_MOSI_1,
	.miso		= SPI_MISO_1,
	.num_chipselect = 1,
};
static struct spi_gpio_platform_data spi_gpio_cfg_2 = {
	.sck		= SPI_SCK_2,
	.mosi		= SPI_MOSI_2,
	.miso		= SPI_MISO_2,
	.num_chipselect = 1,
};
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)

static void kc705_spi_master_release(struct device *dev)
{
	struct spi_gpio_platform_data *pdata = dev->platform_data;
	pdata = pdata;
}

static struct platform_device kc705_spi_master_device = {
	.name		= "spi_gpio",
	.id		= SPI_BUS_NUM,
	.dev.platform_data = &spi_gpio_cfg,
	.dev.release = kc705_spi_master_release,
};
#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
static struct platform_device kc705_spi_master_device_1 = {
	.name		= "spi_gpio_1",
	.id		= SPI_BUS_NUM_1,
	.dev.platform_data = &spi_gpio_cfg_1,
	.dev.release = kc705_spi_master_release,
};
static struct platform_device kc705_spi_master_device_2 = {
	.name		= "spi_gpio_2",
	.id		= SPI_BUS_NUM_2,
	.dev.platform_data = &spi_gpio_cfg_2,
	.dev.release = kc705_spi_master_release,
};
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)

static struct platform_device *pdev = &kc705_spi_master_device;
#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
static struct platform_device *pdev_1 = &kc705_spi_master_device_1;
static struct platform_device *pdev_2 = &kc705_spi_master_device_2;
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)

static int __init kc705_spi_master_init(void)
{
	int ret = 0;

	printk("%s\n", __PRETTY_FUNCTION__);

	ret = platform_device_register(pdev);

	if(ret != 0) {
		pdev = NULL;
		goto kc705_spi_master_init;
	}

#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
	ret = platform_device_register(pdev_1);

	if(ret != 0) {
		pdev_1 = NULL;
		goto kc705_spi_master_init;
	}

	ret = platform_device_register(pdev_2);

	if(ret != 0) {
		pdev_2 = NULL;
		goto kc705_spi_master_init;
	}
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)

kc705_spi_master_init:
	return ret;
}

static void __exit kc705_spi_master_exit(void)
{
	printk("%s\n", __PRETTY_FUNCTION__);

	if(pdev != NULL) {
		platform_device_unregister(pdev);
	}

#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
	if(pdev_1 != NULL) {
		platform_device_unregister(pdev_1);
	}
	if(pdev_2 != NULL) {
		platform_device_unregister(pdev_2);
	}
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)
}

module_init(kc705_spi_master_init);
module_exit(kc705_spi_master_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("kc705 spi-gpio master");

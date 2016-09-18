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
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>

#include "interface_conf.h"

#define MY_NAME "kc705_i2c"

#define I2C_BUS_NUM (-1)

static struct i2c_gpio_platform_data i2c_gpio_data = {
	.sda_pin		= IIC_SDA0,
	.scl_pin		= IIC_SCL0,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 10,
	.scl_is_output_only	= 1,
};

#if defined(KC705_TSP)
#if (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
static struct i2c_gpio_platform_data i2c_gpio_data_1 = {
	.sda_pin		= IIC_SDA1,
	.scl_pin		= IIC_SCL1,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 10,
	.scl_is_output_only	= 1,
};
static struct i2c_gpio_platform_data i2c_gpio_data_2 = {
	.sda_pin		= IIC_SDA2,
	.scl_pin		= IIC_SCL2,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 10,
	.scl_is_output_only	= 1,
};
static struct i2c_gpio_platform_data i2c_gpio_data_3 = {
	.sda_pin		= IIC_SDA3,
	.scl_pin		= IIC_SCL3,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 10,
	.scl_is_output_only	= 1,
};
#endif//if (DEFINED_TSP == 2)
#endif//#if defined(KC705_TSP)

void gpio_free(unsigned gpio);
static void release_gpio(struct device *dev)
{
	struct i2c_gpio_platform_data *data = dev->platform_data;
	gpio_free(data->sda_pin);
	gpio_free(data->scl_pin);
}

static struct platform_device i2c_gpio_device = {
	.name		= "i2c-gpio",
	.id		= I2C_BUS_NUM,
	.dev		= {
		.platform_data	= &i2c_gpio_data,
		.release = release_gpio,
	},
};
#if defined(KC705_TSP)
#if (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
static struct platform_device i2c_gpio_device_1 = {
	.name		= "i2c-gpio",
	.id		= I2C_BUS_NUM,
	.dev		= {
		.platform_data	= &i2c_gpio_data_1,
		.release = release_gpio,
	},
};
static struct platform_device i2c_gpio_device_2 = {
	.name		= "i2c-gpio",
	.id		= I2C_BUS_NUM,
	.dev		= {
		.platform_data	= &i2c_gpio_data_2,
		.release = release_gpio,
	},
};
static struct platform_device i2c_gpio_device_3 = {
	.name		= "i2c-gpio",
	.id		= I2C_BUS_NUM,
	.dev		= {
		.platform_data	= &i2c_gpio_data_3,
		.release = release_gpio,
	},
};
#endif//if (DEFINED_TSP == 2)
#endif//#if defined(KC705_TSP)

static struct platform_device *pdev = &i2c_gpio_device;
#if defined(KC705_TSP)
#if (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
static struct platform_device *pdev_1 = &i2c_gpio_device_1;
static struct platform_device *pdev_2 = &i2c_gpio_device_2;
static struct platform_device *pdev_3 = &i2c_gpio_device_3;
#endif//if (DEFINED_TSP == 2)
#endif//#if defined(KC705_TSP)

static int __init kc705_i2c_master_init(void)
{
	int ret = 0;
	printk("%s\n", __PRETTY_FUNCTION__);
	ret = platform_device_register(pdev);

	if(ret != 0) {
		pdev_1 = NULL;
		goto kc705_i2c_master_init_failed;
	}

#if defined(KC705_TSP)
#if (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
	ret = platform_device_register(pdev_1);

	if(ret != 0) {
		pdev_1 = NULL;
		goto kc705_i2c_master_init_failed;
	}

	ret = platform_device_register(pdev_2);

	if(ret != 0) {
		pdev_2 = NULL;
		goto kc705_i2c_master_init_failed;
	}

	ret = platform_device_register(pdev_3);

	if(ret != 0) {
		pdev_3 = NULL;
		goto kc705_i2c_master_init_failed;
	}

#endif//if (DEFINED_TSP == 2)
#endif//#if defined(KC705_TSP)

kc705_i2c_master_init_failed:
	return ret;
}

static void __exit kc705_i2c_master_exit(void)
{
	printk("%s\n", __PRETTY_FUNCTION__);

	if(pdev != NULL) {
		platform_device_unregister(pdev);
	}

#if defined(KC705_TSP)
#if (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")

	if(pdev_1 != NULL) {
		platform_device_unregister(pdev_1);
	}

	if(pdev_2 != NULL) {
		platform_device_unregister(pdev_2);
	}

	if(pdev_3 != NULL) {
		platform_device_unregister(pdev_3);
	}

#endif//if (DEFINED_TSP == 2)
#endif//#if defined(KC705_TSP)
}

module_init(kc705_i2c_master_init);
module_exit(kc705_i2c_master_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("kc705 i2c-gpio platform driver");

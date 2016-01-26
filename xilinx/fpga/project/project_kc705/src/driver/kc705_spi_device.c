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
#define MY_NAME "spidev_device"
#define CS 164//FMC_LPC_LA22_P

#define SPI_BUS_NUM 1

static struct spi_device *spidevice = NULL;
static struct spi_master *spi_master = NULL;

static struct spi_board_info kc705_spi_info = {
		.modalias = "spidev",
		.max_speed_hz = 4000000,	/* max spi clock (SCK) speed in HZ */
		.bus_num = SPI_BUS_NUM,
		.chip_select = 0,
		.controller_data = (void *)CS,
		.mode = SPI_MODE_0,
};

static int add_spi_device(struct spi_board_info *info) {
	int ret = 0;
	struct spi_device *spi_device;

	spi_master = spi_busnum_to_master(info->bus_num);
	if (!spi_master) {
		printk("spi_busnum_to_master(%d) returned NULL\n", info->bus_num);
		ret = -ENOSYS;
		goto spi_busnum_to_master_failed;
	}

	spi_device = spi_alloc_device(spi_master);
	if (!spi_device) {
		//put_device(&spi_master->dev);
		printk("spi_alloc_device() failed\n");
		ret = -ENOMEM;
		goto spi_alloc_device_failed;
	}

	strncpy(spi_device->modalias, info->modalias, sizeof(spi_device->modalias) - 1);
	spi_device->chip_select = info->chip_select;
	spi_device->max_speed_hz = info->max_speed_hz;
	spi_device->mode = info->mode;
	spi_device->bits_per_word = 8;
	spi_device->controller_data = info->controller_data;

	ret = spi_add_device(spi_device);
	if (ret < 0) {
		//spi_dev_put(spi_device);
		printk("spi_add_device() failed %d\n", ret);
		goto spi_add_device_failed;
	}
	spidevice = spi_device;

	return ret;

	//device_del(spi_device->dev);
spi_add_device_failed:
	spi_dev_put(spi_device);
spi_alloc_device_failed:
	spi_master_put(spi_master);
spi_busnum_to_master_failed:
	return ret;
}

static void remove_spi_device(void) {
	if(spidevice != NULL) {
		spi_unregister_device(spidevice);
	}
	if(spi_master != NULL) {
		spi_master_put(spi_master);
	}
}

static int __init kc705_spi_device_init(void)
{
	int ret = 0;
	printk("%s\n", __PRETTY_FUNCTION__);
	ret = add_spi_device(&kc705_spi_info);

	return ret;
}

static void __exit kc705_spi_device_exit(void) {
	printk("%s\n", __PRETTY_FUNCTION__);
	remove_spi_device();
}

module_init(kc705_spi_device_init);
module_exit(kc705_spi_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("spidev_device driver");

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

#define MY_NAME "spidev_device"

#define SPI_BUS_NUM 1
static struct spi_board_info kc705_spi_info = {
	.modalias = "spidev",
	.max_speed_hz = 4000000,	/* max spi clock (SCK) speed in HZ */
	.bus_num = SPI_BUS_NUM,
	.chip_select = 0,
	.controller_data = (void *)SPI_CS,
	.mode = SPI_MODE_0,
};

#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
#define SPI_BUS_NUM_1 2
#define SPI_BUS_NUM_2 3

static struct spi_board_info kc705_spi_info_1 = {
	.modalias = "spidev",
	.max_speed_hz = 4000000,	/* max spi clock (SCK) speed in HZ */
	.bus_num = SPI_BUS_NUM_1,
	.chip_select = 0,
	.controller_data = (void *)SPI_CS_1,
	.mode = SPI_MODE_0,
};
static struct spi_board_info kc705_spi_info_2 = {
	.modalias = "spidev",
	.max_speed_hz = 4000000,	/* max spi clock (SCK) speed in HZ */
	.bus_num = SPI_BUS_NUM_2,
	.chip_select = 0,
	.controller_data = (void *)SPI_CS_2,
	.mode = SPI_MODE_0,
};
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)

static struct spi_board_info *kc705_spi_infos[] = {
	&kc705_spi_info,
#if defined(KC705_I2S_RECEIVER)
#if (DEFINED_I2S == 1)
#warning("DEFINED_I2S == 1")
	&kc705_spi_info_1,
	&kc705_spi_info_2,
#endif//if (DEFINED_I2S == 1)
#endif//#if defined(KC705_I2S_RECEIVER)
};

#define DEV_NUM (sizeof(kc705_spi_infos) / sizeof(struct spi_board_info *))

static struct spi_master *spi_masters[DEV_NUM] = {NULL};
static struct spi_device *spi_devices[DEV_NUM] = {NULL};

static int add_spi_device(int index)
{
	int ret = 0;
	struct spi_master *spi_master;
	struct spi_device *spi_device;

	spi_master = spi_busnum_to_master(kc705_spi_infos[index]->bus_num);

	if (!spi_master) {
		printk("spi_busnum_to_master(%d) returned NULL\n", kc705_spi_infos[index]->bus_num);
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

	strncpy(spi_device->modalias, kc705_spi_infos[index]->modalias, sizeof(spi_device->modalias) - 1);
	spi_device->chip_select = kc705_spi_infos[index]->chip_select;
	spi_device->max_speed_hz = kc705_spi_infos[index]->max_speed_hz;
	spi_device->mode = kc705_spi_infos[index]->mode;
	spi_device->bits_per_word = 8;
	spi_device->controller_data = kc705_spi_infos[index]->controller_data;

	ret = spi_add_device(spi_device);

	if (ret < 0) {
		//spi_dev_put(spi_device);
		printk("spi_add_device() failed %d\n", ret);
		goto spi_add_device_failed;
	}

	spi_masters[index] = spi_master;
	spi_devices[index] = spi_device;

	return ret;

	//device_del(spi_device->dev);
spi_add_device_failed:
	spi_dev_put(spi_device);
spi_alloc_device_failed:
	spi_master_put(spi_master);
spi_busnum_to_master_failed:
	return ret;
}

static void remove_spi_device(int index)
{
	if(spi_devices[index] != NULL) {
		spi_unregister_device(spi_devices[index]);
	}

	if(spi_masters[index] != NULL) {
		spi_master_put(spi_masters[index]);
	}
}

static int __init kc705_spi_device_init(void)
{
	int ret = 0;
	int index;
	printk("%s\n", __PRETTY_FUNCTION__);

	for(index = 0; index < DEV_NUM; index++) {
		ret = add_spi_device(index);
	}

	return ret;
}

static void __exit kc705_spi_device_exit(void)
{
	int index;
	printk("%s\n", __PRETTY_FUNCTION__);

	for(index = 0; index < DEV_NUM; index++) {
		remove_spi_device(index);
	}
}

module_init(kc705_spi_device_init);
module_exit(kc705_spi_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiaofei");
MODULE_DESCRIPTION("spidev_device driver");

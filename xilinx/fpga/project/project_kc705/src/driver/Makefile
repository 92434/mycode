#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

#KBUILD_CFLAGS += -Dtest_timer
KBUILD_CFLAGS += -Dtest_buffer_list

#kc705-dma-objs := kc705_pcie_xdma.o
#KERNEL-OBJS += kc705-dma.o
#
#kc705-cdma-objs := kc705_pcie_xcdma.o
#KERNEL-OBJS += kc705-cdma.o
#

kc705-module-objs := kc705.o pcie.o dma_common.o xdma_device.o utils/xiaofei_kthread.o utils/xiaofei_timer.o utils/xiaofei_list_buffer.o axi_dma.o axi_cdma.o pseudo_dma.o gpio.o gpio_proc.o
KERNEL-OBJS += kc705-module.o

spi-gpio-all-objs := spi-gpio.o spi-bitbang.o
KERNEL-OBJS += i2c-gpio.o i2c-dev.o spi-gpio-all.o spidev.o

include $(BUILD_KO)


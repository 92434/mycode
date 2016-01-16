#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

#KBUILD_CFLAGS += -DKC705_DVBS2
KBUILD_CFLAGS += -DKC705_CSA

kc705-module-objs := kc705.o \
	pcie.o \
	gpio.o \
	gpio_proc.o \
	pcie_gpio.o \
	pcie_device.o \
	pcie_dma.o \
	pcie_performance.o \
	pcie_tr_thread.o \
	pcie_dma_thread.o \
	pcie_dma_device.o \
	axi_cdma.o \
	axi_dma.o \
	pseudo_dma.o \
	pcie_utils.o \
	utils/xiaofei_kthread.o \
	utils/xiaofei_timer.o \
	utils/xiaofei_list_buffer.o
KERNEL-OBJS += kc705-module.o

spi-gpio-all-objs := spi-gpio.o spi-bitbang.o
KERNEL-OBJS += i2c-gpio.o i2c-dev.o spi-gpio-all.o spidev.o kc705_i2c_master.o kc705_spi_master.o kc705_spi_device.o

include $(BUILD_KO)


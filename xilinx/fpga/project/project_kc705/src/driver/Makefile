#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

#KBUILD_CFLAGS += -Dtest_timer
kc705-dma-objs := kc705_pcie_xdma.o
KERNEL-OBJS += kc705-dma.o

kc705-cdma-objs := kc705_pcie_xcdma.o
KERNEL-OBJS += kc705-cdma.o

include $(BUILD_KO)


install:
#	sudo insmod kc705-cdma.ko my_pci_device_id=0x7028
	sudo insmod kc705-cdma.ko
	dmesg
uninstall:
	sudo rmmod kc705-cdma
	sudo dmesg -c

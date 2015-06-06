#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

kc705-objs := kc705_pcie_xcdma.o
KERNEL-OBJS += kc705.o

include $(BUILD_KO)


install:
	sudo insmod kc705.ko
uninstall:
	sudo rmmod kc705

#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

KERNEL-OBJS += pmodoled-gpio.o
KERNEL-OBJS += timer_test.o

include $(BUILD_KO)

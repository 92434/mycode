export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
export ARCH := arm
export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o
local_dirs := i2s_receiver/
local_dirs += i2s_sender/
local_dirs += spidev/
local_dirs += wm8804_spi/
KERNEL-OBJS := $(local_dirs)

include $(BUILD_KO)

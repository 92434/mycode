export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
export ARCH := arm
export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#local_modules += modules-name.o
local_modules := my_uio_device.o

local_dirs := i2s/i2s_receiver/
local_dirs += i2s/i2s_sender/
local_dirs += i2s/spidev/
local_dirs += i2s/wm8804_spi/
local_dirs += filter/
local_dirs += glyphy/
local_dirs += xotsu2/
local_dirs += zynq-xdma/dev/
local_dirs += zynq-xvdma/

KERNEL-OBJS := $(local_objs)
KERNEL-OBJS += $(local_dirs)

include $(BUILD_KO)

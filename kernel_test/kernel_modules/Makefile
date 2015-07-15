#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

KERNEL-OBJS += pmodoled-gpio.o
KERNEL-OBJS += mod_info.o

include $(BUILD_KO)

insert:
	sudo insmod mod_info.ko int_var=0x123 str_var=module_need_to_reset_ref_count int_array=1,2,3,4,5,6
	dmesg
remove:
	sudo rmmod mod_info
	sudo dmesg -c

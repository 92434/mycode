#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

#KBUILD_CFLAGS += -Dtest_timer
KBUILD_CFLAGS += -Dtest_buffer_list

KERNEL-OBJS += pmodoled-gpio.o
KERNEL-OBJS += mod_info.o
KERNEL-OBJS += cdev.o
test-module-objs := test_utils.o utils/xiaofei_timer.o utils/xiaofei_list_buffer.o
KERNEL-OBJS += test-module.o

include $(BUILD_KO)

insert:
	#sudo insmod mod_info.ko int_var=0x123 str_var=module_need_to_reset_ref_count int_array=1,2,3,4,5,6
#	sudo insmod cdev.ko
	sudo insmod test-module.ko
	dmesg
remove:
	#sudo rmmod mod_info
#	sudo rmmod cdev
	sudo rmmod test-module
	sudo dmesg -c

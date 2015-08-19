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
test-module-objs := test_utils.o utils/xiaofei_timer.o utils/xiaofei_list_buffer.o
KERNEL-OBJS += test-module.o

kc705-module-objs := kc705.o pcie.o dma_common.o pcie_device.o utils/xiaofei_kthread.o utils/xiaofei_timer.o utils/xiaofei_list_buffer.o axi_dma.o axi_cdma.o
KERNEL-OBJS += kc705-module.o

include $(BUILD_KO)

app : kc705_device.c
	gcc -o app kc705_device.c

all : app

clean-app :
	-rm app

clean : clean-app

#install-dma:
##	sudo insmod kc705-dma.ko my_pci_device_id=0x7028
#	sudo insmod kc705-dma.ko
#	dmesg
#
#uninstall-dma:
#	sudo rmmod kc705-dma
#	sudo dmesg -c
#
#install-cdma:
##	sudo insmod kc705-dma.ko
#	sudo insmod kc705-cdma.ko
#	dmesg
#
#uninstall-cdma:
#	sudo rmmod kc705-cdma
#	sudo dmesg -c
#
#
install-test:
	sudo insmod test-module.ko
	dmesg

uninstall-test:
	sudo rmmod test-module
	sudo dmesg -c

install: kc705-module.ko
	sudo insmod kc705-module.ko
	dmesg
	sudo ./app /dev/pciedma_0 &
	sudo ./app /dev/pciedma_1 &

uninstall:
	-sudo killall -9 app
	while test -n "$(ps -elf | grep app 2>/dev/null)"; do sleep 1;done
	sudo rmmod kc705-module
	sudo dmesg -c

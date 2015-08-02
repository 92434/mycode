#export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
#export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#export MOD_SRC := $(shell pwd)

#modules-name-objs := obj1.o obj2.o obj3.o
#KERNEL-OBJS += modules-name.o

#KBUILD_CFLAGS += -Dtest_timer
#kc705-dma-objs := kc705_pcie_xdma.o
#KERNEL-OBJS += kc705-dma.o
#
#kc705-cdma-objs := kc705_pcie_xcdma.o
#KERNEL-OBJS += kc705-cdma.o

kc705-modules-objs := kc705.o pcie.o dma_common.o utils/xiaofei_kthread.o utils/xiaofei_timer.o axi_cdma.o
KERNEL-OBJS += kc705-modules.o

include $(BUILD_KO)


#install-cdma:
##	sudo insmod kc705-dma.ko
#	sudo insmod kc705-cdma.ko
#	dmesg
#
#uninstall-cdma:
#	sudo rmmod kc705-cdma
#	sudo dmesg -c
#
#install-dma:
##	sudo insmod kc705-dma.ko my_pci_device_id=0x7028
#	sudo insmod kc705-dma.ko
#	dmesg
#
#uninstall-dma:
#	sudo rmmod kc705-dma
#	sudo dmesg -c

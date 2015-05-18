export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
export ARCH := arm
#export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-digilent/output
export KERNEL_DIR := /home/xiaofei/workspace/src/xilinx/src/linux-xlnx/output
#dirs := filter/
#dirs += zynq-xdma/dev/
#dirs += zynq-xvdma/
#dirs += xotsu2/
#dirs += glyphy/
dirs += i2s_axidma/
dirs += spi/
ifneq ($(KERNELRELEASE),)
obj-m := $(dirs)
#obj-m += my_uio_device.o
else
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(shell pwd) modules

#clean:
#	@for dir in $(dirs) ; do \
#		if test -d $$dir ; then \
#			echo "$$dir: $(MAKE)" ; \
#			if ($(MAKE) -C $$dir clean) ; then \
#				true; \
#			else \
#				exit 1; \
#			fi; \
#		fi; \
#	done
#	-rm *.o *.ko *.mod.c modules.order Module.symvers
#	-ls -a |grep "^\.[[:alpha:]]"|xargs rm -rf

clean:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(shell pwd) clean
endif

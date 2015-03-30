ifneq ($(KERNELRELEASE),)
obj-m := $(KERNEL-OBJS)
else
ifeq ($(KERNEL_DIR),)
export KERNEL_DIR := /lib/modules/$(shell uname -r)/build
endif
ifeq ($(MOD_SRC),)
export MOD_SRC := $(shell pwd)
endif
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(MOD_SRC) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(MOD_SRC) clean
endif

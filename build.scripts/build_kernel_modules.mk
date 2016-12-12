include $(utils)

ifneq ($(KERNELRELEASE),)
obj-m := $(KERNEL-OBJS)
else
ifeq ($(KERNEL_DIR),)
export KERNEL_DIR := /lib/modules/$(shell uname -r)/build
endif
ifeq ($(MOD_SRC),)
export MOD_SRC := $(shell pwd)
endif

target: sub_dirs_make
	$(silent)$(MAKE) -C $(KERNEL_DIR) M=$(MOD_SRC) modules
	$(call target-echo-why)

sub_dirs_make:
	$(silent)$(call sub-dirs-make,$(SUB_DIRS))
	$(call target-echo-why)

clean: sub_dirs_clean
	$(silent)$(MAKE) -C $(KERNEL_DIR) M=$(MOD_SRC) clean
	$(call target-echo-why)

sub_dirs_clean: 
	$(silent)$(call sub-dirs-make,$(SUB_DIRS),clean)
	$(call target-echo-why)
endif

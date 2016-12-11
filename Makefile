include $(default_rules)

#all_dirs := $(strip $(call all-dirs-under))

SUB_DIRS := c mytest kernel_test make_test rom_parse linuxstart-20120111 test_utils sqlite lzma DES java

ifneq ($(shell which arm-xilinx-linux-gnueabi-gcc),)
SUB_DIRS += xilinx
endif

ifneq ($(shell which mips-linux-gcc),)
SUB_DIRS += mboot
endif

include $(ADD_TARGET)

local_targets:
	$(info "compile ok!")

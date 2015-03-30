include $(default_rules)

#all_dirs := $(strip $(call all-dirs-under))

SUB_DIRS := utils mytest kernel_test make_test rom_parse linuxstart-20120111 test_utils sqlite lzma

ifneq ($(shell which arm-xilinx-linux-gnueabi-gcc),)
SUB_DIRS += xilinx
endif

ifneq ($(shell which mips-linux-gcc),)
SUB_DIRS += mboot
endif

include $(ADD_TARGET)

local_targets:
	$(info "compile ok!")

add_dep_tags:
	mv dep_files .dep_files
	cat .dep_files | sort | uniq | sed 's/^\(.*\)$$/\"\1\"/g' >> cscope/cscope.files
	cat .dep_files | sort | uniq >> cscope/ctags.files
	tags.sh cscope
	tags.sh tags


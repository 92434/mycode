include $(default_rules)

local_lib_src := $(top)/c/utils

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := test_cpp_formatter.cpp
test_cpp_formatter.cpp.LOCAL_CFLAGS := -I$(local_lib_src)

ifneq ("$(origin XIAOFEI)", "command line")
XIAOFEI := xiaofei
endif

ifneq ("$(XIAOFEI)", "")
XIAOFEI := xiaofei1
endif

c_files += test_macro.cpp
test_macro.cpp.LOCAL_CFLAGS := -I$(local_lib_src) -DXIAOFEI=\"$(XIAOFEI)\"
c_files += virtual.cpp
include $(BUILD_BINS)

include $(ADD_TARGET)

include $(default_rules)

local_lib_src := $(top)/c/utils

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := test_cpp_formater.cpp
test_cpp_formater.cpp.LOCAL_CFLAGS := -I$(local_lib_src)
include $(BUILD_BINS)

include $(ADD_TARGET)

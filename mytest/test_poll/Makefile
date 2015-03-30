include $(default_rules)

local_lib_src := $(top)/utils

CFLAGS += -I$(local_lib_src)

LDFLAGS += -static
LDFLAGS += -L$(local_lib_src)/$(out_dir)/lib
LDFLAGS += -lxiaofei

LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := poll_test.cpp
include $(BUILD_BINS)

include $(ADD_TARGET)

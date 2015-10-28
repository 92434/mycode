include $(default_rules)

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := poll_test.cpp
local_lib_src := $(top)/c/utils
poll_test.cpp.LOCAL_CFLAGS := -I$(local_lib_src)
poll_test.cpp.LOCAL_CFLAGS += -g
poll_test.cpp.LOCAL_LDFLAGS := -L$(local_lib_src)/$(out_dir)/lib
poll_test.cpp.LOCAL_LDFLAGS += -lxiaofei
poll_test.cpp.LOCAL_DEPS := $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(BUILD_BINS)

include $(ADD_TARGET)

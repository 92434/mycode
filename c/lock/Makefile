include $(default_rules)

local_lib_src := $(top)/c/utils

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := test_file_lock.cpp
test_file_lock.cpp.LOCAL_CFLAGS := -I$(local_lib_src)
test_file_lock.cpp.LOCAL_LDFLAGS := -L$(local_lib_src)/$(out_dir)/lib
test_file_lock.cpp.LOCAL_LDFLAGS += -lxiaofei
LOCAL_PRECONDITION = $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(BUILD_BINS)

include $(ADD_TARGET)

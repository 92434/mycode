include $(default_rules)

LDFLAGS := -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files :=
c_files += thread_cleanup.cpp
c_files += pthreads_sched_test.cpp
c_files += thread_test.cpp
local_lib_src := $(top)/c/utils
thread_test.cpp.LOCAL_CFLAGS := -I$(local_lib_src)
thread_test.cpp.LOCAL_LDFLAGS := -L$(local_lib_src)/$(out_dir)/lib
thread_test.cpp.LOCAL_LDFLAGS += -lxiaofei
include $(BUILD_BINS)

app_c_files :=
include $(BUILD_APP)

include $(ADD_TARGET)

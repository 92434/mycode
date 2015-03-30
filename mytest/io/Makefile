include $(default_rules)

local_lib_src := $(top)/utils
CFLAGS += -I$(local_lib_src)
CFLAGS += -DLOOPER_USES_EPOLL

LDFLAGS += -L$(local_lib_src)/$(out_dir)/lib
LDFLAGS += -static
LDFLAGS += -lxiaofei
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files :=
c_files += opendir.cpp
c_files += test_string_op.cpp
c_files += data_process.cpp
include $(BUILD_BINS)

app_c_files := io_epoll_test.cpp
app_c_files += poll_thread.cpp
app_c_files += io_thread.cpp
app_name := poll_test
include $(BUILD_APP)

$(target_files) : $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(ADD_TARGET)

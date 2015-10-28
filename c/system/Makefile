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
c_files += test_settimer.c
c_files += test_getopt.cpp
c_files += test_optparse.cpp
c_files += timer.c
include $(BUILD_BINS)

$(target_files) : $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(ADD_TARGET)

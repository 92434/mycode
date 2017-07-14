include $(default_rules)

CC := gcc

CFLAGS += -g -lstdc++
CFLAGS += -I$(shell pwd)

LDFLAGS += -lpthread

c_files := ft_lib.c

staticd_lib_c_files := $(c_files)
staticd_lib_name := ft_lib
LOCAL_LDFLAGS += -static
include $(BUILD_LIB)

shared_lib_c_files := $(c_files)
shared_lib_name := ft_lib
LOCAL_LDFLAGS += -fPIC
include $(BUILD_SO)

include $(ADD_TARGET)

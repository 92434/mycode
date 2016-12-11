include $(default_rules)

cscope_top := $(shell pwd)

CFLAGS += -g
LDFLAGS += -lpthread
LDFLAGS += -static

c_files := lock.cpp
c_files += thread.cpp
c_files += file_cache.cpp
c_files += hex_char_search.cpp
c_files += data_process.cpp
c_files += aio_rw.cpp
c_files += optparse.cpp
c_files += poll.cpp
c_files += log.cpp

staticd_lib_c_files := $(c_files)
staticd_lib_name := xiaofei
include $(BUILD_LIB)

shared_lib_c_files := $(c_files)
shared_lib_name := xiaofei
include $(BUILD_SO)

SUB_DIRS := sub

include $(ADD_TARGET)

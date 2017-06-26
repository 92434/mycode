include $(default_rules)

CFLAGS += -g
#CFLAGS += -Ifs
#CFLAGS += -Iregexp

LDFLAGS += -lpthread

c_files := lock.cpp
c_files += thread.cpp
c_files += file_cache.cpp
c_files += hex_char_search.cpp
c_files += data_process.cpp
c_files += aio_rw.cpp
c_files += optparse.cpp
c_files += poll.cpp
c_files += log.cpp
c_files += filesystem/filesystem.cpp
c_files += regexp/regexp.cpp

staticd_lib_c_files := $(c_files)
staticd_lib_name := xiaofei
LOCAL_LDFLAGS += -static
include $(BUILD_LIB)

shared_lib_c_files := $(c_files)
shared_lib_name := xiaofei
LOCAL_LDFLAGS += -fPIC
include $(BUILD_SO)

include $(ADD_TARGET)

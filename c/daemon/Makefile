include $(default_rules)

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := daemon_clib.cpp
c_files += dup_clib.cpp
c_files += log_clib.cpp
include $(BUILD_BINS)

include $(ADD_TARGET)

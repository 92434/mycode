include $(default_rules)

LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files :=
c_files += test_settimer.c
c_files += test_getopt.cpp
c_files += test_optparse.cpp
test_optparse.cpp.LOCAL_DEPS := $(top)/c/utils/out/lib/libxiaofei.a
test_optparse.cpp.LOCAL_LIBS := $(top)/c/utils/out/lib/libxiaofei.a
test_optparse.cpp.LOCAL_CFLAGS := -I$(top)/c/utils
c_files += timer.c
c_files += time.c
include $(BUILD_BINS)

include $(ADD_TARGET)

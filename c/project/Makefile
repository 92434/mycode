include $(default_rules)

c_files := test_regexp/regexp.cpp
test_regexp/regexp.cpp.LOCAL_DEPS := $(top)/c/utils/out/lib/libxiaofei.a
test_regexp/regexp.cpp.LOCAL_LIBS := $(top)/c/utils/out/lib/libxiaofei.a
test_regexp/regexp.cpp.LOCAL_CFLAGS := -I$(top)/c/utils
#test_regexp/regexp.cpp.LOCAL_LDFLAGS := -lxiaofei

c_files += test_scan_files/scanfiles.cpp
test_scan_files/scanfiles.cpp.LOCAL_DEPS := $(top)/c/utils/out/lib/libxiaofei.a
test_scan_files/scanfiles.cpp.LOCAL_LIBS := $(top)/c/utils/out/lib/libxiaofei.a
test_scan_files/scanfiles.cpp.LOCAL_CFLAGS := -I$(top)/c/utils
#test_scan_files/scanfiles.cpp.LOCAL_LDFLAGS := -lxiaofei
include $(BUILD_BINS)

include $(ADD_TARGET)


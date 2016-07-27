include $(default_rules)

CFLAGS += -g
LDFLAGS += -lpthread
LDFLAGS += -static

c_files := test.cpp

staticd_lib_c_files := $(c_files)
staticd_lib_name := test
include $(BUILD_LIB)

shared_lib_c_files := $(c_files)
shared_lib_name := test
include $(BUILD_SO)

include $(ADD_TARGET)

test:
	python test.py

include $(default_rules)

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files := GraphMatrix.cpp
local_lib_src :=
poll_test.cpp.LOCAL_CFLAGS :=
poll_test.cpp.LOCAL_LDFLAGS :=
poll_test.cpp.LOCAL_DEPS :=

include $(BUILD_BINS)

include $(ADD_TARGET)

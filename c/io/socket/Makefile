include $(default_rules)

CFLAGS += -g

#LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

c_files :=
c_files += socket_info.cpp
c_files += msg_server.cpp
c_files += msg_client.cpp
include $(BUILD_BINS)

include $(ADD_TARGET)

include $(default_rules)

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

CC := gcc

c_files :=
c_files += example1/udp_client.c
c_files += example1/udp_server.c
c_files += example2/nat_node.c

include $(BUILD_BINS)

include $(ADD_TARGET)

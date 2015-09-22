include $(default_rules)

CC = gcc

c_files := xdma_device.c
xdma_device.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver
include $(BUILD_BINS)

include $(ADD_TARGET)

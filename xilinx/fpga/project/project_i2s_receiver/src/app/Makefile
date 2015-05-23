include $(default_rules)

TOOLCHAIN_PREFIX := arm-xilinx-linux-gnueabi-
CC = gcc

c_files := i2s/my_i2s_receiver.c
c_files += i2s/spidev.c
i2s/my_i2s_receiver.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver/i2s_receiver
include $(BUILD_BINS)

include $(ADD_TARGET)

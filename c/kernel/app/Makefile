include $(default_rules)
CC := gcc
sysroot := /home/xiaofei/workspace/src/6818/x6818_linux_160719/buildroot/output/host

CFLAGS_INCLUDE += -I$(sysroot)/usr/include
CFLAGS += $(CFLAGS_INCLUDE)
LIBS_DIR += -L$(sysroot)/usr/lib
LIBS := 
LDFLAGS += $(LIBS_DIR) $(LIBS)
TOOLCHAIN_PREFIX := /home/xiaofei/workspace/src/6818/x6818_linux_160719/buildroot/output/host/opt/ext-toolchain/bin/arm-cortex_a9-linux-gnueabi-


c_files := rtctest.c
c_files += spi-test.c
c_files += spi-flash-otp.c
c_files += test_select.c
c_files += serial_test.c
c_files += serial_test.c
c_files += serial_test.c
c_files += getevent.c
c_files += cdev_test.c
include $(BUILD_BINS)

app_c_files := lsusb.c
app_c_files += usbhost/usbhost.c
app_name := lsusb
include $(BUILD_APP)

include $(ADD_TARGET)

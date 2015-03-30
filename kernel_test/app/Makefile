include $(default_rules)
CC := gcc

c_files := rtctest.c
c_files += spi-test.c
c_files += spi-flash-otp.c
c_files += test_select.c
c_files += serial_test.c
c_files += serial_test.c
c_files += serial_test.c
c_files += getevent.c
include $(BUILD_BINS)

app_c_files := lsusb.c
app_c_files += usbhost/usbhost.c
app_name := lsusb
include $(BUILD_APP)

include $(ADD_TARGET)

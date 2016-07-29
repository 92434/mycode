include $(default_rules)

TOOLCHAIN_PREFIX := arm-xilinx-linux-gnueabi-
CC = gcc

#CFLAGS += -U__linux__

LDFLAGS += -lpthread
LDFLAGS += -static

c_files := uio_test.c
c_files += glyphy/glyphy_test.c
glyphy/glyphy_test.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver/glyphy
include $(BUILD_BINS)

c_files := i2s/spidev.c
c_files += i2s/my_i2s_receiver.c
i2s/my_i2s_receiver.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver/i2s/i2s_receiver
include $(BUILD_BINS)

app_c_files := gpio_utils.c
app_c_files += my_sobel.c
app_c_files += vdma/VDMA.c
app_c_files += filter_driver/ximage_filter_linux.c
app_c_files += filter_driver/ximage_filter.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver/zynq-xvdma
app_name := sobel
include $(BUILD_APP)

app_c_files := gpio_utils.c
app_c_files += my-otsu2.c
app_c_files += vdma/VDMA.c
app_c_files += xotsu2/xotsu2.c
app_c_files += xotsu2/xotsu2_linux.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver/xotsu2
app_name := otsu2
include $(BUILD_APP)

app_c_files := image-filter.c
app_c_files += image-filter/ximage_filter.c
app_c_files += image-filter/ximage_filter_linux.c
LOCAL_CFLAGS := -I$(shell pwd)/image-filter
app_name := imagefilter
include $(BUILD_APP)

app_c_files := vdma/main.c
app_c_files += vdma/VDMA.c
app_name := mmio_vdma
include $(BUILD_APP)

SUB_DIRS := opencv_xilinx_sample

include $(ADD_TARGET)

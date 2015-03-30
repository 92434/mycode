include $(default_rules)

TOOLCHAIN_PREFIX := arm-xilinx-linux-gnueabi-

#CFLAGS += -I$(shell pwd)/filter_driver
#CFLAGS += -I$(shell pwd)/vdma
#CFLAGS += -I$(shell pwd)/../xilinx_driver/zynq-xvdma
#CFLAGS += -I$(shell pwd)/../xilinx_driver/xotsu2
#CFLAGS += -I$(shell pwd)/image-filter
CFLAGS += -I$(shell pwd)/../xilinx_driver/glyphy
#CFLAGS += -U__linux__

LDFLAGS += -lpthread
LDFLAGS += -static

#c_files := uio_test.c
c_files := glyphy/glyphy_test.c
include $(BUILD_BINS)

#app_c_files := gpio_utils.c
#app_c_files += my_sobel.c
#app_c_files += vdma/VDMA.c
#app_c_files += filter_driver/ximage_filter_linux.c
#app_c_files += filter_driver/ximage_filter.c
#app_name := sobel
#include $(BUILD_APP)

#app_c_files := gpio_utils.c
#app_c_files += my-otsu2.c
#app_c_files += vdma/VDMA.c
#app_c_files += xotsu2/xotsu2.c
#app_c_files += xotsu2/xotsu2_linux.c
#app_name := otsu2
#include $(BUILD_APP)

#app_c_files := image-filter.c
#app_c_files += image-filter/ximage_filter.c
#app_c_files += image-filter/ximage_filter_linux.c
#app_name := imagefilter
#include $(BUILD_APP)

#app_c_files := vdma/main.c
#app_c_files += vdma/VDMA.c
#app_name := mmio_vdma
#include $(BUILD_APP)

include $(ADD_TARGET)

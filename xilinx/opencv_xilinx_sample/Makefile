include $(default_rules)

xilinx_sysroot := /home/xiaofei/workspace/src/xilinx/src/release/rootfs

CFLAGS += -I$(xilinx_sysroot)/include/opencv
LDFLAGS += -L$(xilinx_sysroot)/lib
FFMPEGLIBS := -lavcodec -lavformat -lswscale -lavcore -lavutil -lxvidcore -lx264
GTHREAD_LIBS := -lgmodule-2.0 -lgobject-2.0 -lglib-2.0 -lgthread-2.0 -pthread  -lrt
GTK_LIBS := -latk-1.0 -lpango-1.0 -lcairo -lpangoft2-1.0 -lpangocairo-1.0 -lgtk-directfb-2.0 -lgdk-directfb-2.0 -lgdk_pixbuf-2.0 -lm -lpixman-1 -lfontconfig -lxml2 -lfreetype -ldirectfb -lfusion -ldirect -lpng12 -lgio-2.0 -lresolv -lexpat
IMAGELIBS := -lpng12  -ljpeg -lz
OPENCVLIBS += -lcv -lcxcore -lhighgui -lml -lcvaux
LIBS := -lrt -lz -lpthread -ldl -lm
LDFLAGS += $(LIBS)
LDFLAGS += $(FFMPEGLIBS)
LDFLAGS += $(GTHREAD_LIBS)
LDFLAGS += $(GTK_LIBS)
LDFLAGS += $(IMAGELIBS)
LDFLAGS += $(OPENCVLIBS)
TOOLCHAIN_PREFIX := arm-xilinx-linux-gnueabi-

c_files := approxpoly_contours.cpp
c_files += filter.cpp
c_files += histogram.cpp
c_files += mixgaussians.cpp
c_files += rotate.cpp
c_files += threshold_contours.cpp
c_files += xiaofei_test.cpp
c_files += capture.cpp
c_files += drawing.c
c_files += measure/tool/grabPicture.cpp
include $(BUILD_BINS)

app_c_files := $(patsubst ./%,%,$(shell echo measure/*.cpp 2>/dev/null))
app_name := MDistance
include $(BUILD_APP)

include $(ADD_TARGET)

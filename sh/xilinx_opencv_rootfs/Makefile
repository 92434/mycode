export top := $(shell pwd)
#export default_rules := $(shell pwd)/Makefile.in
#include $(default_rules)
export CROSS_COMPILE := arm-xilinx-linux-gnueabi-
export ARCH := arm
export TOOLCHAIN_CLIB := /home/xiaofei/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/arm-xilinx-linux-gnueabi/libc
export rootfs_path := $(top)/release/rootfs_
export ramdisk_path := $(top)/release/ramdisk_

export PATH := $(PATH):$(rootfs_path)

PHONY := target
target : uboot kernel release

define build-uboot
#	make -C $(1) O=output $(2)
	make -C $(1) O=output -j8
endef

PHONY += u-boot-xarm
u-boot-xarm :
	$(call build-uboot,$@,zynq_zed_config)

PHONY += u-boot-xlnx
u-boot-xlnx :
	$(call build-uboot,$@,zynq_zed_config)

PHONY += uboot
uboot :
	$(info start compile $@ ...)
#	make u-boot-xarm
#	make u-boot-xlnx

define build-kernel
#	make -C $(1) O=output mrproper
#	make -C $(1) O=output $(2)
#	make -C $(1) O=output clean
#	make -C $(1) O=output menuconfig
#	make -C $(1) O=output V=2 -j8
#	make -C $(1) O=output UIMAGE_LOADADDR=0x8000 uImage -j8
#	$(1)/output/scripts/dtc/dtc -I dts -O dtb $(1)/arch/arm/boot/dts/$(3) > $(1)/output/devicetree.dtb
endef

PHONY += linux-digilent
linux-digilent :
	$(call build-kernel,$@,digilent_zed_defconfig,digilent-zed.dts)

PHONY += linux-xlnx
linux-xlnx :
	$(call build-kernel,$@,xilinx_zynq_defconfig,zynq-zed.dts)

PHONY += Digilent-linux-3.3-digilent-7f8e908
Digilent-linux-3.3-digilent-7f8e908 :
	$(call build-kernel,$@,digilent_zed_defconfig,digilent-zed.dts)

PHONY += kernel
kernel :
	$(info start compile $@ ...)
	make linux-digilent
#	make Digilent-linux-3.3-digilent-7f8e908
#	make linux-xlnx

define build-busybox
	make -C $(1) O=output $(2);
	make -C $(1) O=output -j8;
	make -C $(1) O=output install;
endef

PHONY += busybox
busybox :
	$(info start compile $@ ...)
#	$(call build-busybox,$@,defconfig)
	rsync -aR $@/output/_install/./ $(ramdisk_path);
	


define config-dropbear
	cd $(1) && ./configure --prefix=$(2) --host=arm-xilinx-linux-gnueabi --disable-zlib CC=arm-xilinx-linux-gnueabi-gcc LDFLAGS="-Wl,--gc-sections" CFLAGS="-ffunction-sections -fdata-sections -Os"
endef

PHONY += dropbear-0.53.1
dropbear-0.53.1 :
#	$(call config-dropbear,$@,$(ramdisk_path))
#	make -C $@ PROGRAMS="dropbear dbclient dropbearkey dropbearconvert scp" MULTI=1 strip
	sudo make -C $@ install
	sudo chown -R xiaofei:xiaofei $(ramdisk_path)
	if ! test -L $(ramdisk_path)/usr/bin/scp;then ln -s ../../sbin/dropbear $(ramdisk_path)/usr/bin/scp;fi;

PHONY += dropbear
dropbear :
	$(info start compile $@ ...)
	make dropbear-0.53.1

define config-x264
	cd $(1); \
	./configure --cross-prefix=arm-xilinx-linux-gnueabi- --enable-shared --host=arm-linux --disable-asm --prefix=$(2); 
endef

PHONY += x264
x264 :
	$(info start compile $@ ...)
#	$(call config-x264,$@,$(rootfs_path))
#	make -C $@ -j8
	make -C $@ install

define config-xvidcore
	cd $(1); \
	./configure CC=arm-xilinx-linux-gnueabi-gcc --prefix=$(2) --host=arm-linux --disable-assembly;
endef
PHONY += xvidcore
xvidcore :
	$(info start compile $@ ...)
#	$(call config-xvidcore,$@/build/generic,$(rootfs_path))
#	make -C $@/build/generic -j8
	make -C $@/build/generic install

define config-ffmpeg
	cd $(1); \
	./configure --prefix=$(2) --enable-shared --disable-static --disable-asm --enable-gpl --enable-cross-compile --arch=arm --disable-stripping --target-os=linux --enable-libx264 --enable-libxvid --cc=arm-xilinx-linux-gnueabi-gcc --enable-swscale --xiaofei_lib=$(2)/lib --xiaofei_include=$(2)/include;
endef
PHONY += ffmpeg
ffmpeg : x264 xvidcore
	$(info start compile $@ ...)
#	$(call config-ffmpeg,$@,$(rootfs_path))
#	make -C $@ -j8;
	make -C $@ install
	

PHONY += gtk
gtk :
	$(info start compile $@ ...)
	cd $@; \
	./compile.sh
	rsync -aR $@/gtk+/arm/./ $(rootfs_path);

#---------------------------------------------------------------------------------------
#patch for opencv
##############Index: CMakeLists.txt
##############===================================================================
##############--- CMakeLists.txt	(revision 2281)
##############+++ CMakeLists.txt	(working copy)
##############@@ -240,7 +240,7 @@
############## # After it adds module to build and define
############## # constants passed as second arg
############## # ===================================================
##############-
##############+
 macro(CHECK_MODULE module_name define)
##############     set(${define} 0)
##############     if(PKG_CONFIG_FOUND)
##############Index: configure
##############===================================================================
##############--- configure	(revision 2281)
##############+++ configure	(working copy)
##############@@ -17222,7 +17222,14 @@
##############             GTK_LIBS=""
##############             ## If we have a custom action on failure, don't print errors, but
##############             ## do set a variable so people can do so.
##############+##xiaofei add
##############             GTK_PKG_ERRORS=`$PKG_CONFIG --errors-to-stdout --print-errors ""gtk+-2.0 gdk-pixbuf-2.0""`
##############+            GTK_CFLAGS="-I${prefix}/include/atk-1.0 -I${prefix}/include/pango-1.0 -I${prefix}/include/cairo -I${prefix}/include/gtk-2.0 -I${prefix}/lib/gtk-2.0/include -I${prefix}/include/gdk-pixbuf-2.0"
##############+            GTK_LIBS="-L${prefix}/lib -latk-1.0 -lpango-1.0 -lcairo -lgtk-directfb-2.0 -lgdk_pixbuf-2.0 -lm"
##############+            ## If we have a custom action on failure, don't print errors, but
##############+            ## do set a variable so people can do so.
##############+            GTK_PKG_ERRORS=`$PKG_CONFIG --errors-to-stdout --print-errors ""gtk+-2.0 gdk-pixbuf-2.0""`
##############+            succeeded=yes
############## 
##############         fi
############## 
##############@@ -17328,6 +17335,8 @@
##############             { $as_echo "$as_me:${as_lineno-$LINENO}: checking GTHREAD_CFLAGS" >&5
############## $as_echo_n "checking GTHREAD_CFLAGS... " >&6; }
##############             GTHREAD_CFLAGS=`$PKG_CONFIG --cflags ""gthread-2.0""`
##############+#xiaofei add
##############+            GTHREAD_CFLAGS="-I${prefix}/include -I${prefix}/include/glib-2.0 -I${prefix}/lib/glib-2.0/include"
##############             { $as_echo "$as_me:${as_lineno-$LINENO}: result: $GTHREAD_CFLAGS" >&5
############## $as_echo "$GTHREAD_CFLAGS" >&6; }
############## 
##############@@ -17334,6 +17343,8 @@
##############             { $as_echo "$as_me:${as_lineno-$LINENO}: checking GTHREAD_LIBS" >&5
############## $as_echo_n "checking GTHREAD_LIBS... " >&6; }
##############             GTHREAD_LIBS=`$PKG_CONFIG --libs ""gthread-2.0""`
##############+#xiaofei add
##############+            GTHREAD_LIBS="-L${prefix}/lib -lglib-2.0 -lglib-2.0 -lgthread-2.0 -pthread  -lrt"
##############             { $as_echo "$as_me:${as_lineno-$LINENO}: result: $GTHREAD_LIBS" >&5
############## $as_echo "$GTHREAD_LIBS" >&6; }
##############         else
##############Index: include/opencv/cxcore.hpp
##############===================================================================
##############--- include/opencv/cxcore.hpp	(revision 2281)
##############+++ include/opencv/cxcore.hpp	(working copy)
##############@@ -66,6 +66,7 @@
############## 
############## using std::vector;
############## using std::string;
##############+using std::ptrdiff_t;
##############     
############## template<typename _Tp> class CV_EXPORTS Size_;
############## template<typename _Tp> class CV_EXPORTS Point_;
##############Index: src/highgui/cvcap_v4l.cpp
##############===================================================================
##############--- src/highgui/cvcap_v4l.cpp	(revision 2281)
##############+++ src/highgui/cvcap_v4l.cpp	(working copy)
##############@@ -14,7 +14,9 @@
############## First Patch:  August 24, 2004 Travis Wood   TravisOCV@tkwood.com
############## For Release:  OpenCV-Linux Beta4  opencv-0.9.6
############## Tested On:    LMLBT44 with 8 video inputs
##############-Problems?     Post problems/fixes to OpenCV group on groups.yahoo.com
##############+Problems?     Post your questions at answers.opencv.org,
##############+              Report bugs at code.opencv.org,
##############+              Submit your fixes at https://github.com/Itseez/opencv/
############## Patched Comments:
############## 
############## TW: The cv cam utils that came with the initial release of OpenCV for LINUX Beta4
##############@@ -154,7 +156,12 @@
############## - USE_TEMP_BUFFER fixes the main problem (improper buffer management) and
##############   prevents bad images in the first place
############## 
##############+11th patch: April 2, 2013, Forrest Reiling forrest.reiling@gmail.com
##############+Added v4l2 support for getting capture property CV_CAP_PROP_POS_MSEC.
##############+Returns the millisecond timestamp of the last frame grabbed or 0 if no frames have been grabbed
##############+Used to successfully synchonize 2 Logitech C310 USB webcams to within 16 ms of one another
############## 
##############+
############## make & enjoy!
############## 
############## */
##############@@ -200,9 +207,9 @@
############## //
############## //M*/
############## 
##############-#include "_highgui.h"
##############+#include "precomp.cpp"
############## 
##############-#if !defined WIN32 && defined HAVE_CAMV4L
##############+#if !defined WIN32 && (defined HAVE_CAMV4L || defined HAVE_CAMV4L2 || defined HAVE_VIDEOIO)
############## 
############## #define CLEAR(x) memset (&(x), 0, sizeof (x))
############## 
##############@@ -214,19 +221,26 @@
############## #include <sys/types.h>
############## #include <sys/mman.h>
############## 
##############+#ifdef HAVE_CAMV4L
############## #include <linux/videodev.h>
##############+#endif
############## 
############## #include <string.h>
############## #include <stdlib.h>
##############-#include <asm/types.h>          /* for videodev2.h */
############## #include <assert.h>
############## #include <sys/stat.h>
############## #include <sys/ioctl.h>
############## 
############## #ifdef HAVE_CAMV4L2
##############+#include <asm/types.h>          /* for videodev2.h */
############## #include <linux/videodev2.h>
############## #endif
############## 
##############+#ifdef HAVE_VIDEOIO
##############+#include <sys/videoio.h>
##############+#define HAVE_CAMV4L2
##############+#endif
##############+
############## /* Defaults - If your board can do better, set it here.  Set for the most common type inputs. */
############## #define DEFAULT_V4L_WIDTH  640
############## #define DEFAULT_V4L_HEIGHT 480
##############@@ -271,15 +285,17 @@
############## 
############## #endif  /* HAVE_CAMV4L2 */
############## 
##############-int  PALETTE_BGR24 = 0,
##############-     PALETTE_YVU420 = 0,
##############-     PALETTE_YUV411P = 0,
##############-     PALETTE_YUYV = 0,
##############-     PALETTE_UYVY= 0,
##############-     PALETTE_SBGGR8 = 0,
##############-     PALETTE_SN9C10X = 0,
##############-     PALETTE_MJPEG = 0,
##############-     PALETTE_SGBRG = 0;
##############+enum PALETTE_TYPE {
##############+  PALETTE_BGR24 = 1,
##############+  PALETTE_YVU420,
##############+  PALETTE_YUV411P,
##############+  PALETTE_YUYV,
##############+  PALETTE_UYVY,
##############+  PALETTE_SBGGR8,
##############+  PALETTE_SN9C10X,
##############+  PALETTE_MJPEG,
##############+  PALETTE_SGBRG
##############+};
############## 
############## typedef struct CvCaptureCAM_V4L
############## {
##############@@ -286,16 +302,18 @@
##############     int deviceHandle;
##############     int bufferIndex;
##############     int FirstCapture;
##############+#ifdef HAVE_CAMV4L
##############     struct video_capability capability;
##############     struct video_window     captureWindow;
##############     struct video_picture    imageProperties;
##############     struct video_mbuf       memoryBuffer;
##############     struct video_mmap       *mmaps;
##############+#endif /* HAVE_CAMV4L */
##############     char *memoryMap;
##############     IplImage frame;
############## 
############## #ifdef HAVE_CAMV4L2
##############-
##############+   enum PALETTE_TYPE palette;
##############    /* V4L2 variables */
##############    buffer buffers[MAX_V4L_BUFFERS + 1];
##############    struct v4l2_capability cap;
##############@@ -307,8 +325,9 @@
##############    struct v4l2_control control;
##############    enum v4l2_buf_type type;
##############    struct v4l2_queryctrl queryctrl;
##############-   struct v4l2_querymenu querymenu;
############## 
##############+   struct timeval timestamp;
##############+
##############    /* V4L2 control variables */
##############    int v4l2_brightness, v4l2_brightness_min, v4l2_brightness_max;
##############    int v4l2_contrast, v4l2_contrast_min, v4l2_contrast_max;
##############@@ -343,24 +362,6 @@
############## static int numCameras = 0;
############## static int indexList = 0;
############## 
##############-#ifdef HAVE_CAMV4L2
##############-
##############-// IOCTL handling for V4L2
##############-static int xioctl( int fd, int request, void *arg)
##############-{
##############-
##############-  int r;
##############-
##############-
##############-  do r = ioctl (fd, request, arg);
##############-  while (-1 == r && EINTR == errno);
##############-
##############-  return r;
##############-
##############-}
##############-
##############-#endif /* HAVE_CAMV4L2 */
##############-
############## /* Simple test program: Find number of Video Sources available.
##############    Start from 0 and go to MAX_CAMERAS while checking for the device with that name.
##############    If it fails on the first attempt of /dev/video0, then check if /dev/video is valid.
##############@@ -391,6 +392,8 @@
############## 
############## }; /* End icvInitCapture_V4L */
############## 
##############+#ifdef HAVE_CAMV4L
##############+
############## static int
############## try_palette(int fd,
##############             struct video_picture *cam_pic,
##############@@ -408,6 +411,8 @@
##############   return 0;
############## }
############## 
##############+#endif /* HAVE_CAMV4L */
##############+
############## #ifdef HAVE_CAMV4L2
############## 
############## static int try_palette_v4l2(CvCaptureCAM_V4L* capture, unsigned long colorspace)
##############@@ -420,7 +425,7 @@
##############   capture->form.fmt.pix.width = DEFAULT_V4L_WIDTH;
##############   capture->form.fmt.pix.height = DEFAULT_V4L_HEIGHT;
############## 
##############-  if (-1 == xioctl (capture->deviceHandle, VIDIOC_S_FMT, &capture->form))
##############+  if (-1 == ioctl (capture->deviceHandle, VIDIOC_S_FMT, &capture->form))
##############       return -1;
############## 
############## 
##############@@ -432,6 +437,8 @@
############## 
############## #endif /* HAVE_CAMV4L2 */
############## 
##############+#ifdef HAVE_CAMV4L
##############+
############## static int try_init_v4l(CvCaptureCAM_V4L* capture, char *deviceName)
############## {
############## 
##############@@ -447,7 +454,6 @@
##############   /* No matter what the name - it still must be opened! */
##############   capture->deviceHandle = open(deviceName, O_RDWR);
############## 
##############-
##############   if (capture->deviceHandle == 0)
##############   {
##############     detect = -1;
##############@@ -461,7 +467,6 @@
##############     if (ioctl(capture->deviceHandle, VIDIOCGCAP, &capture->capability) < 0)
##############     {
##############       detect = 0;
##############-
##############       icvCloseCAM_V4L(capture);
##############     }
##############       else
##############@@ -474,54 +479,64 @@
############## 
############## }
############## 
##############+#endif /* HAVE_CAMV4L */
##############+
############## #ifdef HAVE_CAMV4L2
############## 
############## static int try_init_v4l2(CvCaptureCAM_V4L* capture, char *deviceName)
############## {
##############+  // Test device for V4L2 compability
##############+  // Return value:
##############+  // -1 then unable to open device
##############+  //  0 then detected nothing
##############+  //  1 then V4L2 device
############## 
##############-  // if detect = -1 then unable to open device
##############-  // if detect = 0 then detected nothing
##############-  // if detect = 1 then V4L2 device
##############-  int detect = 0;
##############+  int deviceIndex;
############## 
##############-
##############-  // Test device for V4L2 compability
##############-
##############   /* Open and test V4L2 device */
##############   capture->deviceHandle = open (deviceName, O_RDWR /* required */ | O_NONBLOCK, 0);
##############+  if (-1 == capture->deviceHandle)
##############+  {
##############+#ifndef NDEBUG
##############+    fprintf(stderr, "(DEBUG) try_init_v4l2 open \"%s\": %s\n", deviceName, strerror(errno));
##############+#endif
##############+    icvCloseCAM_V4L(capture);
##############+    return -1;
##############+  }
############## 
##############+  CLEAR (capture->cap);
##############+  if (-1 == ioctl (capture->deviceHandle, VIDIOC_QUERYCAP, &capture->cap))
##############+  {
##############+#ifndef NDEBUG
##############+    fprintf(stderr, "(DEBUG) try_init_v4l2 VIDIOC_QUERYCAP \"%s\": %s\n", deviceName, strerror(errno));
##############+#endif
##############+    icvCloseCAM_V4L(capture);
##############+    return 0;
##############+  }
############## 
##############-
##############-  if (capture->deviceHandle == 0)
##############+  /* Query channels number */
##############+  if (-1 == ioctl (capture->deviceHandle, VIDIOC_G_INPUT, &deviceIndex))
##############   {
##############-    detect = -1;
##############-
##############+#ifndef NDEBUG
##############+    fprintf(stderr, "(DEBUG) try_init_v4l2 VIDIOC_G_INPUT \"%s\": %s\n", deviceName, strerror(errno));
##############+#endif
##############     icvCloseCAM_V4L(capture);
##############+    return 0;
##############   }
############## 
##############-  if (detect == 0)
##############+  /* Query information about current input */
##############+  CLEAR (capture->inp);
##############+  capture->inp.index = deviceIndex;
##############+  if (-1 == ioctl (capture->deviceHandle, VIDIOC_ENUMINPUT, &capture->inp))
##############   {
##############-    CLEAR (capture->cap);
##############-    if (-1 == xioctl (capture->deviceHandle, VIDIOC_QUERYCAP, &capture->cap))
##############-    {
##############-      detect = 0;
##############-
##############-      icvCloseCAM_V4L(capture);
##############-    }
##############-      else
##############-    {
##############-      CLEAR (capture->capability);
##############-      capture->capability.type = capture->cap.capabilities;
##############-
##############-      /* Query channels number */
##############-      if (-1 != xioctl (capture->deviceHandle, VIDIOC_G_INPUT, &capture->capability.channels))
##############-      {
##############-        detect = 1;
##############-      }
##############-    }
##############+#ifndef NDEBUG
##############+    fprintf(stderr, "(DEBUG) try_init_v4l2 VIDIOC_ENUMINPUT \"%s\": %s\n", deviceName, strerror(errno));
##############+#endif
##############+    icvCloseCAM_V4L(capture);
##############+    return 0;
##############   }
############## 
##############-  return detect;
##############+  return 1;
############## 
############## }
############## 
##############@@ -529,58 +544,53 @@
############## {
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_BGR24) == 0)
##############   {
##############-    PALETTE_BGR24 = 1;
##############+    capture->palette = PALETTE_BGR24;
##############   }
##############   else
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_YVU420) == 0)
##############   {
##############-    PALETTE_YVU420 = 1;
##############+    capture->palette = PALETTE_YVU420;
##############   }
##############   else
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_YUV411P) == 0)
##############   {
##############-    PALETTE_YUV411P = 1;
##############+    capture->palette = PALETTE_YUV411P;
##############   }
##############   else
############## 
############## #ifdef HAVE_JPEG
##############-#ifdef __USE_GNU
##############-      /* support for MJPEG is only available with libjpeg and gcc,
##############-	 because it's use libjepg and fmemopen()
##############-      */
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_MJPEG) == 0 ||
##############       try_palette_v4l2(capture, V4L2_PIX_FMT_JPEG) == 0)
##############   {
##############-    PALETTE_MJPEG = 1;
##############+    capture->palette = PALETTE_MJPEG;
##############   }
##############   else
############## #endif
##############-#endif
############## 
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_YUYV) == 0)
##############   {
##############-    PALETTE_YUYV = 1;
##############+    capture->palette = PALETTE_YUYV;
##############   }
##############   else if (try_palette_v4l2(capture, V4L2_PIX_FMT_UYVY) == 0)
##############   {
##############-    PALETTE_UYVY = 1;
##############+    capture->palette = PALETTE_UYVY;
##############   }
##############   else
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_SN9C10X) == 0)
##############   {
##############-    PALETTE_SN9C10X = 1;
##############+    capture->palette = PALETTE_SN9C10X;
##############   } else
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_SBGGR8) == 0)
##############   {
##############-    PALETTE_SBGGR8 = 1;
##############+    capture->palette = PALETTE_SBGGR8;
##############   } else
##############   if (try_palette_v4l2(capture, V4L2_PIX_FMT_SGBRG) == 0)
##############   {
##############-    PALETTE_SGBRG = 1;
##############+    capture->palette = PALETTE_SGBRG;
##############   }
##############       else
##############   {
##############-	fprintf(stderr, "HIGHGUI ERROR: V4L2: Pixel format of incoming image is unsupported by OpenCV\n");
##############+    fprintf(stderr, "HIGHGUI ERROR: V4L2: Pixel format of incoming image is unsupported by OpenCV\n");
##############     icvCloseCAM_V4L(capture);
##############     return -1;
##############   }
##############@@ -591,6 +601,8 @@
############## 
############## #endif /* HAVE_CAMV4L2 */
############## 
##############+#ifdef HAVE_CAMV4L
##############+
############## static int autosetup_capture_mode_v4l(CvCaptureCAM_V4L* capture)
############## {
############## 
##############@@ -615,7 +627,7 @@
##############       //printf("negotiated palette YUV420P\n");
##############   }
##############   else {
##############-	fprintf(stderr, "HIGHGUI ERROR: V4L: Pixel format of incoming image is unsupported by OpenCV\n");
##############+    fprintf(stderr, "HIGHGUI ERROR: V4L: Pixel format of incoming image is unsupported by OpenCV\n");
##############     icvCloseCAM_V4L(capture);
##############     return -1;
##############   }
##############@@ -624,26 +636,10 @@
############## 
############## }
############## 
##############+#endif /* HAVE_CAMV4L */
##############+
############## #ifdef HAVE_CAMV4L2
############## 
##############-static void v4l2_scan_controls_enumerate_menu(CvCaptureCAM_V4L* capture)
##############-{
##############-//  printf (" Menu items:\n");
##############-  CLEAR (capture->querymenu);
##############-  capture->querymenu.id = capture->queryctrl.id;
##############-  for (capture->querymenu.index = capture->queryctrl.minimum;
##############-       (int)capture->querymenu.index <= capture->queryctrl.maximum;
##############-       capture->querymenu.index++)
##############-  {
##############-    if (0 == xioctl (capture->deviceHandle, VIDIOC_QUERYMENU,
##############-                     &capture->querymenu))
##############-    {
##############-//      printf (" %s\n", capture->querymenu.name);
##############-    } else {
##############-        perror ("VIDIOC_QUERYMENU");
##############-    }
##############-  }
##############-}
############## 
############## static void v4l2_scan_controls(CvCaptureCAM_V4L* capture)
############## {
##############@@ -659,7 +655,7 @@
##############     CLEAR (capture->queryctrl);
##############     capture->queryctrl.id = ctrl_id;
############## 
##############-    if (0 == xioctl (capture->deviceHandle, VIDIOC_QUERYCTRL,
##############+    if (0 == ioctl (capture->deviceHandle, VIDIOC_QUERYCTRL,
##############                      &capture->queryctrl))
##############     {
############## 
##############@@ -708,8 +704,6 @@
##############         capture->v4l2_exposure_max = capture->queryctrl.maximum;
##############       }
############## 
##############-      if (capture->queryctrl.type == V4L2_CTRL_TYPE_MENU)
##############-        v4l2_scan_controls_enumerate_menu(capture);
############## 
##############     } else {
############## 
##############@@ -729,7 +723,7 @@
##############     CLEAR (capture->queryctrl);
##############     capture->queryctrl.id = ctrl_id;
############## 
##############-    if (0 == xioctl (capture->deviceHandle, VIDIOC_QUERYCTRL,
##############+    if (0 == ioctl (capture->deviceHandle, VIDIOC_QUERYCTRL,
##############                      &capture->queryctrl))
##############     {
############## 
##############@@ -778,9 +772,6 @@
##############         capture->v4l2_exposure_max = capture->queryctrl.maximum;
##############       }
############## 
##############-      if (capture->queryctrl.type == V4L2_CTRL_TYPE_MENU)
##############-        v4l2_scan_controls_enumerate_menu(capture);
##############-
##############     } else {
############## 
##############       if (errno == EINVAL)
##############@@ -830,6 +821,9 @@
##############    capture->v4l2_gain_max = 0;
##############    capture->v4l2_exposure_max = 0;
############## 
##############+   capture->timestamp.tv_sec = 0;
##############+   capture->timestamp.tv_usec = 0;
##############+
##############    /* Scan V4L2 controls */
##############    v4l2_scan_controls(capture);
############## 
##############@@ -852,7 +846,7 @@
##############        capture->inp.index = CHANNEL_NUMBER;
##############        /* Set only channel number to CHANNEL_NUMBER */
##############        /* V4L2 have a status field from selected video mode */
##############-       if (-1 == xioctl (capture->deviceHandle, VIDIOC_ENUMINPUT, &capture->inp))
##############+       if (-1 == ioctl (capture->deviceHandle, VIDIOC_ENUMINPUT, &capture->inp))
##############        {
##############          fprintf (stderr, "HIGHGUI ERROR: V4L2: Aren't able to set channel number\n");
##############          icvCloseCAM_V4L (capture);
##############@@ -864,7 +858,7 @@
##############    CLEAR (capture->form);
##############    capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
############## 
##############-   if (-1 == xioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form)) {
##############+   if (-1 == ioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form)) {
##############        fprintf( stderr, "HIGHGUI ERROR: V4L2: Could not obtain specifics of capture window.\n\n");
##############        icvCloseCAM_V4L(capture);
##############        return -1;
##############@@ -902,7 +896,7 @@
##############    capture->req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############    capture->req.memory = V4L2_MEMORY_MMAP;
############## 
##############-   if (-1 == xioctl (capture->deviceHandle, VIDIOC_REQBUFS, &capture->req))
##############+   if (-1 == ioctl (capture->deviceHandle, VIDIOC_REQBUFS, &capture->req))
##############    {
##############        if (EINVAL == errno)
##############        {
##############@@ -926,9 +920,9 @@
##############            return -1;
##############        } else {
##############          buffer_number--;
##############-	 fprintf (stderr, "Insufficient buffer memory on %s -- decreaseing buffers\n", deviceName);
##############+     fprintf (stderr, "Insufficient buffer memory on %s -- decreaseing buffers\n", deviceName);
############## 
##############-	 goto try_again;
##############+     goto try_again;
##############        }
##############    }
############## 
##############@@ -942,7 +936,7 @@
##############        buf.memory = V4L2_MEMORY_MMAP;
##############        buf.index = n_buffers;
############## 
##############-       if (-1 == xioctl (capture->deviceHandle, VIDIOC_QUERYBUF, &buf)) {
##############+       if (-1 == ioctl (capture->deviceHandle, VIDIOC_QUERYBUF, &buf)) {
##############            perror ("VIDIOC_QUERYBUF");
############## 
##############            /* free capture, and returns an error code */
##############@@ -967,15 +961,15 @@
##############        }
############## 
##############        if (n_buffers == 0) {
##############-	 capture->buffers[MAX_V4L_BUFFERS].start = malloc( buf.length );
##############-	 capture->buffers[MAX_V4L_BUFFERS].length = buf.length;
##############+     capture->buffers[MAX_V4L_BUFFERS].start = malloc( buf.length );
##############+     capture->buffers[MAX_V4L_BUFFERS].length = buf.length;
##############        }
##############    }
############## 
##############    /* Set up Image data */
##############    cvInitImageHeader( &capture->frame,
##############-                      cvSize( capture->captureWindow.width,
##############-                              capture->captureWindow.height ),
##############+                      cvSize( capture->form.fmt.pix.width,
##############+                              capture->form.fmt.pix.height ),
##############                       IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4 );
##############    /* Allocate space for RGBA data */
##############    capture->frame.imageData = (char *)cvAlloc(capture->frame.imageSize);
##############@@ -985,6 +979,8 @@
############## 
############## #endif /* HAVE_CAMV4L2 */
############## 
##############+#ifdef HAVE_CAMV4L
##############+
############## static int _capture_V4L (CvCaptureCAM_V4L *capture, char *deviceName)
############## {
##############    int detect_v4l = 0;
##############@@ -1103,6 +1099,8 @@
##############    return 1;
############## }; /* End _capture_V4L */
############## 
##############+#endif /* HAVE_CAMV4L */
##############+
############## static CvCaptureCAM_V4L * icvCaptureFromCAM_V4L (int index)
############## {
##############    static int autoindex;
##############@@ -1152,10 +1150,12 @@
##############        icvCloseCAM_V4L(capture);
##############        V4L2_SUPPORT = 0;
############## #endif  /* HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############        if (_capture_V4L (capture, deviceName) == -1) {
##############            icvCloseCAM_V4L(capture);
##############            return NULL;
##############        }
##############+#endif  /* HAVE_CAMV4L */
############## #ifdef HAVE_CAMV4L2
##############    } else {
##############        V4L2_SUPPORT = 1;
##############@@ -1175,20 +1175,20 @@
##############     buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############     buf.memory = V4L2_MEMORY_MMAP;
############## 
##############-    if (-1 == xioctl (capture->deviceHandle, VIDIOC_DQBUF, &buf)) {
##############+    if (-1 == ioctl (capture->deviceHandle, VIDIOC_DQBUF, &buf)) {
##############         switch (errno) {
##############         case EAGAIN:
##############             return 0;
############## 
##############         case EIO:
##############-	    if (!(buf.flags & (V4L2_BUF_FLAG_QUEUED | V4L2_BUF_FLAG_DONE)))
##############-	    {
##############-	      if (xioctl(capture->deviceHandle, VIDIOC_QBUF, &buf) == -1)
##############-	      {
##############-	        return 0;
##############-	      }
##############-	    }
##############-	    return 0;
##############+        if (!(buf.flags & (V4L2_BUF_FLAG_QUEUED | V4L2_BUF_FLAG_DONE)))
##############+        {
##############+          if (ioctl(capture->deviceHandle, VIDIOC_QBUF, &buf) == -1)
##############+          {
##############+            return 0;
##############+          }
##############+        }
##############+        return 0;
############## 
##############         default:
##############             /* display the error and stop processing */
##############@@ -1200,15 +1200,18 @@
##############    assert(buf.index < capture->req.count);
############## 
##############    memcpy(capture->buffers[MAX_V4L_BUFFERS].start,
##############-	  capture->buffers[buf.index].start,
##############-	  capture->buffers[MAX_V4L_BUFFERS].length );
##############+      capture->buffers[buf.index].start,
##############+      capture->buffers[MAX_V4L_BUFFERS].length );
##############    capture->bufferIndex = MAX_V4L_BUFFERS;
##############    //printf("got data in buff %d, len=%d, flags=0x%X, seq=%d, used=%d)\n",
##############    //	  buf.index, buf.length, buf.flags, buf.sequence, buf.bytesused);
############## 
##############-   if (-1 == xioctl (capture->deviceHandle, VIDIOC_QBUF, &buf))
##############+   if (-1 == ioctl (capture->deviceHandle, VIDIOC_QBUF, &buf))
##############        perror ("VIDIOC_QBUF");
############## 
##############+   //set timestamp in capture struct to be timestamp of most recent frame
##############+   capture->timestamp = buf.timestamp;
##############+
##############    return 1;
############## }
############## 
##############@@ -1227,7 +1230,7 @@
##############             FD_SET (capture->deviceHandle, &fds);
############## 
##############             /* Timeout. */
##############-            tv.tv_sec = 2;
##############+            tv.tv_sec = 10;
##############             tv.tv_usec = 0;
############## 
##############             r = select (capture->deviceHandle+1, &fds, NULL, NULL, &tv);
##############@@ -1264,7 +1267,9 @@
############## 
############## #ifdef HAVE_CAMV4L2
############## 
##############+#ifdef HAVE_CAMV4L
##############       if (V4L2_SUPPORT == 1)
##############+#endif
##############       {
############## 
##############         for (capture->bufferIndex = 0;
##############@@ -1280,7 +1285,7 @@
##############           buf.memory      = V4L2_MEMORY_MMAP;
##############           buf.index       = (unsigned long)capture->bufferIndex;
############## 
##############-          if (-1 == xioctl (capture->deviceHandle, VIDIOC_QBUF, &buf)) {
##############+          if (-1 == ioctl (capture->deviceHandle, VIDIOC_QBUF, &buf)) {
##############               perror ("VIDIOC_QBUF");
##############               return 0;
##############           }
##############@@ -1288,14 +1293,18 @@
############## 
##############         /* enable the streaming */
##############         capture->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############-        if (-1 == xioctl (capture->deviceHandle, VIDIOC_STREAMON,
##############+        if (-1 == ioctl (capture->deviceHandle, VIDIOC_STREAMON,
##############                           &capture->type)) {
##############             /* error enabling the stream */
##############             perror ("VIDIOC_STREAMON");
##############             return 0;
##############         }
##############-      } else
##############+      }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+      else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############       {
############## 
##############         for (capture->bufferIndex = 0;
##############@@ -1314,6 +1323,7 @@
##############         }
############## 
##############       }
##############+#endif /* HAVE_CAMV4L */
############## 
############## #if defined(V4L_ABORT_BADJPEG) && defined(HAVE_CAMV4L2)
##############      if (V4L2_SUPPORT == 1)
##############@@ -1335,8 +1345,12 @@
############## 
##############      mainloop_v4l2(capture);
############## 
##############-   } else
##############+   }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+     else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############    {
############## 
##############      capture->mmaps[capture->bufferIndex].frame  = capture->bufferIndex;
##############@@ -1345,9 +1359,9 @@
##############      capture->mmaps[capture->bufferIndex].format = capture->imageProperties.palette;
############## 
##############      if (ioctl (capture->deviceHandle, VIDIOCMCAPTURE,
##############-		&capture->mmaps[capture->bufferIndex]) == -1) {
##############-	 /* capture is on the way, so just exit */
##############-	 return 1;
##############+        &capture->mmaps[capture->bufferIndex]) == -1) {
##############+     /* capture is on the way, so just exit */
##############+     return 1;
##############      }
############## 
##############      ++capture->bufferIndex;
##############@@ -1356,6 +1370,7 @@
##############      }
############## 
##############    }
##############+#endif /* HAVE_CAMV4L */
############## 
##############    return(1);
############## }
##############@@ -1391,7 +1406,7 @@
############## 
############## static inline void
############## move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v,
##############-           int rowPixels, unsigned char * rgb)
##############+               int rowPixels, unsigned char * rgb)
############## {
##############     const int rvScale = 91881;
##############     const int guScale = -22553;
##############@@ -1430,7 +1445,7 @@
############## 
############## static inline void
############## move_411_block(int yTL, int yTR, int yBL, int yBR, int u, int v,
##############-           int rowPixels, unsigned char * rgb)
##############+               int /*rowPixels*/, unsigned char * rgb)
############## {
##############     const int rvScale = 91881;
##############     const int guScale = -22553;
##############@@ -1522,6 +1537,7 @@
############## //
############## /* Converts from interlaced YUV420 to RGB24. */
############## /* [FD] untested... */
##############+#ifdef HAVE_CAMV4L
############## static void
############## yuv420_to_rgb24(int width, int height,
##############         unsigned char *pIn0, unsigned char *pOut0)
##############@@ -1566,6 +1582,7 @@
##############         pOut += width * bytes;
##############     }
############## }
##############+#endif //HAVE_CAMV4L
############## 
############## // Consider a YUV411P image of 8x2 pixels.
############## //
##############@@ -1617,6 +1634,8 @@
############## /* based on ccvt_yuyv_bgr32() from camstream */
############## #define SAT(c) \
##############         if (c & (~255)) { if (c < 0) c = 0; else c = 255; }
##############+
##############+#ifdef HAVE_CAMV4L2
############## static void
############## yuyv_to_rgb24 (int width, int height, unsigned char *src, unsigned char *dst)
############## {
##############@@ -1645,9 +1664,9 @@
##############          SAT(g);
##############          SAT(b);
############## 
##############-	 *d++ = b;
##############-	 *d++ = g;
##############-	 *d++ = r;
##############+     *d++ = b;
##############+     *d++ = g;
##############+     *d++ = r;
############## 
##############          r = y2 + cr;
##############          b = y2 + cb;
##############@@ -1656,9 +1675,9 @@
##############          SAT(g);
##############          SAT(b);
############## 
##############-	 *d++ = b;
##############-	 *d++ = g;
##############-	 *d++ = r;
##############+     *d++ = b;
##############+     *d++ = g;
##############+     *d++ = r;
##############       }
##############    }
############## }
##############@@ -1691,9 +1710,9 @@
##############          SAT(g);
##############          SAT(b);
############## 
##############-	 *d++ = b;
##############-	 *d++ = g;
##############-	 *d++ = r;
##############+     *d++ = b;
##############+     *d++ = g;
##############+     *d++ = r;
############## 
##############          r = y2 + cr;
##############          b = y2 + cb;
##############@@ -1702,12 +1721,13 @@
##############          SAT(g);
##############          SAT(b);
############## 
##############-	 *d++ = b;
##############-	 *d++ = g;
##############-	 *d++ = r;
##############+     *d++ = b;
##############+     *d++ = g;
##############+     *d++ = r;
##############       }
##############    }
############## }
##############+#endif //HAVE_CAMV4L2
############## 
############## #ifdef HAVE_JPEG
############## 
##############@@ -1714,10 +1734,10 @@
############## /* convert from mjpeg to rgb24 */
############## static bool
############## mjpeg_to_rgb24 (int width, int height,
##############-		unsigned char *src, int length,
##############-		unsigned char *dst)
##############+        unsigned char *src, int length,
##############+        unsigned char *dst)
############## {
##############-  cv::Mat temp=cv::imdecode(cv::vector<uchar>(src, src + length), 1);
##############+  cv::Mat temp=cv::imdecode(cv::Mat(std::vector<uchar>(src, src + length)), 1);
##############   if( !temp.data || temp.cols != width || temp.rows != height )
##############     return false;
##############   memcpy(dst, temp.data, width*height*3);
##############@@ -1734,7 +1754,8 @@
##############  *
##############  */
############## 
##############-void bayer2rgb24(long int WIDTH, long int HEIGHT, unsigned char *src, unsigned char *dst)
##############+#ifdef HAVE_CAMV4L2
##############+static void bayer2rgb24(long int WIDTH, long int HEIGHT, unsigned char *src, unsigned char *dst)
############## {
##############     long int i;
##############     unsigned char *rawpt, *scanpt;
##############@@ -1812,7 +1833,7 @@
############## // at least for  046d:092f Logitech, Inc. QuickCam Express Plus to work
############## //see: http://www.siliconimaging.com/RGB%20Bayer.htm
############## //and 4.6 at http://tldp.org/HOWTO/html_single/libdc1394-HOWTO/
##############-void sgbrg2rgb24(long int WIDTH, long int HEIGHT, unsigned char *src, unsigned char *dst)
##############+static void sgbrg2rgb24(long int WIDTH, long int HEIGHT, unsigned char *src, unsigned char *dst)
############## {
##############     long int i;
##############     unsigned char *rawpt, *scanpt;
##############@@ -1895,7 +1916,6 @@
##############     }
############## }
############## 
##############-
############## #define CLAMP(x)        ((x)<0?0:((x)>255)?255:(x))
############## 
############## typedef struct {
##############@@ -1919,7 +1939,7 @@
##############   present at the MSB of byte x.
############## 
############## */
##############-void sonix_decompress_init(void)
##############+static void sonix_decompress_init(void)
############## {
##############   int i;
##############   int is_abs, val, len;
##############@@ -1997,7 +2017,7 @@
##############   Returns <0 if operation failed.
############## 
############## */
##############-int sonix_decompress(int width, int height, unsigned char *inp, unsigned char *outp)
##############+static int sonix_decompress(int width, int height, unsigned char *inp, unsigned char *outp)
############## {
##############   int row, col;
##############   int val;
##############@@ -2066,13 +2086,14 @@
############## 
##############   return 0;
############## }
##############+#endif //HAVE_CAMV4L2
############## 
##############-
############## static IplImage* icvRetrieveFrameCAM_V4L( CvCaptureCAM_V4L* capture, int) {
############## 
############## #ifdef HAVE_CAMV4L2
##############   if (V4L2_SUPPORT == 0)
############## #endif /* HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############     /* [FD] this really belongs here */
##############@@ -2081,6 +2102,7 @@
##############     }
############## 
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
##############    /* Now get what has already been captured as a IplImage return */
############## 
##############@@ -2101,8 +2123,12 @@
##############        capture->frame.imageData = (char *)cvAlloc(capture->frame.imageSize);
##############     }
############## 
##############-  } else
##############+  }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+    else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############     if((capture->frame.width != capture->mmaps[capture->bufferIndex].width)
##############@@ -2116,117 +2142,118 @@
##############     }
############## 
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
############## #ifdef HAVE_CAMV4L2
############## 
##############   if (V4L2_SUPPORT == 1)
##############   {
##############+    switch (capture->palette)
##############+    {
##############+    case PALETTE_BGR24:
##############+        memcpy((char *)capture->frame.imageData,
##############+               (char *)capture->buffers[capture->bufferIndex].start,
##############+               capture->frame.imageSize);
##############+        break;
############## 
##############-    if (PALETTE_BGR24 == 1)
##############-      memcpy((char *)capture->frame.imageData,
##############-             (char *)capture->buffers[capture->bufferIndex].start,
##############-             capture->frame.imageSize);
##############+    case PALETTE_YVU420:
##############+        yuv420p_to_rgb24(capture->form.fmt.pix.width,
##############+                 capture->form.fmt.pix.height,
##############+                 (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############+                 (unsigned char*)capture->frame.imageData);
##############+        break;
############## 
##############-    if (PALETTE_YVU420 == 1)
##############-      yuv420p_to_rgb24(capture->form.fmt.pix.width,
##############-                       capture->form.fmt.pix.height,
##############-                       (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############-                       (unsigned char*)capture->frame.imageData);
##############-
##############-    if (PALETTE_YUV411P == 1)
##############-      yuv411p_to_rgb24(capture->form.fmt.pix.width,
##############-                       capture->form.fmt.pix.height,
##############-                       (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############-                       (unsigned char*)capture->frame.imageData);
##############-
##############+    case PALETTE_YUV411P:
##############+        yuv411p_to_rgb24(capture->form.fmt.pix.width,
##############+                 capture->form.fmt.pix.height,
##############+                 (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############+                 (unsigned char*)capture->frame.imageData);
##############+        break;
############## #ifdef HAVE_JPEG
##############-#ifdef __USE_GNU
##############-    /* support for MJPEG is only available with libjpeg and gcc,
##############-       because it's use libjepg and fmemopen()
##############-    */
##############-    if (PALETTE_MJPEG == 1)
##############-      if (!mjpeg_to_rgb24(capture->form.fmt.pix.width,
##############-			  capture->form.fmt.pix.height,
##############-			  (unsigned char*)(capture->buffers[capture->bufferIndex]
##############-					   .start),
##############-			  capture->buffers[capture->bufferIndex].length,
##############-			  (unsigned char*)capture->frame.imageData))
##############-	return 0;
##############+    case PALETTE_MJPEG:
##############+        if (!mjpeg_to_rgb24(capture->form.fmt.pix.width,
##############+                    capture->form.fmt.pix.height,
##############+                    (unsigned char*)(capture->buffers[capture->bufferIndex]
##############+                             .start),
##############+                    capture->buffers[capture->bufferIndex].length,
##############+                    (unsigned char*)capture->frame.imageData))
##############+          return 0;
##############+        break;
############## #endif
##############-#endif
############## 
##############-    if (PALETTE_YUYV == 1)
##############-	yuyv_to_rgb24(capture->form.fmt.pix.width,
##############-		      capture->form.fmt.pix.height,
##############-		      (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############-		      (unsigned char*)capture->frame.imageData);
##############-
##############-    if (PALETTE_UYVY == 1)
##############-	uyvy_to_rgb24(capture->form.fmt.pix.width,
##############-		      capture->form.fmt.pix.height,
##############-		      (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############-		      (unsigned char*)capture->frame.imageData);
##############-
##############-    if (PALETTE_SBGGR8 == 1)
##############-    {
##############-      bayer2rgb24(capture->form.fmt.pix.width,
##############+    case PALETTE_YUYV:
##############+        yuyv_to_rgb24(capture->form.fmt.pix.width,
##############                   capture->form.fmt.pix.height,
##############-                  (unsigned char*)capture->buffers[capture->bufferIndex].start,
##############+                  (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############                   (unsigned char*)capture->frame.imageData);
##############-    }
##############+        break;
##############+    case PALETTE_UYVY:
##############+        uyvy_to_rgb24(capture->form.fmt.pix.width,
##############+                  capture->form.fmt.pix.height,
##############+                  (unsigned char*)(capture->buffers[capture->bufferIndex].start),
##############+                  (unsigned char*)capture->frame.imageData);
##############+        break;
##############+    case PALETTE_SBGGR8:
##############+        bayer2rgb24(capture->form.fmt.pix.width,
##############+                capture->form.fmt.pix.height,
##############+                (unsigned char*)capture->buffers[capture->bufferIndex].start,
##############+                (unsigned char*)capture->frame.imageData);
##############+        break;
############## 
##############-    if (PALETTE_SN9C10X == 1)
##############-    {
##############-      sonix_decompress_init();
##############+    case PALETTE_SN9C10X:
##############+        sonix_decompress_init();
##############+        sonix_decompress(capture->form.fmt.pix.width,
##############+                 capture->form.fmt.pix.height,
##############+                 (unsigned char*)capture->buffers[capture->bufferIndex].start,
##############+                 (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start);
############## 
##############-      sonix_decompress(capture->form.fmt.pix.width,
##############-                       capture->form.fmt.pix.height,
##############-                       (unsigned char*)capture->buffers[capture->bufferIndex].start,
##############-                       (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start);
##############+        bayer2rgb24(capture->form.fmt.pix.width,
##############+                capture->form.fmt.pix.height,
##############+                (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start,
##############+                (unsigned char*)capture->frame.imageData);
##############+        break;
############## 
##############-      bayer2rgb24(capture->form.fmt.pix.width,
##############-                  capture->form.fmt.pix.height,
##############-                  (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start,
##############-                  (unsigned char*)capture->frame.imageData);
##############+    case PALETTE_SGBRG:
##############+        sgbrg2rgb24(capture->form.fmt.pix.width,
##############+                capture->form.fmt.pix.height,
##############+                (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start,
##############+                (unsigned char*)capture->frame.imageData);
##############+        break;
##############     }
##############-
##############-    if (PALETTE_SGBRG == 1)
##############-    {
##############-       sgbrg2rgb24(capture->form.fmt.pix.width,
##############-                  capture->form.fmt.pix.height,
##############-                  (unsigned char*)capture->buffers[(capture->bufferIndex+1) % capture->req.count].start,
##############-                  (unsigned char*)capture->frame.imageData);
##############-    }
##############-
##############-  } else
##############+  }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+    else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############-    switch(capture->imageProperties.palette) {
##############-      case VIDEO_PALETTE_RGB24:
##############+    switch(capture->imageProperties.palette)
##############+    {
##############+    case VIDEO_PALETTE_RGB24:
##############         memcpy((char *)capture->frame.imageData,
##############            (char *)(capture->memoryMap + capture->memoryBuffer.offsets[capture->bufferIndex]),
##############            capture->frame.imageSize);
##############         break;
##############-      case VIDEO_PALETTE_YUV420P:
##############+    case VIDEO_PALETTE_YUV420P:
##############         yuv420p_to_rgb24(capture->captureWindow.width,
##############              capture->captureWindow.height,
##############              (unsigned char*)(capture->memoryMap + capture->memoryBuffer.offsets[capture->bufferIndex]),
##############              (unsigned char*)capture->frame.imageData);
##############         break;
##############-      case VIDEO_PALETTE_YUV420:
##############+    case VIDEO_PALETTE_YUV420:
##############         yuv420_to_rgb24(capture->captureWindow.width,
##############           capture->captureWindow.height,
##############           (unsigned char*)(capture->memoryMap + capture->memoryBuffer.offsets[capture->bufferIndex]),
##############           (unsigned char*)capture->frame.imageData);
##############         break;
##############-      case VIDEO_PALETTE_YUV411P:
##############+    case VIDEO_PALETTE_YUV411P:
##############         yuv411p_to_rgb24(capture->captureWindow.width,
##############           capture->captureWindow.height,
##############           (unsigned char*)(capture->memoryMap + capture->memoryBuffer.offsets[capture->bufferIndex]),
##############           (unsigned char*)capture->frame.imageData);
##############         break;
##############-      default:
##############+    default:
##############         fprintf( stderr,
##############                  "HIGHGUI ERROR: V4L: Cannot convert from palette %d to RGB\n",
##############                  capture->imageProperties.palette);
##############@@ -2235,6 +2262,7 @@
##############     }
############## 
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
##############    return(&capture->frame);
############## }
##############@@ -2244,7 +2272,9 @@
############## 
############## #ifdef HAVE_CAMV4L2
############## 
##############+#ifdef HAVE_CAMV4L
##############   if (V4L2_SUPPORT == 1)
##############+#endif
##############   {
############## 
##############       /* default value for min and max */
##############@@ -2253,7 +2283,7 @@
############## 
##############       CLEAR (capture->form);
##############       capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############-      if (-1 == xioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form)) {
##############+      if (-1 == ioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form)) {
##############           /* display an error message, and return an error code */
##############           perror ("VIDIOC_G_FMT");
##############           return -1;
##############@@ -2269,6 +2299,13 @@
##############       /* initialize the control structure */
############## 
##############       switch (property_id) {
##############+      case CV_CAP_PROP_POS_MSEC:
##############+          if (capture->FirstCapture) {
##############+            return 0;
##############+          } else {
##############+            return 1000 * capture->timestamp.tv_sec + ((double) capture->timestamp.tv_usec) / 1000;
##############+          }
##############+          break;
##############       case CV_CAP_PROP_BRIGHTNESS:
##############           capture->control.id = V4L2_CID_BRIGHTNESS;
##############           break;
##############@@ -2294,7 +2331,7 @@
##############         return -1;
##############       }
############## 
##############-      if (-1 == xioctl (capture->deviceHandle, VIDIOC_G_CTRL,
##############+      if (-1 == ioctl (capture->deviceHandle, VIDIOC_G_CTRL,
##############                         &capture->control)) {
############## 
##############           fprintf( stderr, "HIGHGUI ERROR: V4L2: ");
##############@@ -2355,8 +2392,12 @@
##############       /* all was OK, so convert to 0.0 - 1.0 range, and return the value */
##############       return ((float)capture->control.value - v4l2_min + 1) / (v4l2_max - v4l2_min);
############## 
##############-  } else
##############+  }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+    else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############     int retval = -1;
##############@@ -2414,6 +2455,7 @@
##############     return float (retval) / 0xFFFF;
############## 
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
############## };
############## 
##############@@ -2424,21 +2466,28 @@
##############   if (V4L2_SUPPORT == 1)
##############   {
############## 
##############-    CLEAR (capture->crop);
##############-    capture->crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############-    capture->crop.c.left       = 0;
##############-    capture->crop.c.top        = 0;
##############-    capture->crop.c.height     = h*24;
##############-    capture->crop.c.width      = w*24;
##############+    CLEAR (capture->cropcap);
##############+    capture->cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
############## 
##############-    /* set the crop area, but don't exit if the device don't support croping */
##############-    xioctl (capture->deviceHandle, VIDIOC_S_CROP, &capture->crop);
##############+    if (ioctl (capture->deviceHandle, VIDIOC_CROPCAP, &capture->cropcap) < 0) {
##############+        fprintf(stderr, "HIGHGUI ERROR: V4L/V4L2: VIDIOC_CROPCAP\n");
##############+    } else {
############## 
##############+        CLEAR (capture->crop);
##############+        capture->crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############+        capture->crop.c= capture->cropcap.defrect;
##############+
##############+        /* set the crop area, but don't exit if the device don't support croping */
##############+        if (ioctl (capture->deviceHandle, VIDIOC_S_CROP, &capture->crop) < 0) {
##############+            fprintf(stderr, "HIGHGUI ERROR: V4L/V4L2: VIDIOC_S_CROP\n");
##############+        }
##############+    }
##############+
##############     CLEAR (capture->form);
##############     capture->form.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
############## 
##############     /* read the current setting, mainly to retreive the pixelformat information */
##############-    xioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form);
##############+    ioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form);
############## 
##############     /* set the values we want to change */
##############     capture->form.fmt.pix.width = w;
##############@@ -2453,7 +2502,7 @@
##############      * don't test if the set of the size is ok, because some device
##############      * don't allow changing the size, and we will get the real size
##############      * later */
##############-    xioctl (capture->deviceHandle, VIDIOC_S_FMT, &capture->form);
##############+    ioctl (capture->deviceHandle, VIDIOC_S_FMT, &capture->form);
############## 
##############     /* try to set framerate to 30 fps */
##############     struct v4l2_streamparm setfps;
##############@@ -2461,7 +2510,7 @@
##############     setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############     setfps.parm.capture.timeperframe.numerator = 1;
##############     setfps.parm.capture.timeperframe.denominator = 30;
##############-    xioctl (capture->deviceHandle, VIDIOC_S_PARM, &setfps);
##############+    ioctl (capture->deviceHandle, VIDIOC_S_PARM, &setfps);
############## 
##############     /* we need to re-initialize some things, like buffers, because the size has
##############      * changed */
##############@@ -2468,7 +2517,7 @@
##############     capture->FirstCapture = 1;
############## 
##############     /* Get window info again, to get the real value */
##############-    if (-1 == xioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form))
##############+    if (-1 == ioctl (capture->deviceHandle, VIDIOC_G_FMT, &capture->form))
##############     {
##############       fprintf(stderr, "HIGHGUI ERROR: V4L/V4L2: Could not obtain specifics of capture window.\n\n");
############## 
##############@@ -2479,8 +2528,12 @@
############## 
##############     return 0;
############## 
##############-  } else
##############+  }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+    else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############     if (capture==0) return 0;
##############@@ -2507,6 +2560,7 @@
##############      capture->FirstCapture = 1;
############## 
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
##############   return 0;
############## 
##############@@ -2563,7 +2617,7 @@
##############     }
############## 
##############     /* get the min and max values */
##############-    if (-1 == xioctl (capture->deviceHandle,
##############+    if (-1 == ioctl (capture->deviceHandle,
##############                       VIDIOC_G_CTRL, &capture->control)) {
############## //          perror ("VIDIOC_G_CTRL for getting min/max values");
##############           return -1;
##############@@ -2633,13 +2687,17 @@
##############     capture->control.value = (int)(value * (v4l2_max - v4l2_min) + v4l2_min);
############## 
##############     /* The driver may clamp the value or return ERANGE, ignored here */
##############-    if (-1 == xioctl (capture->deviceHandle,
##############+    if (-1 == ioctl (capture->deviceHandle,
##############                       VIDIOC_S_CTRL, &capture->control) && errno != ERANGE) {
##############         perror ("VIDIOC_S_CTRL");
##############         return -1;
##############     }
##############-  } else
##############+  }
############## #endif /* HAVE_CAMV4L2 */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+    else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############   {
############## 
##############     int v4l_value;
##############@@ -2684,6 +2742,7 @@
##############        return -1;
##############     }
##############   }
##############+#endif /* HAVE_CAMV4L */
############## 
##############   /* all was OK */
##############   return 0;
##############@@ -2744,6 +2803,7 @@
############## #ifdef HAVE_CAMV4L2
##############      if (V4L2_SUPPORT == 0)
############## #endif /* HAVE_CAMV4L2 */
##############+#ifdef HAVE_CAMV4L
##############      {
############## 
##############        if (capture->mmaps)
##############@@ -2752,16 +2812,20 @@
##############          munmap(capture->memoryMap, capture->memoryBuffer.size);
############## 
##############      }
##############+#endif /* HAVE_CAMV4L */
##############+#if defined(HAVE_CAMV4L) && defined(HAVE_CAMV4L2)
##############+     else
##############+#endif /* HAVE_CAMV4L && HAVE_CAMV4L2 */
############## #ifdef HAVE_CAMV4L2
##############-     else {
##############+       {
##############        capture->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
##############-       if (ioctl(capture->deviceHandle, VIDIOC_STREAMOFF, &capture->type) < 0) {
##############+       if (-1 == ioctl(capture->deviceHandle, VIDIOC_STREAMOFF, &capture->type)) {
##############            perror ("Unable to stop the stream.");
##############        }
############## 
##############-       for (unsigned int n_buffers = 0; n_buffers < capture->req.count; ++n_buffers)
##############+       for (unsigned int n_buffers_ = 0; n_buffers_ < capture->req.count; ++n_buffers_)
##############        {
##############-           if (-1 == munmap (capture->buffers[n_buffers].start, capture->buffers[n_buffers].length)) {
##############+           if (-1 == munmap (capture->buffers[n_buffers_].start, capture->buffers[n_buffers_].length)) {
##############                perror ("munmap");
##############            }
##############        }
##############@@ -2768,8 +2832,8 @@
############## 
##############        if (capture->buffers[MAX_V4L_BUFFERS].start)
##############        {
##############-    	   free(capture->buffers[MAX_V4L_BUFFERS].start);
##############-    	   capture->buffers[MAX_V4L_BUFFERS].start = 0;
##############+           free(capture->buffers[MAX_V4L_BUFFERS].start);
##############+           capture->buffers[MAX_V4L_BUFFERS].start = 0;
##############        }
##############      }
############## #endif /* HAVE_CAMV4L2 */
##############Index: src/Makefile.in
##############===================================================================
##############--- src/Makefile.in	(revision 2281)
##############+++ src/Makefile.in	(working copy)
##############@@ -187,7 +187,8 @@
############## 	lib_cxcore_la-cxmatmul.lo lib_cxcore_la-cxmatrix.lo \
############## 	lib_cxcore_la-cxpersistence.lo lib_cxcore_la-cxrand.lo \
############## 	lib_cxcore_la-cxstat.lo lib_cxcore_la-cxsystem.lo \
##############-	lib_cxcore_la-cxtables.lo
##############+	lib_cxcore_la-cxtables.lo \
##############+	lib_cxcore_la-cxflann.lo
############## lib_cxcore_la_OBJECTS = $(am_lib_cxcore_la_OBJECTS)
############## lib_cxcore_la_LINK = $(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) \
############## 	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(lib_cxcore_la_CXXFLAGS) \
##############@@ -611,7 +612,8 @@
##############   cxcore/cxrand.cpp \
##############   cxcore/cxstat.cpp \
##############   cxcore/cxsystem.cpp \
##############-  cxcore/cxtables.cpp
##############+  cxcore/cxtables.cpp \
##############+  cxcore/cxflann.cpp
############## 
############## lib_cxcore_la_CPPFLAGS = $(LAPACK_INCL) $(AM_CPPFLAGS)
############## lib_cxcore_la_CXXFLAGS = @SIMD_FLAGS@ @OPENMP_CXXFLAGS@ $(AM_CXXFLAGS)
##############@@ -1178,6 +1180,7 @@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_cxcore_la-cxstat.Plo@am__quote@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_cxcore_la-cxsystem.Plo@am__quote@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_cxcore_la-cxtables.Plo@am__quote@
##############+@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_cxcore_la-cxflann.Plo@am__quote@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_highgui_la-bitstrm.Plo@am__quote@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_highgui_la-cvcap.Plo@am__quote@
############## @AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/lib_highgui_la-cvcap_dc1394.Plo@am__quote@
##############@@ -2386,6 +2389,13 @@
############## @AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
############## @am__fastdepCXX_FALSE@	$(LIBTOOL)  --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(lib_cxcore_la_CPPFLAGS) $(CPPFLAGS) $(lib_cxcore_la_CXXFLAGS) $(CXXFLAGS) -c -o lib_cxcore_la-cxtables.lo `test -f 'cxcore/cxtables.cpp' || echo '$(srcdir)/'`cxcore/cxtables.cpp
############## 
##############+lib_cxcore_la-cxflann.lo: cxcore/cxflann.cpp
##############+@am__fastdepCXX_TRUE@	$(LIBTOOL)  --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(lib_cxcore_la_CPPFLAGS) $(CPPFLAGS) $(lib_cxcore_la_CXXFLAGS) $(CXXFLAGS) -MT lib_cxcore_la-cxflann.lo -MD -MP -MF $(DEPDIR)/lib_cxcore_la-cxflann.Tpo -c -o lib_cxcore_la-cxflann.lo `test -f 'cxcore/cxflann.cpp' || echo '$(srcdir)/'`cxcore/cxflann.cpp
##############+@am__fastdepCXX_TRUE@	$(am__mv) $(DEPDIR)/lib_cxcore_la-cxflann.Tpo $(DEPDIR)/lib_cxcore_la-cxflann.Plo
##############+@AMDEP_TRUE@@am__fastdepCXX_FALSE@	source='cxcore/cxflann.cpp' object='lib_cxcore_la-cxflann.lo' libtool=yes @AMDEPBACKSLASH@
##############+@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
##############+@am__fastdepCXX_FALSE@	$(LIBTOOL)  --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(lib_cxcore_la_CPPFLAGS) $(CPPFLAGS) $(lib_cxcore_la_CXXFLAGS) $(CXXFLAGS) -c -o lib_cxcore_la-cxflann.lo `test -f 'cxcore/cxflann.cpp' || echo '$(srcdir)/'`cxcore/cxflann.cpp
##############+
############## lib_highgui_la-bitstrm.lo: highgui/bitstrm.cpp
############## @am__fastdepCXX_TRUE@	$(LIBTOOL)  --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(lib_highgui_la_CXXFLAGS) $(CXXFLAGS) -MT lib_highgui_la-bitstrm.lo -MD -MP -MF $(DEPDIR)/lib_highgui_la-bitstrm.Tpo -c -o lib_highgui_la-bitstrm.lo `test -f 'highgui/bitstrm.cpp' || echo '$(srcdir)/'`highgui/bitstrm.cpp
############## @am__fastdepCXX_TRUE@	$(am__mv) $(DEPDIR)/lib_highgui_la-bitstrm.Tpo $(DEPDIR)/lib_highgui_la-bitstrm.Plo
#---------------------------------------------------------------------------------------
define config-opencv_svn
	mkdir -p $(1); \
	cd $(1); \
	$(2)/configure --host=arm-xilinx-linux-gnueabi --with-ffmpeg --without-quicktime --with-v4l --with-gtk --disable-apps --enable-shared CXX=arm-xilinx-linux-gnueabi-g++ CPPFLAGS=-I$(3)/include LDFLAGS="-L$(3)/lib" --prefix=$(3) CXXFLAGS=-D__STDC_CONSTANT_MACROS LIBS="-lavcore -lavutil -lxvidcore -lx264"
endef
PHONY += opencv_svn
opencv_svn :
	$(info start compile $@ ...)
#notice:1.prefix;2.link-flag;3.3rd-lib;add-entry:UNIX
#	$(call config-opencv_svn,$(top)/$@/build,$(top)/$@/opencv,$(rootfs_path))
	make -C $(top)/$@/build -j8;
	make -C $(top)/$@/build install

PHONY += opencv
opencv : ffmpeg gtk
	$(info start compile $@ ...)
	make opencv_svn


define build-ramdisk
	set -x; \
	cd $(1); \
	rm ramdisk8M.image.gz; \
	dd if=/dev/zero of=ramdisk8M.image bs=1024 count=8192; \
	mke2fs -F ramdisk8M.image -L "ramdisk" -b 1024 -m 0; \
	tune2fs ramdisk8M.image -i 0; \
	chmod 777 ramdisk8M.image; \
	mkdir -p ramdisk; \
	sudo mount -o loop ramdisk8M.image ramdisk; \
	sudo rsync -aR $(2)/./ ramdisk; \
	sudo chown -R root:root ramdisk; \
	sudo umount ramdisk; \
	gzip ramdisk8M.image; \
	mkimage -A arm -T ramdisk -C gzip -d ramdisk8M.image.gz uramdisk.image.gz \
	rmdir ramdisk
endef

PHONY += ramdisk
ramdisk : busybox dropbear
	$(info start compile $@ ...)
	mkdir -p $(ramdisk_path)/dev/pts $(ramdisk_path)/dev/i2c $(ramdisk_path)/etc/dropbear $(ramdisk_path)/etc/init.d $(ramdisk_path)/mnt $(ramdisk_path)/opt $(ramdisk_path)/proc $(ramdisk_path)/home $(ramdisk_path)/sys $(ramdisk_path)/tmp $(ramdisk_path)/var $(ramdisk_path)/var/log $(ramdisk_path)/var/www; \
	rsync -aR $(TOOLCHAIN_CLIB)/./lib $(ramdisk_path); \
	rsync -aR $(TOOLCHAIN_CLIB)/./sbin $(ramdisk_path); \
	rsync -aR $(TOOLCHAIN_CLIB)/./usr/bin $(ramdisk_path); \
	sudo chmod 4755 $(ramdisk_path)/bin/busybox; \
	rsync -aR prepare/./etc $(ramdisk_path); \
	for libfile in $$(find $(ramdisk_path)/lib -type f); do \
		if ($(CROSS_COMPILE)strip $$libfile) ; then \
			true; \
		fi; \
		echo $$libfile; \
	done; \
	$(call build-ramdisk,$(shell dirname $(ramdisk_path)),$(ramdisk_path));
	if test -d $(rootfs_path);then rm -rf $(rootfs_path);fi;
	mv $(ramdisk_path) $(rootfs_path);

define config-qt-everywhere
	cd $(1); \
	./configure -embedded arm \
		       -xplatform qws/linux-arm-gnueabi-g++ \
		       -little-endian \
		       -opensource \
		       -host-little-endian \
		       -confirm-license \
		       -nomake demos \
		       -nomake examples \
		       -prefix $(2);
endef

PHONY += qt/qt-everywhere-opensource-src-4.7.3
qt/qt-everywhere-opensource-src-4.7.3 :
	cp $$(dirname $@)/qmake.conf $@/mkspecs/qws/linux-arm-gnueabi-g++
#	$(call config-qt-everywhere,$@,$(top)/$$(dirname $@)/out)
	make -C $@ -j8
	make -C $@ install

define build-qwt
	cd $(1); \
	$$(dirname $(1))/out/bin/qmake qwt.pro; \
	make -j8; \
	make install;
endef

PHONY += qt/qwt-6.0.1
qt/qwt-6.0.1 : qt/qt-everywhere-opensource-src-4.7.3
	cp $$(dirname $@)/qwtconfig.pri $@
	$(call build-qwt,$(top)/$@)
#	cp -P $(TOOLCHAIN_CLIB)/usr/lib/libstdc++.so* 

PHONY += qt
qt : qt/qwt-6.0.1

PHONY += rootfs
rootfs : ramdisk opencv
	$(info start compile $@ ...)
	rsync -aR --exclude=locale $(TOOLCHAIN_CLIB)/./usr/lib $(rootfs_path)
	rsync -aR prepare/./usr $(rootfs_path); \
#	grep -R "$(top)/gtk/gtk+/arm" $(rootfs_path) |sed -e 's/^\([^:]*\):.*/\1/p'|grep "^$(rootfs_path)" |xargs sed -i 's:$(top)/gtk/gtk+/arm::gp'
#	grep -R "$(rootfs_path)" $(rootfs_path) |sed -e 's/^\([^:]*\):.*/\1/p'|grep "^$(rootfs_path)" |xargs sed -i 's:$(rootfs_path)::gp'
	mkdir -p $(rootfs_path)$(top)/gtk/gtk+
	ln -s / $(rootfs_path)$(top)/gtk/gtk+/arm
	mkdir -p $(rootfs_path)$(top)/release
	ln -s / $(rootfs_path)$(top)/release/rootfs_

define build-release
	sudo chown -R root:root $(rootfs_path)
	sudo tar zcvf $(1)/rootfs.tar.gz -C $(rootfs_path) ./
	sudo chown -R xiaofei:xiaofei $(rootfs_path)
endef
PHONY += release
release : rootfs
	$(info start compile $@ ...)
	$(call build-release,$(@));
	if test -d $(@)/rootfs;then rm -rf $(@)/rootfs;fi;
	mv $(rootfs_path) $(@)/rootfs;

xiaofei : 
	$(info start compile $@ ...)
	$(call build-release,release);

.PHONY: $(PHONY)

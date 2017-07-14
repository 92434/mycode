#
#
#================================================================
#   Copyright (C) 2017年07月14日 肖飞 All rights reserved.
#   
#   文件名称：Android.mk
#   创 建 者：肖飞
#   创建日期：2017年07月14日 星期五 16时08分37秒
#   修改日期：2017年07月14日 星期五 16时39分59秒
#   描    述：
#
#================================================================
LOCAL_PATH := $(call my-dir)/..
BMF_DIR := $(LOCAL_PATH)/BMF
SRC_DIR := $(LOCAL_PATH)/algorithm

include $(CLEAR_VARS)
LOCAL_MODULE := BMFLIB
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES := $(BMF_DIR)/lib/armeabi-v7a-linux-android/libBMF.a
endif
ifeq ($(TARGET_ARCH_ABI),x86_64)
LOCAL_SRC_FILES := $(BMF_DIR)/lib/x86_64-linux-gnu/libBMF.a
endif
include $(PREBUILT_STATIC_LIBRARY)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE := BMFLIB_NEON
LOCAL_SRC_FILES := $(BMF_DIR)/lib/armeabi-v7a-linux-android/libBMF_neon.a
include $(PREBUILT_STATIC_LIBRARY)
endif


include $(CLEAR_VARS)
LOCAL_MODULE := ftalg
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc $(BMF_DIR)/inc
LOCAL_CFLAGS := -Werror --debug -fstack-protector-strong -fno-tree-scev-cprop
LOCAL_LDFLAGS := -lgcc -llog
LOCAL_SRC_FILES := $(SRC_DIR)/focal_fp_spi.c $(SRC_DIR)/focal_sensor.c $(SRC_DIR)/fpsensorlib.c $(SRC_DIR)/focal_spa.c $(SRC_DIR)/pb_alg.c $(SRC_DIR)/timestamp.c
LOCAL_STATIC_LIBRARIES := BMFLIB
include $(BUILD_STATIC_LIBRARY)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE := ftalg_neon
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc $(BMF_DIR)/inc
LOCAL_CFLAGS := -Werror --debug -fstack-protector-strong -fno-tree-scev-cprop
LOCAL_LDFLAGS := -lgcc -llog
LOCAL_SRC_FILES := $(SRC_DIR)/focal_fp_spi.c $(SRC_DIR)/focal_sensor.c $(SRC_DIR)/fpsensorlib.c $(SRC_DIR)/focal_spa.c $(SRC_DIR)/pb_alg.c $(SRC_DIR)/timestamp.c
LOCAL_STATIC_LIBRARIES := BMFLIB_NEON
include $(BUILD_STATIC_LIBRARY)
endif

include $(CLEAR_VARS)
LOCAL_MODULE := ft_lib
LOCAL_SRC_FILES := $(LOCAL_PATH)/ft_lib/ft_lib.c
include $(BUILD_STATIC_LIBRARY)

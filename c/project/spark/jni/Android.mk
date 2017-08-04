#
#
#================================================================
#   Copyright (C) 2017年07月14日 肖飞 All rights reserved.
#   
#   文件名称：Android.mk
#   创 建 者：肖飞
#   创建日期：2017年07月14日 星期五 16时08分37秒
#   修改日期：2017年08月04日 星期五 12时43分33秒
#   描    述：
#
#================================================================
LOCAL_PATH := $(call my-dir)/..
FT_LIB_SRC_DIR := $(LOCAL_PATH)/ft_lib

ifeq ($(T),1)
include $(CLEAR_VARS)
LOCAL_MODULE := ft_lib
LOCAL_SRC_FILES := $(FT_LIB_SRC_DIR)/FpSensorLib.c
include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(T),2)
include $(CLEAR_VARS)
LOCAL_MODULE := FTLIB
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES := $(LOCAL_PATH)/obj/local/armeabi-v7a/libft_lib.a
endif
ifeq ($(TARGET_ARCH_ABI),x86_64)
LOCAL_SRC_FILES := $(LOCAL_PATH)/obj/local/x86_64/libft_lib.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
PROJECT := 9348

app_c_files := $(LOCAL_PATH)/main.cpp
app_c_files += $(LOCAL_PATH)/configuration.cpp
app_c_files += $(LOCAL_PATH)/filesystem.cpp
ifneq ($(PROJECT),)
app_c_files += $(LOCAL_PATH)/hardware_$(PROJECT).cpp
endif
app_c_files += $(LOCAL_PATH)/hardware.cpp
app_c_files += $(LOCAL_PATH)/optparse.cpp
app_c_files += $(LOCAL_PATH)/regexp.cpp
app_c_files += $(LOCAL_PATH)/samples_list.cpp
app_c_files += $(LOCAL_PATH)/settings.cpp
app_c_files += $(LOCAL_PATH)/test_task.cpp

LOCAL_CFLAGS := -g
LOCAL_CFLAGS += -Ift_lib/
ifneq ($(PROJECT),)
LOCAL_CFLAGS += -DPROJECT=HW$(PROJECT)
endif

#LOCAL_LDFLAGS := -Wl,-Map,main.map
LOCAL_MODULE := spark
LOCAL_STATIC_LIBRARIES := FTLIB
LOCAL_SRC_FILES := $(app_c_files)
include $(BUILD_EXECUTABLE)
endif

#
#
#================================================================
#   Copyright (C) 2017年07月14日 肖飞 All rights reserved.
#   
#   文件名称：Makefile
#   创 建 者：肖飞
#   创建日期：2017年07月14日 星期五 12时52分37秒
#   修改日期：2017年08月15日 星期二 10时46分26秒
#   描    述：
#
#================================================================

include $(default_rules)

export LIB_NAME := 9348

app_c_files := main.cpp
app_c_files += configuration.cpp
app_c_files += filesystem.cpp
ifneq ($(LIB_NAME),)
app_c_files += hardware_$(LIB_NAME).cpp
endif
app_c_files += hardware.cpp
app_c_files += optparse.cpp
app_c_files += regexp.cpp
app_c_files += samples_list.cpp
app_c_files += settings.cpp
app_c_files += test_task.cpp

app_name := spark

LOCAL_DEPS += ft_lib/out/lib/libft_lib.a
LOCAL_LIBS := ft_lib/out/lib/libft_lib.a

#LOCAL_DEPS += obj/local/x86_64/libft_lib.a
#LOCAL_LIBS := obj/local/x86_64/libft_lib.a

LOCAL_CFLAGS := -g
LOCAL_CFLAGS += -Ift_lib/
ifneq ($(LIB_NAME),)
LOCAL_CFLAGS += -DHW$(LIB_NAME)
endif

#LOCAL_LDFLAGS := -Wl,-Map,main.map
include $(BUILD_APP)

SUB_DIRS += ft_lib

clean : other-clean

include $(ADD_TARGET)

other-clean:
	rm -rf obj
	rm -rf libs

#
#
#================================================================
#   Copyright (C) 2017年09月18日 肖飞 All rights reserved.
#   
#   文件名称：Makefile
#   创 建 者：肖飞
#   创建日期：2017年09月18日 星期一 12时34分43秒
#   修改日期：2017年09月18日 星期一 12时37分23秒
#   描    述：
#
#================================================================
include $(default_rules)

CC := gcc

CFLAGS += -g -lstdc++
CFLAGS += -I$(LINUX_RBTREE)

c_files := rbtree.c

staticd_lib_c_files := $(c_files)
staticd_lib_name := rbtree
LOCAL_LDFLAGS += -static
include $(BUILD_LIB)

shared_lib_c_files := $(c_files)
shared_lib_name := rbtree
LOCAL_LDFLAGS += -fPIC
include $(BUILD_SO)

include $(ADD_TARGET)

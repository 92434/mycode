#
#
#================================================================
#   Copyright (C) 2017年09月18日 肖飞 All rights reserved.
#   
#   文件名称：Makefile
#   创 建 者：肖飞
#   创建日期：2017年09月18日 星期一 12时31分00秒
#   修改日期：2017年09月18日 星期一 12时49分25秒
#   描    述：
#
#================================================================
include $(default_rules)

export LINUX_RBTREE := $(shell pwd)

SUB_DIRS += rbtree

app_c_files := test_rbtree.cpp
app_name := test_rbtree
LOCAL_DEPS += rbtree/out/lib/librbtree.a
LOCAL_LIBS := rbtree/out/lib/librbtree.a
LOCAL_CFLAGS := -g
LOCAL_CFLAGS += -Irbtree/
#LOCAL_LDFLAGS := -Wl,-Map,main.map
include $(BUILD_APP)

include $(ADD_TARGET)

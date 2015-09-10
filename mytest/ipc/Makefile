include $(default_rules)

#local_lib_src := $(top)/utils
#CFLAGS += -I$(local_lib_src)
#CFLAGS += -DLOOPER_USES_EPOLL

#LDFLAGS += -L$(local_lib_src)/$(out_dir)/lib
#LDFLAGS += -static
#LDFLAGS += -lxiaofei
#LDFLAGS += -lrt
#LDFLAGS += -lpthread

c_files :=
c_files += msgctl.c
c_files += msgsnd.c
c_files += msgrcv.c
include $(BUILD_BINS)

app_c_files :=
app_name :=
include $(BUILD_APP)

#$(target_files) : $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(ADD_TARGET)

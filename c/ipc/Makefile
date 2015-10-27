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
c_files += shm.c
c_files += shmwrite.c
c_files += shmread.c
c_files += sem.c
c_files += ftok.c
c_files += signal.c
c_files += sigaction.c
c_files += sigset.c
c_files += alarm.c
c_files += kill.c
c_files += pause.c
c_files += sys_sleep.c
include $(BUILD_BINS)

app_c_files :=
app_name :=
include $(BUILD_APP)

#$(target_files) : $(local_lib_src)/$(out_dir)/lib/libxiaofei.a

include $(ADD_TARGET)

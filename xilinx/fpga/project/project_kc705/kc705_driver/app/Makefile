include $(default_rules)

CC = gcc

c_files := kc705_device.c
kc705_device.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver
include $(BUILD_BINS)

app_c_files := tsp_dma.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := tsp_dma
include $(BUILD_APP)

app_c_files := asi_dma.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := asi_dma
include $(BUILD_APP)

app_c_files := i2s_dma.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := i2s_dma
include $(BUILD_APP)

app_c_files := tsp_regs.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := tsp_regs
include $(BUILD_APP)

app_c_files := csa_dma.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := csa_dma
include $(BUILD_APP)

app_c_files := csa_reg.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := csa_reg
include $(BUILD_APP)

app_c_files := dvbs2_regs.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := dvbs2_regs
include $(BUILD_APP)

include $(ADD_TARGET)

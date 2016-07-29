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

app_c_files := i2s_spi_dev.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := i2s_spi_dev
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

app_c_files := iic_slave.c
app_c_files += utils.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := iic_slave
include $(BUILD_APP)

app_c_files := A6211/src/A6211.c
app_c_files += A6211/src/AVL6211_patch.dat.c
app_c_files += A6211/src/AVL6222_patch.dat.c
app_c_files += A6211/src/avl.c
app_c_files += A6211/src/avl_dvbsx.c
app_c_files += A6211/src/ExtAV2011.c
app_c_files += A6211/src/ExtAV2020.c
app_c_files += A6211/src/ExtSharpBS2S7VZ7801.c
app_c_files += A6211/src/IBase.c
app_c_files += A6211/src/IBlindScan.c
app_c_files += A6211/src/IBSP.c
app_c_files += A6211/src/IDiseqc.c
app_c_files += A6211/src/II2C.c
app_c_files += A6211/src/II2CRepeater.c
app_c_files += A6211/src/IRx.c
app_c_files += A6211/src/ITuner.c
LOCAL_CFLAGS := -I$(shell pwd)/../driver
LOCAL_LDFLAGS := -pthread
app_name := A6211
include $(BUILD_APP)

include $(ADD_TARGET)

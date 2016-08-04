include $(default_rules)

CC := gcc

LDFLAGS += -static
LDFLAGS += -lrt
LDFLAGS += -lpthread

app_c_files := src/main.c
app_c_files += src/common_fun.c
app_c_files += src/utils.c
app_c_files += src/csa.c
app_c_files += src/dvb_process_si.c
app_c_files += src/i2s_dma.c
app_c_files += src/tsp_regs.c
app_name := main
include $(BUILD_APP)

include $(ADD_TARGET)

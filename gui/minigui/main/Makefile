include $(default_rules)
CC := gcc

LDFLAGS += -L/usr/local/lib -lminigui_ths -ljpeg -lpng -ldl -lm -lpthread


apps := metext mycontrol main
c_files := metext.c
c_files += mycontrol.c

include $(BUILD_BINS)

app_c_files := main.c
app_c_files += main_common.c
app_name := main
include $(BUILD_APP)

include $(ADD_TARGET)

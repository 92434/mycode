include $(default_rules)

CC = gcc

c_files := pcie_dma_device.c
c_files += pcie_pseudo_dma_device.c
c_files += kc705_device.c
pcie_dma_device.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver
kc705_device.c.LOCAL_CFLAGS := -I$(shell pwd)/../driver
include $(BUILD_BINS)

include $(ADD_TARGET)

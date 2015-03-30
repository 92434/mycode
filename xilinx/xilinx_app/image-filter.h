#ifndef __MY_OTSU2_H

#define USE_VDMA
#define RST_ACTIVE	0
#define RST_INACTIVE	1

#include "ximage_filter.h"
#define IMAGE_WIDTH 16
#define IMAGE_HEIGHT 32

typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XImagefilter_Config;

#endif//#ifndef __MY_OTSU2_H

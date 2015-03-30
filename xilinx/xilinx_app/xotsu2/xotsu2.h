// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2014.3
// Copyright (C) 2014 Xilinx Inc. All rights reserved.
// 
// ==============================================================

#ifndef XOTSU2_H
#define XOTSU2_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xotsu2_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XOtsu2_Config;
#endif

typedef struct {
    u32 Control_bus_BaseAddress;
    u32 IsReady;
} XOtsu2;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XOtsu2_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XOtsu2_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XOtsu2_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XOtsu2_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XOtsu2_Initialize(XOtsu2 *InstancePtr, u16 DeviceId);
XOtsu2_Config* XOtsu2_LookupConfig(u16 DeviceId);
int XOtsu2_CfgInitialize(XOtsu2 *InstancePtr, XOtsu2_Config *ConfigPtr);
#else
int XOtsu2_Initialize(XOtsu2 *InstancePtr, const char* InstanceName);
int XOtsu2_Release(XOtsu2 *InstancePtr);
#endif

void XOtsu2_Start(XOtsu2 *InstancePtr);
u32 XOtsu2_IsDone(XOtsu2 *InstancePtr);
u32 XOtsu2_IsIdle(XOtsu2 *InstancePtr);
u32 XOtsu2_IsReady(XOtsu2 *InstancePtr);
void XOtsu2_EnableAutoRestart(XOtsu2 *InstancePtr);
void XOtsu2_DisableAutoRestart(XOtsu2 *InstancePtr);
u32 XOtsu2_GetReturn(XOtsu2 *InstancePtr);

void XOtsu2_SetRows(XOtsu2 *InstancePtr, u32 Data);
u32 XOtsu2_GetRows(XOtsu2 *InstancePtr);
void XOtsu2_SetCols(XOtsu2 *InstancePtr, u32 Data);
u32 XOtsu2_GetCols(XOtsu2 *InstancePtr);

void XOtsu2_InterruptGlobalEnable(XOtsu2 *InstancePtr);
void XOtsu2_InterruptGlobalDisable(XOtsu2 *InstancePtr);
void XOtsu2_InterruptEnable(XOtsu2 *InstancePtr, u32 Mask);
void XOtsu2_InterruptDisable(XOtsu2 *InstancePtr, u32 Mask);
void XOtsu2_InterruptClear(XOtsu2 *InstancePtr, u32 Mask);
u32 XOtsu2_InterruptGetEnabled(XOtsu2 *InstancePtr);
u32 XOtsu2_InterruptGetStatus(XOtsu2 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif

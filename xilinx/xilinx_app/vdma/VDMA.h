/*
 * VDMA.h
 *
 *  Created on: 16.3.2013
 *      Author: Ales
 */

#ifndef VDMA_H_
#define VDMA_H_

#include<pthread.h>

#define VDMACount 64
#define VDMAMapLen VDMACount*4

typedef struct
{
    unsigned int baseAddr;
    int vdmaHandler;
    int width;
    int height;
    int pixelLength;
    int fbLength;
    unsigned int* vdmaVirtualAddress;
    unsigned int* fb1VirtualAddress;
    unsigned int* fb2VirtualAddress;

    pthread_mutex_t lock;
} VDMA_info;

int VDMA_Init(VDMA_info *info, unsigned int baseAddr, int width, int height, int pixelLength, unsigned int fb1Addr, unsigned int fb2Addr);
void VDMA_UnInit(VDMA_info *info);
unsigned int VDMA_Get(VDMA_info *info, int num);
void VDMA_Set(VDMA_info *info, int num, unsigned int val);
void VDMA_Start(VDMA_info *info, unsigned int adr);
void VDMA_OutStart(VDMA_info *info, unsigned int adr, int circular);
int VDMA_IsRunning(VDMA_info *info);
int VDMA_IsDone(VDMA_info *info);
void VDMA_Disp(VDMA_info *info, char *str, int num);


#endif /* VDMA_H_ */

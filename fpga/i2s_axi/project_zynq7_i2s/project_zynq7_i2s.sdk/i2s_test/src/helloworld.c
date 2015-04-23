/******************************************************************************
 *
 * Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

//#include <stdio.h>
//#include "platform.h"
//
//void print(char *str);
//
//int main()
//{
//    init_platform();
//
//    print("Hello World\n\r");
//
//    return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_cache.h"          //蹇呴』鍖呭惈璇ュご鏂囦欢锛屽疄鐜癱ache鎿嶄綔

#define sendram ((int *)0x10000000)          //鍙戦�佺紦鍐插尯
#define recvram ((int *)0x10001000)          //鎺ユ敹缂撳啿鍖�
#define sizeofbuffer 32

void print(char *str);
#define WITH_SG 0
#define AXI_DMA_BASE 0x40400000

//#define MM2S_DMACR 0
//#define MM2S_DMASR 1
//#if WITH_SG
//#define MM2S_CURDESC 2
//#define MM2S_TAILDESC 4
//#else
//#define MM2S_SA 6
//#define MM2S_LENGTH 10
//#endif
#define S2MM_DMACR 12
#define S2MM_DMASR 13
#if WITH_SG
#define S2MM_CURDESC14
#define S2MM_TAILDESC16
#else
#define S2MM_DA 18
#define S2MM_LENGTH 22
#endif

void debug_axi_dma_register(unsigned int *p)
{
//	printf("MM2S_DMACR = 0x%x\n",*(p+MM2S_DMACR));
//	printf("MM2S_DMASR = 0x%x\n",*(p+MM2S_DMASR));
//#if WITH_SG
//	printf("MM2S_CURDESC = 0x%x\n",*(p+MM2S_CURDESC));
//	printf("MM2S_TAILDESC = 0x%x\n",*(p+MM2S_TAILDESC));
//#else
//	printf("MM2S_SA = 0x%x\n",*(p+MM2S_SA));
//	printf("MM2S_LENGTH = 0x%x\n",*(p+MM2S_LENGTH));
//#endif
	printf("S2MM_DMACR =0x%x\n",*(p+S2MM_DMACR));
	printf("S2MM_DMACSR =0x%x\n",*(p+S2MM_DMASR));
#if WITH_SG
	printf("S2MM_CURDESC =0x%x\n",*(p+S2MM_CURDESC));
	printf("S2MM_TAILDESC= 0x%x\n",*(p+S2MM_TAILDESC));
#else
	printf("S2MM_DA =0x%x\n",*(p+S2MM_DA));
	printf("S2MM_LENGTH =0x%x\n",*(p+S2MM_LENGTH));
#endif
}
void init_axi_dma_simple(unsigned int * p)
{
//	*(p+MM2S_DMACR) = 0x04;  //reset send axi dma
//	while(*(p+MM2S_DMACR)&0x04);
	*(p+S2MM_DMACR) =0x04;  //reset send axi dma
	while(*(p+S2MM_DMACR)&0x04);
//	*(p+MM2S_DMACR)=1;
//	while((*(p+MM2S_DMASR)&0x01));
	*(p+S2MM_DMACR)=1;
	while((*(p+S2MM_DMASR)&0x01));
//	*(p+MM2S_SA) = (unsigned int )sendram;
	*(p+S2MM_DA) =(unsigned int )recvram;
	Xil_DCacheFlushRange((u32)sendram,sizeofbuffer); //灏哻ache鍐呭鍚屾鍒癉DR2
	*(p+S2MM_LENGTH) =sizeofbuffer;//sizeof(recvram);
	while(!(*(p+S2MM_DMASR)&0x1000)); //wait for send ok
//	*(p+MM2S_LENGTH) = sizeofbuffer;//sizeof(sendram);
//	while(!(*(p+MM2S_DMASR)&0x1000)); //wait for send ok

}
void init_sendbuffer()
{
	int i;
	for(i=0;i< sizeofbuffer/4;i++)
	{
		sendram[i]=i*2;
	}

	for(i=0;i< sizeofbuffer/4;i++)
	{
		recvram[i]=0;
	}
}
void show_recvbuffer()
{
	int i;
	printf("Recv contents are:\n");
	for(i=0;i< sizeofbuffer/4;i++)
	{
		printf("%d\t",recvram[i]);
	}
	printf("\r\n");
}
void show_sendbuffer()
{
	int i;
	printf("Send contents are:\n");
	for(i=0;i< sizeofbuffer/4;i++)
	{
		printf("%d\t",sendram[i]);
	}
	printf("\r\n");
}
int main()
{
	unsigned int status=0;

	int rxlen;
	init_platform();
	init_sendbuffer();

	init_axi_dma_simple((unsigned int *)AXI_DMA_BASE);
	printf("Hello World\n\rPlease input data:");
	while(1)
	{
		scanf("%x",&status);
		printf("Got 0x%x\n",status);
		debug_axi_dma_register((unsigned int *)AXI_DMA_BASE);
		if(status==0)
		{
			break;
		}
	}
	show_sendbuffer();

	Xil_DCacheInvalidateRange((u32)recvram,sizeofbuffer);      //灏咲DR2鍐呭鍚屾鍒癱ache

	while(1) {
		show_recvbuffer();
	}
	cleanup_platform();

	return 0;
} 

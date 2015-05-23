#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_cache.h"          //必须包含该头文件，实现cache操作

#define sendram ((int *)0x10000000)          //发送缓冲区
#define recvram ((int *)0x10001000)          //接收缓冲区
#define sizeofbuffer 32

void print(char *str);
#define WITH_SG 0
#define AXI_DMA_BASE 0x40400000

//#define MM2S_DMACR 0
//#define MM2S_DMASR 1
#if WITH_SG
//#define MM2S_CURDESC 2
//#define MM2S_TAILDESC 4
#else
//#define MM2S_SA 6
//#define MM2S_LENGTH 10
#endif
#define S2MM_DMACR 12
#define S2MM_DMASR 13
#if WITH_SG
#define S2MM_CURDESC14
#define S2MM_TAILDESC16
#else
#define S2MM_DA 18
#define S2MM_LENGTH 22
#endif

void debug_axi_dma_register(unsigned int *p) {
//	printf("MM2S_DMACR = 0x%x\n", *(p + MM2S_DMACR));
//	printf("MM2S_DMASR = 0x%x\n", *(p + MM2S_DMASR));
#if WITH_SG
//	printf("MM2S_CURDESC = 0x%x\n",*(p+MM2S_CURDESC));
//	printf("MM2S_TAILDESC = 0x%x\n",*(p+MM2S_TAILDESC));
#else
//	printf("MM2S_SA = 0x%x\n", *(p + MM2S_SA));
//	printf("MM2S_LENGTH = 0x%x\n", *(p + MM2S_LENGTH));
#endif
	printf("S2MM_DMACR =0x%x\n", *(p + S2MM_DMACR));
	printf("S2MM_DMACSR =0x%x\n", *(p + S2MM_DMASR));
#if WITH_SG
	printf("S2MM_CURDESC =0x%x\n",*(p+S2MM_CURDESC));
	printf("S2MM_TAILDESC= 0x%x\n",*(p+S2MM_TAILDESC));
#else
	printf("S2MM_DA =0x%x\n", *(p + S2MM_DA));
	printf("S2MM_LENGTH =0x%x\n", *(p + S2MM_LENGTH));
#endif
}
void init_axi_dma_simple(unsigned int * p) {
//	*(p + MM2S_DMACR) = 0x04;  //reset send axi dma
//	while (*(p + MM2S_DMACR) & 0x04);

	*(p + S2MM_DMACR) = 0x04;  //reset send axi dma
	while (*(p + S2MM_DMACR) & 0x04);

//	*(p + MM2S_DMACR) = 1 | (1 << 12);
//	while ((*(p + MM2S_DMASR) & 0x01));

	*(p + S2MM_DMACR) = 1 | (1 << 12);
	while ((*(p + S2MM_DMASR) & 0x01));

//	*(p + MM2S_SA) = (unsigned int) sendram;

	*(p + S2MM_DA) = (unsigned int) recvram;

//	Xil_DCacheFlushRange((u32) sendram, sizeofbuffer); //将cache内容同步到DDR2

//	*(p + MM2S_LENGTH) = sizeofbuffer; //sizeof(sendram);

	*(p + S2MM_LENGTH) = sizeofbuffer; //sizeof(recvram);


//	while (!(*(p + MM2S_DMASR) & 0x1000)); //wait for send ok

	while (!(*(p + S2MM_DMASR) & 0x1000)); //wait for recv ok
}
void init_sendbuffer() {
	int i;
	for (i = 0; i < sizeofbuffer / 4; i++) {
		sendram[i] = i * 2;
	}
}

void init_recvbuffer() {
	int i;
	for (i = 0; i < sizeofbuffer / 4; i++) {
		recvram[i] = 0;
	}
}

void show_recvbuffer() {
	int i;
	printf("Recv contents are:\n");
	for (i = 0; i < sizeofbuffer / 4; i++) {
		printf("%x\t", recvram[i] & 0xffffff);
	}
	printf("\r\n");
}
void show_sendbuffer() {
	int i;
	printf("Send contents are:\n");
	for (i = 0; i < sizeofbuffer / 4; i++) {
		printf("%d\t", sendram[i]);
	}
	printf("\r\n");
}
int main() {

#define AXI_MYIP_BASE 0x43c00000
#define PWM_CYCLE (3000000)

	int rxlen;
	init_platform();

	unsigned int *p = (unsigned int *)AXI_MYIP_BASE;
    *p = (100000000 / PWM_CYCLE);
    *(p + 1) = 0x80000000 + ((100000000 / PWM_CYCLE) / 2);

	printf("Hello World\n\r");
	init_sendbuffer();
	while (1) {
		init_recvbuffer();
		init_axi_dma_simple((unsigned int *) AXI_DMA_BASE);
//		debug_axi_dma_register((unsigned int *) AXI_DMA_BASE);
		Xil_DCacheInvalidateRange((u32) recvram, sizeofbuffer);    //将DDR2内容同步到cache

//		show_sendbuffer();
		show_recvbuffer();
	}



	cleanup_platform();

	return 0;
}





//#include <stdio.h>
//#include <stdlib.h>
//#include "platform.h"
//#include "xil_cache.h"          //必须包含该头文件，实现cache操作
//
//#define sendram ((int *)0x10000000)          //发送缓冲区
//#define recvram ((int *)0x10001000)          //接收缓冲区
//#define sizeofbuffer 32
//
//void print(char *str);
//#define WITH_SG 0
//#define AXI_DMA_BASE 0x40400000
//
//#define MM2S_DMACR 0
//#define MM2S_DMASR 1
//#if WITH_SG
//#define MM2S_CURDESC 2
//#define MM2S_TAILDESC 4
//#else
//#define MM2S_SA 6
//#define MM2S_LENGTH 10
//#endif
//#define S2MM_DMACR 12
//#define S2MM_DMASR 13
//#if WITH_SG
//#define S2MM_CURDESC14
//#define S2MM_TAILDESC16
//#else
//#define S2MM_DA 18
//#define S2MM_LENGTH 22
//#endif
//
//void debug_axi_dma_register(unsigned int *p) {
//	printf("MM2S_DMACR = 0x%x\n", *(p + MM2S_DMACR));
//	printf("MM2S_DMASR = 0x%x\n", *(p + MM2S_DMASR));
//#if WITH_SG
//	printf("MM2S_CURDESC = 0x%x\n",*(p+MM2S_CURDESC));
//	printf("MM2S_TAILDESC = 0x%x\n",*(p+MM2S_TAILDESC));
//#else
//	printf("MM2S_SA = 0x%x\n", *(p + MM2S_SA));
//	printf("MM2S_LENGTH = 0x%x\n", *(p + MM2S_LENGTH));
//#endif
//	printf("S2MM_DMACR =0x%x\n", *(p + S2MM_DMACR));
//	printf("S2MM_DMACSR =0x%x\n", *(p + S2MM_DMASR));
//#if WITH_SG
//	printf("S2MM_CURDESC =0x%x\n",*(p+S2MM_CURDESC));
//	printf("S2MM_TAILDESC= 0x%x\n",*(p+S2MM_TAILDESC));
//#else
//	printf("S2MM_DA =0x%x\n", *(p + S2MM_DA));
//	printf("S2MM_LENGTH =0x%x\n", *(p + S2MM_LENGTH));
//#endif
//}
//void init_axi_dma_simple(unsigned int * p) {
//	*(p + MM2S_DMACR) = 0x04;  //reset send axi dma
//	while (*(p + MM2S_DMACR) & 0x04);
//
//	*(p + S2MM_DMACR) = 0x04;  //reset send axi dma
//	while (*(p + S2MM_DMACR) & 0x04);
//
//	*(p + MM2S_DMACR) = 1 | (1 << 12);
//	while ((*(p + MM2S_DMASR) & 0x01));
//
//	*(p + S2MM_DMACR) = 1 | (1 << 12);
//	while ((*(p + S2MM_DMASR) & 0x01));
//
//	*(p + MM2S_SA) = (unsigned int) sendram;
//
//	*(p + S2MM_DA) = (unsigned int) recvram;
//
//	Xil_DCacheFlushRange((u32) sendram, sizeofbuffer); //将cache内容同步到DDR2
//
//	*(p + MM2S_LENGTH) = sizeofbuffer; //sizeof(sendram);
//
//	*(p + S2MM_LENGTH) = sizeofbuffer; //sizeof(recvram);
//
//
//	while (!(*(p + MM2S_DMASR) & 0x1000)); //wait for send ok
//
//	while (!(*(p + S2MM_DMASR) & 0x1000)); //wait for recv ok
//}
//void init_sendbuffer() {
//	int i;
//	for (i = 0; i < sizeofbuffer / 4; i++) {
//		sendram[i] = i * 2;
//	}
//}
//
//void init_recvbuffer() {
//	int i;
//	for (i = 0; i < sizeofbuffer / 4; i++) {
//		recvram[i] = 0;
//	}
//}
//
//void show_recvbuffer() {
//	int i;
//	printf("Recv contents are:\n");
//	for (i = 0; i < sizeofbuffer / 4; i++) {
//		printf("%d\t", recvram[i]);
//	}
//	printf("\r\n");
//}
//void show_sendbuffer() {
//	int i;
//	printf("Send contents are:\n");
//	for (i = 0; i < sizeofbuffer / 4; i++) {
//		printf("%d\t", sendram[i]);
//	}
//	printf("\r\n");
//}
//int main() {
//	int rxlen;
//	init_platform();
//	printf("Hello World\n\r");
//	init_sendbuffer();
//	while (1) {
//		init_recvbuffer();
//		init_axi_dma_simple((unsigned int *) AXI_DMA_BASE);
////		debug_axi_dma_register((unsigned int *) AXI_DMA_BASE);
//		Xil_DCacheInvalidateRange((u32) recvram, sizeofbuffer);    //将DDR2内容同步到cache
//		show_sendbuffer();
//		show_recvbuffer();
//	}
//
//
//
//	cleanup_platform();
//
//	return 0;
//}

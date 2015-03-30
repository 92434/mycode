#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include "VDMA.h"

#define VDMAWidth 752
#define VDMAHeight 480
#define VDMAPixelWidth 8
#define VDMAFB1Adr 0x1e000000
#define VDMAFB2Adr 0x1f000000
#define VDMABaseAddr 0x43040000


int main()
{
	VDMA_info vdma;

	VDMA_Init(&vdma, VDMABaseAddr, VDMAWidth, VDMAHeight, VDMAPixelWidth, VDMAFB1Adr, VDMAFB2Adr);
	
	int i;

	for(i = 0; i < VDMAWidth * VDMAHeight * VDMAPixelWidth; i++) {
		char *buffer = (char *)vdma.fb1VirtualAddress;
		buffer[i] = (char)i;
	}

	for(i = 0; i < VDMAWidth * VDMAHeight * VDMAPixelWidth; i++) {
		char *buffer = (char *)vdma.fb2VirtualAddress;
		buffer[i] = (char)0;
	}

	VDMA_Start(&vdma, VDMAFB2Adr);
	VDMA_OutStart(&vdma, VDMAFB1Adr, 0);

	printf("Waiting for end...\n");
	while(VDMA_IsDone(&vdma)==0) {
		static int count = 0;

		sleep(1);
		count++;
		if(count == 3) {
			printf("xiaofei:time out!\n");
			break;
		}
	}

	remove("data0.bin");
	int fd_data0 = open("data0.bin", O_WRONLY | O_CREAT);
	if(fd_data0 != -1) {
		char *buffer = (char *)vdma.fb2VirtualAddress;
		write(fd_data0, buffer, sizeof(char) * VDMAWidth * VDMAHeight * VDMAPixelWidth);
		close(fd_data0);
	} else {
		printf("xiaofei:open data0.bin failed!(%s)\n", strerror(errno));
	}

	VDMA_UnInit(&vdma);
}




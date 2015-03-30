#include <stdio.h>
#include <stdlib.h>

//printf ...27...00...8f...27...af...af...af...00
//test ...27...8f...3c...af...8c...8c...3c
//api ...27...af...af...af...af...af...af...af...00...8f...10...83...94...2e...14...27...24...8f...02...8f...8f...8f...8f...8f...8f

//jal = 0x0c000000
//printf_addr = ((offset + 0x80008000) & 0fffffff) >> 2
//command is = jal | printf_addr

//a0:"xiaofei:%x %x\n"
//a1:byte[4-8]
//a2:byte[0-1]

//encode function:
////////////
////////////int xiaofei_111(void * channelHandle, int nKeyIdx, int nSubKey, int eKeyType, Boolean fOddKey, UINT8 * pabKeyValue) {
////////////	unsigned long long constant8B_ = 0xca13f6e51d093b6dLL;
////////////	unsigned long long var8B_ = 0x1234567812345678LL;
////////////	unsigned long long temp_ = 0x0000000000000000LL;
////////////	unsigned long long output_ = 0x2222222222222222LL;
////////////
////////////	unsigned char *constant8B = (unsigned char *)((void *)&constant8B_);
////////////	unsigned char *var8B = (unsigned char *)((void *)&var8B_);
////////////	unsigned char *temp = (unsigned char *)((void *)&temp_);
////////////	unsigned char *output = (unsigned char *)((void *)&output_);
////////////	int i;
////////////
////////////	for(i=0;i<8;i++){
////////////		temp[i]^=(var8B[3*i%8]+var8B[i]+constant8B[7-i]);
////////////		//printf("temp1[%d]:[%02x]\n",i,temp[i]);
////////////		temp[i]^=pabKeyValue[i];
////////////		//printf("temp2[%d]:[%02x]\n",i,temp[i]);
////////////	}
////////////	//整体循环左移3位
////////////	output[0]=(temp[0]<<3)+(temp[7]>>5);
////////////	for(i=1;i<8;i++){
////////////		output[i]=(temp[i]<<3)+(temp[i-1]>>5);
////////////	}
////////////
////////////	printf("fffe%016llx%016llx", *((unsigned long long *)output), *((unsigned long long *)var8B));
////////////
////////////	return 0;
////////////}
////////////
////#if 0//orig 1
////int hexcode[] = {
////	0x27bdffc0, //addiu sp,sp,-64
////	0x8fa20054, //lw v0,84(sp)//84是64 + (n-1)*4?
////	0x8c460000, //lw a2,0(v0)
////	0x8c450004, //lw a1,4(v0)
////	0x3c040000, //lui a0,0x0000
////	0x34840a78, //ori a0, a0, 0x0a78
////	0xafa40034, //sw a0, 52(sp)
////	0x3c042520, //lui a0,0x2520
////	0x34847825, //ori a0, a0, 0x7825
////	0xafa40030, //sw a0,48(sp)
////	0x3c043a69, //lui a0,0x3a69
////	0x34846566, //ori a0, a0, 0x6566
////	0xafa4002c, //sw a0,44(sp)
////	0x3c046f61, //lui a0,0x6f61
////	0x34846978, //ori a0, a0, 0x6978
////	0xafa40028, //sw a0, 40(sp)
////	0x27a40028, //addiu a0, sp, 40
////	0xafbf0024, //sw ra, 36(sp)
////	//0x0c05e6d8, //jal <diag_printf>?
////	0x0c08230e, //jal <diag_printf>?
////	0x00001021, //move v0,zero
////	0x8fbf0024, //lw ra, 36(sp)
////	0x00001021, //move v0,zero
////	0x03e00008, //jr ra
////	0x27bd0040, //addiu sp, sp, 64
////};
////#endif
////#if 0//EN25Q.bin
////int hexcode[] = {
////	//---------------------------------------------------------------------
////	//0x27bdffc0,        //addiu   sp,sp,-64
////	0x27bdff80,        //addiu   sp,sp,-128
////	//---------------------------------------------------------------------
////	0x3c021d09,        //lui     v0,0x1d09
////	0x3c03ca13,        //lui     v1,0xca13
////	0x34423b6d,        //ori     v0,v0,0x3b6d
////	0x3463f6e5,        //ori     v1,v1,0xf6e5//v1v0 = constant8B_
////	0xafa20018,        //sw      v0,24(sp)
////	0xafa3001c,        //sw      v1,28(sp)//24-28(sp) = constant8B_
////
////	//---------------------------------------------------------------------
////	//0x3c021234,        //lui     v0,0x1234
////	//0x3c031234,        //lui     v1,0x1234
////	//0x34425678,        //ori     v0,v0,0x5678
////	//0x34635678,        //ori     v1,v1,0x5678//v1v0 = var8B_
////	0x01221021,        //addu    v0,t1,v0
////	0x01231821,        //addu    v1,t1,v1//v1v0 = var8B_
////	0xafa20020,        //sw      v0,32(sp)
////	0xafa30024,        //sw      v1,36(sp)//32-36(sp) = var8B_
////	//---------------------------------------------------------------------
////
////	//---------------------------------------------------------------------
////	//0x3c022222,        //lui     v0,0x2222
////	//0x3c032222,        //lui     v1,0x2222
////	//0x34422222,        //ori     v0,v0,0x2222
////	//0x34632222,        //ori     v1,v1,0x2222//v1v0 = output_
////	0x00001021,        //move    v0,zero
////	0x00001821,        //move    v1,zero//v1v0 = output_
////	//---------------------------------------------------------------------
////
////	//---------------------------------------------------------------------
////	//0x8fae0054,        //lw      t6,84(sp)//t6 = pabKeyValue
////	0x8fae0094,        //lw      t6,148(sp)//t6 = pabKeyValue
////	//---------------------------------------------------------------------
////
////	0x27af0028,        //addiu   t7,sp,40//t7 = &temp_
////	0xafa20030,        //sw      v0,48(sp)
////	0xafa30034,        //sw      v1,52(sp)//48-52(sp) = output_
////
////	//---------------------------------------------------------------------
////	//0x3c026169,        //lui     v0,0x6169
////	//0x3c036965,        //lui     v1,0x6965
////	//0x34427878,        //ori     v0,v0,0x7878
////	//0x3463666f,        //ori     v1,v1,0x666f//v1v0 = *pabKeyValue
////	//0xafa20058,        //sw      v0,88(sp)
////	//0xafa3005c,        //sw      v1,92(sp)//88-92(sp) = *pabKeyValue
////	//0x27ae0058,        //addiu   t6,sp,88//t6 = &pabKeyValue
////	//---------------------------------------------------------------------
////
////	0x00001021,        //move    v0,zero
////	0x00001821,        //move    v1,zero//v1v0 = 0
////	0xafa20028,        //sw      v0,40(sp)
////	0xafa3002c,        //sw      v1,44(sp)//40-44(sp) = temp_ --------0
////	0xafbf003c,        //sw      ra,60(sp)//60(sp) = ra
////	0x27a70020,        //addiu   a3,sp,32//a3 = &var8B_
////	0x27b80030,        //addiu   t8,sp,48//t8 = &output_
////	0x01e01821,        //move    v1,t7//v1 = &temp_
////	0x27a60018,        //addiu   a2,sp,24//a2 = &constant8B_
////	0x01e02021,        //move    a0,t7//a0 = &temp_
////	0x00002821,        //move    a1,zero//a1 = 0
////	0x00001021,        //move    v0,zero//v0 = 0
////	0x240d0008,        //li      t5,8
////	0x00e24021,        //addu    t0,a3,v0//loop1
////	0x30a90007,        //andi    t1,a1,0x7
////	0x90cc0007,        //lbu     t4,7(a2)
////	0x91080000,        //lbu     t0,0(t0)
////	0x00e94821,        //addu    t1,a3,t1
////	0x912b0000,        //lbu     t3,0(t1)
////	0x908a0000,        //lbu     t2,0(a0)
////	0x01c24821,        //addu    t1,t6,v0
////	0x91290000,        //lbu     t1,0(t1)
////	0x01884021,        //addu    t0,t4,t0
////	0x010b4021,        //addu    t0,t0,t3
////	0x010a4026,        //xor     t0,t0,t2
////	0x01094026,        //xor     t0,t0,t1
////	0x24420001,        //addiu   v0,v0,1
////	0xa0880000,        //sb      t0,0(a0)
////	0x24a50003,        //addiu   a1,a1,3
////	0x24840001,        //addiu   a0,a0,1
////	0x144dffee,        //bne     v0,t5,-18 <loop1>
////	0x24c6ffff,        //addiu   a2,a2,-1
////	0x91e50007,        //lbu     a1,7(t7)
////	0x91e40000,        //lbu     a0,0(t7)
////	0x03001021,        //move    v0,t8
////	0x00052942,        //srl     a1,a1,0x5
////	0x000420c0,        //sll     a0,a0,0x3
////	0x00a42021,        //addu    a0,a1,a0
////	0xa3040000,        //sb      a0,0(t8)
////	0x25e60007,        //addiu   a2,t7,7
////	0x90650000,        //lbu     a1,0(v1)//loop2
////	0x90640001,        //lbu     a0,1(v1)
////	0x24630001,        //addiu   v1,v1,1
////	0x00052942,        //srl     a1,a1,0x5
////	0x000420c0,        //sll     a0,a0,0x3
////	0x00a42021,        //addu    a0,a1,a0
////	0xa0440001,        //sb      a0,1(v0)
////	0x1466fff8,        //bne     v1,a2,-8 <loop2>
////	0x24420001,        //addiu   v0,v0,1
////	0x8fa20020,        //lw      v0,32(sp)
////	0x8fa30024,        //lw      v1,36(sp)
////	0x8fa60030,        //lw      a2,48(sp)
////	0x8fa70034,        //lw      a3,52(sp)
////
////	//---------------------------------------------------------------------//fffe%016llx%016llx
////	//0x3c048130,        //lui     a0,0x8130
////	//0x2484627c,        //addiu   a0,a0,25212
////	0x3c046566,        //lui a0,0x6566
////	0x34846666,        //ori a0, a0, 0x6666
////	0xafa40040,        //sw a0,64(sp)
////	0x3c043631,        //lui a0,0x3631
////	0x34843025,        //ori a0, a0, 0x3025
////	0xafa40044,        //sw a0,68(sp)
////	0x3c042578,        //lui a0,0x2578
////	0x34846c6c,        //ori a0, a0, 0x6c6c
////	0xafa40048,        //sw a0,72(sp)
////	0x3c046c36,        //lui a0,0x6c36
////	0x34843130,        //ori a0, a0, 0x3130
////	0xafa4004c,        //sw a0,76(sp)
////	0x3c040000,        //lui a0,0x0000
////	0x3484786c,        //ori a0, a0, 0x786c
////	0xafa40050,        //sw a0,80(sp)
////	0x27a40040,        //addiu a0, sp, 64
////	//---------------------------------------------------------------------
////
////	0xafa20010,        //sw      v0,16(sp)
////	0x0c08230e,        //jal     <diag_printf>
////	0xafa30014,        //sw      v1,20(sp)
////
////	//---------------------------------------------------------------------
////	//0x8fa20020,        //lw      v0,32(sp)
////	//0x8fa30024,        //lw      v1,36(sp)
////	//0x8dc60000,        //lw      a2,0(t6)
////	//0x8dc70004,        //lw      a3,4(t6)
////	//0x27a40040,        //addiu   a0, sp, 64
////	//0xafa20010,        //sw      v0,16(sp)
////	//0x0c03c5e7,        //jal     <diag_printf>
////	//0xafa30014,        //sw      v1,20(sp)
////	//---------------------------------------------------------------------
////
////	0x8fbf003c,        //lw      ra,60(sp)
////	0x00001021,        //move    v0,zero
////	0x03e00008,        //jr      ra
////	//---------------------------------------------------------------------
////	//0x27bd0040,        //addiu   sp,sp,64
////	0x27bd0080,        //addiu   sp,sp,128
////	//---------------------------------------------------------------------
////};
////#endif

int hexcode[] = {
	//---------------------------------------------------------------------
	//0x27bdffc0,        //addiu   sp,sp,-64
	0x27bdff80,        //addiu   sp,sp,-128
	//---------------------------------------------------------------------
	0x3c021d09,        //lui     v0,0x1d09
	0x3c03ca13,        //lui     v1,0xca13
	0x34423b6d,        //ori     v0,v0,0x3b6d
	0x3463f6e5,        //ori     v1,v1,0xf6e5//v1v0 = constant8B_
	0xafa20018,        //sw      v0,24(sp)
	0xafa3001c,        //sw      v1,28(sp)//24-28(sp) = constant8B_

	//---------------------------------------------------------------------
	//0x3c021234,        //lui     v0,0x1234
	//0x3c031234,        //lui     v1,0x1234
	//0x34425678,        //ori     v0,v0,0x5678
	//0x34635678,        //ori     v1,v1,0x5678//v1v0 = var8B_
	0x01221021,        //addu    v0,t1,v0
	0x01231821,        //addu    v1,t1,v1//v1v0 = var8B_
	0xafa20020,        //sw      v0,32(sp)
	0xafa30024,        //sw      v1,36(sp)//32-36(sp) = var8B_
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//0x3c022222,        //lui     v0,0x2222
	//0x3c032222,        //lui     v1,0x2222
	//0x34422222,        //ori     v0,v0,0x2222
	//0x34632222,        //ori     v1,v1,0x2222//v1v0 = output_
	0x00001021,        //move    v0,zero
	0x00001821,        //move    v1,zero//v1v0 = output_
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//0x8fae0054,        //lw      t6,84(sp)//t6 = pabKeyValue
	0x8fae0094,        //lw      t6,148(sp)//t6 = pabKeyValue
	//---------------------------------------------------------------------

	0x27af0028,        //addiu   t7,sp,40//t7 = &temp_
	0xafa20030,        //sw      v0,48(sp)
	0xafa30034,        //sw      v1,52(sp)//48-52(sp) = output_

	//---------------------------------------------------------------------
	//0x3c026169,        //lui     v0,0x6169
	//0x3c036965,        //lui     v1,0x6965
	//0x34427878,        //ori     v0,v0,0x7878
	//0x3463666f,        //ori     v1,v1,0x666f//v1v0 = *pabKeyValue
	//0xafa20058,        //sw      v0,88(sp)
	//0xafa3005c,        //sw      v1,92(sp)//88-92(sp) = *pabKeyValue
	//0x27ae0058,        //addiu   t6,sp,88//t6 = &pabKeyValue
	//---------------------------------------------------------------------

	0x00001021,        //move    v0,zero
	0x00001821,        //move    v1,zero//v1v0 = 0
	0xafa20028,        //sw      v0,40(sp)
	0xafa3002c,        //sw      v1,44(sp)//40-44(sp) = temp_ --------0
	0xafbf003c,        //sw      ra,60(sp)//60(sp) = ra
	0x27a70020,        //addiu   a3,sp,32//a3 = &var8B_
	0x27b80030,        //addiu   t8,sp,48//t8 = &output_
	0x01e01821,        //move    v1,t7//v1 = &temp_
	0x27a60018,        //addiu   a2,sp,24//a2 = &constant8B_
	0x01e02021,        //move    a0,t7//a0 = &temp_
	0x00002821,        //move    a1,zero//a1 = 0
	0x00001021,        //move    v0,zero//v0 = 0
	0x240d0008,        //li      t5,8
	0x00e24021,        //addu    t0,a3,v0//loop1
	0x30a90007,        //andi    t1,a1,0x7
	0x90cc0007,        //lbu     t4,7(a2)
	0x91080000,        //lbu     t0,0(t0)
	0x00e94821,        //addu    t1,a3,t1
	0x912b0000,        //lbu     t3,0(t1)
	0x908a0000,        //lbu     t2,0(a0)
	0x01c24821,        //addu    t1,t6,v0
	0x91290000,        //lbu     t1,0(t1)
	0x01884021,        //addu    t0,t4,t0
	0x010b4021,        //addu    t0,t0,t3
	0x010a4026,        //xor     t0,t0,t2
	0x01094026,        //xor     t0,t0,t1
	0x24420001,        //addiu   v0,v0,1
	0xa0880000,        //sb      t0,0(a0)
	0x24a50003,        //addiu   a1,a1,3
	0x24840001,        //addiu   a0,a0,1
	0x144dffee,        //bne     v0,t5,-18 <loop1>
	0x24c6ffff,        //addiu   a2,a2,-1
	0x91e50007,        //lbu     a1,7(t7)
	0x91e40000,        //lbu     a0,0(t7)
	0x03001021,        //move    v0,t8
	0x00052942,        //srl     a1,a1,0x5
	0x000420c0,        //sll     a0,a0,0x3
	0x00a42021,        //addu    a0,a1,a0
	0xa3040000,        //sb      a0,0(t8)
	0x25e60007,        //addiu   a2,t7,7
	0x90650000,        //lbu     a1,0(v1)//loop2
	0x90640001,        //lbu     a0,1(v1)
	0x24630001,        //addiu   v1,v1,1
	0x00052942,        //srl     a1,a1,0x5
	0x000420c0,        //sll     a0,a0,0x3
	0x00a42021,        //addu    a0,a1,a0
	0xa0440001,        //sb      a0,1(v0)
	0x1466fff8,        //bne     v1,a2,-8 <loop2>
	0x24420001,        //addiu   v0,v0,1
	0x8fa20020,        //lw      v0,32(sp)
	0x8fa30024,        //lw      v1,36(sp)
	0x8fa60030,        //lw      a2,48(sp)
	0x8fa70034,        //lw      a3,52(sp)

	//---------------------------------------------------------------------//fffe%016llx%016llx
	//0x3c048130,        //lui     a0,0x8130
	//0x2484627c,        //addiu   a0,a0,25212
	0x3c046566,        //lui a0,0x6566
	0x34846666,        //ori a0, a0, 0x6666
	0xafa40040,        //sw a0,64(sp)
	0x3c043631,        //lui a0,0x3631
	0x34843025,        //ori a0, a0, 0x3025
	0xafa40044,        //sw a0,68(sp)
	0x3c042578,        //lui a0,0x2578
	0x34846c6c,        //ori a0, a0, 0x6c6c
	0xafa40048,        //sw a0,72(sp)
	0x3c046c36,        //lui a0,0x6c36
	0x34843130,        //ori a0, a0, 0x3130
	0xafa4004c,        //sw a0,76(sp)
	0x3c040000,        //lui a0,0x0000
	0x3484786c,        //ori a0, a0, 0x786c
	0xafa40050,        //sw a0,80(sp)
	0x27a40040,        //addiu a0, sp, 64
	//---------------------------------------------------------------------

	0xafa20010,        //sw      v0,16(sp)
	0x0c03dedb,        //jal     <diag_printf>
	0xafa30014,        //sw      v1,20(sp)

	//---------------------------------------------------------------------
	//0x8fa20020,        //lw      v0,32(sp)
	//0x8fa30024,        //lw      v1,36(sp)
	//0x8dc60000,        //lw      a2,0(t6)
	//0x8dc70004,        //lw      a3,4(t6)
	//0x27a40040,        //addiu   a0, sp, 64
	//0xafa20010,        //sw      v0,16(sp)
	//0x0c03c5e7,        //jal     <diag_printf>
	//0xafa30014,        //sw      v1,20(sp)
	//---------------------------------------------------------------------

	0x8fbf003c,        //lw      ra,60(sp)
	0x00001021,        //move    v0,zero
	0x03e00008,        //jr      ra
	//---------------------------------------------------------------------
	//0x27bd0040,        //addiu   sp,sp,64
	0x27bd0080,        //addiu   sp,sp,128
	//---------------------------------------------------------------------
};

static void myread(FILE *binfile, char *buffer, int offset, int size) {
	if(fseek(binfile, offset, SEEK_SET) != 0) {
		printf("error: %m\n");
		exit(-1);
	}

	int toread = size;
	while(toread > 0) {
		int read = fread(buffer, sizeof(unsigned char), toread, binfile);
		if(read <= 0) {
			printf("error: %m\n");
			exit(-1);
		}
		toread -= read;
		buffer += read;
	}

}

static void mywrite(FILE *binfile, char *buffer, int offset, int size) {
	if(fseek(binfile, offset, SEEK_SET) != 0) {
		printf("error: %m\n");
		exit(-1);
	}

	int towrite = size;
	while(towrite > 0) {
		int count;
		count = fwrite(buffer , sizeof(char), towrite, binfile);
		if(count == 0) {
			printf("error: %m\n");
			exit(-1);
		}

		towrite -= count;
		buffer += count;
	}
}

void encrypt(){
	unsigned long long constant8B_ = 0xca13f6e51d093b6dLL;
	unsigned long long var8B_ = 0x1234567812345678LL;
	unsigned long long temp_ = 0x0000000000000000LL;
	unsigned long long output_ = 0x2222222222222222LL;

	unsigned char *constant8B = (unsigned char *)((void *)&constant8B_);
	unsigned char *var8B = (unsigned char *)((void *)&var8B_);
	unsigned char *temp = (unsigned char *)((void *)&temp_);
	unsigned char *output = (unsigned char *)((void *)&output_);
	int i;

	unsigned char pabKeyValue[] = {0x78, 0x78, 0x69, 0x61, 0x6f, 0x66, 0x65, 0x69};
	for(i=0;i<8;i++){
		temp[i]^=(var8B[3*i%8]+var8B[i]+constant8B[7-i]);
		//printf("temp1[%d]:[%02x]\n",i,temp[i]);
		temp[i]^=pabKeyValue[i];
		//printf("temp2[%d]:[%02x]\n",i,temp[i]);
	}
	//整体循环左移3位
	output[0]=(temp[0]<<3)+(temp[7]>>5);
	for(i=1;i<8;i++){
		output[i]=(temp[i]<<3)+(temp[i-1]>>5);
	}

	printf("fffe%016llx%016llx\n", *((unsigned long long *)output), *((unsigned long long *)var8B));
}

int main(int argc, char **argv) {
	//encrypt();
	printf("argc:%d\n", argc);
	if(argc != 4) {
		printf("error: para error!\n");
		return -1;
	}


	char *pend;
	char *filename = argv[1];
	int start = strtol(argv[2], &pend, 16);
	int size = strtol(argv[3], &pend, 16);
	printf("filename:%s, start:%08x, size:%d\n", filename, start, size);

	int codesize = sizeof(hexcode);

	if(size != 0) {
		if(codesize > size) {
			printf("error: no space for code!");
			return -1;
		}
	}

	FILE * binfile = fopen((const char *)filename, "rb+");
	if(binfile == NULL) {
		printf("error: %m\n");
		return -1;
	}

	mywrite(binfile, (char *)hexcode, start, codesize);

	printf("insert (%08x)bytes ok!\n", codesize);

	fclose(binfile);

	return 0;
}

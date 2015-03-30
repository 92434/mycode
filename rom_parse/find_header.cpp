#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define FIRSTBOOT_SIZE 	  32768
#define FLAG_LEN (6)
#define BOOTLOADER_SIZE_UNIT	(4*1024)
#define BOOTLOADER_CODESIZE  (332 * 1024)
unsigned int FIRSTBOOT_SIZE = 32768;
int filesize = 0;

/* reserve memory space to store the address of module table */

typedef struct module_header_s {
	unsigned int module_table_crc;
	unsigned int version;
	unsigned int attr;
	unsigned int next_table_offset;
} module_header_t;

typedef struct module_info_s {
	unsigned char	*lma;
	unsigned int	lma_size;
	unsigned char	*vma;
	unsigned int	vma_size;
	unsigned int	attr;
	unsigned int	scramble_key;
	unsigned int	crc32;
} module_info_t;

enum MODULE_IDX {
	MOD_ECOS		= 0,
	MOD_M_KERNEL	= 1,
	MOD_FILESYSTEM	= 2,
	MOD_M_DRAM_B	= 3,
	MOD_PLATFORM	= 4,
	MOD_DRIVERS		= 5,
	MOD_MIDDLEWARES	= 6,
	MOD_GUI_ENG		= 7,
	MOD_APP_INFRA	= 8,
	MOD_APPLICATION = 9,
	MOD_UNICODE		= 10,
	MOD_GAME		= 11,
	MOD_ROMVER		= 12,
	MOD_NVM			= 13,
	MOD_END
};


/**
 *	\brief rtc_head
 */
typedef struct  __attribute__ ((packed)) {
	unsigned short year;
	unsigned char  mon;
	unsigned char  date;
	unsigned char  hour;
	unsigned char  min;
	unsigned char  sec;
}rtc_head;

typedef struct __attribute__ ((packed)) {
	unsigned char  flag[FLAG_LEN];
	unsigned short bootloader_size;
	unsigned long  crc32;
	unsigned long  file_len;
	rtc_head main_rtc;
	unsigned char  OUI[3];
	unsigned short sw_model;
	unsigned short sw_version;
	unsigned short hw_model;
	unsigned short hw_version;
	unsigned int	uiDefaultDBStartAddr;
	unsigned short uiDefaultDBSizeK;
	unsigned short	uiDefaultDBVersion;
	unsigned int	uiLogoStartAddr;
	unsigned short uiLogoSizeK;
	unsigned int	uiUiStartAddr;
	unsigned short uiUISizeK;
	unsigned int	uiFontStartAddr;
	unsigned short	uiFontSizeK;
	unsigned int	uiBrowserStartAddr;
	unsigned short uiBrowserSizeK;
	unsigned int	uiBackupStartAddr;
	unsigned int  uiBackupSize;
	unsigned int  uiUserDataAllSize;
	unsigned int  uiHeaderRealSize;
	unsigned char ucDemuxSet;//cflu add nim setting in main code header 20110222
	unsigned char ucDemux0TunerType;
	unsigned char ucDemux0DemodType;
	unsigned char ucDemux1TunerType;
	unsigned char ucDemux1DemodType;
	unsigned char ucEraseDbSysSetting;
	unsigned int uiUserDbStartAddr;
	unsigned short usUserDbSize_K;
	unsigned int uiSysSetStartAddr;
	unsigned short usSysSetSize_K;
	unsigned char  reserve[152];
	unsigned int  ui32HeaderCrc;
} MAIN_HEAD_t;


typedef struct __attribute__ ((packed)) {
	unsigned char AreaID_hi   : 8;
	unsigned char AreaID_lo   : 8;
	unsigned char SW_ver_hi   : 8;
	unsigned char SW_ver_lo   : 8;
	unsigned char  DDR_type   : 4;
	unsigned char  Flash_type : 4;
	unsigned char  Tuner_type : 4;
	unsigned char  CPU_type	  : 2;
	unsigned char  Audio_type : 2;
	unsigned char  NetIC_type : 2;
	unsigned char  Front_panel_type : 2;
	unsigned char  CA_type    : 4;
	unsigned char  Factory_ID : 4;
	unsigned char  Reserved   : 4;
}DVB_HWINFO_t;

typedef struct __attribute__ ((packed)) {
	char	head[6];
	short main_ver;
	short sub_ver;
	short app_ver;
	unsigned short bootloader_type;
	unsigned long  crc32;
	unsigned long  file_len;
	rtc_head	   loader_rtc;
	unsigned char  OUI[3];
	unsigned short sw_model;
	unsigned short hw_model;
	unsigned short hw_version;
	unsigned char  DemuxNo;
	unsigned char  DemuxNo0_Tuner;
	unsigned char  DemuxNo0_Demod;
	unsigned char  DemuxNo1_Tuner;
	unsigned char  DemuxNo1_Demod;
	DVB_HWINFO_t   stHwinfo;
	unsigned int  uiMainFreqStartAddr; //cflu move it from main code header.20101228
	unsigned char  ucMainFreqSize_K;
	unsigned int  uiOtaInfoStartAddr;
	unsigned char  ucOtaInfoSize_K;
	unsigned int uiBackupMainCodeHeaderStartAddr;
	unsigned short usBackupMainCodeHeaderSize_K;
	unsigned int uiPrivateStartAddr;
	unsigned short usPrivateSize_K;
	unsigned char ucBootloaderDouble;
	unsigned short usLoaderSwVer;
	unsigned int  uiNetSettingStartAddr;//fanzhang add 2011/05/04
	unsigned char  ucNetSettingSize_K;//fanzhang add 2011/05/04
	unsigned char  reserve[47];

} LOADER_HEAD_t;


static const unsigned long crc_table[256] = {
	0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 
	0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 
	0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7, 
	0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75, 
	0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 
	0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 
	0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef, 
	0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d, 
	0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 
	0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 
	0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0, 
	0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072, 
	0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 
	0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 
	0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08, 
	0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba, 
	0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 
	0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 
	0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050, 
	0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2, 
	0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 
	0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 
	0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1, 
	0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53, 
	0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 
	0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 
	0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9, 
	0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b, 
	0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 
	0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 
	0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71, 
	0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3, 
	0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 
	0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 
	0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e, 
	0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec, 
	0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 
	0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 
	0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676, 
	0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4, 
	0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 
	0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 
	0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

unsigned long crc32(char *data, int len)
{
	register int i;
	unsigned int crc = 0xffffffff;
	for (i = 0; i < len; i++)
		crc = (crc << 8) ^ crc_table[((crc >> 24) ^ *data++) & 0xff];
	return crc;

}

static const unsigned short crc16_table[256]={
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

unsigned short CRC_16(const unsigned char * aData, unsigned long aSize )
{
	unsigned long  i;
	unsigned short nAccum = 0;
	for ( i = 0; i < aSize; i++ )
		nAccum = ( nAccum << 8 ) ^ ( unsigned short )crc16_table[( nAccum >> 8 ) ^ *aData++];
	return nAccum;
}


static unsigned char check_time(unsigned short u16Year, unsigned char u8Month, unsigned char u8Day)
{

	if(u16Year > 2006 && u16Year < 2050)
	{
	}
	else if(u16Year == 2006 && u8Month >= 1 && u8Day >= 1)
	{
	}
	else if(u16Year == 2050 && u8Month == 1 && u8Day == 1)
	{
	}else
	{
		printf("day error  : i16Year = %x u8Month = %x u8Day = %x \n",u16Year,u8Month,u8Day);
		return 0;
	}
	return 1;
}

char *get_char_buffer(int size) {
	char *buffer = (char *)malloc(size);
	if(buffer) {
		memset(buffer, 0, size);
	}

	return buffer;
}

static int myread(FILE *binfile, char *buffer, int offset, int size) {
	if(filesize < offset) {
		return -1;
	}

	if(fseek(binfile, offset, SEEK_SET) != 0) {
		//printf("error: %m\n");
		return -1;
	}

	int toread = size;
	while(toread > 0) {
		int read = fread(buffer, sizeof(unsigned char), toread, binfile);
		if(read <= 0) {
			//printf("error: %m\n");
			fclose(binfile);
			return -1;
		}
		toread -= read;
		buffer += read;
	}

	return 0;
}

static int mywrite(FILE *binfile, char *buffer, int offset, int size) {
	if(filesize < offset) {
		return -1;
	}

	if(fseek(binfile, offset, SEEK_SET) != 0) {
		printf("error: %m\n");
		return -1;
	}

	int towrite = size;
	while(towrite > 0) {
		int count;
		count = fwrite(buffer , sizeof(char), towrite, binfile);
		if(count == 0) {
			printf("error: %m\n");
			fclose(binfile);
			exit(-1);
		}

		towrite -= count;
		buffer += count;
	}
}

static int getModuleTable(FILE *binfile, int m_table_idx, unsigned int off, module_header_t *m_table, size_t size) {
	unsigned int LastModTableOffset = off;

	if(myread(binfile, (char *)m_table, off, size) != 0) {
		return -1;
	}

	/* find the last module, we assume lb86 is the last one */
	while(m_table->next_table_offset != 0) {
		/* store module table offset in order to get the offset when read the data of module info */
		LastModTableOffset = m_table->next_table_offset;
		if(myread(binfile, (char *)m_table, LastModTableOffset, size) != 0) {
			return -1;
		}
	}

	return LastModTableOffset;
}

static int getModuleInfo(FILE *binfile, int index, unsigned int off, module_info_t *m_info, size_t size) {
	if(myread(binfile, (char *)m_info, off + size * index, size) != 0) {
		return -1;
	}
	return 0;
}

int PrintModuleTable(FILE *binfile) {
	module_header_t mod_header;
	int LastModTableOffset = getModuleTable(binfile, 0, (unsigned int)FIRSTBOOT_SIZE, &mod_header, sizeof(module_header_t));
	if(LastModTableOffset == -1) {
		return -1;
	}

	/* Dump contents of module table header */
	printf("module table header = 0x%08x\n", mod_header.attr);

	/* Dump contents of moudle table */
	printf("%5s %10s %10s %10s %10s %10s %10s %10s\n",
			"Index", "LMA", "LMA_SIZE", "VMA", "VMA_SIZE", "Attribute", "Key", "CRC32");

	module_info_t mod_info;
	for (int i = 0; i < MOD_END; ++i) {
		if(getModuleInfo(binfile, i, (unsigned int)(LastModTableOffset + sizeof(module_header_t)), &mod_info, sizeof(mod_info)) != 0) {
			return -1;
		}
		printf("%5d 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
				i,
				(unsigned int)mod_info.lma,
				mod_info.lma_size,
				(unsigned int)mod_info.vma,
				mod_info.vma_size,
				mod_info.attr,
				mod_info.scramble_key,
				mod_info.crc32);
	}
	printf("-----------------------------------------------------\n");

	return 0;
}


int find_moudle_info(FILE *binfile) {

	module_header_t mod_header;
	int LastModTableOffset = getModuleTable(binfile, 0, (unsigned int)FIRSTBOOT_SIZE, &mod_header, sizeof(module_header_t));
	if(LastModTableOffset == -1) {
		return -1;
	}

	module_info_t mod_info;
	for (int i = 0; i < MOD_END; ++i) {
		if(getModuleInfo(binfile, i, (unsigned int)(LastModTableOffset + sizeof(module_header_t)), &mod_info, sizeof(mod_info)) != 0) {
			return -1;
		}

		if(((unsigned int)mod_info.vma & 0xffff0000) == (0x80010000 & 0xffff0000)) {
			printf("FIRSTBOOT_SIZE is 0x%08x\n", FIRSTBOOT_SIZE);
			printf("%5d 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
					i,
					(unsigned int)mod_info.lma,
					mod_info.lma_size,
					(unsigned int)mod_info.vma,
					mod_info.vma_size,
					mod_info.attr,
					mod_info.scramble_key,
					mod_info.crc32);
			return 0;
		}
	}
	//printf("-----------------------------------------------------\n");

	return -1;
}


int GetHeaderInfo(FILE *binfile, bool b_fix, int fix_modindex, int fix_modsize) {
	MAIN_HEAD_t stMainHeadInfo;
	module_header_t mod_header;
	module_info_t mod_info;

	int LastModTableOffset = getModuleTable(binfile, 0, (unsigned int)FIRSTBOOT_SIZE, &mod_header, sizeof(module_header_t));
	if(LastModTableOffset == -1) {
		return -1;
	}

	if(b_fix) {
		if(getModuleInfo(binfile, fix_modindex, (unsigned int)(LastModTableOffset + sizeof(module_header_t)), &mod_info, sizeof(mod_info)) != 0) {
			return -1;
		}
		mod_info.lma_size = fix_modsize;
		int fix_mod_offset = LastModTableOffset + sizeof(module_header_t) + sizeof(mod_info) * fix_modindex;
		if(mywrite(binfile, (char *)&mod_info, fix_mod_offset, sizeof(mod_info)) != 0) {
			return -1;
		}
	}

	if(getModuleInfo(binfile, MOD_NVM, (unsigned int)(LastModTableOffset + sizeof(module_header_t)), &mod_info, sizeof(mod_info)) != 0) {
		return -1;
	}

	int nvm_mod_offset = (unsigned int)mod_info.lma;
	printf("sizeof(MAIN_HEAD_t):=0x%x, nvm_mod_offset:=0x%x \n",sizeof(MAIN_HEAD_t), nvm_mod_offset);

	if(myread(binfile, (char *)&stMainHeadInfo, nvm_mod_offset, sizeof(MAIN_HEAD_t)) != 0) {
		return -1;
	}
	if(!((stMainHeadInfo.flag[0] == 'S')
				&&(stMainHeadInfo.flag[1] == 'P')
				&&(stMainHeadInfo.flag[2] == 'H')
				&&(stMainHeadInfo.flag[3] == 'E')
				&&(stMainHeadInfo.flag[4] == '1')
				&&(stMainHeadInfo.flag[5] == '5')
	    )) {
		return -1;
	}
	printf("\n stMainHeadInfo.sw_version:=0x%x  \n",stMainHeadInfo.sw_version);
	printf("\n stMainHeadInfo.sw_model:=0x%x  \n",stMainHeadInfo.sw_model);
	printf("\n stMainHeadInfo.OUI[0]:=0x%x  \n",stMainHeadInfo.OUI[0]);
	printf("\n stMainHeadInfo.OUI[1]:=0x%x  \n",stMainHeadInfo.OUI[1]);
	printf("\n stMainHeadInfo.OUI[2]:=0x%x  \n",stMainHeadInfo.OUI[2]);
	printf("\n stMainHeadInfo.file_len:=0x%x  \n",(unsigned int)stMainHeadInfo.file_len);
	printf("\n stMainHeadInfo.crc32:=0x%x  \n",(unsigned int)stMainHeadInfo.crc32);
	printf("\n stMainHeadInfo.flag[0]:=%c  \n",stMainHeadInfo.flag[0]);
	printf("\n stMainHeadInfo.flag[1]:=%c  \n",stMainHeadInfo.flag[1]);
	printf("\n stMainHeadInfo.flag[2]:=%c  \n",stMainHeadInfo.flag[2]);
	printf("\n stMainHeadInfo.flag[3]:=%c  \n",stMainHeadInfo.flag[3]);
	printf("\n stMainHeadInfo.flag[4]:=%c  \n",stMainHeadInfo.flag[4]);
	printf("\n stMainHeadInfo.flag[5]:=%c  \n",stMainHeadInfo.flag[5]);
	printf("\n stMainHeadInfo.bootloader_size:=0x%x  \n",stMainHeadInfo.bootloader_size);
	printf("\n stMainHeadInfo.uiDefaultDBStartAddr:=%x  \n",stMainHeadInfo.uiDefaultDBStartAddr);
	printf("\n stMainHeadInfo.uiDefaultDBSizeK:=0x%x  \n",stMainHeadInfo.uiDefaultDBSizeK);
	printf("\n bootloader size:=0X%X,BOOTLOADER_CODESIZE/1024:=%d  \n",stMainHeadInfo.bootloader_size*BOOTLOADER_SIZE_UNIT, BOOTLOADER_CODESIZE/1024);

	if((stMainHeadInfo.flag[0] == 'S')
			&&(stMainHeadInfo.flag[1] == 'P')
			&&(stMainHeadInfo.flag[2] == 'H')
			&&(stMainHeadInfo.flag[3] == 'E')
			&&(stMainHeadInfo.flag[4] == '1')
			&&(stMainHeadInfo.flag[5] == '5')
	  ) {

		//main code time
		printf("\n stMainHeadInfo.main_rtc.year:=0x%x  \n",(unsigned int)stMainHeadInfo.main_rtc.year);
		printf("\n stMainHeadInfo.main_rtc.mon:=0x%x  \n",(unsigned int)stMainHeadInfo.main_rtc.mon);
		printf("\n stMainHeadInfo.main_rtc.date:=0x%x  \n",(unsigned int)stMainHeadInfo.main_rtc.date);
		if(check_time(stMainHeadInfo.main_rtc.year, stMainHeadInfo.main_rtc.mon,stMainHeadInfo.main_rtc.date) <= 0)
			return -1;

		// main code crc//xiaofei
		char *buffer = get_char_buffer(stMainHeadInfo.file_len);
		if(stMainHeadInfo.bootloader_size==0){
			if(myread(binfile, buffer, LastModTableOffset, stMainHeadInfo.file_len) != 0) {
				return -1;
			}
			unsigned int crcval = crc32(buffer, stMainHeadInfo.file_len);
			printf("crcval:%08x\n", crcval);
			printf("stMainHeadInfo.crc32:%08x\n", (unsigned int)stMainHeadInfo.crc32);
			if(stMainHeadInfo.crc32 != crcval && !b_fix) {
				return -1;
			} else {
				MAIN_HEAD_t fix_stMainHeadInfo = stMainHeadInfo;
				fix_stMainHeadInfo.crc32 = crcval;
				if(mywrite(binfile, (char *)&fix_stMainHeadInfo, nvm_mod_offset, sizeof(MAIN_HEAD_t)) != 0) {
					return -1;
				}
			}
		}
		else
		{
			if(myread(binfile, buffer, LastModTableOffset, stMainHeadInfo.file_len) != 0) {
				return -1;
			}
			unsigned int crcval = crc32(buffer, stMainHeadInfo.file_len);
			printf("crcval:%08x\n", crcval);
			printf("stMainHeadInfo.crc32:%08x\n", (unsigned int)stMainHeadInfo.crc32);
			if(stMainHeadInfo.crc32 != crcval && !b_fix) {
				return -1;
			} else {
				stMainHeadInfo.crc32 = crcval;
				if(mywrite(binfile, (char *)&stMainHeadInfo, nvm_mod_offset, sizeof(MAIN_HEAD_t)) != 0) {
					return -1;
				}
			}
		}
		free(buffer);

		unsigned int crcval = crc32((char *)&stMainHeadInfo, sizeof(MAIN_HEAD_t) - sizeof(stMainHeadInfo.ui32HeaderCrc));
		printf("crcval:%08x\n", crcval);
		printf("stMainHeadInfo.ui32HeaderCrc:%08x\n", stMainHeadInfo.ui32HeaderCrc);
		if(stMainHeadInfo.ui32HeaderCrc != crcval && !b_fix) {
			return -1;
		} else {
			stMainHeadInfo.ui32HeaderCrc = crcval;
			if(mywrite(binfile, (char *)&stMainHeadInfo, nvm_mod_offset, sizeof(MAIN_HEAD_t)) != 0) {
				return -1;
			}
		}

		if(BOOTLOADER_CODESIZE != 0)//fanzhang 2011/01/24 add for mantis bug 123241
		{
			if(stMainHeadInfo.bootloader_size * BOOTLOADER_SIZE_UNIT != BOOTLOADER_CODESIZE)
				return -1;
		}
		//loader
		if(stMainHeadInfo.bootloader_size != 0)
		{
			int loader_header_addr = FIRSTBOOT_SIZE + sizeof(module_header_t);
			while(true) {
				if(myread(binfile, (char *)&mod_info, loader_header_addr, sizeof(module_info_t)) != 0) {
					return -1;
				}
				if(((mod_info.attr >> 31) & 0x1) == 0)
				{
					break;
				}
				loader_header_addr += sizeof(module_info_t);
			}

			if(((unsigned int)mod_info.lma +  mod_info.lma_size) % 4)
				loader_header_addr =(unsigned int)mod_info.lma +  mod_info.lma_size + 4 - ((unsigned int)mod_info.lma + mod_info.lma_size) % 4;
			else
				loader_header_addr =(unsigned int)mod_info.lma +  mod_info.lma_size;

			printf(" loader_header_addr:=0x%x \n",loader_header_addr);

			LOADER_HEAD_t loader_header_info;

			if(myread(binfile, (char *)&loader_header_info, loader_header_addr, sizeof(LOADER_HEAD_t)) != 0) {
				return -1;
			}
			printf("\n loader_header_info.loader_rtc.year:=0x%x  \n",(unsigned int)loader_header_info.loader_rtc.year);
			printf("\n loader_header_info.loader_rtc.mon:=0x%x  \n",(unsigned int)loader_header_info.loader_rtc.mon);
			printf("\n loader_header_info.loader_rtc.date:=0x%x  \n",(unsigned int)loader_header_info.loader_rtc.date);
			printf("\n loader_header_info.bootloader_type:=0x%x  \n",(unsigned int)loader_header_info.bootloader_type);
			if(check_time(loader_header_info.loader_rtc.year,loader_header_info.loader_rtc.mon,loader_header_info.loader_rtc.date) <= 0)
				return -1;

			char *buffer = get_char_buffer(loader_header_info.file_len);
			if(myread(binfile, (char *)buffer, FIRSTBOOT_SIZE, loader_header_info.file_len) != 0) {
				return -1;
			}
			unsigned int crcval = crc32(buffer, loader_header_info.file_len);
			printf("crcval:%08x\n", crcval);
			printf("loader_header_info.crc32:%08x\n", (unsigned int)loader_header_info.crc32);
			if(crcval != loader_header_info.crc32 && !b_fix) {
				return -1;
			} else {
				LOADER_HEAD_t fix_loader_header_info = loader_header_info;
				fix_loader_header_info.crc32 = crcval;
				if(mywrite(binfile, (char *)&fix_loader_header_info, loader_header_addr, sizeof(LOADER_HEAD_t)) != 0) {
					return -1;
				}
			}
			free(buffer);
			if((loader_header_info.sw_model != stMainHeadInfo.sw_model)
					||(loader_header_info.OUI[0] != stMainHeadInfo.OUI[0])
					||(loader_header_info.OUI[1] != stMainHeadInfo.OUI[1])
					||(loader_header_info.OUI[2] != stMainHeadInfo.OUI[2])
			  ) {
				return -1;
			}
		}

		return 0;
	}
}


int main(int argc, char **argv) {
	//printf("argc:%d\n", argc);
	if(argc != 5) {
		printf("error: para error!\n");
		return -1;
	}

	char *pend;
	char *infilename = argv[1];
	int modindex = strtol(argv[2], &pend, 16);
	int modsize = strtol(argv[3], &pend, 16);
	bool b_fix = strcmp(argv[4], "fix") == 0;

	//printf("infilename:%s modindex:%d modsize:%08x bi_fix:%s FIRSTBOOT_SIZE:%08x\n", infilename, modindex, modsize, b_fix ? "true" : "false", FIRSTBOOT_SIZE);

	FILE * binfile = fopen((const char *)infilename, "rb+");
	if(binfile == NULL) {
		printf("error: %m\n");
		return -1;
	}

	if(fseek(binfile, 0, SEEK_END) != 0) {
		//printf("error: %m\n");
		return -1;
	}

	filesize = ftell(binfile);
	if(filesize == -1) {
		return -1;
	}

	for(int i = 0;i < 332 * 1024;i += 4) {
		FIRSTBOOT_SIZE = i;

		if(find_moudle_info(binfile) == 0) {
		}
	}

	fclose(binfile);

	return 0;
}

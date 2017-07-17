

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *   
 *   文件名称：FpSensorLib.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时09分03秒
 *   修改日期：2017年07月17日 星期一 17时14分54秒
 *   描    述：
 *
 *================================================================*/
#ifndef _FPSENSORLIB_H
#define _FPSENSORLIB_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef char __ft_s8;
typedef short __ft_s16;
typedef int __ft_s32;
typedef float __ft_float;
typedef unsigned char __ft_u8;
typedef unsigned short __ft_u16;
typedef unsigned int __ft_u32;
typedef unsigned long long __ft_u64;

typedef int (*save_bmp_t)(char *label, char *buffer, int len);

//typedef __ft_s32 (*FtFpSpiWriteFunc)(__ft_u8 *, __ft_u32);
//typedef __ft_s32 (*FtFpSpiReadFunc)(__ft_u8 *, __ft_u8 *, __ft_u32);
typedef void (*FtFpUsleepFunc)(__ft_u32);
typedef __ft_s32 (*FtGetMcuStatusFunc)(void);
typedef __ft_u64 (*FtGetSystemTimeFunc)(void);
typedef void (*FtSetLogFunc)(const char *fmt, ...);

//void focal_InitFuncSpiWrite(FtFpSpiWriteFunc func);//xiaofei
//void focal_InitFuncSpiRead(FtFpSpiReadFunc func);//xiaofei
void focal_InitFuncUsleep(FtFpUsleepFunc func);//xiaofei
void focal_InitFuncGetMcuStatus(FtGetMcuStatusFunc func);//xiaofei
void focal_InitFuncGetSystemTime(FtGetSystemTimeFunc func);//xiaofei
void focal_InitFuncLog(FtSetLogFunc func);


int ft_lib_init();
int ft_lib_uninit();
int ft_lib_set_save_bmp(save_bmp_t save_bmp);
int ft_get_image(char *buffer, int len);
int ft_set_image(const char *buffer, int len);
#ifdef __cplusplus
}
#endif
#endif //FPSENSORLIB_H



/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *
 *   文件名称：FpSensorLib.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时09分03秒
 *   修改日期：2017年08月10日 星期四 18时34分46秒
 *   描    述：
 *
 *================================================================*/
#ifndef _FPSENSORLIB_H
#define _FPSENSORLIB_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef enum ft_far_e {
	FT_FAR_1 = 0,
	FT_FAR_2 = 1,
	FT_FAR_5 = 2,
	FT_FAR_10 = 3,
	FT_FAR_20 = 4,
	FT_FAR_50 = 5,
	FT_FAR_100 = 6,
	FT_FAR_200 = 7,
	FT_FAR_500 = 8,
	FT_FAR_1000 = 9,
	FT_FAR_2K = 10,
	FT_FAR_5000 = 11,
	FT_FAR_10000 = 12,
	FT_FAR_20K = 13,
	FT_FAR_50000 = 14,
	FT_FAR_100000 = 15,
	FT_FAR_200K = 16,
	FT_FAR_500000 = 17,
	FT_FAR_1000000 = 18,
	FT_FAR_2M = 19,
	FT_FAR_5M = 20,
	FT_FAR_10M = 21,
	FT_FAR_20M = 22,
	FT_FAR_50M = 23,
	FT_FAR_100M = 24,
	FT_FAR_200M = 25,
	FT_FAR_500M = 26,
	FT_FAR_1000M = 27,
	FT_FAR_Inf = 28
} ft_far_t;

#define FOCAL_PER_TEMPLATE_SIZE (400 * 1024 - 8)//(32768*32 - 8)

typedef char __ft_s8;
typedef short __ft_s16;
typedef int __ft_s32;
typedef float __ft_float;
typedef unsigned char __ft_u8;
typedef unsigned short __ft_u16;
typedef unsigned int __ft_u32;
typedef unsigned long long __ft_u64;

typedef int (*save_bmp_t)(char *label, const char *buffer, int len);

typedef __ft_s32 (*FtFpSpiWriteFunc)(__ft_u8 *, __ft_u32);
typedef __ft_s32 (*FtFpSpiReadFunc)(__ft_u8 *, __ft_u8 *, __ft_u32);
typedef void (*FtFpUsleepFunc)(__ft_u32);
typedef __ft_s32 (*FtGetMcuStatusFunc)(void);
typedef __ft_u64 (*FtGetSystemTimeFunc)(void);
typedef void (*FtSetLogFunc)(const char *fmt, ...);

void focal_InitFuncSpiWrite(FtFpSpiWriteFunc func);//xiaofei
void focal_InitFuncSpiRead(FtFpSpiReadFunc func);//xiaofei
void focal_InitFuncUsleep(FtFpUsleepFunc func);//xiaofei
void focal_InitFuncGetMcuStatus(FtGetMcuStatusFunc func);//xiaofei
void focal_InitFuncGetSystemTime(FtGetSystemTimeFunc func);//xiaofei
void focal_InitFuncLog(FtSetLogFunc func);


int ft_lib_init();
int ft_lib_uninit();
int ft_lib_set_save_bmp(save_bmp_t save_bmp);
int ft_get_image(char *buffer, int len);
int ft_set_image(const char *buffer, int len);
void focal_debuginfo_switch(__ft_u8 debug);
void focal_GetLibVersion(__ft_u8 *pstrBuf);
void focal_SetFpSensorColsRows(__ft_u8 cols, __ft_u8 rows);
void focal_SetEnrollMaxTpl(__ft_s32 maxtpl);
void focal_SetAlgoMode(__ft_u8 ucMode);
void focal_spa_en(__ft_u8 ucEnable);
void focal_spa_ratio(__ft_u8 uc_ratio);
void focal_SetFpAlgMaxTpl(__ft_s32 maxtpl);
__ft_s32 focal_InitFpAlg(void);
void focal_SetAlgFarLevel(ft_far_t far_level);
void focal_SetUpdateTemplateFarLevel(ft_far_t far_level);
void focal_SetUpdateTemplateThreshold(__ft_u16 usThreshold);
void focal_verify_quickly_en(__ft_u8 ucEnable);
void focal_updateTemplate_Outside_en(__ft_u8 ucenable);
void focal_SetImageQualityScore(__ft_u8 imageQuality);
void focal_SetImageVerifyQualityScore(__ft_u8 imageVerifyQuality);
void focal_enrollment_duplicatecheck_en(__ft_u8 ucCheckEn);
__ft_s32 focal_SetImageValidAreaScale(__ft_float validAreaScale);
void focal_SetEnrollmentTipsEn(__ft_u8 ucenable);
void focal_SetEnrollmentTipsParameter(__ft_u32 usScores, __ft_u8 ucDeltaxyThr, __ft_u8 ucDeltaAngThr);
void focal_SetImprovedFrrEn(__ft_u8 ucFlg);
void focal_SetImprovedLevel(__ft_u8 ucFlg);
__ft_s32 focal_SetImageBit(__ft_u8 bit);
__ft_s32 focal_SetMcuInterrupMode(__ft_u8 ucMode);
__ft_u8 focal_setMcuStateCheckMode(__ft_u8 mode);
void focal_SetImageRepeatEn(__ft_u8 ucMode);
void focal_SetTemplateBuffEn(__ft_u8 ucFlg);
void focal_SetTransferBytesMax(__ft_u32 usTransferBytesMax);
void focal_config_debuginfo_switch(__ft_u8 debug);
void focal_use_dyn_far(__ft_u32 use_dyn_far);
__ft_s32 focal_Enroll(__ft_u16 fingerId, __ft_u8 enroll_index, __ft_u8 *penroll_coverage);
__ft_s32 focal_Identify(__ft_u8 *pfingerId, __ft_u8 *pupdateTpl);
__ft_s32 focal_UpdateTemplate(__ft_s32 update, __ft_s32 *finger_id);
__ft_s32 focal_DelFinger(__ft_u8 fingerId);
__ft_s32 focal_SaveAlgTplData(__ft_u16 fingerId, __ft_u8 *tpl_type, __ft_u32 *tpl_size, __ft_u8 * tpl_data);
__ft_s32 focal_SaveAlgTplData(__ft_u16 fingerId, __ft_u8 *tpl_type, __ft_u32 *tpl_size, __ft_u8 * tpl_data);

#ifdef __cplusplus
}
#endif
#endif //FPSENSORLIB_H

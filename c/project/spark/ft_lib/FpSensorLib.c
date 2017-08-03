

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *
 *   文件名称：FpSensorLib.c
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时08分57秒
 *   修改日期：2017年08月03日 星期四 10时25分13秒
 *   描    述：
 *
 *================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "FpSensorLib.h"
#define BUFFER_LEN 4096

typedef struct _ft_device {
	char *buffer;
	int size;
	FtSetLogFunc ft_printf;
	save_bmp_t save_bmp;

	FtFpUsleepFunc usleep;
	FtGetMcuStatusFunc get_mcu_status;
	FtGetSystemTimeFunc get_system_time;
} ft_device_t;

static ft_device_t *ft_device;

int ft_lib_init()
{
	int ret = 0;

	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	ft_device = (ft_device_t *)malloc(sizeof(ft_device_t));

	if(ft_device != NULL) {
		bzero(ft_device, sizeof(ft_device_t));
	} else {
		ret = -1;
	}

	return ret;
}

int ft_lib_uninit()
{
	int ret = 0;

	if(ft_device == NULL) {
		return ret;
	}

	if(ft_device->buffer != NULL) {
		free(ft_device->buffer);
		ft_device->buffer = NULL;
	}

	free(ft_device);
	ft_device = NULL;

	return ret;
}

static int ft_printf(char *fmt, ...)
{
	int ret = 0;
	va_list ap;
	char buffer[BUFFER_LEN];

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if(ft_device->ft_printf == NULL) {
		ret = -1;
		return ret;
	}

	va_start(ap, fmt);
	ret = vsnprintf(buffer, BUFFER_LEN, fmt, ap);
	va_end(ap);

	ft_device->ft_printf(buffer);
	return ret;
}

int ft_lib_set_save_bmp(save_bmp_t save_bmp)
{
	int ret = 0;

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	ft_device->save_bmp = save_bmp;
	return ret;
}

int save_bmp(char *label, const char *buffer, int len)
{
	int ret = 0;

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if(ft_device->save_bmp == NULL) {
		ret = -1;
		return ret;
	}

	ret = ft_device->save_bmp(label, buffer, len);
	return ret;
}

int ft_set_image(const char *buffer, int len)
{
	int ret = 0;

	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		ret = -1;
		return ret;
	}

	if(ft_device->size < len) {
		if(ft_device->buffer != NULL) {
			free(ft_device->buffer);

			ft_device->size = 0;
			ft_device->buffer = NULL;
		}

		ft_device->buffer = (char *)malloc(len);

		if(ft_device->buffer != NULL) {
			ft_device->size = len;
		}
	}


	if(ft_device->buffer != NULL) {
		memcpy((void *)ft_device->buffer, (void *)buffer, len);
		save_bmp("spa26", buffer, len);

	} else {
		ret = -1;
	}

	return ret;
}

void focal_InitFuncGetSystemTime(FtGetSystemTimeFunc func)
{
	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		return;
	}

	ft_device->get_system_time = func;
}

void focal_InitFuncUsleep(FtFpUsleepFunc func)
{
	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		return;
	}

	ft_device->usleep = func;
}

void focal_InitFuncGetMcuStatus(FtGetMcuStatusFunc func)
{
	ft_printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	if(ft_device == NULL) {
		return;
	}

	ft_device->get_mcu_status = func;
}

void focal_InitFuncLog(FtSetLogFunc func)
{
	if(ft_device == NULL) {
		return;
	}

	ft_device->ft_printf = func;
}


void focal_debuginfo_switch(__ft_u8 debug)
{
}

void focal_GetLibVersion(__ft_u8 *pstrBuf)
{
	snprintf(pstrBuf, 100, "test lib version!!!");
}

void focal_SetFpSensorColsRows(__ft_u8 cols, __ft_u8 rows)
{
}

void focal_SetEnrollMaxTpl(__ft_s32 maxtpl)
{
}

void focal_SetAlgoMode(__ft_u8 ucMode)
{
}

void focal_spa_en(__ft_u8 ucEnable)
{
}


void focal_SetFpAlgMaxTpl(__ft_s32 maxtpl)
{
}

__ft_s32 focal_InitFpAlg(void)
{
	__ft_s32 ret = 0;
	return ret;
}

void focal_SetAlgFarLevel(ft_far_t far_level)
{
}

void focal_SetUpdateTemplateFarLevel(ft_far_t far_level)
{
}

void focal_SetUpdateTemplateThreshold(__ft_u16 usThreshold)
{
}

void focal_verify_quickly_en(__ft_u8 ucEnable)
{
}

void focal_updateTemplate_Outside_en(__ft_u8 ucenable)
{
}

void focal_SetImageQualityScore(__ft_u8 imageQuality)
{
}

void focal_SetImageVerifyQualityScore(__ft_u8 imageVerifyQuality)
{
}

void focal_enrollment_duplicatecheck_en(__ft_u8 ucCheckEn)
{
}

__ft_s32 focal_SetImageValidAreaScale(__ft_float validAreaScale)
{
}

void focal_SetEnrollmentTipsEn(__ft_u8 ucenable)
{
}

void focal_SetEnrollmentTipsParameter(__ft_u32 usScores, __ft_u8 ucDeltaxyThr, __ft_u8 ucDeltaAngThr)
{
}

void focal_SetImprovedFrrEn(__ft_u8 ucFlg)
{
}

void focal_SetImprovedLevel(__ft_u8 ucFlg)
{
}

__ft_s32 focal_SetImageBit(__ft_u8 bit)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 focal_SetMcuInterrupMode(__ft_u8 ucMode)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_u8 focal_setMcuStateCheckMode(__ft_u8 mode)
{
	__ft_u8 ret = 0;
	return ret;
}

void focal_SetImageRepeatEn(__ft_u8 ucMode)
{
}

void focal_SetTemplateBuffEn(__ft_u8 ucFlg)
{
}

void focal_SetTransferBytesMax(__ft_u32 usTransferBytesMax)
{
}

void focal_config_debuginfo_switch(__ft_u8 debug)
{
}

void focal_use_dyn_far(__ft_u32 use_dyn_far)
{
}

__ft_s32 focal_Enroll(__ft_u16 fingerId, __ft_u8 enroll_index, __ft_u8 *penroll_coverage)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 focal_Identify(__ft_u8 *pfingerId, __ft_u8 *pupdateTpl)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 focal_UpdateTemplate(__ft_s32 update, __ft_s32 *finger_id)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 focal_DelFinger(__ft_u8 fingerId)
{
	__ft_s32 ret = 0;
	return ret;
}

void focal_InitFuncSpiWrite(FtFpSpiWriteFunc func)
{
}

void focal_InitFuncSpiRead(FtFpSpiReadFunc func)
{
}


__ft_s32 focal_SaveAlgTplData(__ft_u16 fingerId, __ft_u8 *tpl_type, __ft_u32 *tpl_size, __ft_u8 *tpl_data)
{
	__ft_s32 ret = 0;
	return ret;
}

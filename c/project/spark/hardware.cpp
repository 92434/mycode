

/*================================================================
 *   Copyright (C) 2017年07月17日 肖飞 All rights reserved
 *   
 *   文件名称：hardware.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月17日 星期一 12时27分14秒
 *   修改日期：2017年07月17日 星期一 17时08分41秒
 *   描    述：
 *
 *================================================================*/
#include "hardware.h"

std::ofstream hardware_base::ofs_hardware;
std::string hardware_base::cur_bmp_path;

hardware_base::hardware_base()
{
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware_base::hardware_base(int width, int height)
{
	this->width = width;
	this->height = height;
	printf("%s:%s:%d:this->width:%d, this->height:%d\n", __FILE__, __func__, __LINE__, this->width, this->height);
}

hardware_base::~hardware_base()
{
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

__ft_u64 hardware_base::get_system_time(void)
{
	__ft_u64 ret = 0;

	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	return ret;
}

int hardware_base::hardware_init()
{
	int ret = 0;

	unsigned char ucVersion[100] = {0};

	//focal_InitFuncSpiWrite(qseeSpiWrite);
	//focal_InitFuncSpiRead(qseeSpiRead);
	//focal_InitFuncUsleep(platform_sleepus);
	//focal_InitFuncGetMcuStatus(platform_get_mcu_status);
	focal_InitFuncGetSystemTime((FtGetSystemTimeFunc)&hardware_base::get_system_time);
	focal_InitFuncLog((FtSetLogFunc)&hardware_base::hardware_log);

	//focal_debuginfo_switch(1);

	//focal_GetLibVersion(ucVersion);
	//QSEE_LOG(QSEE_LOG_MSG_ERROR, );
	//VERSION_DBG("Lib = %s", ucVersion);
	//memset(ucVersion, 0x00, sizeof(ucVersion));
	//focal_GetFwVersion(ucVersion, sizeof(ucVersion));
	//VERSION_DBG("Fw = %s", ucVersion);
	//VERSION_DBG("TA = %s,            compile date:%s,time:%s", TA_VERSION, __DATE__, __TIME__);
	//QSEE_LOG(QSEE_LOG_MSG_ERROR, );
	//QSEE_LOG(QSEE_LOG_MSG_ERROR, );

	//focal_SetFpSensorColsRows(g_fingerdata.fp_sensor_width, g_fingerdata.fp_sensor_height);//96 96
	//focal_SetEnrollMaxTpl(g_fingerdata.enroll_cnt); // TCL 14

#if ALOG_MODE
	//focal_SetAlgoMode(43);  //43
#else
	//focal_SetAlgoMode(18);  //
	//focal_spa_en(1);
#endif

	//focal_SetFpAlgMaxTpl(32);//32 48

	//err = focal_InitFpAlg();

	//if (err) {
	//	QSEE_LOG(QSEE_LOG_MSG_ERROR, "%s line:%d, focal focal_InitFpAlg ERROR return :%d. ", __FUNCTION__, __LINE__, err);
	//	return err;
	//}

	//focal_SetAlgFarLevel(FT_FAR_100K);//100K 50K

	//focal_SetUpdateTemplateFarLevel(FT_FAR_500K);//500 100K
	//focal_SetUpdateTemplateThreshold(0);//0 0

	//focal_verify_quickly_en(1);//

#ifdef UPDATEOUTSIDE
	//focal_updateTemplate_Outside_en(1);//
#else
	//focal_updateTemplate_Outside_en(0);
#endif

	//focal_SetImageQualityScore(10);// 30 10
	//focal_SetImageVerifyQualityScore(20);//30 20

	//focal_enrollment_duplicatecheck_en(0);// 相同手指检测
	//focal_SetImageValidAreaScale(0.6);//
	//focal_SetEnrollmentTipsEn(1);// 重复区域使能
	//focal_SetEnrollmentTipsParameter(0, 20, 45);// 0 30 45   10000 30 45
	//focal_SetImprovedFrrEn(1);// 1 1

	//focal_SetImprovedLevel(4);// 5 5

	//err = focal_SetImageBit(FT_IMG_SET_8BIT);// 1 1

	//if(0 != err) {
	//	QSEE_LOG(QSEE_LOG_MSG_ERROR, "%s line:%d, focal_SetImageBit error : %d", __FUNCTION__, __LINE__, err);
	//}

	//----用于消除竖条纹问题--------
	//err = ft9348_info_write(0x68, 0x01);

	//if(0 != err) {
	//	QSEE_LOG(QSEE_LOG_MSG_ERROR, "ft9348_info_write error,err = %d", err);
	//}

	//focal_SetMcuInterrupMode(1);// 1 1 get bmp time

	//focal_setMcuStateCheckMode(1);// 1 --

	//focal_SetImageRepeatEn(2);//
	//focal_setMcuStateCheckMode(1);

	//----focal_SetTemplateBuffEn(1);//错误模板缓存

	//focal_SetTransferBytesMax(10 * 1024);

#if (SAVE_CONFIG_DEBUG == 1)
	//focal_config_debuginfo_switch(1);//打开debug信息
#else
	//focal_config_debuginfo_switch(0);
#endif

	return ret;
}

int hardware_base::save_bmp(char *label, char *buffer, int len)
{
	int ret = 0;
	int pos = cur_bmp_path.rfind('.');
	char *buffer_bmp = new char [len + 1];
	std::string filename;

	if(pos != std::string::npos) {
		filename = cur_bmp_path.substr(0, pos);
	} else {
		ret = -1;
		return ret;
	}

	filename = filename + "." + label + ".bmp";
	memcpy(buffer_bmp, buffer, len);
	buffer_bmp[len] = 0;
	//printf("new filename:%s, buffer:%s, len:%d\n", filename.c_str(), buffer_bmp, len);

	return ret;
}

int hardware_base::set_log_file_name(std::string filename) {
	int ret = 0;

	logfile_hardware = filename;

	return ret;
}

int hardware_base::log_file_start()
{
	int ret = 0;
	char buffer[1024];
	int len = 0;
	filesystem fs;

	if(logfile_hardware.size() == 0) {
		ret = -1;
		return ret;
	}

	fs.mkdirs(logfile_hardware);

	ofs_hardware.open(logfile_hardware.c_str());

	if(!ofs_hardware.good()) {
		printf("open:%s failed!!! (%s)!\n", logfile_hardware.c_str(), strerror(errno));
		ret = -1;
		return ret;
	}

	return ret;
}

void hardware_base::hardware_log(const char *fmt, ...)
{
	int len = 0;
	char buffer[1024];
	va_list ap;

	va_start(ap, fmt);
	len = vsnprintf(buffer, 1023, fmt, ap);
	buffer[len] = 0;
	va_end(ap);
	ofs_hardware.write(buffer, len);
}

int hardware_base::log_file_end()
{
	int ret = 0;

	ofs_hardware.close();

	return ret;
}

int hardware_base::set_image(std::string bmp_path)
{
	int ret = 0;

	cur_bmp_path = bmp_path;
	ft_set_image(cur_bmp_path.c_str(), (int)cur_bmp_path.size());
	return ret;
}

int hardware_base::get_image(char *buffer, int len)
{
	int ret = len;
	ft_get_image(buffer, len);
	return ret;
}

int hardware_base::set_save_bmp()
{
	int ret = 0;
	ret = ft_lib_set_save_bmp((save_bmp_t)&hardware_base::save_bmp);
	return ret;
}

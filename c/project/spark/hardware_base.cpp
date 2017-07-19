

/*================================================================
 *   Copyright (C) 2017年07月19日 肖飞 All rights reserved
 *
 *   文件名称：hardware_base.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月19日 星期三 09时09分48秒
 *   修改日期：2017年07月19日 星期三 19时40分31秒
 *   描    述：
 *
 *================================================================*/
#include "hardware_base.h"

std::ofstream hardware_base::ofs_hardware;
std::string hardware_base::cur_bmp_path;

BMP_FILEHDR hardware_base::bmp_file_header;
BMP_INFOHDR hardware_base::bmp_info_header;
BGR_PALETTE hardware_base::bmp_palette[256];

hardware_base::hardware_base()
{
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware_base::hardware_base(int width, int height)
{
	this->width = width;
	this->height = height;
	//printf("%s:%s:%d:this->width:%d, this->height:%d\n", __FILE__, __func__, __LINE__, this->width, this->height);
}

hardware_base::~hardware_base()
{
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}


__ft_s32 hardware_base::spi_write(__ft_u8 *buffer, __ft_u32 len)
{
	__ft_s32 ret = 0;
	return ret;
}

__ft_s32 hardware_base::spi_read(__ft_u8 *buffer_out, __ft_u8 *buffer_in, __ft_u32 len)
{
	__ft_s32 ret = 0;
	return ret;
}

void hardware_base::hw_usleep(__ft_u32 usec)
{
	usleep(usec);
}

__ft_s32 hardware_base::get_mcu_status(void)
{
	__ft_s32 ret = 1;
	return ret;
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

double hw_strtod(std::string number)
{
	double ret = 0;
	char *invalid_pos;

	if(number.size() == 0) {
		printf("parameter not set!!!\n");
	}

	ret = strtod(number.c_str(), &invalid_pos);

	return ret;
}

int hardware_base::hardware_init()
{
	int ret = 0;

	unsigned char ucVersion[100] = {0};
	settings *g_settings = settings::get_instance();

	focal_InitFuncSpiWrite((FtFpSpiWriteFunc)&hardware_base::spi_write);
	focal_InitFuncSpiRead((FtFpSpiReadFunc)&hardware_base::spi_read);
	focal_InitFuncUsleep((FtFpUsleepFunc)&hardware_base::hw_usleep);
	focal_InitFuncGetMcuStatus((FtGetMcuStatusFunc)&hardware_base::get_mcu_status);
	focal_InitFuncGetSystemTime((FtGetSystemTimeFunc)&hardware_base::get_system_time);
	focal_InitFuncLog((FtSetLogFunc)&hardware_base::hardware_log);

	focal_debuginfo_switch((int)hw_strtod(g_settings->debug_switch));//1

	focal_GetLibVersion(ucVersion);
	hardware_log("Lib = %s\n", ucVersion);
	memset(ucVersion, 0x00, sizeof(ucVersion));

	focal_SetFpSensorColsRows(width, height);//96 96
	focal_SetEnrollMaxTpl((int)hw_strtod(g_settings->enroll_max_templates)); // TCL 14

	focal_SetAlgoMode((int)hw_strtod(g_settings->algorithm_mode));  //43
	focal_spa_en((int)hw_strtod(g_settings->spa_enable));//1

	focal_SetFpAlgMaxTpl((int)hw_strtod(g_settings->algorithm_max_templates));//32 48

	ret = focal_InitFpAlg();

	if (ret != 0) {
		hardware_log("%s line:%d, focal focal_InitFpAlg ERROR return :%d.\n", __FUNCTION__, __LINE__, ret);
		return ret;
	}

	focal_SetAlgFarLevel((ft_far_t)hw_strtod(g_settings->algorithm_far_level));//100K 50K

	focal_SetUpdateTemplateFarLevel((ft_far_t)hw_strtod(g_settings->update_template_far_level));//500 100K

	focal_SetUpdateTemplateThreshold((int)hw_strtod(g_settings->update_template_threshold));//0 0

	focal_verify_quickly_en((int)hw_strtod(g_settings->verify_quickly_enable));//

	focal_updateTemplate_Outside_en((int)hw_strtod(g_settings->update_template_outside_enable));//

	focal_SetImageQualityScore((int)hw_strtod(g_settings->image_quality_score));// 30 10
	focal_SetImageVerifyQualityScore((int)hw_strtod(g_settings->verify_image_quality_score));//30 20

	focal_enrollment_duplicatecheck_en((int)hw_strtod(g_settings->enroll_duplicate_area_check_enable));// 相同手指检测
	focal_SetImageValidAreaScale(hw_strtod(g_settings->valid_area_scale));//
	focal_SetEnrollmentTipsEn((int)hw_strtod(g_settings->enrollment_tips_enable));// 重复区域使能
	focal_SetEnrollmentTipsParameter((int)hw_strtod(g_settings->enrollment_tips_parameter1),
									 (int)hw_strtod(g_settings->enrollment_tips_parameter2),
									 (int)hw_strtod(g_settings->enrollment_tips_parameter3));// 0 30 45   10000 30 45
	focal_SetImprovedFrrEn((int)hw_strtod(g_settings->verify_improve_enable));// 1 1

	focal_SetImprovedLevel((int)hw_strtod(g_settings->verify_improve_level));// 5 5

	ret = focal_SetImageBit((int)hw_strtod(g_settings->mcu_image_bit));// 1 1

	if(0 != ret) {
		hardware_log("%s line:%d, focal_SetImageBit error : %d", __FUNCTION__, __LINE__, ret);
	}

	//----用于消除竖条纹问题--------
	//ret = ft9348_info_write(0x68, 0x01);

	//if(0 != ret) {
	//	QSEE_LOG(QSEE_LOG_MSG_ERROR, "ft9348_info_write error,ret = %d", ret);
	//}

	focal_SetMcuInterrupMode((int)hw_strtod(g_settings->mcu_interrupt_mode));// 1 1 get bmp time

	focal_setMcuStateCheckMode((int)hw_strtod(g_settings->mcu_state_check_mode));// 1 --

	focal_SetImageRepeatEn((int)hw_strtod(g_settings->repeat_get_image_count));//

	focal_SetTemplateBuffEn((int)hw_strtod(g_settings->template_buffer_enable));//错误模板缓存

	focal_SetTransferBytesMax((int)hw_strtod(g_settings->transfer_bytes_max));

	focal_config_debuginfo_switch((int)hw_strtod(g_settings->config_debuginfo_switch));//打开debug信息

	return ret;
}

int hardware_base::save_bmp(char *label, char *buffer, int len)
{
	int ret = 0;
	int pos = cur_bmp_path.rfind('.');
	char *buffer_bmp = new char [len + 1];
	std::string filename;
	std::ofstream ofs;
	int offset = 0;

	if(pos != std::string::npos) {
		filename = cur_bmp_path.substr(0, pos);
	} else {
		ret = -1;
		return ret;
	}

	filename = filename + "." + label + ".bmp";

	ofs.open(filename.c_str());

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&bmp_file_header, sizeof(BMP_FILEHDR));

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset += sizeof(BMP_FILEHDR);

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&bmp_info_header, sizeof(BMP_INFOHDR));

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset += sizeof(BMP_INFOHDR);

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&bmp_palette[0], sizeof(BGR_PALETTE) * 256);

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset += sizeof(BGR_PALETTE) * 256;

	ofs.seekp(offset, ofs.beg);
	ofs.write(buffer, len);

exit:
	ofs.close();

	return ret;
}

int hardware_base::set_log_file_name(std::string filename)
{
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
	ofs_hardware.write("\n", 1);
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
	std::ifstream ifs;
	int offset = 0;
	char *buffer = NULL;
	int len;
	int i;

	cur_bmp_path = bmp_path;

	ifs.open(cur_bmp_path.c_str());

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	ifs.seekg(offset, ifs.beg);
	ifs.read((char *)&bmp_file_header, sizeof(BMP_FILEHDR));

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset += sizeof(BMP_FILEHDR);

	ifs.seekg(offset, ifs.beg);
	ifs.read((char *)&bmp_info_header, sizeof(BMP_INFOHDR));

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset += sizeof(BMP_INFOHDR);
	ifs.read((char *)&bmp_palette[0], sizeof(BGR_PALETTE) * 256);

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	offset = bmp_file_header.bfOffBits;
	len = bmp_info_header.biWidth * bmp_info_header.biHeight * bmp_info_header.biBitCount / 8;
	buffer = new char[len];

	ifs.seekg(offset, ifs.beg);
	ifs.read(buffer, len);

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit;
	}

	//printf("len:%d, width * height:%d\n", len, bmp_info_header.biWidth * bmp_info_header.biHeight);

	ft_set_image(buffer, len);

exit:

	if(buffer != NULL) {
		delete buffer;
	}

	ifs.close();

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

int hardware_base::enroll(unsigned short finger_id, unsigned char enroll_index, unsigned char *penroll_coverage)
{
	int ret = 0;
	settings *g_settings = settings::get_instance();

	if(enroll_index < (unsigned short)hw_strtod(g_settings->enroll_max_templates)) {
		ret = focal_Enroll(finger_id, enroll_index, penroll_coverage);

		if(ret == 0) {
			if(enroll_index == ((unsigned short)hw_strtod(g_settings->enroll_max_templates) - 1)) {
				fp_alg_tpl_t fp_tpl;
				ret = focal_SaveAlgTplData(finger_id, &fp_tpl.tpl_type, &fp_tpl.tpl_size, fp_tpl.tpl_data);
			}
		}
	}

	return ret;
}

int hardware_base::identify(unsigned char *finger_id, unsigned char *update, unsigned char *update_outside, int *update_template_finger_id)
{
	int ret = 0;
	ret = focal_Identify(finger_id, update);

	if(ret == 0) {
		settings *g_settings = settings::get_instance();

		if(((int)hw_strtod(g_settings->update_template_outside_enable) == 1)
		   && (update_outside != NULL)
		   && (update_template_finger_id != NULL)) {
			ret = update_template(1, update_template_finger_id);

			if(ret == 0) {
				*update_outside = 1;
			}
		}

		ret = 0;
	}

	return ret;
}

int hardware_base::update_template(int update, int *finger_id)
{
	int ret = 0;
	ret = focal_UpdateTemplate(update, finger_id);
	return ret;
}

int hardware_base::delete_template(char finger_id)
{
	int ret = 0;
	ret = focal_DelFinger(finger_id);
	return ret;
}

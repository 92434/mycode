

/*================================================================
 *   Copyright (C) 2017年07月20日 肖飞 All rights reserved
 *
 *   文件名称：hardware.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月20日 星期四 17时56分34秒
 *   修改日期：2017年08月13日 星期日 16时45分06秒
 *   描    述：
 *
 *================================================================*/
#include "hardware.h"

hardware *hardware::hw = NULL;
bmp_cache_item *hardware::cache_item = NULL;

std::ofstream hardware::ofs_hardware;

bmp_cache_item::bmp_cache_item()
{
	buffer = NULL;
	len = 0;
}

bmp_cache_item::~bmp_cache_item()
{
	if(buffer != NULL) {
		//printf("%s:%s:%d:delete %s buffer\n", __FILE__, __func__, __LINE__, bmp_path.c_str());
		delete buffer;
		buffer = NULL;
		len = 0;
	}
}

int bmp_cache_item::alloc_bmp_buffer(int len)
{
	int ret = 0;

	this->len = len;

	buffer = new char[this->len];

	if(buffer == NULL) {
		printf("%s:%s:%d:alloc %s while no memory!\n", __FILE__, __func__, __LINE__, bmp_path.c_str());
		ret = -1;
	}

	return ret;
}

int hardware::clear_cache()
{
	int ret = 0;
	std::map<std::string, bmp_cache_item *>::iterator bmp_cache_it;

	for(bmp_cache_it = bmp_cache.begin(); bmp_cache_it != bmp_cache.end(); bmp_cache_it++) {
		//printf("%s:%s:%d:delete %s cache item\n", __FILE__, __func__, __LINE__, bmp_cache_it->first.c_str());
		delete bmp_cache_it->second;
	}

	bmp_cache.clear();
	return ret;
}

hardware::hardware()
{
	clear_cache();
	ft_lib_init();
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware::~hardware()
{
	hw = NULL;

	ft_lib_uninit();
	clear_cache();
	//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

hardware *hardware::get_instance()
{
	if(hw == NULL) {
		hw = new hardware();
	}

	//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, width, height);

	return hw;
}

int hardware::hardware_init()
{
	int ret = 0;

	unsigned char ucVersion[100] = {0};
	settings *g_settings = settings::get_instance();

	focal_InitFuncSpiWrite((FtFpSpiWriteFunc)&hardware_base::spi_write);
	focal_InitFuncSpiRead((FtFpSpiReadFunc)&hardware_base::spi_read);
	focal_InitFuncUsleep((FtFpUsleepFunc)&hardware::hw_usleep);
	focal_InitFuncGetMcuStatus((FtGetMcuStatusFunc)&hardware::get_mcu_status);
	focal_InitFuncGetSystemTime((FtGetSystemTimeFunc)&hardware::get_system_time);
	focal_InitFuncLog((FtSetLogFunc)&hardware::hardware_log);

	focal_debuginfo_switch((int)g_settings->value_strtod(g_settings->debug_switch));//1

	focal_GetLibVersion(ucVersion);
	hardware_log("Lib = %s\n", ucVersion);
	memset(ucVersion, 0x00, sizeof(ucVersion));

	focal_SetFpSensorColsRows(width, height);//96 96
	focal_SetEnrollMaxTpl((int)g_settings->value_strtod(g_settings->enroll_max_templates)); // TCL 14

	focal_SetAlgoMode((int)g_settings->value_strtod(g_settings->algorithm_mode));  //43
	focal_spa_en((int)g_settings->value_strtod(g_settings->spa_enable));//1
	focal_spa_ratio((int)g_settings->value_strtod(g_settings->spa_ratio));

	focal_SetFpAlgMaxTpl((int)g_settings->value_strtod(g_settings->algorithm_max_templates));//32 48

	ret = focal_InitFpAlg();

	if (ret != 0) {
		hardware_log("%s line:%d, focal focal_InitFpAlg ERROR return :%d.\n", __FUNCTION__, __LINE__, ret);
		return ret;
	}

	focal_SetAlgFarLevel((ft_far_t)g_settings->value_strtod(g_settings->algorithm_far_level));//100K 50K

	focal_SetUpdateTemplateFarLevel((ft_far_t)g_settings->value_strtod(g_settings->update_template_far_level));//500 100K

	focal_SetUpdateTemplateThreshold((int)g_settings->value_strtod(g_settings->update_template_threshold));//0 0

	focal_verify_quickly_en((int)g_settings->value_strtod(g_settings->verify_quickly_enable));//

	focal_updateTemplate_Outside_en((int)g_settings->value_strtod(g_settings->update_template_outside_enable));//

	focal_SetImageQualityScore((int)g_settings->value_strtod(g_settings->image_quality_score));// 30 10
	focal_SetImageVerifyQualityScore((int)g_settings->value_strtod(g_settings->verify_image_quality_score));//30 20

	focal_enrollment_duplicatecheck_en((int)g_settings->value_strtod(g_settings->enroll_duplicate_area_check_enable));// 相同手指检测
	focal_SetImageValidAreaScale(g_settings->value_strtod(g_settings->valid_area_scale));//
	focal_SetEnrollmentTipsEn((int)g_settings->value_strtod(g_settings->enrollment_tips_enable));// 重复区域使能
	focal_SetEnrollmentTipsParameter((int)g_settings->value_strtod(g_settings->enrollment_tips_parameter1),
									 (int)g_settings->value_strtod(g_settings->enrollment_tips_parameter2),
									 (int)g_settings->value_strtod(g_settings->enrollment_tips_parameter3));// 0 30 45   10000 30 45
	focal_SetImprovedFrrEn((int)g_settings->value_strtod(g_settings->verify_improve_enable));// 1 1

	focal_SetImprovedLevel((int)g_settings->value_strtod(g_settings->verify_improve_level));// 5 5

	ret = focal_SetImageBit((int)g_settings->value_strtod(g_settings->mcu_image_bit));// 1 1

	if(0 != ret) {
		hardware_log("%s line:%d, focal_SetImageBit error : %d", __FUNCTION__, __LINE__, ret);
	}

	//----用于消除竖条纹问题--------
	//ret = ft9348_info_write(0x68, 0x01);

	//if(0 != ret) {
	//	QSEE_LOG(QSEE_LOG_MSG_ERROR, "ft9348_info_write error,ret = %d", ret);
	//}

	focal_SetMcuInterrupMode((int)g_settings->value_strtod(g_settings->mcu_interrupt_mode));// 1 1 get bmp time

	focal_setMcuStateCheckMode((int)g_settings->value_strtod(g_settings->mcu_state_check_mode));// 1 --

	focal_SetImageRepeatEn((int)g_settings->value_strtod(g_settings->repeat_get_image_count));//

	focal_SetTemplateBuffEn((int)g_settings->value_strtod(g_settings->template_buffer_enable));//错误模板缓存

	focal_SetTransferBytesMax((int)g_settings->value_strtod(g_settings->transfer_bytes_max));

	focal_config_debuginfo_switch((int)g_settings->value_strtod(g_settings->config_debuginfo_switch));//打开debug信息

	focal_use_dyn_far((int)g_settings->value_strtod(g_settings->use_dyn_far));

	return ret;
}

int hardware::save_bmp(char *label, const char *buffer, int len)
{
	int ret = 0;
	int pos;
	std::string filename;
	std::ofstream ofs;
	int offset = 0;

	if(cache_item == NULL) {
		ret = -1;
		return ret;
	}

	pos = cache_item->bmp_path.rfind('.');

	if(pos != std::string::npos) {
		filename = cache_item->bmp_path.substr(0, pos);
	} else {
		ret = -1;
		return ret;
	}

	filename = filename + "." + label + ".bmp";

	ofs.open(filename.c_str());

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&cache_item->bmp_file_header, sizeof(BMP_FILEHDR));

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset += sizeof(BMP_FILEHDR);

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&cache_item->bmp_info_header, sizeof(BMP_INFOHDR));

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset += sizeof(BMP_INFOHDR);

	ofs.seekp(offset, ofs.beg);
	ofs.write((char *)&cache_item->bmp_palette[0], sizeof(BGR_PALETTE) * 256);

	if(!ofs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset += sizeof(BGR_PALETTE) * 256;

	ofs.seekp(offset, ofs.beg);
	ofs.write(buffer, len);

exit_close:
	ofs.close();

	return ret;
}

int hardware::set_log_file_name(std::string filename)
{
	int ret = 0;

	logfile_hardware = filename;

	return ret;
}

int hardware::log_file_start()
{
	int ret = 0;
	char buffer[BUFFER_LEN];
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

void hardware::hardware_log(const char *fmt, ...)
{
	int len = 0;
	char buffer[BUFFER_LEN];
	va_list ap;

	va_start(ap, fmt);
	len = vsnprintf(buffer, BUFFER_LEN, fmt, ap);
	buffer[len] = 0;
	va_end(ap);
	ofs_hardware.write(buffer, len);
	ofs_hardware.write("\n", 1);
}

int hardware::log_file_end()
{
	int ret = 0;

	ofs_hardware.close();

	return ret;
}

int hardware::set_save_bmp()
{
	int ret = 0;
	ret = ft_lib_set_save_bmp((save_bmp_t)&hardware::save_bmp);
	return ret;
}

int hardware::image_v_mirror(char *buffer, int width, int height, int bytes_per_pixel)
{
	int ret = 0;
	int i;
	int j;
	int loops = height / 2;
	int step = width * bytes_per_pixel;
	char *pixel_buffer = new char [bytes_per_pixel];

	if(pixel_buffer == NULL) {
		ret = -1;
		return ret;
	}

	for(i = 0; i < loops; i++) {
		char *a_line = buffer + step * i;
		char *b_line = buffer + step * ((height - 1) - i);
		char *a_pixel;
		char *b_pixel;

		for(j = 0; j < width; j++) {
			a_pixel = a_line + bytes_per_pixel * j;
			b_pixel = b_line + bytes_per_pixel * j;

			memcpy(pixel_buffer, a_pixel, bytes_per_pixel);
			memcpy(a_pixel, b_pixel, bytes_per_pixel);
			memcpy(b_pixel, pixel_buffer, bytes_per_pixel);
		}
	}

	if(pixel_buffer != NULL) {
		delete pixel_buffer;
	}

	return ret;
}

int hardware::image_h_mirror(char *buffer, int width, int height, int bytes_per_pixel)
{
	int ret = 0;
	int i;
	int j;
	int loops = width / 2;
	int step = width * bytes_per_pixel;
	char *pixel_buffer = new char [bytes_per_pixel];

	if(pixel_buffer == NULL) {
		ret = -1;
		return ret;
	}

	for(i = 0; i < height; i++) {
		char *line = buffer + i * step;
		char *a_pixel;
		char *b_pixel;

		for(j = 0; j < loops; j++) {
			a_pixel = line + bytes_per_pixel * j;
			b_pixel = line + bytes_per_pixel * ((width - 1) - j);

			memcpy(pixel_buffer, a_pixel, bytes_per_pixel);
			memcpy(a_pixel, b_pixel, bytes_per_pixel);
			memcpy(b_pixel, pixel_buffer, bytes_per_pixel);
		}
	}

	if(pixel_buffer != NULL) {
		delete pixel_buffer;
	}

	return ret;
}

int hardware::get_image_info(std::string bmp_path)
{
	int ret = 0;
	std::ifstream ifs;
	int offset = 0;
	int len;

	std::map<std::string, bmp_cache_item *>::iterator bmp_cache_it;

	bmp_cache_it = bmp_cache.find(bmp_path);

	if(bmp_cache_it != bmp_cache.end()) {
		cache_item = bmp_cache_it->second;
		goto exit;
	} else {
		cache_item = new bmp_cache_item;

		if(cache_item == NULL) {
			ret = -1;
			goto exit;
		}

		bmp_cache[bmp_path] = cache_item;
	}

	cache_item->bmp_path = bmp_path;

	ifs.open(cache_item->bmp_path.c_str());

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	ifs.seekg(offset, ifs.beg);
	ifs.read((char *)&cache_item->bmp_file_header, sizeof(BMP_FILEHDR));

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset += sizeof(BMP_FILEHDR);

	ifs.seekg(offset, ifs.beg);
	ifs.read((char *)&cache_item->bmp_info_header, sizeof(BMP_INFOHDR));

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset += sizeof(BMP_INFOHDR);
	ifs.read((char *)&cache_item->bmp_palette[0], sizeof(BGR_PALETTE) * 256);

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	offset = cache_item->bmp_file_header.bfOffBits;
	len = cache_item->bmp_info_header.biWidth * cache_item->bmp_info_header.biHeight * cache_item->bmp_info_header.biBitCount / 8;
	ret = cache_item->alloc_bmp_buffer(len);

	if(ret != 0) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	ifs.seekg(offset, ifs.beg);
	ifs.read(cache_item->buffer, cache_item->len);

	if(!ifs.good()) {
		printf("%s:%s:%d:%s", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		ret = -1;
		goto exit_close;
	}

	//printf("len:%d, width * height:%d\n", len, bmp_info_header.biWidth * bmp_info_header.biHeight);
exit_close:
	ifs.close();
exit:

	return ret;
}

int hardware::set_image(std::string bmp_path)
{
	int ret = 0;

	ret = get_image_info(bmp_path);

	if(ret != 0) {
		return ret;
	}

	ft_set_image(cache_item->buffer, cache_item->len);

	return ret;
}

void hardware::hw_usleep(__ft_u32 usec)
{
	usleep(usec);
}

__ft_s32 hardware::get_mcu_status(void)
{
	__ft_s32 ret = 1;
	return ret;
}

__ft_u64 hardware::get_system_time(void)
{
	__ft_u64 ret = 0;
	settings *g_settings = settings::get_instance();

	struct timeval tv;

	g_settings->get_time_val(&tv);

	ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	return ret;
}

int hardware::save_one_template(unsigned short finger_id)
{
	int ret = 0;
	fp_alg_tpl_t fp_tpl;
	ret = focal_SaveAlgTplData(finger_id, &fp_tpl.tpl_type, &fp_tpl.tpl_size, fp_tpl.tpl_data);
	return ret;
}

int hardware::enroll(unsigned short finger_id, unsigned char enroll_index, unsigned char *penroll_coverage)
{
	int ret = 0;

	ret = focal_Enroll(finger_id, enroll_index, penroll_coverage);

	return ret;
}

int hardware::identify(unsigned char *finger_id, unsigned char *update, unsigned char *update_outside, int *update_template_finger_id)
{
	int ret = 0;
	ret = focal_Identify(finger_id, update);

	if(ret == 0) {
		settings *g_settings = settings::get_instance();

		if(((int)g_settings->value_strtod(g_settings->update_template_outside_enable) == 1)
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

int hardware::update_template(int update, int *finger_id)
{
	int ret = 0;
	ret = focal_UpdateTemplate(update, finger_id);
	return ret;
}

int hardware::delete_template(char finger_id)
{
	int ret = 0;
	ret = focal_DelFinger(finger_id);
	return ret;
}

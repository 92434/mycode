

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：task_bmp.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时25分02秒
 *   修改日期：2017年07月18日 星期二 14时36分55秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TASK_BMP_H
#define _TASK_BMP_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <stdio.h>

typedef enum _matched_type {
	UNKNOW = 0,
	MATCHED,
	UNMATCHED
} matched_type_t;

typedef enum _test_type {
	FOR_UNKNOW = 0,
	FOR_ENROLL,
	FOR_FR,
	FOR_FA
} test_type_t;


class task_bmp
{
public:
	matched_type_t matched_type;
	test_type_t test_type;
	std::string bmp_path;
	std::string catagory;
	std::string id;
	std::string serial_no;

	int current_enroll_id;

	matched_type_t new_matched_type;
	std::string new_matched_catagory;
	std::string new_matched_id;

	task_bmp()
	{
		matched_type = UNKNOW;
		test_type = FOR_UNKNOW;
		current_enroll_id = -1;

		new_matched_type = UNKNOW;
	}
	~task_bmp()
	{
	}


	unsigned long int get_finger_id()
	{
		unsigned long int finger_id;
		char *invalid_pos;
		finger_id = strtoul(id.c_str(), &invalid_pos, 10);

		return finger_id;
	}

	int set_finger_id(int finger_id)
	{
		int ret = 0;
		char buffer[1024];
		int len;

		len = snprintf(buffer, 1024, "%d", finger_id);
		buffer[len] = 0;
		id = buffer;
		return ret;
	}

	unsigned long int get_matched_finger_id()
	{
		unsigned long int matched_finger_id;
		char *invalid_pos;
		matched_finger_id = strtoul(new_matched_id.c_str(), &invalid_pos, 10);

		return matched_finger_id;
	}

	int set_matched_finger_id(int matched_finger_id)
	{
		int ret = 0;
		char buffer[1024];
		int len;

		len = snprintf(buffer, 1024, "%d", matched_finger_id);
		buffer[len] = 0;
		new_matched_id = buffer;
		return ret;
	}

	std::string get_matched_type()
	{

		std::string ret;

		ret = (matched_type == MATCHED) ? "MATCHED" :
			  (matched_type == UNMATCHED) ? "UNMATCHED" :
			  "UNKNOW";
		return ret;
	}

	std::string get_new_matched_type()
	{

		std::string ret;

		ret = (new_matched_type == MATCHED) ? "MATCHED" :
			  (new_matched_type == UNMATCHED) ? "UNMATCHED" :
			  "UNKNOW";
		return ret;
	}
};
#endif //TASK_BMP_H

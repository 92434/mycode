

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *   
 *   文件名称：task_bmp.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时25分02秒
 *   修改日期：2017年07月14日 星期五 13时50分05秒
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
};
#endif //TASK_BMP_H

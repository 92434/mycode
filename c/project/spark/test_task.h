

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：test_task.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时27分38秒
 *   修改日期：2017年07月14日 星期五 15时59分04秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TEST_TASK_H
#define _TEST_TASK_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include <fstream>
#include <algorithm>    // std::sort
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdarg.h>
#include "task_bmp.h"
#include "settings.h"
#include "filesystem.h"
#include "hardware.h"
typedef enum _task_start_reason {
	ADD_ID_FINISHED = 0,
	ADD_CATAGORY_FINISHED,
} task_start_reason_t;

typedef struct _result_notifier {
	int pid;
	char filename[1024];
} result_notifier_t;

struct bmp_enroll_set_comp {
	bool operator() (const task_bmp &bmp1, const task_bmp &bmp2) const
	{
		bool ret = false;

		if(bmp1.catagory < bmp2.catagory) {
			ret = true;
			return ret;
		} else if(bmp1.catagory == bmp2.catagory) {
			if(bmp1.id < bmp2.id) {
				ret = true;
				return ret;
			}
		}


		return ret;
	}
};

class test_task
{
private:
	std::vector<task_bmp> enroll_list;
	std::vector<task_bmp> fr_identify_list;
	std::vector<task_bmp> fa_identify_list;
	std::set<task_bmp, bmp_enroll_set_comp> enroll_ids;
	std::string timestamp;
	std::ofstream ofs;
	static std::ofstream ofs_hardware;
	std::string logfile;
	std::string logfile_hardware;

	static std::string cur_bmp_path;

public:
	test_task();

	~test_task();

	int clear();

	int add_enroll_item(task_bmp bmp);

	int add_identify_item(task_bmp bmp);

	int get_timestamp();

	int log_file_start();

	int log_file(const char *fmt, ...);

	static int log_file_hardware(const char *fmt, ...);

	int log_file_end();

	static int save_bmp(char *label, char *buffer, int len);

	static bool enroll_less_than(task_bmp bmp1, task_bmp bmp2);

	static bool identify_less_than(task_bmp bmp1, task_bmp bmp2);

	int do_task();

	int send_client_result(std::string server_path);

	std::set<task_bmp, bmp_enroll_set_comp> &get_enroll_ids();

	bool can_start_task(task_start_reason_t reason);

	int start_task(task_start_reason_t reason, std::string server_path);

};
#endif //TEST_TASK_H

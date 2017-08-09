

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：test_task.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时27分38秒
 *   修改日期：2017年08月09日 星期三 17时59分40秒
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

typedef enum _report_type {
	REPORT_PROCESS = 0,
	REPORT_RESULT,
	REPORT_EXIT,
} report_type_t;

typedef struct _result_notifier {
	report_type_t type;
	int pid;
	char buffer[BUFFER_LEN];
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
	std::string logfile;
	std::string logfile_hardware;
	std::string server_path;
	std::set<task_bmp, bmp_enroll_set_comp> current_enroll_ids;

	int fr_total_tasks;
	int fa_total_tasks;
	int total_tasks;
	int current_tasks;
	int start_time;
	int current_percent;

	int fr_fail_count;
	int fa_success_count;

public:
	test_task();

	~test_task();

	int clear();

	int add_enroll_item(task_bmp bmp);

	int add_identify_item(task_bmp bmp);

	int gen_log_file_names();

	int log_file_start();

	int log_file(const char *fmt, ...);

	int log_file_end();

	static bool enroll_less_than(task_bmp bmp1, task_bmp bmp2);

	static bool identify_less_than(task_bmp bmp1, task_bmp bmp2);

	int account_task(int tasks);

	int wait_for_gdb();

	int pre_task();

	int task_enroll_id(int finger_id, std::vector<task_bmp> &enroll_id_list, int start_index, int number);

	int task_verify(std::vector<task_bmp> &identify_list, test_type_t test_type);

	int report_result();

	int do_task_list();

	int post_task();

	int do_task();

	int send_client_result(report_type_t report_type, std::string content);

	std::set<task_bmp, bmp_enroll_set_comp> &get_enroll_ids();

	bool can_start_task(task_start_reason_t reason);

	int start_task(std::string server_path);
};
#endif //TEST_TASK_H

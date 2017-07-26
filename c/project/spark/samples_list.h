

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：samples_list.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时28分39秒
 *   修改日期：2017年07月26日 星期三 15时45分22秒
 *   描    述：
 *
 *================================================================*/
#ifndef _SAMPLES_LIST_H
#define _SAMPLES_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
#include <string>
#include <map>
#include <vector>
#include <set>

#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include "task_bmp.h"
#include "test_task.h"
#include "filesystem.h"

class samples_list
{
private:
	int server_fd;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *enroll_samples;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fr_samples;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fa_samples;

	std::set<int> pid_list;
	std::string server_path;

	int fr_fail_count;
	int fr_total_count;
	int fa_success_count;
	int fa_total_count;

	std::ofstream ofs;

	std::string logfile;

public:
	int gen_server_path();
	samples_list();

	int release_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples);

	~samples_list();

	std::vector<std::string> get_bmp_filelist(std::string dirname);

	int new_type_bmp(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> **p_samples, task_bmp bmp);

	int get_pattern_order(std::string pattern, int index);

	int update_sub_samples_list(
		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> **samples,
		std::vector<std::string> &bmp_file_list,
		std::vector<pattern_info> &vec_pattern);

	int update_samples_list();

	int p_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples);


	int p_result();

	int add_test_task_catagory(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples,
							   test_task *task,
							   std::set<task_bmp, bmp_enroll_set_comp> &enroll_ids,
							   test_type_t test_type);

	int create_server();

	int stop_server();

	int parse_pid_result(char *buffer);

	int report_result();

	int get_client_result();

	int try_to_start_task_and_wait(test_task *task, task_start_reason_t reason, bool wait = false);

	int start_test_task();

	int gen_log_file_names();

	int log_file_start();

	int log_file(const char *fmt, ...);

	int log_file_end();
};
#endif //SAMPLES_LIST_H

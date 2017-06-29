

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 *   修改日期：2017年06月29日 星期四 12时44分04秒
 *   描    述：
 *
 *================================================================*/
#include "main.h"
#include "optparse/optparse.h"
#include "filesystem/filesystem.h"
#include "regexp/regexp.h"

typedef enum _matched_type {
	UNKNOW = 0,
	MATCHED,
	UNMATCHED
} matched_type_t;

class task_bmp
{
public:
	matched_type_t matched_type;
	std::string bmp_path;
	std::string id;
	std::string serial_no;

	matched_type_t new_matched_type;
	std::string matched_id;

	task_bmp()
	{
		matched_type = UNKNOW;
		new_matched_type = UNKNOW;
	}
	~task_bmp()
	{
	}
};

class task_list
{
public:
	task_list()
	{
	};

	~task_list()
	{
	};

	std::string dirname;
	std::string enroll_pattern;
	std::map<std::string, std::vector<task_bmp> > enroll_task_list;

	std::string fr_pattern;
	std::string fr_success_pattern;
	std::string fr_fail_pattern;
	std::map<std::string, std::vector<task_bmp> > fr_task_list;

	std::string fa_pattern;
	std::string fa_success_pattern;
	std::string fa_fail_pattern;
	std::map<std::string, std::vector<task_bmp> > fa_task_list;

	int parse_args(int argc, char **argv)
	{
		int ret = 0;
		bool have_dir = false;
		bool have_enroll_pattern = false;
		bool have_fr_pattern = false;
		bool have_fa_pattern = false;

		optparse opt;
		opt.add_long_option("dir", true, false, 'a', "picture directory");
		opt.add_long_option("enroll-pattern", true, false, 'b', "enroll pattern");

		opt.add_long_option("fr-pattern", true, false, 'c', "fr pattern");
		opt.add_long_option("fr-success-pattern", true, false, 'd', "fr success pattern");
		opt.add_long_option("fr-fail-pattern", true, false, 'e', "fr fail pattern");

		opt.add_long_option("fa-pattern", true, false, 'f', "fa pattern");
		opt.add_long_option("fa-success-pattern", true, false, 'g', "fa success pattern");
		opt.add_long_option("fa-fail-pattern", true, false, 'h', "fa fail pattern");

		ret = opt.get_long_option(argc, argv);

		if(ret != 0) {
			opt.p_help();
		} else {
			if(opt.have_option("a")) {
				have_dir = true;
				dirname = opt.option_value("a");
			}

			if(opt.have_option("b")) {
				have_enroll_pattern = true;
				enroll_pattern = opt.option_value("b");
			}

			if(opt.have_option("c")) {
				have_fr_pattern = true;
				fr_pattern = opt.option_value("c");
			} else if(opt.have_option("d") && opt.have_option("e")) {
				have_fr_pattern = true;
				fr_success_pattern = opt.option_value("d");
				fr_fail_pattern = opt.option_value("e");
			}

			if(opt.have_option("f")) {
				have_fa_pattern = true;
				fa_pattern = opt.option_value("f");
			} else if(opt.have_option("g") && opt.have_option("h")) {
				have_fa_pattern = true;
				fa_success_pattern = opt.option_value("g");
				fa_fail_pattern = opt.option_value("h");
			}
		}

		if(have_dir && have_enroll_pattern && (have_fr_pattern || have_fa_pattern)) {
		} else {
			ret = -1;
			printf("%s!!!\n", "参数不全");
			opt.p_help();
		}

		if(ret == 0) {
			opt.p_result();
			ret = update_task_list();
		}

		return ret;
	}

	std::vector<std::string> get_bmp_filelist(std::string dirname)
	{
		std::vector<std::string> bmp_file_list;
		std::vector<std::string> filter_list;
		filesystem fs;

		filter_list.push_back(".bmp");
		bmp_file_list = fs.dir_scan_files(dirname, filter_list);

		return bmp_file_list;
	}

	int update_task_list()
	{
		int ret = 0;
		std::vector<std::string> bmp_file_list;
		std::map<std::string, std::vector<task_bmp> >::iterator it;

		bmp_file_list = get_bmp_filelist(dirname);

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::vector<std::string> matched_list = r.match(*it, enroll_pattern);

			if(matched_list.size() == 3) {
				std::map<std::string, std::vector<task_bmp> >::iterator it_task;
				std::vector<task_bmp> bmp_set;
				bmp.bmp_path = *it;
				bmp.id = matched_list.at(1);
				bmp.serial_no = matched_list.at(2);

				printf("enroll:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str());
				it_task = enroll_task_list.find(bmp.id);

				if(it_task != enroll_task_list.end()) {
					bmp_set = it_task->second;
				}

				bmp_set.push_back(bmp);
				enroll_task_list[bmp.id] = bmp_set;
			}
		}

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::vector<std::string> matched_list = r.match(*it, fr_pattern);

			if(matched_list.size() == 3) {
				bmp.matched_type = UNKNOW;
			} else {
				matched_list = r.match(*it, fr_success_pattern);

				if(matched_list.size() == 3) {
					bmp.matched_type = MATCHED;
				} else {
					matched_list = r.match(*it, fr_fail_pattern);

					if(matched_list.size() == 3) {
						bmp.matched_type = UNMATCHED;
					}
				}
			}

			if(matched_list.size() == 3) {
				std::map<std::string, std::vector<task_bmp> >::iterator it_task;
				std::vector<task_bmp> bmp_set;
				bmp.bmp_path = *it;
				bmp.id = matched_list.at(1);
				bmp.serial_no = matched_list.at(2);

				printf("fr:%s, id:%s, serial_no:%s, matched_type:%s\n", 
						it->c_str(), 
						matched_list.at(1).c_str(), 
						matched_list.at(2).c_str(), 
						bmp.matched_type == UNKNOW ? "UNKNOW" : 
						bmp.matched_type == MATCHED ? "MATCHED":
						"UNMATCHED");
				it_task = fr_task_list.find(bmp.id);

				if(it_task != fr_task_list.end()) {
					bmp_set = it_task->second;
				}

				bmp_set.push_back(bmp);
				fr_task_list[bmp.id] = bmp_set;
			}

		}

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::vector<std::string> matched_list = r.match(*it, fa_pattern);

			if(matched_list.size() == 3) {
				bmp.matched_type = UNKNOW;
			} else {
				matched_list = r.match(*it, fa_success_pattern);

				if(matched_list.size() == 3) {
					bmp.matched_type = MATCHED;
				} else {
					matched_list = r.match(*it, fa_fail_pattern);

					if(matched_list.size() == 3) {
						bmp.matched_type = UNMATCHED;
					}
				}
			}

			if(matched_list.size() == 3) {
				std::map<std::string, std::vector<task_bmp> >::iterator it_task;
				std::vector<task_bmp> bmp_set;
				bmp.bmp_path = *it;
				bmp.id = matched_list.at(1);
				bmp.serial_no = matched_list.at(2);

				printf("fa:%s, id:%s, serial_no:%s, matched_type:%s\n", 
						it->c_str(), 
						matched_list.at(1).c_str(), 
						matched_list.at(2).c_str(), 
						bmp.matched_type == UNKNOW ? "UNKNOW" : 
						bmp.matched_type == MATCHED ? "MATCHED":
						"UNMATCHED");
				it_task = fa_task_list.find(bmp.id);

				if(it_task != fa_task_list.end()) {
					bmp_set = it_task->second;
				}

				bmp_set.push_back(bmp);
				fa_task_list[bmp.id] = bmp_set;
			}

		}

		return ret;
	}

};

int main(int argc, char **argv)
{
	int ret = 0;

	task_list task_list;

	ret = task_list.parse_args(argc, argv);

	if(ret != 0) {
		return ret;
	}


	printf("Done!\n");

	return ret;
}



/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 *   修改日期：2017年06月28日 星期三 19时11分34秒
 *   描    述：
 *
 *================================================================*/
#include "main.h"
#include "optparse/optparse.h"
#include "filesystem/filesystem.h"
#include "regexp/regexp.h"

class task_bmp
{
public:
	std::string bmp_path;
	std::string id;
	std::string serial_no;
	task_bmp()
	{
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

	std::string enroll_dirname;
	std::string enroll_task_bmp_pattern;
	std::string enroll_task_bmp_pattern_info_order;
	std::map<std::string, std::vector<task_bmp> > enroll_task_list;

	std::string fr_task_dirname;
	std::string fr_task_bmp_pattern;
	std::string fr_task_bmp_pattern_info_order;
	std::map<std::string, std::vector<task_bmp> > fr_task_list;

	std::string fa_task_dirname;
	std::string fa_task_bmp_pattern;
	std::string fa_task_bmp_pattern_info_order;
	std::map<std::string, std::vector<task_bmp> > fa_task_list;
};

int update_task_list(task_list &task_list) {
	int ret = 0;
	std::vector<std::string> bmp_file_list;
	std::map<std::string, std::vector<task_bmp> >::iterator it;

	bmp_file_list = get_bmp_filelist(task_list.enroll_dirname);

	for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
		//printf("file in list:%s\n", it->c_str());
		regexp r;
		std::vector<std::string> matched_list = r.match(*it, task_list.enroll_task_bmp_pattern);
		if(matched_list.size() == 3) {
			std::vector<task_bmp> bmp_set;
			task_bmp bmp;
			bmp.bmp_path = *it;
			bmp.id = matched_list.at(1);
			bmp.serial_no = matched_list.at(1);

			printf("path:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str());
			it = task_list.enroll_task_list.find(bmp.id);
			if(it != task_list.enroll_task_list.end()) {
				bmp_set = it->second;
			}
			bmp_set.push_back(bmp);
			task_list.enroll_task_list[bmp.id] = bmp_set;
		}
	}
	return ret;
}

int parse_args(task_list &task_list, int argc, char **argv)
{
	int ret = 0;

	optparse opt;
	opt.add_long_option("enroll-dir", true, false, 'a', "enroll picture directory");
	opt.add_long_option("enroll-file-pattern", true, false, 'b', "enroll file pattern");

	opt.add_long_option("fr-test-dir", true, false, 'c', "fr test picture directory");
	opt.add_long_option("fr-test-file-pattern", true, false, 'd', "enroll file pattern");

	opt.add_long_option("fa-test-dir", true, false, 'e', "fa test picture directory");
	opt.add_long_option("fa-test-file-pattern", true, false, 'f', "enroll file pattern");

	ret = opt.get_long_option(argc, argv);

	if(ret != 0) {
		opt.p_help();
	} else {
		if(opt.have_option("a") && opt.have_option("b")) {
			task_list.enroll_dirname = opt.option_value("a");
			task_list.enroll_task_bmp_pattern = opt.option_value("b");
		} else if(opt.have_option("c") && opt.have_option("d")) {
			task_list.fr_task_dirname = opt.option_value("c");
			task_list.fr_task_bmp_pattern = opt.option_value("d");
		} else if(opt.have_option("e") && opt.have_option("f")) {
			task_list.fa_task_dirname = opt.option_value("e");
			task_list.fa_task_bmp_pattern = opt.option_value("f");
		} else {
			ret = -1;
			opt.p_help();
		}
	}
	opt.p_result();
	update_task_list(task_list);

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

int main(int argc, char **argv)
{
	int ret = 0;

	task_list task_list;

	ret = parse_args(task_list, argc, argv);

	if(ret != 0) {
		return ret;
	}


	printf("Done!\n");

	return ret;
}

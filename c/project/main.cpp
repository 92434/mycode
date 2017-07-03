

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 *   修改日期：2017年07月03日 星期一 20时04分59秒
 *   描    述：
 *
 *================================================================*/
#include "main.h"
#include "optparse/optparse.h"
#include "filesystem/filesystem.h"
#include "regexp/regexp.h"
#include "configure/configure.h"

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
	std::string person;
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

class samples_list
{
public:
	samples_list()
	{
		enroll_samples = NULL;
		fr_samples = NULL;
		fa_samples = NULL;
	};

	int release_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples)
	{
		int ret = 0;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;

		if(samples != NULL) {
			for(sample_it = samples->begin(); sample_it != samples->end(); sample_it++) {
				std::map<std::string, std::vector<task_bmp> *> *person = sample_it->second;
				std::map<std::string, std::vector<task_bmp> *>::iterator person_it;

				for(person_it = person->begin(); person_it != person->end(); person_it++) {
					std::vector<task_bmp> *id = person_it->second;

					delete id;
				}

				delete person;
			}

			delete samples;
		}

		return ret;
	}

	~samples_list()
	{
		release_type_samples(enroll_samples);
		release_type_samples(fr_samples);
		release_type_samples(fa_samples);
	};

	std::string configure_file;
	std::string dirname;
	std::string enroll_pattern;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *enroll_samples;

	std::string fr_pattern;
	std::string fr_success_pattern;
	std::string fr_fail_pattern;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fr_samples;

	std::string fa_pattern;
	std::string fa_success_pattern;
	std::string fa_fail_pattern;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fa_samples;

	int parse_args_from_console(int argc, char **argv)
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

	int parse_args_from_configuration(int argc, char **argv)
	{
		int ret = 0;
		bool have_configure_file = false;
		bool have_dir = false;
		bool have_enroll_pattern = false;
		bool have_fr_pattern = false;
		bool have_fa_pattern = false;

		optparse opt;
		opt.add_long_option("configure-file", true, false, 'f', "assign configuration file");

		ret = opt.get_long_option(argc, argv);

		if(ret != 0) {
			opt.p_help();
		} else {
			have_configure_file = true;

			if(opt.have_option("f")) {
				configure_file = opt.option_value("f");
			} else {
				configure_file = "default.ini";
			}
		}

		if(have_configure_file) {
			configure cfg;
			ret = cfg.load(configure_file);

			if(ret == 0) {
				cfg.p_configure();
				dirname = cfg.get("dir", "directory");

				enroll_pattern = cfg.get("resource-pattern", "enroll-pattern");

				fr_pattern = cfg.get("resource-pattern", "fr-pattern");
				fr_success_pattern = cfg.get("resource-pattern", "fr-success-pattern");
				fr_fail_pattern = cfg.get("resource-pattern", "fr-fail-pattern");

				fa_pattern = cfg.get("resource-pattern", "fa-pattern");
				fa_success_pattern = cfg.get("resource-pattern", "fa-success-pattern");
				fa_fail_pattern = cfg.get("resource-pattern", "fa-fail-pattern");
			} else {
				printf("load configuration file failed!!!\n");
			}


			if(dirname.size()) {
				have_dir = true;
			} else {
				printf("no dirname!\n");
			}

			if(enroll_pattern.size()) {
				have_enroll_pattern = true;
			} else {
				printf("no enroll_pattern!\n");
			}

			if(fr_pattern.size()) {
				have_fr_pattern = true;
			} else if(fr_success_pattern.size() && fr_fail_pattern.size()) {
				have_fr_pattern = true;
			} else {
				printf("no fr pattern!\n");
			}

			if(fa_pattern.size()) {
				have_fa_pattern = true;
			} else if(fa_success_pattern.size() && fa_fail_pattern.size()) {
				have_fa_pattern = true;
			} else {
				printf("no fa pattern!\n");
			}
		}


		if(have_dir && have_enroll_pattern && (have_fr_pattern || have_fa_pattern)) {
		} else {
			ret = -1;
			printf("%s!!!\n", "参数信息不足");
		}

		if(ret == 0) {
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

	int new_type_bmp(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> **p_samples, task_bmp bmp)
	{
		int ret = 0;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;
		std::map<std::string, std::vector<task_bmp> *> *person;

		std::map<std::string, std::vector<task_bmp> *>::iterator person_it;
		std::vector<task_bmp> *id;

		if(p_samples == 0) {
			ret = -1;
			return ret;
		}

		samples = *p_samples;

		if(samples == NULL) {
			samples = new std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>;
			*p_samples = samples;
		}

		sample_it = samples->find(bmp.person);

		if(sample_it != samples->end()) {
			person = sample_it->second;
		} else {
			person = new std::map<std::string, std::vector<task_bmp> *>;
			samples->insert(std::pair<std::string, std::map<std::string, std::vector<task_bmp> *> *>(bmp.person, person));
		}

		person_it = person->find(bmp.id);

		if(person_it != person->end()) {
			id = person_it->second;
		} else {
			id = new std::vector<task_bmp>;
			person->insert(std::pair<std::string, std::vector<task_bmp> *>(bmp.id, id));
		}

		id->push_back(bmp);

		return ret;
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
			std::vector<std::string> matched_list = r.match(*it, enroll_pattern);

			if(matched_list.size() == 4) {
				task_bmp bmp;

				bmp.bmp_path = *it;
				bmp.person = matched_list.at(1);
				bmp.id = matched_list.at(2);
				bmp.serial_no = matched_list.at(3);

				//printf("enroll:%s, person:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str(), matched_list.at(3).c_str());

				new_type_bmp(&enroll_samples, bmp);
			}
		}

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::vector<std::string> matched_list = r.match(*it, fr_pattern);

			if(matched_list.size() == 4) {
				bmp.matched_type = UNKNOW;
			} else {
				matched_list = r.match(*it, fr_success_pattern);

				if(matched_list.size() == 4) {
					bmp.matched_type = MATCHED;
				} else {
					matched_list = r.match(*it, fr_fail_pattern);

					if(matched_list.size() == 4) {
						bmp.matched_type = UNMATCHED;
					}
				}
			}

			if(matched_list.size() == 4) {
				bmp.bmp_path = *it;
				bmp.person = matched_list.at(1);
				bmp.id = matched_list.at(2);
				bmp.serial_no = matched_list.at(3);

				//printf("fr:%s, person:%s, id:%s, serial_no:%s, matched_type:%s\n",
				//	   it->c_str(),
				//	   matched_list.at(1).c_str(),
				//	   matched_list.at(2).c_str(),
				//	   matched_list.at(3).c_str(),
				//	   bmp.matched_type == UNKNOW ? "UNKNOW" :
				//	   bmp.matched_type == MATCHED ? "MATCHED" :
				//	   "UNMATCHED");
				new_type_bmp(&fr_samples, bmp);
			}

		}

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::vector<std::string> matched_list = r.match(*it, fa_pattern);

			if(matched_list.size() == 4) {
				bmp.matched_type = UNKNOW;
			} else {
				matched_list = r.match(*it, fa_success_pattern);

				if(matched_list.size() == 4) {
					bmp.matched_type = MATCHED;
				} else {
					matched_list = r.match(*it, fa_fail_pattern);

					if(matched_list.size() == 4) {
						bmp.matched_type = UNMATCHED;
					}
				}
			}

			if(matched_list.size() == 4) {
				bmp.bmp_path = *it;
				bmp.person = matched_list.at(1);
				bmp.id = matched_list.at(2);
				bmp.serial_no = matched_list.at(3);

				//printf("fa:%s, person:%s, id:%s, serial_no:%s, matched_type:%s\n",
				//	   it->c_str(),
				//	   matched_list.at(1).c_str(),
				//	   matched_list.at(2).c_str(),
				//	   matched_list.at(3).c_str(),
				//	   bmp.matched_type == UNKNOW ? "UNKNOW" :
				//	   bmp.matched_type == MATCHED ? "MATCHED" :
				//	   "UNMATCHED");

				new_type_bmp(&fa_samples, bmp);
			}

		}

		return ret;
	}

	int p_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples)
	{
		int ret = 0;
		int index = 0;
		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;

		std::string type = (samples == enroll_samples ? "enroll" :
							samples == fr_samples ? "fr" :
							"fa");
		//printf("samples type:%s\n", type.c_str());

		if(samples != NULL) {
			for(sample_it = samples->begin(); sample_it != samples->end(); sample_it++) {
				std::map<std::string, std::vector<task_bmp> *> *person = sample_it->second;
				std::map<std::string, std::vector<task_bmp> *>::iterator person_it;

				//printf("person:%s\n", sample_it->first.c_str());
				for(person_it = person->begin(); person_it != person->end(); person_it++) {
					//printf("id:%s\n", person_it->first.c_str());
					std::vector<task_bmp> *id = person_it->second;

					std::vector<task_bmp>::iterator id_it;

					for(id_it = id->begin(); id_it != id->end(); id_it++) {
						//printf("serial_no:%s\n", id_it->serial_no.c_str());
						task_bmp bmp = *id_it;

						printf("%s[%d]:%s, person:%s, id:%s, serial_no:%s, matched_type:%s\n",
							   type.c_str(),
							   index++,
							   bmp.bmp_path.c_str(),
							   bmp.person.c_str(),
							   bmp.id.c_str(),
							   bmp.serial_no.c_str(),
							   bmp.matched_type == UNKNOW ? "UNKNOW" :
							   bmp.matched_type == MATCHED ? "MATCHED" :
							   "UNMATCHED");
					}

				}
			}
		}

		return ret;
	}


	int p_samples()
	{
		int ret = 0;
		p_type_samples(enroll_samples);
		p_type_samples(fr_samples);
		p_type_samples(fa_samples);
		return ret;
	}

};

int main(int argc, char **argv)
{
	int ret = 0;

	samples_list samples_list;

	ret = samples_list.parse_args_from_configuration(argc, argv);

	if(ret != 0) {
		return ret;
	}

	samples_list.p_samples();


	printf("Done!\n");

	return ret;
}

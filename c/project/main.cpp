

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 *   修改日期：2017年07月05日 星期三 17时48分16秒
 *   描    述：
 *
 *================================================================*/
#include "main.h"
#include "optparse/optparse.h"
#include "filesystem/filesystem.h"
#include "regexp/regexp.h"
#include "configure/configure.h"
#include <set>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdarg.h>
#include <sys/time.h>
#include <algorithm>    // std::sort

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

typedef enum _select_type {
	SELECT_ALL = 0,
	SELECT_SAME,
	SELECT_DIFFERENT
} select_type_t;

typedef struct _result_notifier {
	int pid;
	char filename[1024];
} result_notifier_t;

class task_bmp
{
public:
	matched_type_t matched_type;
	test_type_t test_type;
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
		test_type = FOR_UNKNOW;
	}
	~task_bmp()
	{
	}
};

class test_task
{
private:
	std::vector<task_bmp> enroll_list;
	std::vector<task_bmp> fr_identify_list;
	std::vector<task_bmp> fa_identify_list;
	std::set<std::string> enroll_ids;
	std::ofstream ofs;
	std::string logfile;
	//int max_enroll_id_num;

public:
	test_task()
	{
		//max_enroll_id_num = 5;
		clear();
	}

	~test_task()
	{
		clear();
	}

	int clear()
	{
		int ret = 0;
		enroll_list.clear();
		fr_identify_list.clear();
		fa_identify_list.clear();
		enroll_ids.clear();
		logfile.clear();
		return ret;
	}

	int add_enroll_item(task_bmp bmp)
	{
		int ret = 0;
		enroll_list.push_back(bmp);
		enroll_ids.insert(bmp.person + ":" + bmp.id);
		return ret;
	}

	int add_identify_item(task_bmp bmp)
	{
		int ret = 0;

		if(bmp.test_type == FOR_FR) {
			fr_identify_list.push_back(bmp);
		} else if(bmp.test_type == FOR_FA) {
			fa_identify_list.push_back(bmp);
		}

		return ret;
	}

	int run_list()
	{
		int ret = 0;
		return ret;
	}

	int log_file_start()
	{
		int ret = 0;
		char buffer[1024];
		int len = 0;
		struct tm *tm;
		struct timeval tv;
		struct timezone tz;
		filesystem fs;

		gettimeofday(&tv, &tz);

		tm = localtime(&tv.tv_sec);

		len = snprintf(buffer, 1023, "logs/%04d%02d%02d%02d%02d%02d_%06d.log",
					   tm->tm_year + 1900,
					   tm->tm_mon + 1,
					   tm->tm_mday + 1,
					   tm->tm_hour,
					   tm->tm_min,
					   tm->tm_sec,
					   (int)tv.tv_usec
					  );
		buffer[len] = 0;
		logfile = buffer;

		fs.mkdirs(logfile);

		ofs.open(logfile.c_str());

		if(!ofs.good()) {
			printf("open:%s failed!!! (%s)!\n", buffer, strerror(errno));
			ret = -1;
			return ret;
		}


		return ret;
	}

	int log_file(const char *fmt, ...)
	{
		int ret = 0;
		int len = 0;
		char buffer[1024];
		va_list ap;

		va_start(ap, fmt);
		len = vsnprintf(buffer, 1023, fmt, ap);
		buffer[len] = 0;
		va_end(ap);
		ofs.write(buffer, len);

		return ret;
	}

	int log_file_end()
	{
		int ret = 0;
		ofs.close();
		return ret;
	}

	static bool enroll_less_than(task_bmp bmp1, task_bmp bmp2)
	{
		char *invalid_pos;
		unsigned long bmp1_id, bmp2_id;
		unsigned long bmp1_serial_no, bmp2_serial_no;

		bmp1_id = strtoul(bmp1.id.c_str(), &invalid_pos, 10);
		bmp2_id = strtoul(bmp2.id.c_str(), &invalid_pos, 10);

		bmp1_serial_no = strtoul(bmp1.serial_no.c_str(), &invalid_pos, 10);
		bmp2_serial_no = strtoul(bmp2.serial_no.c_str(), &invalid_pos, 10);

		if(bmp1_id < bmp2_id) {
			return true;
		} else if(bmp1_id == bmp2_id) {
			if(bmp1_serial_no < bmp2_serial_no) {
				return true;
			}
		}

		return false;
	}

	static bool identify_less_than(task_bmp bmp1, task_bmp bmp2)
	{
		char *invalid_pos;
		unsigned long bmp1_id, bmp2_id;
		unsigned long bmp1_serial_no, bmp2_serial_no;

		bmp1_id = strtoul(bmp1.id.c_str(), &invalid_pos, 10);
		bmp2_id = strtoul(bmp2.id.c_str(), &invalid_pos, 10);

		bmp1_serial_no = strtoul(bmp1.serial_no.c_str(), &invalid_pos, 10);
		bmp2_serial_no = strtoul(bmp2.serial_no.c_str(), &invalid_pos, 10);

		if(bmp1_serial_no < bmp2_serial_no) {
			return true;
		} else if(bmp1_serial_no == bmp2_serial_no) {
			if(bmp1_id < bmp2_id) {
				return true;
			}
		}

		return false;
	}

	int do_task()
	{
		int ret = 0;
		ret  = log_file_start();

		std::set<std::string>::iterator ids_it;

		std::sort(enroll_list.begin(), enroll_list.end(), enroll_less_than);
		std::sort(fr_identify_list.begin(), fr_identify_list.end(), identify_less_than);

		for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
			log_file("pid:%d, ppid:%d, process:%s\n", (int)getpid(), (int)getppid(), ids_it->c_str());
		}

		log_file("pid:%d, ppid:%d, enroll_list.size():%d\n", (int)getpid(), (int)getppid(), (int)enroll_list.size());
		{
			std::vector<task_bmp>::iterator enroll_list_it;

			for(enroll_list_it = enroll_list.begin(); enroll_list_it != enroll_list.end(); enroll_list_it++) {
				task_bmp bmp = *enroll_list_it;
				log_file("pid:%d, ppid:%d, enroll:person:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.person.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
			}
		}
		log_file("pid:%d, ppid:%d, fr_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fr_identify_list.size());
		{
			std::vector<task_bmp>::iterator identify_list_it;

			for(identify_list_it = fr_identify_list.begin(); identify_list_it != fr_identify_list.end(); identify_list_it++) {
				task_bmp bmp = *identify_list_it;
				log_file("pid:%d, ppid:%d, identify:person:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.person.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
			}
		}
		log_file("pid:%d, ppid:%d, fa_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fa_identify_list.size());
		{
			std::vector<task_bmp>::iterator identify_list_it;

			for(identify_list_it = fa_identify_list.begin(); identify_list_it != fa_identify_list.end(); identify_list_it++) {
				task_bmp bmp = *identify_list_it;
				log_file("pid:%d, ppid:%d, identify:person:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.person.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
			}
		}

		log_file_end();
		return ret;
	}

	int send_client_result(std::string server_path)
	{
		int ret = 0;
		/* create a socket */
		int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		struct sockaddr_un address;
		result_notifier_t notifier;
		int len = 0;

		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, server_path.c_str());

		/* connect to the server */
		ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

		if(ret == -1) {
			perror("connect failed: ");
			exit(1);
		}

		/* exchange data */
		notifier.pid = getpid();
		len = snprintf(notifier.filename, 1023, "%s", logfile.c_str());
		notifier.filename[len] = 0;
		len = write(sockfd, &notifier, sizeof(result_notifier_t));

		if(len != sizeof(result_notifier_t)) {
			perror("send notifier failed!!!");
		}

		/* close the socket */
		close(sockfd);
		return ret;
	}

	int start_task(std::string server_path)
	{
		int ret = 0;

		if(enroll_ids.size() > /*max_enroll_id_num*/0) {
			int pid = fork();

			if(pid == -1) {
			} else if(pid == 0) {
				ret = do_task();
				//sleep(1);
				send_client_result(server_path);
				exit(ret);
			} else {
				ret = pid;
				return ret;
			}

			//run_list();
		} else {
		}

		return ret;
	}

};

class samples_list
{
private:
	int server_fd;
	std::string configure_file;
	std::string dirname;
	std::string enroll_pattern;
	std::string enroll_pattern_order;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *enroll_samples;

	std::string fr_pattern;
	std::string fr_pattern_order;
	std::string fr_success_pattern;
	std::string fr_success_pattern_order;
	std::string fr_fail_pattern;
	std::string fr_fail_pattern_order;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fr_samples;

	std::string fa_pattern;
	std::string fa_pattern_order;
	std::string fa_success_pattern;
	std::string fa_success_pattern_order;
	std::string fa_fail_pattern;
	std::string fa_fail_pattern_order;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fa_samples;

	std::set<int> task_list;
	int max_task_number;
	std::string server_path;

public:
	samples_list()
	{
		server_fd = 0;
		enroll_samples = NULL;
		fr_samples = NULL;
		fa_samples = NULL;
		max_task_number = 32;
		server_path = "/tmp/.server_socket";
	}

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
	}

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
				enroll_pattern_order = cfg.get("resource-pattern", "enroll-pattern-order");

				fr_pattern = cfg.get("resource-pattern", "fr-pattern");
				fr_pattern_order = cfg.get("resource-pattern", "fr-pattern-order");
				fr_success_pattern = cfg.get("resource-pattern", "fr-success-pattern");
				fr_success_pattern_order = cfg.get("resource-pattern", "fr-success-pattern-order");
				fr_fail_pattern = cfg.get("resource-pattern", "fr-fail-pattern");
				fr_fail_pattern_order = cfg.get("resource-pattern", "fr-fail-pattern-order");

				fa_pattern = cfg.get("resource-pattern", "fa-pattern");
				fa_pattern_order = cfg.get("resource-pattern", "fa-pattern-order");
				fa_success_pattern = cfg.get("resource-pattern", "fa-success-pattern");
				fa_success_pattern_order = cfg.get("resource-pattern", "fa-success-pattern-order");
				fa_fail_pattern = cfg.get("resource-pattern", "fa-fail-pattern");
				fa_fail_pattern_order = cfg.get("resource-pattern", "fa-fail-pattern-order");
			} else {
				printf("load configuration file failed!!!\n");
			}


			if(dirname.size()) {
				have_dir = true;
			} else {
				printf("no dirname!\n");
			}

			if(enroll_pattern.size() && enroll_pattern_order.size()) {
				have_enroll_pattern = true;
			} else {
				printf("no enroll_pattern!\n");
			}

			if(fr_pattern.size() && fr_pattern_order.size()) {
				have_fr_pattern = true;
			} else if(fr_success_pattern.size() && fr_fail_pattern.size() && fr_success_pattern_order.size() && fr_fail_pattern_order.size()) {
				have_fr_pattern = true;
			} else {
				printf("no fr pattern!\n");
			}

			if(fa_pattern.size() && fa_pattern_order.size()) {
				have_fa_pattern = true;
			} else if(fa_success_pattern.size() && fa_fail_pattern.size() && fa_success_pattern_order.size() && fa_fail_pattern_order.size()) {
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

	int get_pattern_order(std::string pattern, int index) {
		int ret = 0;
		char c = pattern.at(index);
		if((c >= '0') && (c <= '9')) {
			ret = c - '0';
		} else {
			printf("pattern order invalid!\n");
			exit(0);
		}
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

	int add_test_task_person_id(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples,
								test_task *task,
								std::string person_name,
								std::string id_name,
								test_type_t test_type,
								select_type_t select_type)
	{
		int ret = 0;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;

		if(samples == NULL) {
			ret = -1;
			return ret;
		}

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
					bool add = false;


					if(select_type == SELECT_ALL) {
						add = true;
					} else if(select_type == SELECT_SAME) {
						if((bmp.person.compare(person_name) == 0) && (bmp.id.compare(id_name) == 0)) {
							add = true;
						}
					} else if(select_type == SELECT_DIFFERENT) {
						if((bmp.person.compare(person_name) == 0) && (bmp.id.compare(id_name) == 0)) {
						} else {
							add = true;
						}
					}

					if(add) {
						bmp.test_type = test_type;
						ret = task->add_identify_item(bmp);
					}
				}
			}
		}

		return ret;
	}

	int create_server()
	{
		int ret = 0;
		/* create a socket */
		server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		struct sockaddr_un server_addr;
		server_addr.sun_family = AF_UNIX;
		strcpy(server_addr.sun_path, server_path.c_str());

		/* bind with the local file */
		ret = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

		if(ret != 0) {
			printf("bind socket error!(%s)\n", strerror(errno));
			return ret;
		}

		/* listen */
		ret = listen(server_fd, 64);

		if(ret != 0) {
			printf("bind socket error!(%s)\n", strerror(errno));
			return ret;
		}

		return ret;
	}

	int stop_server()
	{
		int ret = 0;

		if(server_fd != 0) {
			close(server_fd);
		}

		if(access(server_path.c_str(), F_OK) == 0) {
			unlink(server_path.c_str());
		}

		return ret;
	}

	int get_client_result()
	{
		int ret = 0;
		int client_sockfd;
		struct sockaddr_un client_addr;
		socklen_t len = sizeof(client_addr);
		result_notifier_t notifier;

		client_sockfd = accept(server_fd, (struct sockaddr *)&client_addr, &len);

		/* exchange data */
		len = read(client_sockfd, &notifier, sizeof(result_notifier_t));

		if(len != sizeof(result_notifier_t)) {
			perror("read notifier failed!!!");
			return ret;
		}

		printf("get log from client:pid:%d, filename:%s\n", notifier.pid, notifier.filename);

		ret = notifier.pid;

		/* close the socket */
		close(client_sockfd);

		//ret = pid;
		return ret;
	}

	int start_task_and_wait(test_task *task, bool wait = false)
	{
		int ret = 0;

		ret = task->start_task(server_path);

		if(ret != 0) {
			printf("pid start:%d(%x)\n", ret, ret);
			task_list.insert(ret);
			task->clear();
		}

		while((task_list.size() >= max_task_number) || (wait && (task_list.size() > 0))) {
			int pid = get_client_result();
			printf("pid stop:%d(%x)\n", pid, pid);
			task_list.erase(pid);
		}

		return ret;
	}

	int start_test_task()
	{
		int ret = 0;
		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;
		test_task task;

		stop_server();

		if(ret = create_server()) {
			return ret;
		}

		if(enroll_samples != NULL) {
			for(sample_it = enroll_samples->begin(); sample_it != enroll_samples->end(); sample_it++) {
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
						bmp.test_type = FOR_ENROLL;
						ret = task.add_enroll_item(bmp);
					}

					ret = add_test_task_person_id(fr_samples, &task, sample_it->first, person_it->first, FOR_FR, SELECT_SAME);
					ret = add_test_task_person_id(fa_samples, &task, sample_it->first, person_it->first, FOR_FA, SELECT_ALL);

				}

				start_task_and_wait(&task);
			}

		}

		start_task_and_wait(&task, true);

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

	//samples_list.p_samples();
	samples_list.start_test_task();

	printf("Done!\n");

	return ret;
}

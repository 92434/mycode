

/*================================================================
 *   Copyright (C) 2017年06月26日 肖飞 All rights reserved
 *
 *   文件名称：main.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年06月26日 星期一 18时15分41秒
 :   修改日期：2017年07月12日 星期三 19时00分59秒
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

#include "ft_lib.h"

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
	SELECT_SAME_CATAGORY,
} select_type_t;

typedef enum _task_start_reason {
	ADD_ID_FINISHED = 0,
	ADD_CATAGORY_FINISHED,
} task_start_reason_t;

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

class settings
{
public:
	std::string configure_file;
	std::string dirname;
	std::string enroll_pattern;
	std::string enroll_pattern_order;

	std::string fr_pattern;
	std::string fr_pattern_order;
	std::string fr_success_pattern;
	std::string fr_success_pattern_order;
	std::string fr_fail_pattern;
	std::string fr_fail_pattern_order;

	std::string fa_pattern;
	std::string fa_pattern_order;
	std::string fa_success_pattern;
	std::string fa_success_pattern_order;
	std::string fa_fail_pattern;
	std::string fa_fail_pattern_order;

	int max_number_of_id_per_proc;
	int max_number_of_catagory_per_proc;
	select_type_t fr_select_type;
	select_type_t fa_select_type;

	static settings *g_settings;

	settings()
	{
		max_number_of_id_per_proc = 5;
		max_number_of_catagory_per_proc = 1;
		fr_select_type = SELECT_SAME_CATAGORY;
		fa_select_type = SELECT_SAME_CATAGORY;
	}

	~settings()
	{
		printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		abort();

		if(g_settings == NULL) {
			delete g_settings;
			g_settings = NULL;
		}
	}
public:
	static settings *get_instance()
	{
		if(g_settings == NULL) {
			g_settings = new settings();
		}

		return g_settings;
	}

	int check_configuration()
	{
		int ret = 0;

		bool have_dir = false;
		bool have_enroll_pattern = false;
		bool have_fr_pattern = false;
		bool have_fa_pattern = false;

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

		if(have_dir && have_enroll_pattern && (have_fr_pattern || have_fa_pattern)) {
		} else {
			ret = -1;
			printf("%s!!!\n", "参数信息不足");
		}

		return ret;
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

		if(ret == 0) {
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
		} else {
			opt.p_help();
		}

		if(ret == 0) {
			ret = check_configuration();
		}

		return ret;
	}

	int parse_args_from_configuration(int argc, char **argv)
	{
		int ret = 0;
		bool have_configure_file = false;
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

			if(ret == 0) {
				ret = check_configuration();
			}
		}

		return ret;
	}

};

settings *settings::g_settings = NULL;

class hardware
{
private:
	int width;
	int height;
	static hardware *hw;

	hardware()
	{
	}

	hardware(int width, int height)
	{
		this->width = width;
		this->height = height;
		ft_lib_init();

		//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, this->width, this->height);
	}

	~hardware()
	{
		ft_lib_uninit();
		//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, this->width, this->height);
	}

public:
	static hardware *get_instance(int width, int height)
	{
		if(hw == NULL) {
			hw = new hardware(width, height);
		} else if((width != hw->width) || (height != hw->height)) {
			delete hw;
			hw = new hardware(width, height);
		}

		//printf("%s:%s:%d:width:%d, height:%d\n", __FILE__, __func__, __LINE__, width, height);

		return hw;
	}

	int set_log(ft_printf_t ft_printf)
	{
		int ret = 0;

		ret = ft_lib_set_log(ft_printf);

		return ret;
	}

	int set_image(std::string bmp_path)
	{
		int ret = 0;

		ft_set_image(bmp_path.c_str(), (int)bmp_path.size());
		return ret;
	}

	int get_image(char *buffer, int len)
	{
		int ret = len;
		ft_get_image(buffer, len);
		return ret;
	}

	int write_flash(int offset, char *buffer, int len);
	int read_flash(int offset, char *buffer, int len);
};

hardware *hardware::hw = NULL;

//#define HW_GET_INSTANCE() hardware::get_instance(96, 96)

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

public:
	test_task()
	{
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
		logfile_hardware.clear();
		timestamp.clear();
		return ret;
	}

	int add_enroll_item(task_bmp bmp)
	{
		int ret = 0;

		enroll_ids.insert(bmp);
		bmp.current_enroll_id = enroll_ids.size() - 1;

		enroll_ids.insert(bmp);
		enroll_list.push_back(bmp);
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

	int get_timestamp()
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

		len = snprintf(buffer, 1023, "%04d%02d%02d%02d%02d%02d_%06d",
					   tm->tm_year + 1900,
					   tm->tm_mon + 1,
					   tm->tm_mday + 1,
					   tm->tm_hour,
					   tm->tm_min,
					   tm->tm_sec,
					   (int)tv.tv_usec
					  );
		buffer[len] = 0;
		timestamp = buffer;
		return ret;
	}

	int log_file_start()
	{
		int ret = 0;
		char buffer[1024];
		int len = 0;
		filesystem fs;


		len = snprintf(buffer, 1023, "logs/%s.log", timestamp.c_str());
		buffer[len] = 0;
		logfile = buffer;

		len = snprintf(buffer, 1023, "logs/%s_hardware.log", timestamp.c_str());
		buffer[len] = 0;
		logfile_hardware = buffer;

		fs.mkdirs(logfile);
		fs.mkdirs(logfile_hardware);

		ofs.open(logfile.c_str());

		if(!ofs.good()) {
			printf("open:%s failed!!! (%s)!\n", logfile.c_str(), strerror(errno));
			ret = -1;
			return ret;
		}

		ofs_hardware.open(logfile_hardware.c_str());

		if(!ofs_hardware.good()) {
			printf("open:%s failed!!! (%s)!\n", logfile_hardware.c_str(), strerror(errno));
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

	static int log_file_hardware(const char *fmt, ...)
	{
		int ret = 0;
		int len = 0;
		char buffer[1024];
		va_list ap;

		va_start(ap, fmt);
		len = vsnprintf(buffer, 1023, fmt, ap);
		buffer[len] = 0;
		va_end(ap);
		ofs_hardware.write(buffer, len);

		return ret;
	}

	int log_file_end()
	{
		int ret = 0;

		ofs.close();
		ofs_hardware.close();

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

		if(bmp1.catagory < bmp2.catagory) {
			return true;
		} else if(bmp1.catagory == bmp2.catagory) {
			if(bmp1_serial_no < bmp2_serial_no) {
				return true;
			} else if(bmp1_serial_no == bmp2_serial_no) {
				if(bmp1_id < bmp2_id) {
					return true;
				}
			}
		}

		return false;
	}

	int do_task()
	{
		int ret = 0;

		ret = get_timestamp();
		ret = log_file_start();

		std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

		std::sort(enroll_list.begin(), enroll_list.end(), enroll_less_than);
		std::sort(fr_identify_list.begin(), fr_identify_list.end(), identify_less_than);
		std::sort(fa_identify_list.begin(), fa_identify_list.end(), identify_less_than);

		for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
			std::string process = ids_it->catagory + ":" +  ids_it->id;
			log_file("pid:%d, ppid:%d, process:%s\n", (int)getpid(), (int)getppid(), process.c_str());
		}

		log_file("pid:%d, ppid:%d, enroll_list.size():%d\n", (int)getpid(), (int)getppid(), (int)enroll_list.size());
		{
			std::vector<task_bmp>::iterator enroll_list_it;

			for(enroll_list_it = enroll_list.begin(); enroll_list_it != enroll_list.end(); enroll_list_it++) {
				task_bmp bmp = *enroll_list_it;
				log_file("pid:%d, ppid:%d, enroll:catagory:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.catagory.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
				log_file("pid:%d, ppid:%d, path:%s\n\n", (int)getpid(), (int)getppid(), bmp.bmp_path.c_str());

				char *buffer = new char [bmp.bmp_path.size() + 1];
				int len = 0;
				hardware *hw = hardware::get_instance(96, 96);
				hw->set_log((ft_printf_t)&test_task::log_file_hardware);
				hw->set_image(bmp.bmp_path);
				hw->get_image(buffer, (int)bmp.bmp_path.size());
				buffer[bmp.bmp_path.size()] = 0;
				log_file("pid:%d, ppid:%d, buffer:%s\n", (int)getpid(), (int)getppid(), buffer);
				delete buffer;
			}
		}
		log_file("pid:%d, ppid:%d, fr_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fr_identify_list.size());
		{
			std::vector<task_bmp>::iterator identify_list_it;

			for(identify_list_it = fr_identify_list.begin(); identify_list_it != fr_identify_list.end(); identify_list_it++) {
				task_bmp bmp = *identify_list_it;
				log_file("pid:%d, ppid:%d, identify:catagory:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.catagory.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
				log_file("pid:%d, ppid:%d, path:%s\n\n", (int)getpid(), (int)getppid(), bmp.bmp_path.c_str());
			}
		}
		log_file("pid:%d, ppid:%d, fa_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fa_identify_list.size());
		{
			std::vector<task_bmp>::iterator identify_list_it;

			for(identify_list_it = fa_identify_list.begin(); identify_list_it != fa_identify_list.end(); identify_list_it++) {
				task_bmp bmp = *identify_list_it;
				log_file("pid:%d, ppid:%d, identify:catagory:%s, id:%s, serial_no:%s\n", (int)getpid(), (int)getppid(), bmp.catagory.c_str(), bmp.id.c_str(), bmp.serial_no.c_str());
				log_file("pid:%d, ppid:%d, path:%s\n\n", (int)getpid(), (int)getppid(), bmp.bmp_path.c_str());
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

	std::set<task_bmp, bmp_enroll_set_comp> &get_enroll_ids()
	{
		return enroll_ids;
	}

	bool can_start_task(task_start_reason_t reason)
	{
		bool ret = false;
		static int add_catagory_this_proc = 0;
		settings *g_settings = settings::get_instance();

		if(reason == ADD_ID_FINISHED) {
			if(enroll_ids.size() >= g_settings->max_number_of_id_per_proc) {
				ret = true;
			}
		} else if(reason == ADD_CATAGORY_FINISHED) {
			add_catagory_this_proc++;

			if(add_catagory_this_proc >= g_settings->max_number_of_catagory_per_proc) {
				if(enroll_ids.size() > 0) {
					add_catagory_this_proc = 0;

					ret = true;
				}
			}
		}

		return ret;
	}

	int start_task(task_start_reason_t reason, std::string server_path)
	{
		int ret = 0;

		int pid = fork();

		if(pid == -1) {
		} else if(pid == 0) {
			ret = do_task();
			//sleep(1);
			send_client_result(server_path);
			exit(ret);
		} else {
			clear();
			ret = pid;
			return ret;
		}

		return ret;
	}

};

std::ofstream test_task::ofs_hardware;


class samples_list
{
private:
	int server_fd;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *enroll_samples;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fr_samples;
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *fa_samples;

	std::set<int> pid_list;
	int max_proc_number;
	std::string server_path;

public:
	samples_list()
	{
		server_fd = 0;
		enroll_samples = NULL;
		fr_samples = NULL;
		fa_samples = NULL;
		max_proc_number = 32;
		server_path = "/tmp/.server_socket";
	}

	int release_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples)
	{
		int ret = 0;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;

		if(samples != NULL) {
			for(sample_it = samples->begin(); sample_it != samples->end(); sample_it++) {
				std::map<std::string, std::vector<task_bmp> *> *catagory = sample_it->second;
				std::map<std::string, std::vector<task_bmp> *>::iterator catagory_it;

				for(catagory_it = catagory->begin(); catagory_it != catagory->end(); catagory_it++) {
					std::vector<task_bmp> *id = catagory_it->second;

					delete id;
				}

				delete catagory;
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
		std::map<std::string, std::vector<task_bmp> *> *catagory;

		std::map<std::string, std::vector<task_bmp> *>::iterator catagory_it;
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

		sample_it = samples->find(bmp.catagory);

		if(sample_it != samples->end()) {
			catagory = sample_it->second;
		} else {
			catagory = new std::map<std::string, std::vector<task_bmp> *>;
			samples->insert(std::pair<std::string, std::map<std::string, std::vector<task_bmp> *> *>(bmp.catagory, catagory));
		}

		catagory_it = catagory->find(bmp.id);

		if(catagory_it != catagory->end()) {
			id = catagory_it->second;
		} else {
			id = new std::vector<task_bmp>;
			catagory->insert(std::pair<std::string, std::vector<task_bmp> *>(bmp.id, id));
		}

		id->push_back(bmp);

		return ret;
	}

	int get_pattern_order(std::string pattern, int index)
	{
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

	int update_enroll_samples_list(std::vector<std::string> &bmp_file_list)
	{
		int ret = 0;
		settings *g_settings = settings::get_instance();

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			bool matched = false;

			std::vector<std::string> matched_list;

			if((g_settings->enroll_pattern.size() != 0) && (g_settings->enroll_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->enroll_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->enroll_pattern_order, 0)) {
					matched = true;
				}
			}

			if(matched) {
				task_bmp bmp;

				bmp.bmp_path = *it;
				bmp.catagory = matched_list.at(get_pattern_order(g_settings->enroll_pattern_order, 1));
				bmp.id = matched_list.at(get_pattern_order(g_settings->enroll_pattern_order, 2));
				bmp.serial_no = matched_list.at(get_pattern_order(g_settings->enroll_pattern_order, 3));

				//printf("enroll:%s, catagory:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str(), matched_list.at(3).c_str());

				new_type_bmp(&enroll_samples, bmp);
			}
		}

		return ret;
	}

	int update_fr_samples_list(std::vector<std::string> &bmp_file_list)
	{
		int ret = 0;
		settings *g_settings = settings::get_instance();

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::string fr_pattern_order;
			std::vector<std::string> matched_list;
			bool matched = false;

			if((!matched) && (g_settings->fr_pattern.size() != 0) && (g_settings->fr_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fr_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fr_pattern_order, 0)) {
					fr_pattern_order = g_settings->fr_pattern_order;
					bmp.matched_type = UNKNOW;
					matched = true;
				}
			}

			if((!matched) && (g_settings->fr_success_pattern.size() != 0) && (g_settings->fr_success_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fr_success_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fr_success_pattern_order, 0)) {
					fr_pattern_order = g_settings->fr_success_pattern_order;
					bmp.matched_type = MATCHED;
					matched = true;
				}
			}

			if((!matched) && (g_settings->fr_fail_pattern.size() != 0) && (g_settings->fr_fail_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fr_fail_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fr_fail_pattern_order, 0)) {
					fr_pattern_order = g_settings->fr_fail_pattern_order;
					bmp.matched_type = UNMATCHED;
					matched = true;
				}
			}

			if(matched) {
				bmp.bmp_path = *it;
				bmp.catagory = matched_list.at(get_pattern_order(fr_pattern_order, 1));
				bmp.id = matched_list.at(get_pattern_order(fr_pattern_order, 2));
				bmp.serial_no = matched_list.at(get_pattern_order(fr_pattern_order, 3));

				//printf("fr:%s, catagory:%s, id:%s, serial_no:%s, matched_type:%s\n",
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

		return ret;
	}

	int update_fa_samples_list(std::vector<std::string> &bmp_file_list)
	{
		int ret = 0;
		settings *g_settings = settings::get_instance();

		for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
			//printf("file in list:%s\n", it->c_str());
			regexp r;
			task_bmp bmp;
			std::string fa_pattern_order;
			std::vector<std::string> matched_list;
			bool matched = false;

			if((!matched) && (g_settings->fa_pattern.size() != 0) && (g_settings->fa_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fa_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fa_pattern_order, 0)) {
					fa_pattern_order = g_settings->fa_pattern_order;
					bmp.matched_type = UNKNOW;
					matched = true;
				}

			}

			if((!matched) && (g_settings->fa_success_pattern.size() != 0) && (g_settings->fa_success_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fa_success_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fa_success_pattern_order, 0)) {
					fa_pattern_order = g_settings->fa_success_pattern_order;
					bmp.matched_type = MATCHED;
					matched = true;
				}
			}

			if((!matched) && (g_settings->fa_fail_pattern.size() != 0) && (g_settings->fa_fail_pattern_order.size() != 0)) {
				matched_list = r.match(*it, g_settings->fa_fail_pattern);

				if(matched_list.size() == get_pattern_order(g_settings->fa_fail_pattern_order, 0)) {
					fa_pattern_order = g_settings->fa_fail_pattern_order;
					bmp.matched_type = UNMATCHED;
					matched = true;
				}
			}

			if(matched) {
				bmp.bmp_path = *it;
				bmp.catagory = matched_list.at(get_pattern_order(fa_pattern_order, 1));
				bmp.id = matched_list.at(get_pattern_order(fa_pattern_order, 2));
				bmp.serial_no = matched_list.at(get_pattern_order(fa_pattern_order, 3));

				//printf("fa:%s, catagory:%s, id:%s, serial_no:%s, matched_type:%s\n",
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

	int update_samples_list()
	{
		int ret = 0;
		std::vector<std::string> bmp_file_list;
		settings *g_settings = settings::get_instance();

		bmp_file_list = get_bmp_filelist(g_settings->dirname);

		update_enroll_samples_list(bmp_file_list);
		update_fr_samples_list(bmp_file_list);
		update_fa_samples_list(bmp_file_list);

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
				std::map<std::string, std::vector<task_bmp> *> *catagory = sample_it->second;
				std::map<std::string, std::vector<task_bmp> *>::iterator catagory_it;

				//printf("catagory:%s\n", sample_it->first.c_str());
				for(catagory_it = catagory->begin(); catagory_it != catagory->end(); catagory_it++) {
					//printf("id:%s\n", catagory_it->first.c_str());
					std::vector<task_bmp> *id = catagory_it->second;

					std::vector<task_bmp>::iterator id_it;

					for(id_it = id->begin(); id_it != id->end(); id_it++) {
						//printf("serial_no:%s\n", id_it->serial_no.c_str());
						task_bmp bmp = *id_it;

						printf("%s[%d]:%s, catagory:%s, id:%s, serial_no:%s, matched_type:%s\n",
							   type.c_str(),
							   index++,
							   bmp.bmp_path.c_str(),
							   bmp.catagory.c_str(),
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


	int p_result()
	{
		int ret = 0;
		p_type_samples(enroll_samples);
		p_type_samples(fr_samples);
		p_type_samples(fa_samples);
		return ret;
	}

	int add_test_task_catagory(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples,
							   test_task *task,
							   std::string catagory_name,
							   test_type_t test_type)
	{
		int ret = 0;

		std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *>::iterator sample_it;
		settings *g_settings = settings::get_instance();

		if(samples == NULL) {
			ret = -1;
			return ret;
		}

		for(sample_it = samples->begin(); sample_it != samples->end(); sample_it++) {
			std::map<std::string, std::vector<task_bmp> *> *catagory = sample_it->second;
			std::map<std::string, std::vector<task_bmp> *>::iterator catagory_it;

			//printf("catagory:%s\n", sample_it->first.c_str());
			for(catagory_it = catagory->begin(); catagory_it != catagory->end(); catagory_it++) {
				//printf("id:%s\n", catagory_it->first.c_str());
				std::vector<task_bmp> *id = catagory_it->second;

				std::vector<task_bmp>::iterator id_it;

				for(id_it = id->begin(); id_it != id->end(); id_it++) {
					//printf("serial_no:%s\n", id_it->serial_no.c_str());
					task_bmp bmp = *id_it;
					bool add = false;
					select_type_t select_type;

					bmp.test_type = test_type;

					if(bmp.test_type == FOR_FR) {
						select_type = g_settings->fr_select_type;
					} else if(bmp.test_type == FOR_FA) {
						select_type = g_settings->fa_select_type;
					}

					if(select_type == SELECT_ALL) {
						add = true;
					} else if(select_type == SELECT_SAME_CATAGORY) {
						if(bmp.catagory.compare(catagory_name) == 0) {
							add = true;
						}
					}

					if(add) {
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

	int try_to_start_task_and_wait(test_task *task, task_start_reason_t reason, bool wait = false)
	{
		int ret = 0;
		bool ready = task->can_start_task(reason);

		if(ready) {
			std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;
			std::set<task_bmp, bmp_enroll_set_comp> ids = task->get_enroll_ids();
			std::set<std::string> set_catagory;
			std::set<std::string>::iterator catagory_it;

			for(ids_it = ids.begin(); ids_it != ids.end(); ids_it++) {
				task_bmp bmp = *ids_it;
				set_catagory.insert(bmp.catagory);
			}

			for(catagory_it = set_catagory.begin(); catagory_it != set_catagory.end(); catagory_it++) {
				printf("add catagory:%s\n", catagory_it->c_str());
				add_test_task_catagory(fr_samples, task, *catagory_it, FOR_FR);
				add_test_task_catagory(fa_samples, task, *catagory_it, FOR_FA);
			}

			ret = task->start_task(reason, server_path);

			if(ret != 0) {
				printf("pid start:%d(%x)\n", ret, ret);
				pid_list.insert(ret);
			}
		}


		while((pid_list.size() >= max_proc_number) || (wait && (pid_list.size() > 0))) {
			int pid = get_client_result();
			printf("pid stop:%d(%x)\n", pid, pid);
			pid_list.erase(pid);
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
				std::map<std::string, std::vector<task_bmp> *> *catagory = sample_it->second;
				std::map<std::string, std::vector<task_bmp> *>::iterator catagory_it;

				//printf("catagory:%s\n", sample_it->first.c_str());
				for(catagory_it = catagory->begin(); catagory_it != catagory->end(); catagory_it++) {
					//printf("id:%s\n", catagory_it->first.c_str());
					std::vector<task_bmp> *id = catagory_it->second;

					std::vector<task_bmp>::iterator id_it;

					for(id_it = id->begin(); id_it != id->end(); id_it++) {
						//printf("serial_no:%s\n", id_it->serial_no.c_str());
						task_bmp bmp = *id_it;
						bmp.test_type = FOR_ENROLL;
						ret = task.add_enroll_item(bmp);
					}

					try_to_start_task_and_wait(&task, ADD_ID_FINISHED);
				}

				try_to_start_task_and_wait(&task, ADD_CATAGORY_FINISHED);

			}

		}

		try_to_start_task_and_wait(&task, ADD_CATAGORY_FINISHED, true);

		return ret;
	}

};

int main(int argc, char **argv)
{
	int ret = 0;

	settings *g_settings = settings::get_instance();
	ret = g_settings->parse_args_from_configuration(argc, argv);

	if(ret != 0) {
		return ret;
	}

	samples_list samples_list;
	ret = samples_list.update_samples_list();

	if(ret != 0) {
		return ret;
	}

	//samples_list.p_result();
	samples_list.start_test_task();

	printf("Done!\n");

	return ret;
}

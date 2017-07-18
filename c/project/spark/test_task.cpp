

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：test_task.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时46分17秒
 *   修改日期：2017年07月18日 星期二 17时09分45秒
 *   描    述：
 *
 *================================================================*/
#include "test_task.h"

test_task::test_task()
{
	clear();
}

test_task::~test_task()
{
	clear();
}

int test_task::clear()
{
	int ret = 0;
	enroll_list.clear();
	fr_identify_list.clear();
	fa_identify_list.clear();
	enroll_ids.clear();
	logfile.clear();
	timestamp.clear();
	return ret;
}

int test_task::add_enroll_item(task_bmp bmp)
{
	int ret = 0;

	enroll_ids.insert(bmp);
	bmp.current_enroll_id = enroll_ids.size() - 1;

	enroll_ids.erase(bmp);
	enroll_ids.insert(bmp);
	enroll_list.push_back(bmp);
	return ret;
}

int test_task::add_identify_item(task_bmp bmp)
{
	int ret = 0;

	if(bmp.test_type == FOR_FR) {
		fr_identify_list.push_back(bmp);
	} else if(bmp.test_type == FOR_FA) {
		fa_identify_list.push_back(bmp);
	}

	return ret;
}

int test_task::get_timestamp()
{
	int ret = 0;
	char buffer[1024];
	int len = 0;
	struct tm *tm;
	struct timeval tv;
	struct timezone tz;

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

int test_task::gen_log_file_names()
{
	int ret = 0;
	char buffer[1024];
	int len = 0;

	len = snprintf(buffer, 1023, "logs/%s.log", timestamp.c_str());
	buffer[len] = 0;
	logfile = buffer;

	len = snprintf(buffer, 1023, "logs/%s_hardware.log", timestamp.c_str());
	buffer[len] = 0;
	logfile_hardware = buffer;

	return ret;
}
int test_task::log_file_start()
{
	int ret = 0;
	filesystem fs;

	if(logfile.size() == 0) {
		ret = -1;
		return ret;
	}

	fs.mkdirs(logfile);
	ofs.open(logfile.c_str());

	if(!ofs.good()) {
		printf("open:%s failed!!! (%s)!\n", logfile.c_str(), strerror(errno));
		ret = -1;
		return ret;
	}

	return ret;
}

int test_task::log_file(const char *fmt, ...)
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

int test_task::log_file_end()
{
	int ret = 0;

	ofs.close();

	return ret;
}

bool test_task::enroll_less_than(task_bmp bmp1, task_bmp bmp2)
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

bool test_task::identify_less_than(task_bmp bmp1, task_bmp bmp2)
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

int test_task::pre_task()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();

	hw->set_save_bmp();

	ret = get_timestamp();
	ret = gen_log_file_names();

	ret = hw->set_log_file_name(logfile_hardware);

	ret = log_file_start();
	ret = hw->log_file_start();

	ret = hw->hardware_init();

	return ret;
}

int test_task::do_task_list()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();


	std::sort(enroll_list.begin(), enroll_list.end(), enroll_less_than);
	std::sort(fr_identify_list.begin(), fr_identify_list.end(), identify_less_than);
	std::sort(fa_identify_list.begin(), fa_identify_list.end(), identify_less_than);

	//enroll
	{
		std::vector<task_bmp>::iterator enroll_list_it;

		unsigned short finger_id_pre = 0;
		unsigned short index = 0;

		for(enroll_list_it = enroll_list.begin(); enroll_list_it != enroll_list.end(); enroll_list_it++) {
			char *invalid_pos;
			unsigned short finger_id = enroll_list_it->current_enroll_id;
			unsigned char enroll_coverage;

			hw->set_image(enroll_list_it->bmp_path);

			if(finger_id != finger_id_pre) {
				finger_id_pre = finger_id;
				index = 0;
			}

			ret = hw->enroll(finger_id, index, &enroll_coverage);

			if(ret != 0) {
				log_file("pid:%d, ppid:%d, enroll error code:%d, enroll_coverage:%x\n", (int)getpid(), (int)getppid(), ret, enroll_coverage);
			}

			index++;
		}
	}
	//fr test
	{
		std::vector<task_bmp>::iterator identify_list_it;

		for(identify_list_it = fr_identify_list.begin(); identify_list_it != fr_identify_list.end(); identify_list_it++) {
			unsigned char finger_id = 0;
			unsigned char update_template = 0;

			hw->set_image(identify_list_it->bmp_path);

			ret = hw->idnetify(&finger_id, &update_template);

			if(ret == 0) {
				std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

				identify_list_it->new_matched_type = MATCHED;

				for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
					if(ids_it->current_enroll_id == finger_id) {
						identify_list_it->new_matched_catagory = ids_it->catagory;
						identify_list_it->new_matched_id = ids_it->id;
					}
				}
			} else {
				identify_list_it->new_matched_type = UNMATCHED;
			}
		}

		//char *buffer = new char[96 * 96];
		//hw->get_image(buffer, 96 * 96);
		//delete buffer;
	}
	//fa test
	{
		std::vector<task_bmp>::iterator identify_list_it;

		for(identify_list_it = fa_identify_list.begin(); identify_list_it != fa_identify_list.end(); identify_list_it++) {
			unsigned char finger_id = 0;
			unsigned char update_template = 0;

			hw->set_image(identify_list_it->bmp_path);

			ret = hw->idnetify(&finger_id, &update_template);

			if(ret == 0) {
				std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

				identify_list_it->new_matched_type = MATCHED;

				for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
					if(ids_it->current_enroll_id == finger_id) {
						identify_list_it->new_matched_catagory = ids_it->catagory;
						identify_list_it->new_matched_id = ids_it->id;
					}
				}
			}
		}

		return ret;
	}
}

int test_task::report_task()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();

	{
		std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

		for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
			std::string process = ids_it->catagory + ":" +  ids_it->id;
			log_file("pid:%d, ppid:%d, process:%s\n", (int)getpid(), (int)getppid(), process.c_str());
		}
	}

	log_file("pid:%d, ppid:%d, enroll_list.size():%d\n", (int)getpid(), (int)getppid(), (int)enroll_list.size());
	{
		std::vector<task_bmp>::iterator enroll_list_it;

		for(enroll_list_it = enroll_list.begin(); enroll_list_it != enroll_list.end(); enroll_list_it++) {
			log_file("enroll:catagory:%s, id:%s, serial_no:%s, current_enroll_id:%d\n",
					 enroll_list_it->catagory.c_str(),
					 enroll_list_it->id.c_str(),
					 enroll_list_it->serial_no.c_str(),
					 enroll_list_it->current_enroll_id);
			log_file("path:%s\n\n", enroll_list_it->bmp_path.c_str());
		}
	}

	log_file("pid:%d, ppid:%d, fr_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fr_identify_list.size());
	{
		std::vector<task_bmp>::iterator identify_list_it;

		for(identify_list_it = fr_identify_list.begin(); identify_list_it != fr_identify_list.end(); identify_list_it++) {
			log_file("fr:catagory:%s, id:%s, serial_no:%s, match_state:%s, new_match_state:%s, new_matched_catagory:%s, new_matched_id:%s\n",
					 identify_list_it->catagory.c_str(),
					 identify_list_it->id.c_str(),
					 identify_list_it->serial_no.c_str(),
					 identify_list_it->get_matched_type().c_str(),
					 identify_list_it->get_new_matched_type().c_str(),
					 identify_list_it->new_matched_catagory.c_str(),
					 identify_list_it->new_matched_id.c_str());
			log_file("path:%s\n\n", identify_list_it->bmp_path.c_str());
		}
	}

	log_file("pid:%d, ppid:%d, fa_identify_list.size():%d\n", (int)getpid(), (int)getppid(), (int)fa_identify_list.size());
	{
		std::vector<task_bmp>::iterator identify_list_it;

		for(identify_list_it = fa_identify_list.begin(); identify_list_it != fa_identify_list.end(); identify_list_it++) {
			log_file("fa:catagory:%s, id:%s, serial_no:%s, match_state:%s, new_match_state:%s, new_matched_catagory:%s, new_matched_id:%s\n",
					 identify_list_it->catagory.c_str(),
					 identify_list_it->id.c_str(),
					 identify_list_it->serial_no.c_str(),
					 identify_list_it->get_matched_type().c_str(),
					 identify_list_it->get_new_matched_type().c_str(),
					 identify_list_it->new_matched_catagory.c_str(),
					 identify_list_it->new_matched_id.c_str());
			log_file("path:%s\n\n", identify_list_it->bmp_path.c_str());
		}
	}
	return ret;
}

int test_task::post_task()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();

	hw->log_file_end();

	log_file_end();
	return ret;
}

int test_task::do_task()
{
	int ret = 0;

	ret = pre_task();
	ret = do_task_list();
	ret = report_task();
	ret = post_task();

	return ret;
}

int test_task::send_client_result(std::string server_path)
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

std::set<task_bmp, bmp_enroll_set_comp> &test_task::get_enroll_ids()
{
	return enroll_ids;
}

bool test_task::can_start_task(task_start_reason_t reason)
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

int test_task::start_task(task_start_reason_t reason, std::string server_path)
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

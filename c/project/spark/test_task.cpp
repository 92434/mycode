

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：test_task.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时46分17秒
 *   修改日期：2017年08月02日 星期三 15时23分11秒
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
	current_enroll_ids.clear();

	fr_total_tasks = 0;
	fa_total_tasks = 0;
	total_tasks = 0;
	current_tasks = 0;
	start_time = 0;
	current_percent = 0;

	fr_fail_count = 0;
	fa_success_count = 0;
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

int test_task::gen_log_file_names()
{
	int ret = 0;
	struct timeval tv;
	settings *g_settings = settings::get_instance();
	char buffer[BUFFER_LEN];
	int len = 0;

	g_settings->get_time_val(&tv);

	start_time = tv.tv_sec;

	timestamp = g_settings->get_timestamp();

	len = snprintf(buffer, BUFFER_LEN, "logs/%s/%s.log", g_settings->log_dirname.c_str(), timestamp.c_str());
	buffer[len] = 0;
	logfile = buffer;

	len = snprintf(buffer, BUFFER_LEN, "logs/%s/%s_hardware.log", g_settings->log_dirname.c_str(), timestamp.c_str());
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
	char buffer[BUFFER_LEN];
	va_list ap;

	va_start(ap, fmt);
	len = vsnprintf(buffer, BUFFER_LEN, fmt, ap);
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

int test_task::account_task(int tasks)
{
	int ret = 0;
	int percent;
	int elapsed_time;
	int left_time;

	current_tasks += tasks;

	if(total_tasks != 0) {
		percent = current_tasks * 100 / total_tasks;
	}

	if((percent / 10) != (current_percent / 10)) {
		struct timeval tv;
		struct timezone tz;

		current_percent = percent;
		gettimeofday(&tv, &tz);
		elapsed_time = tv.tv_sec - start_time;

		if(current_tasks != 0) {
			char buffer[BUFFER_LEN];
			int len;
			left_time = elapsed_time * (total_tasks - current_tasks) / current_tasks;
			len = snprintf(buffer, BUFFER_LEN, "processed %d/%d(%d%%) %d seconds left!", current_tasks, total_tasks, percent, left_time);
			buffer[len] = 0;
			send_client_result(REPORT_PROCESS, buffer);
		}
	}

	return ret;
}

int test_task::pre_task()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();

	hw->set_save_bmp();

	ret = gen_log_file_names();

	ret = hw->set_log_file_name(logfile_hardware);

	ret = log_file_start();
	ret = hw->log_file_start();

	ret = hw->hardware_init();

	return ret;
}

int test_task::task_enroll_id(int finger_id, std::vector<task_bmp> &enroll_id_list, int start_index, int number)
{
	int ret = 0;
	hardware *hw = hardware::get_instance();
	bool have_valid_template = false;

	int i;
	int size = (int)enroll_id_list.size();

	ret = hw->delete_template(finger_id);

	if(size <= start_index) {
		ret = -1;
		return ret;
	}

	if(size > start_index + number) {
		size = start_index + number;
	}

	for(i = start_index; i < size; i++) {
		unsigned char enroll_coverage;
		task_bmp bmp = enroll_id_list.at(i);

		ret = hw->set_image(bmp.bmp_path);

		if(ret == 0) {
			ret = hw->enroll(finger_id, i - start_index, &enroll_coverage);

			if(ret == 0) {
				have_valid_template = true;
				current_enroll_ids.insert(bmp);
			}
		}


		bmp.ret_code = ret;
		account_task(1);

		log_file("enroll:current_enroll_id:%d, catagory:%s, id:%s, serial_no:%s, path:%s, ret_code:%d\n",
				 bmp.current_enroll_id,
				 bmp.catagory.c_str(),
				 bmp.id.c_str(),
				 bmp.serial_no.c_str(),
				 bmp.bmp_path.c_str(),
				 bmp.ret_code);
	}

	if(have_valid_template) {
		ret = hw->save_one_template(finger_id);
	} else {
		ret = -1;
	}

	return ret;
}

int test_task::task_verify(std::vector<task_bmp> &identify_list, test_type_t test_type)
{
	int ret = 0;
	hardware *hw = hardware::get_instance();
	settings *g_settings = settings::get_instance();
	std::vector<task_bmp>::iterator identify_list_it;

	unsigned char finger_id = 0;
	unsigned char update_template = 0;

	unsigned char update_template_outside = 0;
	int update_template_finger_id = 0;

	unsigned char *pupdate_template_outside;
	int *pupdate_template_finger_id;
	std::string label;

	if(test_type == FOR_FR) {
		label = "fr";
		pupdate_template_outside = &update_template_outside;
		pupdate_template_finger_id = &update_template_finger_id;
	} else {
		label = "fa";
		pupdate_template_outside = NULL;
		pupdate_template_finger_id = NULL;
	}

	for(identify_list_it = identify_list.begin(); identify_list_it != identify_list.end(); identify_list_it++) {
		finger_id = 0;
		update_template = 0;
		update_template_outside = 0;
		update_template_finger_id = 0;

		ret = hw->set_image(identify_list_it->bmp_path);

		if(ret == 0) {
			ret = hw->identify(&finger_id, &update_template, pupdate_template_outside, pupdate_template_finger_id);
		}

		identify_list_it->ret_code = ret;

		if(ret == 0) {
			std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

			identify_list_it->new_matched_type = MATCHED;

			if((update_template == 1) || (update_template_outside == 1)) {
				identify_list_it->update_template = 1;
			}

			for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
				if(ids_it->current_enroll_id == finger_id) {
					identify_list_it->new_matched_catagory = ids_it->catagory;
					identify_list_it->new_matched_id = ids_it->id;
				}

				if((identify_list_it->update_template == 1) && (ids_it->current_enroll_id == update_template_finger_id)) {
					identify_list_it->update_template_catagory = ids_it->catagory;
					identify_list_it->update_template_id = ids_it->id;
				}
			}

			if(test_type == FOR_FA) {
				if(g_settings->strict_identify_mode == 0) {
					fa_success_count++;
				} else {
					if((identify_list_it->catagory.compare(identify_list_it->new_matched_catagory) == 0)
					   && (identify_list_it->id.compare(identify_list_it->new_matched_id) == 0)) {
					} else {
						fa_success_count++;
					}
				}
			}
		} else {
			std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;

			if(test_type == FOR_FR) {
				if(g_settings->strict_identify_mode == 0) {
					fr_fail_count++;
				} else {
					for(ids_it = current_enroll_ids.begin(); ids_it != current_enroll_ids.end(); ids_it++) {
						if((identify_list_it->catagory.compare(ids_it->catagory) == 0)
						   && (identify_list_it->id.compare(ids_it->id) == 0)) {
							fr_fail_count++;
							break;
						} else {
						}
					}
				}

			}

			identify_list_it->new_matched_type = UNMATCHED;
		}

		account_task(1);

		log_file("%s:catagory:%s, id:%s, serial_no:%s, match_state:%s, new_match_state:%s, new_matched_catagory:%s, new_matched_id:%s, update_template_catagory:%s, update_template_id:%s, path:%s, ret_code:%d\n",
				 label.c_str(),
				 identify_list_it->catagory.c_str(),
				 identify_list_it->id.c_str(),
				 identify_list_it->serial_no.c_str(),
				 identify_list_it->get_matched_type().c_str(),
				 identify_list_it->get_new_matched_type().c_str(),
				 identify_list_it->new_matched_catagory.c_str(),
				 identify_list_it->new_matched_id.c_str(),
				 identify_list_it->update_template_catagory.c_str(),
				 identify_list_it->update_template_id.c_str(),
				 identify_list_it->bmp_path.c_str(),
				 identify_list_it->ret_code);
	}

	return ret;
}

int test_task::report_result()
{
	int ret = 0;
	char buffer[BUFFER_LEN];
	int len;
	hardware *hw = hardware::get_instance();

	if(fr_total_tasks > 0) {
		len = snprintf(buffer, BUFFER_LEN, "fr:%d:%d", fr_fail_count, fr_total_tasks);
		buffer[len] = 0;
		send_client_result(REPORT_RESULT, buffer);
		log_file("pid:%d, ppid:%d, fr result:%d/%d(%f%%)\n", (int)getpid(), (int)getppid(), fr_fail_count, fr_total_tasks, fr_fail_count * 100.0 / fr_total_tasks);
	}

	if(fa_total_tasks > 0) {
		len = snprintf(buffer, BUFFER_LEN, "fa:%d:%d", fa_success_count, fa_total_tasks);
		buffer[len] = 0;
		send_client_result(REPORT_RESULT, buffer);
		log_file("pid:%d, ppid:%d, fr result:%d/%d(%f%%)\n", (int)getpid(), (int)getppid(), fa_success_count, fa_total_tasks, fa_success_count * 100.0 / fa_total_tasks);
	}

	return ret;
}

int test_task::do_task_list()
{
	int ret = 0;
	hardware *hw = hardware::get_instance();
	settings *g_settings = settings::get_instance();
	std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;
	std::vector<task_bmp>::iterator enroll_list_it;
	std::map<int, std::vector<task_bmp> > enroll_id_map;
	std::map<int, std::vector<task_bmp> >::iterator enroll_id_map_it;
	int enroll_id_size_max = 0;
	int enroll_max_templates = (int)g_settings->value_strtod(g_settings->enroll_max_templates);
	int enroll_loops;
	int i;
	bool have_valid_template = false;

	std::sort(enroll_list.begin(), enroll_list.end(), enroll_less_than);
	std::sort(fr_identify_list.begin(), fr_identify_list.end(), identify_less_than);
	std::sort(fa_identify_list.begin(), fa_identify_list.end(), identify_less_than);

	for(ids_it = enroll_ids.begin(); ids_it != enroll_ids.end(); ids_it++) {
		std::string process = ids_it->catagory + ":" +  ids_it->id;
		log_file("pid:%d, ppid:%d, process:%s\n", (int)getpid(), (int)getppid(), process.c_str());
	}

	for(enroll_list_it = enroll_list.begin(); enroll_list_it != enroll_list.end(); enroll_list_it++) {
		enroll_id_map[enroll_list_it->current_enroll_id].push_back(*enroll_list_it);
	}

	for(enroll_id_map_it = enroll_id_map.begin(); enroll_id_map_it != enroll_id_map.end(); enroll_id_map_it++) {
		if(enroll_id_size_max < enroll_id_map_it->second.size()) {
			enroll_id_size_max = enroll_id_map_it->second.size();
		}
	}

	enroll_loops = (enroll_id_size_max + enroll_max_templates - 1) / enroll_max_templates;

	fr_total_tasks = fr_identify_list.size() * enroll_loops;
	fa_total_tasks = fa_identify_list.size() * enroll_loops;
	total_tasks = enroll_list.size() + fr_total_tasks + fa_total_tasks;

	for(i = 0; i < enroll_loops; i++) {
		current_enroll_ids.clear();

		for(enroll_id_map_it = enroll_id_map.begin(); enroll_id_map_it != enroll_id_map.end(); enroll_id_map_it++) {
			ret = task_enroll_id(enroll_id_map_it->first, enroll_id_map_it->second, i * enroll_max_templates, enroll_max_templates);

			if(ret == 0) {
				have_valid_template = true;
			}
		}

		if(have_valid_template) {
			task_verify(fr_identify_list, FOR_FR);
			task_verify(fa_identify_list, FOR_FA);
		} else {
			account_task(fr_identify_list.size());
			account_task(fa_identify_list.size());
		}
	}

	report_result();
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
	ret = post_task();

	return ret;
}

int test_task::send_client_result(report_type_t report_type, std::string content)
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
	notifier.type = report_type;
	notifier.pid = getpid();
	len = snprintf(notifier.buffer, BUFFER_LEN, "%s", content.c_str());
	notifier.buffer[len] = 0;
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

int test_task::start_task(std::string server_path)
{
	int ret = 0;

	int pid = fork();

	this->server_path = server_path;

	if(pid == -1) {
	} else if(pid == 0) {
		ret = do_task();
		//sleep(1);
		send_client_result(REPORT_EXIT, logfile);
		exit(ret);
	} else {
		clear();
		ret = pid;
		return ret;
	}

	return ret;
}



/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：samples_list.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时38分19秒
 *   修改日期：2017年08月15日 星期二 10时47分17秒
 *   描    述：
 *
 *================================================================*/
#include "samples_list.h"

int samples_list::gen_server_path()
{
	int ret = 0;
	char buffer[BUFFER_LEN];
	int len = 0;
	len = snprintf(buffer, BUFFER_LEN, "/tmp/.server_socket.pid%d", getpid());
	buffer[len] = 0;
	server_path = buffer;
	return ret;
}

samples_list::samples_list()
{
	server_fd = 0;
	enroll_samples = NULL;
	fr_samples = NULL;
	fa_samples = NULL;
	gen_server_path();

	fr_fail_count = 0;
	fr_total_count = 0;
	fa_success_count = 0;
	fa_total_count = 0;
}

int samples_list::release_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples)
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

samples_list::~samples_list()
{
	release_type_samples(enroll_samples);
	release_type_samples(fr_samples);
	release_type_samples(fa_samples);
}

std::vector<std::string> samples_list::get_bmp_filelist(std::string dirname)
{
	std::vector<std::string> bmp_file_list;
	std::vector<std::string> filter_list;
	filesystem fs;

	filter_list.push_back(".bmp");
	bmp_file_list = fs.dir_scan_files(dirname, filter_list);

	return bmp_file_list;
}

int samples_list::new_type_bmp(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> **p_samples, task_bmp bmp)
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

int samples_list::get_pattern_order(std::string pattern, int index)
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

int samples_list::update_sub_samples_list(
	std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> **samples,
	std::vector<std::string> &bmp_file_list,
	std::vector<pattern_info> &vec_pattern)
{
	int ret = 0;
	settings *g_settings = settings::get_instance();
	regexp r;
	std::vector<std::string> matched_list;

	for(std::vector<std::string>::iterator it = bmp_file_list.begin(); it != bmp_file_list.end(); it++) {
		//printf("file in list:%s\n", it->c_str());


		std::vector<pattern_info>::iterator vec_pattern_it;

		for(
			vec_pattern_it = vec_pattern.begin();
			vec_pattern_it != vec_pattern.end();
			vec_pattern_it++) {

			pattern_info pattern_info = *vec_pattern_it;

			if((pattern_info.pattern.size() != 0) && (pattern_info.pattern_order.size() != 0)) {
				matched_list = r.match(*it, pattern_info.pattern);

				if(matched_list.size() == get_pattern_order(pattern_info.pattern_order, 0)) {
					task_bmp bmp;

					bmp.bmp_path = *it;
					bmp.matched_type = UNKNOW;
					bmp.catagory = matched_list.at(get_pattern_order(pattern_info.pattern_order, 1));
					bmp.id = matched_list.at(get_pattern_order(pattern_info.pattern_order, 2));
					bmp.serial_no = matched_list.at(get_pattern_order(pattern_info.pattern_order, 3));

					//printf("enroll:%s, catagory:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str(), matched_list.at(3).c_str());

					new_type_bmp(samples, bmp);

					break;
				}
			}

			if((pattern_info.success_pattern.size() != 0) && (pattern_info.success_pattern_order.size() != 0)) {
				matched_list = r.match(*it, pattern_info.success_pattern);

				if(matched_list.size() == get_pattern_order(pattern_info.success_pattern_order, 0)) {
					task_bmp bmp;

					bmp.bmp_path = *it;
					bmp.matched_type = MATCHED;
					bmp.catagory = matched_list.at(get_pattern_order(pattern_info.success_pattern_order, 1));
					bmp.id = matched_list.at(get_pattern_order(pattern_info.success_pattern_order, 2));
					bmp.serial_no = matched_list.at(get_pattern_order(pattern_info.success_pattern_order, 3));

					//printf("enroll:%s, catagory:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str(), matched_list.at(3).c_str());

					new_type_bmp(samples, bmp);

					break;
				}
			}

			if((pattern_info.fail_pattern.size() != 0) && (pattern_info.fail_pattern_order.size() != 0)) {
				matched_list = r.match(*it, pattern_info.fail_pattern);

				if(matched_list.size() == get_pattern_order(pattern_info.fail_pattern_order, 0)) {
					task_bmp bmp;

					bmp.bmp_path = *it;
					bmp.matched_type = UNMATCHED;
					bmp.catagory = matched_list.at(get_pattern_order(pattern_info.fail_pattern_order, 1));
					bmp.id = matched_list.at(get_pattern_order(pattern_info.fail_pattern_order, 2));
					bmp.serial_no = matched_list.at(get_pattern_order(pattern_info.fail_pattern_order, 3));

					//printf("enroll:%s, catagory:%s, id:%s, serial_no:%s\n", it->c_str(), matched_list.at(1).c_str(), matched_list.at(2).c_str(), matched_list.at(3).c_str());

					new_type_bmp(samples, bmp);

					break;
				}
			}
		}

	}

	return ret;
}

int samples_list::update_samples_list()
{
	int ret = 0;
	std::vector<std::string> bmp_file_list;
	settings *g_settings = settings::get_instance();

	bmp_file_list = get_bmp_filelist(g_settings->pictures_dirname);

	update_sub_samples_list(&enroll_samples, bmp_file_list, g_settings->enroll_pattern);
	update_sub_samples_list(&fr_samples, bmp_file_list, g_settings->fr_pattern);
	update_sub_samples_list(&fa_samples, bmp_file_list, g_settings->fa_pattern);

	return ret;
}

int samples_list::p_type_samples(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples)
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


int samples_list::p_result()
{
	int ret = 0;
	p_type_samples(enroll_samples);
	p_type_samples(fr_samples);
	p_type_samples(fa_samples);
	return ret;
}

int samples_list::add_test_task_catagory(std::map<std::string, std::map<std::string, std::vector<task_bmp> *> *> *samples,
		test_task *task,
		std::set<task_bmp, bmp_enroll_set_comp> &enroll_ids,
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
				std::set<task_bmp, bmp_enroll_set_comp>::iterator enroll_ids_it;

				bmp.test_type = test_type;

				if(bmp.test_type == FOR_FR) {
					select_type = g_settings->fr_select_type;
				} else if(bmp.test_type == FOR_FA) {
					select_type = g_settings->fa_select_type;
				}

				if(select_type == SELECT_ALL) {
					add = true;
				} else if(select_type == SELECT_SAME_ID) {
					for(enroll_ids_it = enroll_ids.begin(); enroll_ids_it != enroll_ids.end(); enroll_ids_it++) {
						if(bmp.catagory.compare(enroll_ids_it->catagory) == 0) {
							if(bmp.id.compare(enroll_ids_it->id) == 0) {
								add = true;
								break;
							}
						}
					}
				} else if(select_type == SELECT_SAME_CATAGORY) {
					for(enroll_ids_it = enroll_ids.begin(); enroll_ids_it != enroll_ids.end(); enroll_ids_it++) {
						if(bmp.catagory.compare(enroll_ids_it->catagory) == 0) {
							add = true;
							break;
						}
					}
				} else if(select_type == SELECT_DIFFERENT_ID) {
					add = true;

					for(enroll_ids_it = enroll_ids.begin(); enroll_ids_it != enroll_ids.end(); enroll_ids_it++) {
						if(bmp.catagory.compare(enroll_ids_it->catagory) == 0) {
							if(bmp.id.compare(enroll_ids_it->id) == 0) {
								add = false;
								break;
							}
						}
					}
				} else if(select_type == SELECT_DIFFERENT_CATAGORY) {
					add = true;

					for(enroll_ids_it = enroll_ids.begin(); enroll_ids_it != enroll_ids.end(); enroll_ids_it++) {
						if(bmp.catagory.compare(enroll_ids_it->catagory) == 0) {
							add = false;
							break;
						}
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

int samples_list::create_server()
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

int samples_list::stop_server()
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

int samples_list::parse_pid_result(char *buffer)
{
	int ret = 0;
	std::string content = buffer;
	std::string pattern = "([^:]+):([^:]+):([^:]+)";
	std::vector<std::string> matched_list;
	regexp r;
	settings *g_settings = settings::get_instance();

	matched_list = r.match(content, pattern);

	if(matched_list.size() == 4) {
		if(matched_list.at(1) == "fr") {
			fr_fail_count += (int)g_settings->value_strtod(matched_list.at(2));
			fr_total_count += (int)g_settings->value_strtod(matched_list.at(3));
		} else if(matched_list.at(1) == "fa") {
			fa_success_count += (int)g_settings->value_strtod(matched_list.at(2));
			fa_total_count += (int)g_settings->value_strtod(matched_list.at(3));
		}
	}

	return ret;
}

int samples_list::report_result()
{
	int ret = 0;
	settings *g_settings = settings::get_instance();

	gen_log_file_names();

	log_file_start();

	log_file("configuration_name:%s,", g_settings->configure_file.c_str());
	log_file("test_type:%s,", g_settings->test_type.c_str());
	log_file("log_dirname:%s,", g_settings->log_dirname.c_str());
	log_file("pictures_directory:%s,", g_settings->pictures_dirname.c_str());
	log_file("debug_switch:%s,", g_settings->debug_switch.c_str());
	log_file("algorithm_mode:%s,", g_settings->algorithm_mode.c_str());
	log_file("enroll_max_templates:%s,", g_settings->enroll_max_templates.c_str());
	log_file("algorithm_max_templates:%s,", g_settings->algorithm_max_templates.c_str());
	log_file("spa_enable:%s,", g_settings->spa_enable.c_str());
	log_file("spa_ratio:%s,", g_settings->spa_ratio.c_str());
	log_file("algorithm_far_level:%s,", g_settings->algorithm_far_level.c_str());
	log_file("update_template_far_level:%s,", g_settings->update_template_far_level.c_str());
	log_file("update_template_threshold:%s,", g_settings->update_template_threshold.c_str());
	log_file("verify_quickly_enable:%s,", g_settings->verify_quickly_enable.c_str());
	log_file("update_template_outside_enable:%s,", g_settings->update_template_outside_enable.c_str());
	log_file("image_quality_score:%s,", g_settings->image_quality_score.c_str());
	log_file("verify_image_quality_score:%s,", g_settings->verify_image_quality_score.c_str());
	log_file("enroll_duplicate_area_check_enable:%s,", g_settings->enroll_duplicate_area_check_enable.c_str());
	log_file("valid_area_scale:%s,", g_settings->valid_area_scale.c_str());
	log_file("enrollment_tips_enable:%s,", g_settings->enrollment_tips_enable.c_str());
	log_file("enrollment_tips_parameter1:%s,", g_settings->enrollment_tips_parameter1.c_str());
	log_file("enrollment_tips_parameter2:%s,", g_settings->enrollment_tips_parameter2.c_str());
	log_file("enrollment_tips_parameter3:%s,", g_settings->enrollment_tips_parameter3.c_str());
	log_file("verify_improve_enable:%s,", g_settings->verify_improve_enable.c_str());
	log_file("verify_improve_level:%s,", g_settings->verify_improve_level.c_str());
	log_file("mcu_image_bit:%s,", g_settings->mcu_image_bit.c_str());
	log_file("mcu_interrupt_mode:%s,", g_settings->mcu_interrupt_mode.c_str());
	log_file("mcu_state_check_mode:%s,", g_settings->mcu_state_check_mode.c_str());
	log_file("repeat_get_image_count:%s,", g_settings->repeat_get_image_count.c_str());
	log_file("template_buffer_enable:%s,", g_settings->template_buffer_enable.c_str());
	log_file("transfer_bytes_max:%s,", g_settings->transfer_bytes_max.c_str());
	log_file("config_debuginfo_switch:%s,", g_settings->config_debuginfo_switch.c_str());
	log_file("use_dyn_far:%s,", g_settings->use_dyn_far.c_str());

	if(fr_total_count > 0) {
		printf("fr result:%d/%d(%f%%)\n", fr_fail_count, fr_total_count, fr_fail_count * 100.0 / fr_total_count);
		log_file("fr_result:%d/%d,", fr_fail_count, fr_total_count);
		log_file("frr:%f%%,", fr_fail_count * 100.0 / fr_total_count);
	}

	if(fa_total_count > 0) {
		printf("fa result:%d/%d(%f%%)\n", fa_success_count, fa_total_count, fa_success_count * 100.0 / fa_total_count);
		log_file("fa_result:%d/%d,", fa_success_count, fa_total_count);
		log_file("far:%f%%", fa_success_count * 100.0 / fa_total_count);
	}

	log_file_end();
	return ret;
}

int samples_list::get_client_result()
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


	if(notifier.type == REPORT_EXIT) {
		ret = notifier.pid;
	} else if(notifier.type == REPORT_RESULT) {
		parse_pid_result(notifier.buffer);
	}

	printf("client:pid:%d, content:%s\n", notifier.pid, notifier.buffer);


	/* close the socket */
	close(client_sockfd);

	//ret = pid;
	return ret;
}

int samples_list::try_to_start_task_and_wait(test_task *task, task_start_reason_t reason, bool wait)
{
	int ret = 0;
	bool ready = task->can_start_task(reason);
	settings *g_settings = settings::get_instance();

	if(ready) {
		std::set<task_bmp, bmp_enroll_set_comp> enroll_ids = task->get_enroll_ids();

		add_test_task_catagory(fr_samples, task, enroll_ids, FOR_FR);
		add_test_task_catagory(fa_samples, task, enroll_ids, FOR_FA);

		ret = task->start_task(server_path);

		if(ret != 0) {
			printf("pid start:%d(%x)\n", ret, ret);
			pid_list.insert(ret);
		}
	}


	while((pid_list.size() >= g_settings->max_proc_number) || (wait && (pid_list.size() > 0))) {
		int pid = get_client_result();

		if(pid != 0) {
			int w;
			int status;

			printf("pid %d(%x) will stop!\n", pid, pid);
			w = waitpid(pid, &status, WUNTRACED | WCONTINUED);

			if (w == -1) {
				perror("waitpid");
			} else {
				pid_list.erase(w);
			}

			if (WIFEXITED(status)) {
				printf("exited, status=%d\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				printf("killed by signal %d\n", WTERMSIG(status));
			} else if (WIFSTOPPED(status)) {
				printf("stopped by signal %d\n", WSTOPSIG(status));
			}/* else if (WIFCONTINUED(status)) {
				printf("continued\n");
			}*/

		}
	}

	return ret;
}

int samples_list::start_test_task()
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

	report_result();

	stop_server();

	return ret;
}

int samples_list::gen_log_file_names()
{
	int ret = 0;
	settings *g_settings = settings::get_instance();
	char buffer[BUFFER_LEN];
	int len = 0;

	len = snprintf(buffer, BUFFER_LEN, "logs/%s/report.log", g_settings->log_dirname.c_str());
	buffer[len] = 0;
	logfile = buffer;

	return ret;
}

int samples_list::log_file_start()
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

int samples_list::log_file(const char *fmt, ...)
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

int samples_list::log_file_end()
{
	int ret = 0;

	ofs.close();

	return ret;
}

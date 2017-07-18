

/*================================================================
 *   Copyright (C) 2017年07月14日 肖飞 All rights reserved
 *
 *   文件名称：samples_list.cpp
 *   创 建 者：肖飞
 *   创建日期：2017年07月14日 星期五 12时38分19秒
 *   修改日期：2017年07月18日 星期二 12时52分34秒
 *   描    述：
 *
 *================================================================*/
#include "samples_list.h"

samples_list::samples_list()
{
	server_fd = 0;
	enroll_samples = NULL;
	fr_samples = NULL;
	fa_samples = NULL;
	server_path = "/tmp/.server_socket";
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

	bmp_file_list = get_bmp_filelist(g_settings->dirname);

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

	printf("get log from client:pid:%d, filename:%s\n", notifier.pid, notifier.filename);

	ret = notifier.pid;

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
		std::set<task_bmp, bmp_enroll_set_comp>::iterator ids_it;
		std::set<task_bmp, bmp_enroll_set_comp> ids = task->get_enroll_ids();
		std::set<std::string> set_catagory;
		std::set<std::string>::iterator catagory_it;

		for(ids_it = ids.begin(); ids_it != ids.end(); ids_it++) {
			task_bmp bmp = *ids_it;
			set_catagory.insert(bmp.catagory);
		}

		for(catagory_it = set_catagory.begin(); catagory_it != set_catagory.end(); catagory_it++) {
			//printf("add catagory:%s\n", catagory_it->c_str());
			add_test_task_catagory(fr_samples, task, *catagory_it, FOR_FR);
			add_test_task_catagory(fa_samples, task, *catagory_it, FOR_FA);
		}

		ret = task->start_task(reason, server_path);

		if(ret != 0) {
			printf("pid start:%d(%x)\n", ret, ret);
			pid_list.insert(ret);
		}
	}


	while((pid_list.size() >= g_settings->max_proc_number) || (wait && (pid_list.size() > 0))) {
		int pid = get_client_result();
		printf("pid stop:%d(%x)\n", pid, pid);
		pid_list.erase(pid);
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

	return ret;
}

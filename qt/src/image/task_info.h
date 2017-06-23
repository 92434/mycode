

/*================================================================
 *   Copyright (C) 2017年06月23日 肖飞 All rights reserved
 *
 *   文件名称：task_info.h
 *   创 建 者：肖飞
 *   创建日期：2017年06月23日 星期五 12时59分21秒
 *   修改日期：2017年06月23日 星期五 13时27分19秒
 *   描    述：
 *
 *================================================================*/
#ifndef _TASK_INFO_H
#define _TASK_INFO_H
#include <qmap.h>
#include <qobject.h>
#include <qvector.h>
typedef struct _task_item_info {
	QString file_path;
	int person_id;
	int id;
	int origin_state;
	int sequence_num_1;
	int sequence_num_2;
	int row_of_task;
} task_item_info_t;

class task_info : public QObject
{
	Q_OBJECT
private:
public:
	QMap<int, QVector<task_item_info_t>> enroll_map;
	QMap<int, QVector<task_item_info_t>> identify_for_fr_map;
	QMap<int, QVector<task_item_info_t>> identify_for_fa_map;
private:

public:
	task_info();
	~task_info();
	void clear_enroll_map();
	void clear_identify_for_fr_map();
	void clear_identify_for_fa_map();
};
#endif //TASK_INFO_H

#coding:utf-8

import datetime
import random

state_info_map = {
	"SUCCESS": "SUCCESS", #上传成功标记，在UEditor中内不可改变，否则flash判断会出错
	"UPLOAD_FILE_SIZE_EXCEED": "文件大小超出 upload_max_filesize 限制",
	"MAX_FILE_SIZE_EXCEED": "文件大小超出 MAX_FILE_SIZE 限制",
	"FILE_NOT_COMPLETE": "文件未被完整上传",
	"NO_FILES": "没有文件被上传",
	"FILE_EMPTY": "上传文件为空",
	"ERROR_TMP_FILE": "临时文件错误",
	"ERROR_TMP_FILE_NOT_FOUND": "找不到临时文件",
	"ERROR_SIZE_EXCEED": "文件大小超出网站限制",
	"ERROR_TYPE_NOT_ALLOWED": "文件类型不允许",
	"ERROR_CREATE_DIR": "目录创建失败",
	"ERROR_DIR_NOT_WRITEABLE": "目录没有写权限",
	"ERROR_FILE_MOVE": "文件保存时出错",
	"ERROR_FILE_NOT_FOUND": "找不到上传文件",
	"ERROR_WRITE_CONTENT": "写入文件内容错误",
	"ERROR_UNKNOWN": "未知错误",
	"ERROR_DEAD_LINK": "链接不可用",
	"ERROR_HTTP_LINK": "链接不是http链接",
	"ERROR_HTTP_CONTENTTYPE": "链接contentType不正确",
	"INVALID_URL": "非法 URL",
	"INVALID_IP": "非法 IP"
}

def get_http_response_state(state = 'SUCCESS', info = ''):
	if info == '':
		info = state_info_map.get(state)
	if info == '':
		info = 'NO RESPONSE STATE'
		
	return {"state": info}

def get_time_format(dt = None):
	if not isinstance(dt, datetime.datetime):
		dt = datetime.datetime.now()

	return {
		"year": dt.strftime("%Y"),
		"month": dt.strftime("%m"),
		"day": dt.strftime("%d"),
		"hour": dt.strftime("%H"),
		"min": dt.strftime("%M"),
		"sec": dt.strftime("%S"),
		"microsec": "%06d" % dt.microsecond,
		"date": dt.strftime("%Y%m%d"),

		"time": dt.strftime("%H%M%S"),
		"datetime": dt.strftime("%Y%m%d%H%M%S"),
		"rnd": random.randrange(000,999),
	}



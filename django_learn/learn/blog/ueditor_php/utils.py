# -*- coding: utf-8 -*-

import datetime
import random
import re
import os
import urllib

from . import config

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

def list_get_files(base_dir, path, allow_types):
	files = []

	cur_dir = os.path.join(base_dir, path.lstrip('/'))
	if not os.path.exists(cur_dir):
		return files

	items = os.listdir(cur_dir)
	for item in items:
		item = unicode(item)

		item_path = os.path.join(path, item)

		item_full_path = os.path.join(base_dir, item_path.lstrip('/'))
		if os.path.isdir(item_full_path):
			files.extend(list_get_files(base_dir, item_path, allow_types))
		else:
			ext = os.path.splitext(item_path)[1]
			if ext in allow_types:
				files.append(
					{
						"url": urllib.basejoin(config.django_settings.STATIC_URL, item_path.lstrip('/')),
						"mtime": os.path.getmtime(item_full_path)
					}
				)

	return files

def get_local_file_path(base_dir, path_format_string, path_var, file_name):
	path = path_format_string % path_var
	local_path = os.path.join(base_dir, path.lstrip('/'))
	local_file_path = os.path.join(local_path, file_name)
	return (path, local_path, local_file_path)

#文件大小类
class file_size():
	SIZE_UNIT = {
		"Byte":1,
		"KB":1024,
		"MB":1048576,
		"GB":1073741824,
		"TB":1099511627776L
	}

	def __init__(self, size):
		self.size = long(format(size))

	@staticmethod
	def Format(size):
		if isinstance(size, int) or isinstance(size, long):
			return size

		if not isinstance(size, str):
			return 0

		size_upper = size.lstrip().upper().replace(" ","")
		pattern = re.compile(r"(\d*\.?(?=\d)\d*)(byte|kb|mb|gb|tb)", re.I)
		match = pattern.match(size_upper)
		if match:
			m_size, m_unit = match.groups()
			if m_size.find(".") == -1:
				m_size = long(m_size)
			else:
				m_size = float(m_size)
			if m_unit != "BYTE":
				return m_size * file_size.SIZE_UNIT[m_unit]
			else:
				return m_size
		else:
			return 0

	#返回字节为单位的值
	@property
	def size(self):
		return self.size

	@size.setter
	def size(self, newsize):
		try:
			self.size = long(newsize)
		except:
			self.size = 0

	#返回带单位的自动值
	@property
	def FriendValue(self):
		if self.size < file_size.SIZE_UNIT["KB"]:
			unit = "Byte"
		elif self.size < file_size.SIZE_UNIT["MB"]:
			unit = "KB"
		elif self.size < file_size.SIZE_UNIT["GB"]:
			unit = "MB"
		elif self.size < file_size.SIZE_UNIT["TB"]:
			unit = "GB"
		else:
			unit = "TB"

		if (self.size % file_size.SIZE_UNIT[unit]) == 0:
			return "%s%s" % ((self.size / file_size.SIZE_UNIT[unit]), unit)
		else:
			return "%0.2f%s" % (round(float(self.size) / float(file_size.SIZE_UNIT[unit]), 2), unit)

	def __str__(self):
		return self.FriendValue

	#相加
	def __add__(self, other):
		if isinstance(other, file_size):
			return file_size(other.size + self.size)
		else:
			return file_size(file_size(other).size + self.size)
	def __sub__(self, other):
		if isinstance(other, file_size):
			return file_size(self.size - other.size)
		else:
			return file_size(self.size - file_size(other).size)
	def __gt__(self, other):
		if isinstance(other, file_size):
			if self.size > other.size:
				return True
			else:
				return False
		else:
			if self.size > file_size(other).size:
				return True
			else:
				return False
	def __lt__(self,  other):
		if isinstance(other, file_size):
			if other.size > self.size:
				return True
			else:
				return False
		else:
			if file_size(other).size > self.size:
				return True
			else:
				return False
	def __ge__(self, other):
		if isinstance(other, file_size):
			if self.size >= other.size:
				return True
			else:
				return False
		else:
			if self.size >= file_size(other).size:
				return True
			else:
				return False
	def __le__(self, other):
		if isinstance(other, file_size):
			if other.size >= self.size:
				return True
			else:
				return False
		else:
			if file_size(other).size >= self.size:
				return True
			else:
				return False

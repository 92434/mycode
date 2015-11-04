
# -*- coding: utf-8 -*-

import urllib
import chardet 

def get_encoding_by_conn(conn):
	info = conn.info()
	return info.getparam('charset')

def get_encoding_chardet(page):
	chardict = chardet.detect(data1)
	return chardict.get('encoding')


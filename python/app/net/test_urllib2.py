#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib2
import coding_detect
import chardet
import sys
from log import *

def get_request_header(url):
	#get html source  
	request = urllib2.Request(url)  
	request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.48')  
	#request.add_header('Accept-encoding', 'gzip,deflate')	# 向服务器请求压缩数据
	return request

def get_page(url):
	request = get_request_header(url)
	opener = urllib2.build_opener()
	conn = opener.open(request)

	encode = coding_detect.get_encoding_by_conn(conn)
	txt = '\n'
	txt += 'encode by coding_detect.get_encoding_by_conn:%s' %(str(encode))
	logger.debug(txt)

	page = conn.read()

	encode = coding_detect.get_encoding_chardet(page)
	txt = '\n'
	txt += 'encode by coding_detect.get_encoding_chardet:%s' %(str(encode))
	logger.debug(txt)

	encode = chardet.detect(page)
	txt = '\n'
	txt += 'encode by chardet.detect:%s' %(str(encode))
	logger.debug(txt)

	conn.close()
	return page

def main():
	url = None
	if len(sys.argv) > 1:
		url = sys.argv[1]
	else:
		logger.debug('!')
	page = ''
	if url:
		page = get_page(url)
	else:
		logger.debug('!')
	print page

if __name__ == '__main__':
	main()

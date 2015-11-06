#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2, httplib
import zlib
import StringIO, gzip

def write_file(fname, data):
	f = open(fname, "w")
	f.write(data)
	f.close()

def detect_gzip_content(info):
	return info.get('content-encoding') == 'gzip'

def deflate_gzip_content_by_gzip(info, page):
	if detect_gzip_content(info):
		io = StringIO.StringIO(page)
		gziper = gzip.GzipFile(fileobj = io)
		page = gziper.read()
	return page

def deflate_gzip_content_by_zlib(info, page):
	if detect_gzip_content(info):
            page = zlib.decompress(page, 16+zlib.MAX_WBITS);
	return page

def get_request_header(url):
	#get html source  
	request = urllib2.Request(url)  
	request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.48')  
	request.add_header('Accept-encoding', 'gzip,deflate')	# 向服务器请求压缩数据
	return request

if __name__ == '__main__':
	httplib.HTTPConnection.debuglevel = 1
	request = get_request_header('http://www.163.com/')
	opener = urllib2.build_opener()
	conn = opener.open(request)
	info = conn.info()
	page = conn.read()			# 读取页面返回的数据
	conn.close()

	write_file('1.html', deflate_gzip_content_by_zlib(info, page))
	write_file('2.html', deflate_gzip_content_by_gzip(info, page))

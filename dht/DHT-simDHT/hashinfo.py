# -*- coding: utf-8 -*-

import os
import sys
import urllib
import urllib2
import traceback
import StringIO
import gzip   
import libtorrent as lt

def bt_to_magnet2(data):
	info = lt.torrent_info(data, len(data))
	print dir(info)
	print "magnet:?xt=urn:btih:%s&dn=%s" % (info.info_hash(), info.name())
  
#解压gzip  
def gzdecode(filename): 
	data = None
	with open(filename) as f:
		data = f.read()
	if not data:
		return None

	compressedstream = StringIO.StringIO(data)  
	gziper = gzip.GzipFile(fileobj = compressedstream)	
	data = gziper.read()   # 读取解压缩后数据   
	return data


url_base = 'http://torcache.net/torrent/' 

def urllib_get(url):
	content = ''
	user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.152 Safari/537.36'# 将user_agent写入头信息
	values = {
		#'Content-Type': 'application/octet-stream',
		#'Content-Length': '1',
		#'Content-Encoding': 'gzip',
		#'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
		#'Accept-Encoding': 'gzip, deflate, sdch',
		#'Accept-Language': 'zh-CN,zh;q=0.8',
		#'Host': 'torcache.net',
		#'Connection': 'keep-alive',
	}
	headers = { 'User-Agent' : user_agent }

	print url
	try:
		data = urllib.urlencode(values)
		req = urllib2.Request(url, data, headers)
		response = urllib2.urlopen(req)
		print response.info().get('content-length')
		content = response.read()

		return content
	except Exception, e:
		print e
		traceback.print_exc()
	return content

def wget_get(url):
	data = None
	filename = os.path.basename(url)
	cmd = 'wget' + ' --timeout=3' + ' --tries=2' + ' ' + url
	cmd += ' -O' + ' ' + filename
	print cmd
	try:
		os.system(cmd)
		if os.path.exists(filename):
			data = gzdecode(filename)
			os.remove(filename)
	except Exception, e:
		print e
		traceback.print_exc()

	return data

def get_torrent(hash):
	url = url_base + hash + '.torrent'
	data = wget_get(url)
	if data:
		bt_to_magnet2(data)

if __name__ == "__main__":
	print sys.argv
	get_torrent(sys.argv[1])

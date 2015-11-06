# -*- coding: utf-8 -*-

import urllib2
import htmllib
import formatter  
import cookielib
import os

import coding_detect

class LinksExtractor(htmllib.HTMLParser):  
  
	def __init__(self, formatter):  
		htmllib.HTMLParser.__init__(self, formatter)  
		self.links = []
		self.in_anchor = 0
		self.cur_items = {}
	  
	def start_a(self, attrs):  
		# process the attributes  
		self.in_anchor = 1;
		self.cur_items = {}

		for attr in attrs :  
			#print 'attr:%s' %('|'.join(attr))
			if attr[0] == "href" :   
				if not self.cur_items.get('link'):
					self.cur_items.update({'link': []})
				self.cur_items.get('link').append(attr[1])
				#print 'add link:%s' %(attr[1])
	  
	def end_a(self):  
		self.in_anchor = 0  
		self.links.append(self.cur_items)
		  
	def handle_data(self, text):  
		if self.in_anchor:  
			if not self.cur_items.get('text'):
				self.cur_items.update({'text': []})
			self.cur_items.get('text').append(text)
			#print 'add text:%s' %(text)
	  
	def get_links(self) :   
		return self.links  

cookie_file = 'cookie.txt'
#cookie = cookielib.CookieJar()
if os.path.exists(cookie_file):
	cookie = cookielib.MozillaCookieJar()
	cookie.load(cookie_file, ignore_discard=True, ignore_expires=True)
else:
	cookie = cookielib.MozillaCookieJar(cookie_file)

def get_proxy_opener(http_type, ip, port):
	proxy = {http_type : '%s:%d' %(ip, port)}

	proxy_support = urllib2.ProxyHandler(proxy)

	cookie_support = urllib2.HTTPCookieProcessor(cookie)

	opener = urllib2.build_opener(proxy_support, cookie_support)

	return opener

def set_default_proxy(opener):
	urllib2.install_opener(opener)

def get_request_header(url):
	#get html source  
	request = urllib2.Request(url)  
	request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.48')  
	#request.add_header('Accept-encoding', 'gzip,deflate')	# 向服务器请求压缩数据
	return request

def proxy_get_page(url, proxy_ip, proxy_port):
	opener = get_proxy_opener('https', proxy_ip, proxy_port)  
	request = get_request_header(url)
	conn = opener.open(request)
	encode = coding_detect.get_encoding_by_conn(conn)
	page = conn.read()
	encode = coding_detect.get_encoding_chardet(page)
	conn.close()
	print encode
	return page
	
def get_page_links(url, proxy_ip, proxy_port):
	format = formatter.NullFormatter()			
	htmlparser = LinksExtractor(format)		  
	page = proxy_get_page(url, proxy_ip, proxy_port)
	  
	htmlparser.feed(page)		
	htmlparser.close()  
	  
	links = htmlparser.get_links()	

	#for i in range(len(htmlparser.links)):  
	#	temp = htmlparser.links[i]  
	#	if temp.get('url') and temp.get('text'):
	#		print 'url: %s' %('|'.join(temp.get('url')))
	#		print 'text: %s' %('|'.join(temp.get('text')))


if '__main__' == __name__:
	get_page_links('https://www.google.com.hk/?gws_rd=ssl', '192.168.1.211', 8580)
	for i in cookie:
		print i
	if not os.path.exists(cookie_file):
		cookie.save(ignore_discard=True, ignore_expires=True)

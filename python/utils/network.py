# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：network.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 12时30分28秒
#   修改日期：2017年10月20日 星期五 09时32分19秒
#   描    述：
#
#================================================================
import six
import cookielib

class network(object):
    handlers = []
    urllib = six.moves.urllib
    cookie = None
    #cookie_file = 'cookie.txt'

    def __init__(self):
        self.handlers = []

    def set_verbose_level(self, level = 0):
        http_handler = self.urllib.request.HTTPHandler(level)
        https_handler = self.urllib.request.HTTPSHandler(level)

        self.handlers.append(http_handler)
        self.handlers.append(https_handler)

    def reset_handlers(self):
        self.handlers = []

    def add_auth_handler(self, auth_info):
        mgr = self.urllib.request.HTTPPasswordMgrWithDefaultRealm()

        try:
            import netrc
            n = netrc.netrc()
            for host in n.hosts:
                p = n.hosts[host]
                mgr.add_password(p[1], 'http://%s/' % host, p[0], p[2])
                mgr.add_password(p[1], 'https://%s/' % host, p[0], p[2])
        except:
            pass

        for uri, username, password in auth_info:
            mgr.add_password(None, uri, username, password)

        basic_auth_handler = self.urllib.request.HTTPBasicAuthHandler(mgr)
        digest_auth_handler = self.urllib.request.HTTPDigestAuthHandler(mgr)
        self.handlers.append(basic_auth_handler)
        self.handlers.append(digest_auth_handler)

    def add_cookie_handler(self):
        self.cookie = cookielib.CookieJar()
        #self.cookie = cookielib.MozillaCookieJar(self.cookie_file)
        #self.cookie.load()
        cookie_handler = self.urllib.request.HTTPCookieProcessor(self.cookie)  
        for item in self.cookie:
            print 'name:%s\t value:%s' %(item.name, item.value)
        self.handlers.append(cookie_handler)

    def get_cookie_by_name(self, cookie_name):
        for item in self.cookie:
            if item.name == cookie_name:
                return item
        return None

    def update_cookie_file(self):
        for item in self.cookie:
            print 'name:%s\t value:%s' %(item.name, item.value)
        #self.cookie.save(ignore_discard = True, ignore_expires = True)

    def add_proxy_handler(self, uri):
        proxy_handler = self.urllib.request.ProxyHandler({
            'http': uri,
            'https': uri,
        })
        self.handlers.append(proxy_handler)

    def install_opener(self):
        opener = self.urllib.request.build_opener(*self.handlers)
        self.urllib.request.install_opener(opener)

    def default_init(self, verbose_level, proxy_urls = []):
        self.reset_handlers()
        self.set_verbose_level(verbose_level)
        self.add_cookie_handler()
        for i in proxy_urls:
            self.add_proxy_handler(i)
        self.install_opener()

def main():
    fake_headers = {
        #'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
        'Accept': '*/*',
        'Accept-Charset': 'UTF-8,*;q=0.5',
        'Accept-Encoding': 'gzip,deflate,sdch',
        'Accept-Language': 'zh-CN,zh;q=0.8',
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/60.0.3112.78 Chrome/60.0.3112.78 Safari/537.36'
    }


    post_data = None 

    n = network()
    n.default_init(0)

    while not post_data:
        c = n.get_cookie_by_name('csrftoken')
        if c:
            post_data = {}  
            post_data.update({'username' : 'xiaofei'})
            post_data.update({'password' : 'xiaofei'})
            post_data.update({'csrfmiddlewaretoken' : '%s' %(c.value)})
            post_data = n.urllib.parse.urlencode(post_data)  

        r = n.urllib.request.Request(url = 'http://localhost:8001/admin/login/?next=/admin/', data = post_data, headers = fake_headers)
        
        response = n.urllib.request.urlopen(r, None)

        print('%s' %(response.info()))

    data = response.read()
    if response.info().get('Content-Encoding') == 'gzip':
        data = self.ungzip(data)
    elif response.info().get('Content-Encoding') == 'deflate':
        data = self.undeflate(data)
    print('data:%s' %(data))

    return response


if '__main__' == __name__:
    main()

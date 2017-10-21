# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：network.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 12时30分28秒
#   修改日期：2017年10月21日 星期六 14时13分42秒
#   描    述：
#
#================================================================
import six
import cookielib
import re

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

class network(object):
    handlers = []
    urllib = six.moves.urllib
    cookie = None
    #cookie_file = 'cookie.txt'
    fake_header = {
        'Accept' : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
        'Accept-Encoding' : 'gzip, deflate, br',
        'Accept-Language' : 'zh-CN,zh;q=0.8',
        'Cache-Control' : 'max-age=0',
        'Connection' : 'keep-alive',
        'Upgrade-Insecure-Requests' : '1',
        'User-Agent' : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/61.0.3163.79 Chrome/61.0.3163.79 Safari/537.36',
            }

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
        self.handlers.append(cookie_handler)

    def get_cookie_by_name(self, cookie_name):
        for item in self.cookie:
            if item.name == cookie_name:
                return item
        return None

    def update_cookie_file(self):
        for item in self.cookie:
            logger.debug('name:%s\t value:%s' %(item.name, item.value))
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

    def ungzip(self, data):
        """Decompresses data for Content-Encoding: gzip.
        """
        from io import BytesIO
        import gzip
        buffer = BytesIO(data)
        f = gzip.GzipFile(fileobj = buffer)
        return f.read()

    def undeflate(self, data):
        """Decompresses data for Content-Encoding: deflate.
        (the zlib compression is used.)
        """
        import zlib
        decompressobj = zlib.decompressobj(-zlib.MAX_WBITS)
        return decompressobj.decompress(data) + decompressobj.flush()

    def decompresses_response_data(self, response, data):
        content_encoding = response.info().get('Content-Encoding')
        if content_encoding == 'gzip':
            data = self.ungzip(data)
        elif content_encoding == 'gzip':
            data = self.undeflate(data)
        return data

    def r(self, pattern, text, index):
        s = None
        m = re.search(pattern, text)
        if m:
            try:
                s = m.group(index)
            except:
                pass
        return s

    def decode_response_data(self, response, data):
        content_type = response.headers.get('content-type')
        if content_type:
            charset = self.r(r'charset=([\w-]+)', content_type, 1)

        if not charset:
            charset = self.r(r'charset=([\w-]+)', data, 1)

        if charset:
            try:
                data = data.decode(charset, 'ignore')
            except:
                logger.debug('location:%s, charset:%s' %(self.get_location(response), charset))
        else:
            logger.debug('location:%s, charset:%s' %(self.get_location(response), charset))

        return data

    def get_location(self, response):
        return response.geturl()

def main():
    pass

if '__main__' == __name__:
    main()

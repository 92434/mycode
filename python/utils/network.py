# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：network.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 12时30分28秒
#   修改日期：2017年10月31日 星期二 22时46分57秒
#   描    述：
#
#================================================================
import six
import cookielib
import re
import os
import sqlite3
import sys

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

class network(object):
    handlers = []
    urllib = six.moves.urllib
    cookie = None
    #cookie_file = 'cookie.txt'
    fake_header = {
        #'Accept' : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
        #'Accept-Encoding' : 'gzip, deflate, br',
        #'Accept-Language' : 'zh-CN,zh;q=0.8',
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

    def load_chromium_cookie(self, domain = None):
        import Crypto
        import Crypto.Protocol.KDF

        salt = b'saltysalt'
        iv = b' ' * 16
        length = 16

        def chrome_decrypt(encrypted_value, key = None):

            # Encrypted cookies should be prefixed with 'v10' according to the
            # Chromium code. Strip it off.
            encrypted_value = encrypted_value[3:]

            # Strip padding by taking off number indicated by padding
            # eg if last is '\x0e' then ord('\x0e') == 14, so take off 14.
            # You'll need to change this function to use ord() for python2.
            def clean(x):
                return x[:-x[-1]].decode('utf8')

            cipher = Crypto.Cipher.AES.new(key, Crypto.Cipher.AES.MODE_CBC, IV=iv)
            decrypted = cipher.decrypt(encrypted_value)

            return clean(decrypted)

        # If running Chrome on OSX
        if sys.platform == 'darwin':
            my_pass = keyring.get_password('Chrome Safe Storage', 'Chrome')
            my_pass = my_pass.encode('utf8')
            iterations = 1003
            cookie_file = os.path.expanduser(
                '~/Library/Application Support/Google/Chrome/Default/Cookies'
            )

        # If running Chromium on Linux
        elif sys.platform == 'linux' or sys.platform == 'linux2':
            my_pass = 'peanuts'.encode('utf8')
            iterations = 1
            cookie_file = os.path.expanduser(
                '~/.config/chromium/Profile 4/Cookies'
            )
        else:
            raise Exception("This script only works on OSX or Linux.")

        # Generate key from values above
        key = Crypto.Protocol.KDF.PBKDF2(my_pass, salt, length, iterations)

        conn = sqlite3.connect(cookie_file)

        sql = 'select host_key, name, value, encrypted_value from cookies'
        if domain:
            sql += ' where host_key like "%{}%"'.format(domain)

        with conn:
            for host_key, k, v, ev in conn.execute(sql):

                # if there is a not encrypted value or if the encrypted value
                # doesn't start with the 'v10' prefix, return v
                if v or (ev[:3] != b'v10'):
                    logger.debug('cookie %s, %s, %s' %(host_key, k, v))
                    pass
                else:
                    v = chrome_decrypt(ev, key = key)

                cookie_item = cookielib.Cookie(
                        version = 0,
                        name = k,
                        value = v,
                        port = None,
                        port_specified = None,
                        domain = host_key,
                        domain_specified = None,
                        domain_initial_dot = None,
                        path = None,
                        path_specified = None,
                        secure = None,
                        expires = None,
                        discard = None,
                        comment = None,
                        comment_url = None,
                        rest = None,
                        rfc2109 = False,
                        )
                self.cookie.set_cookie(cookie_item)    # Apply each cookie_item to cookiejar
            #conn.close()
    def load_local_cookie(self):
        try:
            import json
        except ImportError:
            import simplejson as json

        local_cookies_file = '.cookies.json'
        if not os.access(local_cookies_file, os.F_OK):
            return

        with open('.cookies.json') as f:
            cookies = f.read()


        for cookie in json.loads(cookies):
            cookie_default = {
                    'version' : None, 'name' : None, 'value' : None, 
                     'port' : None, 'port_specified' : None,
                     'domain' : None, 'domain_specified' : None, 'domain_initial_dot' : None,
                     'path' : None, 'path_specified' : None,
                     'secure' : None,
                     'expires' : None,
                     'discard' : None,
                     'comment' : None,
                     'comment_url' : None,
                     'rest' : {},
                     'rfc2109' : False,
                    }
            for i in cookie:
                if i in cookie_default:
                    cookie_default.update({i : cookie.get(i)})
                else:
                    rest_dict = cookie_default.get('rest')
                    rest_dict.update({i : cookie.get(i)})
                    
            
            #logger.debug('cookie_default %s' %(cookie_default))
            cookie_item = cookielib.Cookie(**cookie_default)
            self.cookie.set_cookie(cookie_item)

    def add_cookie_handler(self):
        self.cookie = cookielib.CookieJar()
        #self.cookie = cookielib.MozillaCookieJar(self.cookie_file)
        #self.cookie.load()

        #self.load_chromium_cookie()

        self.load_local_cookie()

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

    def get_response_content_length(self, response):
        content_length = response.headers.get('content-length')
        if content_length:
            content_length = int(content_length)
        else:
            content_range = response.headers.get('content-range')
            if content_range:
                range_start = int(content_range[6:].split('/')[0].split('-')[0])
                range_end = int(content_range[6:].split('/')[1])
                content_length = range_end - range_start
            else:
                content_length = float('inf')

        return content_length

    def get_location(self, response):
        return response.geturl()

def main():
    pass

if '__main__' == __name__:
    main()

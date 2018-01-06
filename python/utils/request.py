# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月05日 肖飞 All rights reserved
#   
#   文件名称：request.py
#   创 建 者：肖飞
#   创建日期：2017年12月05日 星期二 21时35分55秒
#   修改日期：2018年01月06日 星期六 11时09分08秒
#   描    述：
#
#================================================================
import Crypto
import Crypto.Protocol.KDF
import cookielib
import fcntl
import gzip
import io
try:
    import json
except ImportError:
    import simplejson as json
import log
import optparse
import os
import psutil
import re
import socket
import sqlite3
import struct
import sys
import urlparse
import zlib

logging = log.dict_configure()
logger = logging.getLogger('default')

timeout = 30
socket.setdefaulttimeout(timeout)

class abstract_request(object):
    def __init__(self):
        self.urlparse = urlparse
        self.fake_headers = {
            #'Accept' : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
            #'Accept-Encoding' : 'gzip, deflate, br',
            'Accept-Language' : 'zh-CN,zh;q=0.9',
            'Cache-Control' : 'max-age=0',
            'Connection' : 'keep-alive',
            'Upgrade-Insecure-Requests' : '1',
            'User-Agent' : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/61.0.3163.79 Chrome/61.0.3163.79 Safari/537.36',
                }

        self.parameter = {}

        self.cookie_default = {
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

        #self.cookies = cookielib.CookieJar()
        #self.cookie_file = ''
        #self.cookies = cookielib.MozillaCookieJar(self.cookie_file)
        #self.load_chromium_cookie()
    
    def update_parameter(self, **kwargs):
        level = kwargs.pop('level', None)
        if level != None:
            self.update_parameter_level(level)

        proxies = kwargs.pop('proxies', None)
        if proxies != None:
            self.update_parameter_proxies(proxies)

        headers = kwargs.pop('headers', None)
        if headers != None:
            self.update_parameter_headers(headers)

        list_cookie = kwargs.pop('list_cookie', None)
        cookie_file = kwargs.pop('cookie_file', None)
        self.update_parameter_cookies(list_cookie, cookie_file)

        auth = kwargs.pop('auth', None)
        if auth != None:
            self.update_parameter_auth(auth)

        return kwargs

    def get_cookies(self):
        return self.parameter.get('cookies', None)

    def get_cookie_by_name(self, cookies, cookie_name):
        for item in cookies:
            if item.name == cookie_name:
                return item
        return None

    def load_chromium_cookie(self, cookies, domain = None):
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
                cookies.set_cookie(cookie_item)    # Apply each cookie_item to cookiejar
        return cookies

    def cookiejar_from_list(self, cookies, list_cookie = []):
        for cookie in list_cookie:
            cookie_default = {}
            cookie_default.update(self.cookie_default)

            for key in cookie:
                if key in cookie_default:
                    cookie_default.update({key : cookie.get(key)})
                else:
                    rest_dict = cookie_default.get('rest')
                    rest_dict.update({key : cookie.get(key)})
            
            logger.debug('cookie_default %s' %(cookie_default))
            cookie_item = cookielib.Cookie(**cookie_default)
            cookies.set_cookie(cookie_item)
        return cookies

    def cookiejar_to_list(self, cookies):
        list_cookie = []
        for cookie in cookies:
            cookie_dict = {}
            for key, value in cookie_default.items():
                item = {key : getattr(cookie, key, self.cookie_default.get(key))}
                cookie_dict.update(item)
            list_cookie.append(cookie_dict)

        logger.debug('list_cookie %s' %(list_cookie))
        return list_cookie

    def load_local_cookie(self, cookie_file = None):
        list_cookie = []

        if not os.access(cookie_file, os.F_OK):
            return list_cookie

        with open(cookie_file, 'rb') as f:
            content = f.read()
            list_cookie = json.loads(content)
            if type(list_cookie) == dict:
                list_cookie = [list_cookie]

        logger.debug('list_cookie %s' %(list_cookie))
        return list_cookie
    
    def save_local_cookie(self, cookies, cookie_file = '.cookies.json'):
        with open(cookie_file, 'wb') as f:
            list_cookie = self.cookiejar_to_list(cookies)
            logger.debug('list_cookie %s' %(list_cookie))
            f.write(json.dumps(list_cookie, sort_keys=False, indent=2))

    def update_parameter_cookies(self, list_cookie = None, cookie_file = None):
        cookies = self.parameter.get('cookies')
        if not cookies:
            cookies = cookielib.CookieJar()

        if list_cookie:
            cookies = self.cookiejar_from_list(cookies, list_cookie)
        if cookie_file:
            list_cookie = self.load_local_cookie(cookie_file)
            cookies = self.cookiejar_from_list(cookies, list_cookie)

        item = {'cookies' : cookies}
        self.parameter.update(item)

    def update_parameter_proxies(self, proxies = None):
        #proxies:{'protocol' : 'server:port'}
        if proxies:
            item = {'proxies': proxies}
            self.parameter.update(item)

    def update_parameter_headers(self, headers):
        if headers:
            item = {'headers' : headers}
            self.parameter.update(item)

    def update_parameter_auth(self, auth):
        #auth:[(uri, username, password)]
        if auth:
            item = {'auth' : auth}
            self.parameter.update(item)

    def update_parameter_level(self, level):
        if level:
            item = {'level' : level}
            self.parameter.update(item)

    def process_parameter(self):
        raise Exception('not valid!')
    
    def ungzip(self, content):
        """Decompresses content for Content-Encoding: gzip.
        """
        bi = io.BytesIO(content)
        with gzip.GzipFile(fileobj = bi) as f:
            content = f.read()
        return content

    def undeflate(self, content):
        """Decompresses content for Content-Encoding: deflate.
        (the zlib compression is used.)
        """
        decompressobj = zlib.decompressobj(-zlib.MAX_WBITS)
        content = decompressobj.decompress(content) + decompressobj.flush()
        return content

    def r(self, pattern, text, index):
        s = None
        m = re.search(pattern, text)
        if m:
            try:
                s = m.group(index)
            except:
                pass
        return s
    
    def get_content_encoding(self, response):
        raise Exception('not valid!')

    def decompresses(self, response, content):
        content_encoding = self.get_content_encoding(response)
        if content_encoding == 'gzip':
            content = self.ungzip(content)
        elif content_encoding == 'deflate':
            content = self.undeflate(content)
        return content

    def get_charset(self, response):
        raise Exception('not valid!')

    def decode(self, response, content):
        charset = self.get_charset(response)
        if charset:
            if charset == 'ISO-8859-1':
                charset = 'gb18030'
            try:
                content = content.decode(charset)
            except:
                logger.exception('charset:%s' %(charset))

        return content

    def get(self, url, data = None, **kwargs):
        raise Exception('not valid!')

    def post(self, url, data = None, **kwargs):
        raise Exception('not valid!')

    def url_size(self, url, data = None, **kwargs):
        raise Exception('not valid!')

    def urls_size(self, urls, **kwargs):
        return sum([self.url_size(url, **kwargs) for url in urls])

    def iter_content(self, url, chunk_size = None, data = None, **kwargs):
        raise Exception('not valid!')

    def get_ipv4_address_by_if(ifname):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s_f = s.fileno()
        s_ioc_gifaddr = 0x8915 #SIOCGIFADDR
        if_buffer = struct.pack('256s', ifname[:15])
        r = fcntl.ioctl(s_f, s_ioc_gifaddr, if_buffer)
        ip = socket.inet_ntoa(r[20 : 24])
        return ip

    def get_all_iface_ip_address(self):
        ips = []
        d = psutil.net_if_addrs()
        for k, v in d.items():
            for i in v:
                if i.family != 2:
                    continue
                ips.append(i.address)
        return ips

class urllib_request(abstract_request):
    def __init__(self, level = 0, proxies = None, headers = None, cookie_file = None, auth = None):
        super(urllib_request, self).__init__()
        import six
        import ssl

        ssl._create_default_https_context = ssl._create_unverified_context
        self.urllib = six.moves.urllib

        self.update_parameter(level = level, proxies = proxies, headers = headers, cookie_file = cookie_file, auth = auth)

    def process_parameter(self):
        parameter = {}
        handlers = []
        parameter.update(self.parameter)

        level = parameter.pop('level', None)
        if level != None:
            http_handler = self.urllib.request.HTTPHandler(level)
            handlers.append(http_handler)
            https_handler = self.urllib.request.HTTPSHandler(level)
            handlers.append(https_handler)

        cookies = parameter.pop('cookies', None)
        if cookies != None:
            cookie_handler = self.urllib.request.HTTPCookieProcessor(cookies)  
            handlers.append(cookie_handler)

        proxies = parameter.pop('proxies', None)
        if proxies != None:
            proxy_handler = self.urllib.request.ProxyHandler(proxies)
            handlers.append(proxy_handler)

        auth = parameter.pop('auth', None)
        if auth != None:
            mgr = self.urllib.request.HTTPPasswordMgrWithDefaultRealm()
            try:
                import netrc
                n = netrc.netrc()
                for host in n.hosts:
                    p = n.hosts[host]
                    mgr.add_password(p[1], 'http://%s/' % host, p[0], p[2])
                    mgr.add_password(p[1], 'https://%s/' % host, p[0], p[2])
            except:
                logger.exception('')

            for uri, username, password in auth:
                mgr.add_password(None, uri, username, password)
            basic_auth_handler = self.urllib.request.HTTPBasicAuthHandler(mgr)
            handlers.append(basic_auth_handler)
            digest_auth_handler = self.urllib.request.HTTPDigestAuthHandler(mgr)
            handlers.append(digest_auth_handler)

        #install handler
        #logger.debug('handlers:%s' %(handlers))

        opener = self.urllib.request.build_opener(*handlers)
        self.urllib.request.install_opener(opener)
        
        return parameter

    def urlopen_with_retry(self, *args, **kwargs):
        req = self.urllib.request.Request(*args, **kwargs)

        for i in range(10):
            try:
                #logger.debug('%s, %s' %(args, kwargs))
                return self.urllib.request.urlopen(req)
            except:
                logger.exception('')
        sys.exit(-1)

    def get_content_encoding(self, response):
        return response.headers.get('Content-Encoding')

    def get_charset(self, response):
        charset = None
        content_type = response.headers.get('content-type')
        if content_type:
            charset = self.r(r'charset=([\w-]+)', content_type, 1)

        return charset

    def get(self, url, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        data = kwargs.pop('data', None)
        if data:
            url = '%s?%s' %(url, self.urllib.parse.urlencode(data))

        response = self.urlopen_with_retry(url, **kwargs)

        content = response.read()
        content = self.decompresses(response, content)
        content = self.decode(response, content)

        return content

    def post(self, url, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        data = kwargs.pop('data', None)
        if data:
            data = self.urllib.parse.urlencode(data)
            item = {'data' : data}
            kwargs.update(item)

        response = self.urlopen_with_retry(url, **kwargs)

        content = response.read()
        content = self.decompresses(response, content)
        content = self.decode(response, content)

        return content

    def get_content_size(self, response):
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

    def url_size(self, url, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        data = kwargs.pop('data', None)
        if data:
            data = self.urllib.parse.urlencode(data)
            item = {'data' : data}
            kwargs.update(item)

        response = self.urlopen_with_retry(url, **kwargs)
        size = self.get_content_size(response)
        response.close()
        return size

    def iter_content(self, url, chunk_size = None, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        data = kwargs.pop('data', None)
        if data:
            data = self.urllib.parse.urlencode(data)
            item = {'data' : data}
            kwargs.update(item)

        response = self.urlopen_with_retry(url, **kwargs)

        while True:
            content = response.read(chunk_size)
            if len(content):
                yield content
            else:
                break

class requests_request(abstract_request):
    def __init__(self, level = None, proxies = None, headers = None, cookie_file = None, auth = None):
        super(requests_request, self).__init__()
        import requests
        self.requests = requests
        self.urlparse.urlencode = requests.models.urlencode
        self.auth = auth

        self.update_parameter(level = level, proxies = proxies, headers = headers, cookie_file = cookie_file, auth = auth)
        item = {'verify' : False}
        self.parameter.update(item)

    def process_parameter(self):
        parameter = {}
        parameter.update(self.parameter)

        return parameter

    def urlopen_with_retry(self, method, *args, **kwargs):
        for i in range(10):
            try:
                logger.debug('%s, %s, %s' %(method, args, kwargs))
                return method(*args, **kwargs)
            except:
                logger.exception('')
        sys.exit(-1)

    def get_content_encoding(self, response):
        return response.headers.get('Content-Encoding')

    def get_charset(self, response):
        charset = response.encoding
        return charset

    def get(self, url, **kwargs):
        data = kwargs.pop('data', None)
        if data:
            url = '%s?%s' %(url, self.requests.models.urlencode(data))

        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        method = self.requests.get
        response = self.urlopen_with_retry(method, url, **kwargs)
        cookies = response.cookies
        item = {'cookies' : cookies}
        self.parameter.update(item)

        content = response.content
        #content = self.decompresses(response, content)
        #content = self.decode(response, content)

        return content

    def post(self, url, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        method = self.requests.post

        response = self.urlopen_with_retry(method, url, **kwargs)
        cookies = response.cookies
        item = {'cookies' : cookies}
        self.parameter.update(item)

        content = response.content
        #content = self.decompresses(response, content)
        #content = self.decode(response, content)

        return content

    def get_content_size(self, response):
        content_length = response.headers.get('Content-Length')
        if content_length:
            content_length = int(content_length)
        else:
            content_length = float('inf')

        return content_length

    def url_size(self, url, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        method = self.requests.get

        response = self.urlopen_with_retry(method, url, stream = True, **kwargs)
        cookies = response.cookies
        item = {'cookies' : cookies}
        self.parameter.update(item)

        size = self.get_content_size(response)
        response.close()
        return size

    def iter_content(self, url, chunk_size = None, **kwargs):
        kwargs = self.update_parameter(**kwargs)
        kwargs.update(self.process_parameter())

        kwargs.update(self.parameter)

        method = self.requests.get

        response = self.urlopen_with_retry(method, url, stream = True, **kwargs)
        cookies = response.cookies
        item = {'cookies' : cookies}
        self.parameter.update(item)

        return response.iter_content(chunk_size = chunk_size)


class request(object):
    def __init__(self, level = None, proxies = None, headers = None, cookie_file = '.cookies.json', auth = None):
        #self.request = urllib_request(level = level, proxies = proxies, headers = headers, cookie_file = cookie_file, auth = auth)
        self.request = requests_request(level = level, proxies = proxies, headers = headers, cookie_file = cookie_file, auth = auth)

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-u', '--url', dest='url', help='url', default=None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    urls = [opts.url]

    r = request()

    size = r.request.url_size('http://sw.bos.baidu.com/sw-search-sp/software/e25c4cc36a934/QQ_8.9.6.22427_setup.exe')
    logger.debug('size:%s' %(size))
    logger.debug('cookies:%s' %(r.request.get_cookies()))

    content = r.request.get('https://www.baidu.com', headers = r.request.fake_headers)
    logger.debug('content:%s' %(len(content)))
    logger.debug('cookies:%s' %(r.request.get_cookies()))

    content = r.request.get('https://www.baidu.com/xiaofei')
    logger.debug('content:%s' %(len(content)))
    logger.debug('cookies:%s' %(r.request.get_cookies()))

    iter_content = r.request.iter_content('http://www.baidu.com', chunk_size = None)
    for iter_data in iter_content:
        logger.debug('iter_data:%s' %(len(iter_data)))
    logger.debug('cookies:%s' %(r.request.get_cookies()))

    proxies = {'https' : '127.0.0.1:8087'}
    r = request(level = 0, proxies = proxies)
    content = r.request.get('https://www.google.com/', headers = r.request.fake_headers)
    logger.debug('content:%s' %(len(content)))
    logger.debug('cookies:%s' %(r.request.get_cookies()))

if '__main__' == __name__:
    main()

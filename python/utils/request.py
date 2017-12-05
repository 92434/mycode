# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月05日 肖飞 All rights reserved
#   
#   文件名称：request.py
#   创 建 者：肖飞
#   创建日期：2017年12月05日 星期二 21时35分55秒
#   修改日期：2017年12月05日 星期二 22时45分36秒
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
import six
import socket
import sqlite3
import struct
import sys
import zlib

logging = log.dict_configure()
logger = logging.getLogger('default')

class abstract_request(object):
    def __init__(self):
        self.cookie = cookielib.CookieJar()
        #self.cookie_file = ''
        #self.cookie = cookielib.MozillaCookieJar(self.cookie_file)
        #self.cookie.load()
        self.fake_header = {
            'Accept' : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
            'Accept-Encoding' : 'gzip, deflate, br',
            'Accept-Language' : 'zh-CN,zh;q=0.9',
            'Cache-Control' : 'max-age=0',
            'Connection' : 'keep-alive',
            'Upgrade-Insecure-Requests' : '1',
            'User-Agent' : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/61.0.3163.79 Chrome/61.0.3163.79 Safari/537.36',
                }

        #self.load_chromium_cookie()

        self.load_local_cookie()

    def get_cookie_by_name(self, cookie_name):
        for item in self.cookie:
            if item.name == cookie_name:
                return item
        return None

    def load_chromium_cookie(self, domain = None):
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

    def load_local_cookie(self):

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
                    
            
            logger.debug('cookie_default %s' %(cookie_default))
            cookie_item = cookielib.Cookie(**cookie_default)
            self.cookie.set_cookie(cookie_item)

        def ungzip(self):
            data = self.data
            """Decompresses data for Content-Encoding: gzip.
            """
            buffer = io.BytesIO(data)
            with gzip.GzipFile(fileobj = buffer) as f:
                data = f.read()
            return data

        def undeflate(self):
            data = self.data
            """Decompresses data for Content-Encoding: deflate.
            (the zlib compression is used.)
            """
            decompressobj = zlib.decompressobj(-zlib.MAX_WBITS)
            data = decompressobj.decompress(data) + decompressobj.flush()
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
        
        def get_charset():
            raise Exception('not valid!')

        def get_location():
            raise Exception('not valid!')

        def decode(self):
            data = self.data

            charset = self.get_charset()
            if charset:
                try:
                    data = self.data.decode(charset)
                except:
                    logger.debug('location:%s, charset:%s' %(self.get_location(), charset))
            else:
                logger.debug('location:%s, charset:%s' %(self.get_location(), charset))

            return data

        def get_encoding():
            raise Exception('not valid!')

        def decompresses(self):
            data = self.data

            content_encoding = self.get_encoding()
            if content_encoding == 'gzip':
                data = self.ungzip(data)
            elif content_encoding == 'gzip':
                data = self.undeflate(data)
            return data

        def url_size(self):
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
    def __init__(self):
        super(urllib_request, self).__init__()

class requests_request(abstract_request):
    def __init__(self):
        super(requests_request, self).__init__()

class request(object):
    def __init__(self):
        self.request = urllib_request()

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


if '__main__' == __name__:
    main()

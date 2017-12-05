# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月05日 肖飞 All rights reserved
#   
#   文件名称：baiduyun.py
#   创 建 者：肖飞
#   创建日期：2017年12月05日 星期二 12时24分48秒
#   修改日期：2017年12月05日 星期二 18时18分17秒
#   描    述：
#
#================================================================
import webbrowser
import os

try:
    import json
except ImportError:
    import simplejson as json

import ConfigParser

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

import network
n = network.network()
n.default_init(0)

class baiduyun(object):
    def __init__(self):
        self.conf_section = 'baiduyun'
        self.conf = ConfigParser.ConfigParser()

        conf_file = os.path.join(os.path.dirname(__file__), 'default.ini')
        if not os.path.exists(conf_file):
            self.conf.add_section(self.conf_section)
            self.conf.set(self.conf_section, 'api_key', 'CM6RxGnc8M9U9LheHN48cU27')
            self.conf.set(self.conf_section, 'oauth_url', 'https://openapi.baidu.com/oauth/2.0/authorize')
            self.conf.set(self.conf_section, 'oauth_login', 'https://openapi.baidu.com/oauth/2.0/login_success')
            self.conf.set(self.conf_section, 'pcs_file_url', 'https://c.pcs.baidu.com/rest/2.0/pcs/file')
            self.conf.set(self.conf_section, 'pcs_quota_url', 'https://pcs.baidu.com/rest/2.0/pcs/quota')
            self.conf.set(self.conf_section, 'app_path', '/')
            with open(conf_file, 'w+') as f:
                self.conf.write(f)

        self.conf.read(conf_file)

        if not self.get_conf_section_item(self.conf_section, 'access_token'):
            self.get_access_token()
            with open(conf_file, 'w+') as f:
                self.conf.write(f)

    def get_conf_section_item(self, section, item):
        ret = None
        if not self.conf.has_section(section):
            return ret
        if not self.conf.has_option(section, item):
            return ret
        return self.conf.get(section, item)
    
    def auth_url(self):
        ret = None
        oauth_url = self.get_conf_section_item(self.conf_section, 'oauth_url')
        if not oauth_url:
            return ret
        api_key = self.get_conf_section_item(self.conf_section, 'api_key')
        if not api_key:
            return ret
        query = {}
        query.update({'client_id' : '%s' %(api_key)})
        query.update({'response_type' : '%s' %('token')})
        query.update({'scope' : '%s' %('netdisk')})
        query.update({'redirect_uri' : '%s' %('oob')})
        query = n.urllib.parse.urlencode(query)
        ret = '%s?%s' %(oauth_url, query)
        logger.debug('%s' %(ret))
        return ret

    def get_access_token(self):
        webbrowser.open(self.auth_url())
        access_token = raw_input("请输入登录成功后跳转的 url 地址:\n")
        logger.debug('%s' %(access_token))
        self.conf.set(self.conf_section, 'access_token', access_token)

    def get_access_token_qs(self):
        qs = None
        access_token = self.get_conf_section_item(self.conf_section, 'access_token')
        p = n.urllib.parse.urlparse(access_token)
        if not p:
            return qs
        qs = n.urllib.parse.parse_qs(p.fragment)
        return qs

    def space_info(self):
        ret = None
        qs = self.get_access_token_qs()
        if not qs:
            return ret
        #query = {
        #        "method" : "info",
        #        "access_token": qs.get('access_token', None),
        #        }
        query = {
                "method" : "list",
                "access_token": qs.get('access_token', None),
                "by" : "time",
                "path" : "/",
                }
        #pcs_quota_url = self.get_conf_section_item(self.conf_section, 'pcs_quota_url')
        #logger.debug('%s' %(pcs_quota_url))
        pcs_file_url = self.get_conf_section_item(self.conf_section, 'pcs_file_url')
        logger.debug('%s' %(pcs_file_url))
        query = n.urllib.parse.urlencode(query)  
        #query = n.urllib.parse.quote(query)
        logger.debug('%s' %(query))

        url = '%s?%s' %(pcs_file_url, query)
        logger.debug('%s' %(url))

        #import requests
        #response = requests.get(pcs_file_url, params = query, verify = True)
        #result = response.json()
        #logger.debug('%s' %(result))

        #r = n.urllib.request.Request(url = pcs_quota_url, data = query, headers = n.fake_header)
        r = n.urllib.request.Request(url = url, headers = n.fake_header)

        response = n.urllib.request.urlopen(r, None)
        logger.debug('%s' %(response.info()))

        data = response.read()
        data = n.decompresses_response_data(response, data)
        data = n.decode_response_data(response, data)
        logger.debug('data:%s' %(data))


def main():
    bdy = baiduyun()
    bdy.space_info()

if '__main__' == __name__:
    main()

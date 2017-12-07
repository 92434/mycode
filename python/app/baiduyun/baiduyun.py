# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月05日 肖飞 All rights reserved
#   
#   文件名称：baiduyun.py
#   创 建 者：肖飞
#   创建日期：2017年12月05日 星期二 12时24分48秒
#   修改日期：2017年12月07日 星期四 15时32分59秒
#   描    述：
#
#================================================================
import webbrowser
import os
import prettytable

try:
    import json
except ImportError:
    import simplejson as json

import ConfigParser

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

import request

r = request.request()

class baiduyun(object):
    def __init__(self):
        self.conf_section = 'baiduyun'
        self.conf = ConfigParser.ConfigParser()

        conf_file = os.path.join(os.path.dirname(__file__), 'default.ini')
        if not os.path.exists(conf_file):
            self.conf.add_section(self.conf_section)
            self.conf.set(self.conf_section, 'api_key', 'iG6ghsi9r0RR0jTFCrlvTjX9')
            #self.conf.set(self.conf_section, 'api_key', 'CM6RxGnc8M9U9LheHN48cU27')
            self.conf.set(self.conf_section, 'oauth_url', 'https://openapi.baidu.com/oauth/2.0/authorize')
            self.conf.set(self.conf_section, 'oauth_login', 'https://openapi.baidu.com/oauth/2.0/login_success')
            self.conf.set(self.conf_section, 'pcs_file_url', 'https://pcs.baidu.com/rest/2.0/pcs/file')
            self.conf.set(self.conf_section, 'pcs_quota_url', 'https://pcs.baidu.com/rest/2.0/pcs/quota')
            self.conf.set(self.conf_section, 'pcs_getLoggedInUser_url', 'https://openapi.baidu.com/rest/2.0/passport/users/getLoggedInUser')
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
        query = r.request.urlparse.urlencode(query)
        ret = '%s?%s' %(oauth_url, query)
        logger.debug('%s' %(ret))
        return ret

    def get_access_token(self):
        webbrowser.open(self.auth_url())
        access_token = raw_input("请输入登录成功后跳转的 url 地址:\n")
        self.conf.set(self.conf_section, 'access_token', access_token)

    def get_access_token_qs(self):
        qs = None
        access_token = self.get_conf_section_item(self.conf_section, 'access_token')
        p = r.request.urlparse.urlparse(access_token)
        if not p:
            return qs
        qs = r.request.urlparse.parse_qs(p.fragment)
        return qs

    def get_loggedin_user(self):
        ret = None
        qs = self.get_access_token_qs()
        if not qs:
            return ret

        query = {
                "access_token": qs.get('access_token')[0],
                }

        pcs_getLoggedInUser_url = self.get_conf_section_item(self.conf_section, 'pcs_getLoggedInUser_url')
        content = r.request.get(pcs_getLoggedInUser_url, data = query)
        logger.debug('%s' %(content))

        return ret

    def space_info(self):
        ret = None
        qs = self.get_access_token_qs()
        if not qs:
            return ret

        query = {
                "method" : "info",
                "access_token": qs.get('access_token')[0],
                }
        pcs_quota_url = self.get_conf_section_item(self.conf_section, 'pcs_quota_url')
        content = r.request.get(pcs_quota_url, data = query)

        logger.debug('%s' %(content))
        return ret

    def list_path(self):
        ret = None
        title = ["序列号", "文件/目录名", "文件大小"]

        qs = self.get_access_token_qs()
        if not qs:
            return ret

        query = {
                "method" : "list",
                "access_token": qs.get('access_token')[0],
                "path" : "/apps/pcs_test_12",
                #"path" : "/apps/xiaofei_bdy",
                "by" : "time",
                }
        pcs_file_url = self.get_conf_section_item(self.conf_section, 'pcs_file_url')
        content = r.request.get(pcs_file_url, data = query)
        table = prettytable.PrettyTable(title)
        print('%s' %(table))

        logger.debug('%s' %(content))
        return ret


def main():
    bdy = baiduyun()
    bdy.get_loggedin_user()
    bdy.space_info()
    bdy.list_path()

if '__main__' == __name__:
    main()

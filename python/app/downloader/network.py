# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：network.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 12时30分28秒
#   修改日期：2017年08月01日 星期二 10时53分14秒
#   描    述：
#
#================================================================
import sys
import six

urllib = six.moves.urllib

class network(object):
    handlers = []

    def __init__(self):
        self.handlers = []

    def set_verbose_level(self, level = 0):
            http_handler = urllib.request.HTTPHandler(level)
            https_handler = urllib.request.HTTPSHandler(level)

            self.handlers.append(http_handler)
            self.handlers.append(https_handler)

    def reset_opener(self):
        self.handlers = []

    def add_user_password(self, username, password):
        mgr = urllib.request.HTTPPasswordMgrWithDefaultRealm()
        try:
            import netrc
            n = netrc.netrc()
            for host in n.hosts:
                p = n.hosts[host]
                mgr.add_password(p[1], 'http://%s/' % host, p[0], p[2])
                mgr.add_password(p[1], 'https://%s/' % host, p[0], p[2])
        except:  # pylint: disable=bare-except
            pass
        basic_auth_handler = urllib.request.HTTPBasicAuthHandler(mgr)
        digest_auth_handler = urllib.request.HTTPDigestAuthHandler(mgr)
        self.handlers.append(basic_auth_handler)
        self.handlers.append(digest_auth_handler)

    def add_proxy(self, url):
        proxy_handler = urllib.request.ProxyHandler({
            'http': url,
            'https': url,
        })
        self.handlers.append(proxy_handler)

    def install_opener(self):
        opener = urllib.request.build_opener(*self.handlers)
        urllib.request.install_opener(opener)

    def default_init(self, verbose_level, proxy_urls = []):
        self.reset_opener()
        self.set_verbose_level(verbose_level)
        for i in proxy_urls:
            self.add_proxy(i)
        self.install_opener()
        return urllib

def main():
    pass

if '__main__' == __name__:
    main()

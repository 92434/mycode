# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年01月27日 肖飞 All rights reserved
#   
#   文件名称：webdriver.py
#   创 建 者：肖飞
#   创建日期：2018年01月27日 星期六 11时33分03秒
#   修改日期：2018年01月27日 星期六 19时48分52秒
#   描    述：
#
#================================================================
from selenium import webdriver
from pyvirtualdisplay import Display
try:
    import json
except ImportError:
    import simplejson as json
import os
import sys

reload(sys)  
sys.setdefaultencoding('utf-8')

display = Display(visible=0, size=(800, 600))
#display.start()

class driver(object):
    def __init__(self, proxy = None):
        self.chrome_path = '~/bin/chromedriver'
        self.co = webdriver.ChromeOptions()
        if proxy:
            self.co.add_argument('--proxy-server=%s' %(proxy))  
        self.driver = webdriver.Chrome(executable_path = os.path.expanduser(self.chrome_path), chrome_options = self.co)
        #self.driver.implicitly_wait(5)
        #self.driver.set_script_timeout(3)
        #self.driver.set_page_load_timeout(5)

    def set_cookies(self, cookies):
        for cookie in cookies:
            self.driver.add_cookie(cookie)

    def get_cookies(self):
        return self.driver.get_cookies()
    
    def load_cookie_file(self, cookie_file = '.cookies.json'):
        f = open(cookie_file, 'rb')
        c = f.read()
        f.close()
        return json.loads(c)

def test_proxy():
    d = driver(proxy = 'socks5://127.0.0.1:1080')
    d.driver.get('https://www.google.com')

def test_cookie():
    d = driver()
    d.driver.get('https://www.baidu.com')
    cookies = d.load_cookie_file()
    d.set_cookies(cookies)
    d.driver.get('https://lanjing.tech/user/node')
    print('%s' %(d.driver.page_source))

def main():
    import time
    test_cookie()

if '__main__' == __name__:
    main()

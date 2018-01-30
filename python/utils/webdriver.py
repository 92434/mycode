# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年01月27日 肖飞 All rights reserved
#   
#   文件名称：webdriver.py
#   创 建 者：肖飞
#   创建日期：2018年01月27日 星期六 11时33分03秒
#   修改日期：2018年01月30日 星期二 13时41分03秒
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
import threading
import time

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
        self.driver = None
        self.content = ''
        self.timer = None

    def get_page(self, url, timeout = None, delay = None, list_cookies = None):
        self.driver = webdriver.Chrome(executable_path = os.path.expanduser(self.chrome_path), chrome_options = self.co)

        if list_cookies:
            self.driver.get('https://www.baidu.com')
            self.set_cookies(list_cookies)

        if timeout:
            if timeout < 3:
                timeout = 3
            #self.start_timer(timeout - 2)

            #self.driver.implicitly_wait(timeout)
            #self.driver.set_script_timeout(timeout)
            self.driver.set_page_load_timeout(timeout)

        try:
            self.driver.get(url)
            if delay:
                time.sleep(delay)
            #self.timer.cancel()
            #self.timer = None
            self.content = self.driver.page_source
            self.list_cookie = self.driver.get_cookies()
        except:
            pass
        finally:
            self.driver.quit()
            self.driver = None

        return self.content

    def timeout(self):
        self.content = self.driver.page_source
        self.list_cookie = self.driver.get_cookies()
        self.timer = None

    def start_timer(self, timeout):
        self.timer = threading.Timer(timeout, self.timeout)
        self.timer.start()

    def set_cookies(self, cookies):
        for cookie in cookies:
            self.driver.add_cookie(cookie)

    def get_cookies(self):
        return self.list_cookie
    
    def load_cookie_file(self, cookie_file = '.cookies.json'):
        f = open(cookie_file, 'rb')
        c = f.read()
        f.close()
        return json.loads(c)

def test_proxy():
    d = driver(proxy = 'socks5://127.0.0.1:1080')
    d.get_page('https://www.google.com')

def test_cookie():
    d = driver()
    cookies = d.load_cookie_file()
    d.get_page('https://lanjing.tech/user/node', list_cookies = cookies)
    print('%s' %(d.content))

def main():
    import time
    test_cookie()

if '__main__' == __name__:
    main()

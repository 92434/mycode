# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月24日 肖飞 All rights reserved
#   
#   文件名称：spider.py
#   创 建 者：肖飞
#   创建日期：2017年12月24日 星期日 12时41分32秒
#   修改日期：2017年12月24日 星期日 19时31分27秒
#   描    述：
#
#================================================================
from selenium import webdriver
from public import create_proxyauth_extension
from pyvirtualdisplay import Display
try:
    import json
except ImportError:
    import simplejson as json
import sys

reload(sys)  
sys.setdefaultencoding('utf-8')

display = Display(visible=0, size=(800, 600))
display.start()
 
chrome_path = './chromedriver'

def get_page_with_proxy():
    proxyauth_plugin_path = create_proxyauth_extension(
            proxy_host = "127.0.0.1",
            proxy_port = 8087,
            proxy_username = 'xiaofei',
            proxy_password = 'xiaofei',
        )
 
 
    co = webdriver.ChromeOptions()
    # co.add_argument("--start-maximized")
    co.add_extension(proxyauth_plugin_path)
     
     
    driver = webdriver.Chrome(executable_path = chrome_path, chrome_options = co)
    driver.get("http://ip138.com/")
    #print(driver.page_source)
 
def get_page():
    with open('.cookies.json') as f:
        cookies = f.read()
    co = webdriver.ChromeOptions()
    co.add_argument('--proxy-server=http://127.0.0.1:8087')  
    driver = webdriver.Chrome(executable_path = chrome_path, chrome_options = co)
    for cookie in json.loads(cookies):
        print('cookie:%s' %(type(cookie)))
        driver.add_cookie(cookie)
    #driver.get("http://ip138.com/")
    driver.get("https://www.google.com")
    #print(driver.page_source)
    for cookie in driver.get_cookies():
        print('cookie:%s' %(type(cookie)))
        print('cookie:%s' %(cookie))
 

def main():
    get_page()
    #get_page_with_proxy()

if '__main__' == __name__:
    main()
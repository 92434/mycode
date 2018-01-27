# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年01月27日 肖飞 All rights reserved
#   
#   文件名称：demo.py
#   创 建 者：肖飞
#   创建日期：2018年01月27日 星期六 10时21分49秒
#   修改日期：2018年01月27日 星期六 11时35分55秒
#   描    述：
#
#================================================================
from selenium import webdriver
from pyvirtualdisplay import Display
try:
    import json
except ImportError:
    import simplejson as json
import sys
import time

reload(sys)  
sys.setdefaultencoding('utf-8')

display = Display(visible=0, size=(800, 600))
#display.start()

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

def load_cookies(cookies_file = '.cookies.json'):
    with open(cookies_file, 'rb') as f:
        cookies = f.read()
    return json.loads(cookies, encoding='utf-8')

def get_page():
    co = webdriver.ChromeOptions()
    #co.add_argument('--proxy-server=http://127.0.0.1:8087')  
    co.add_argument('--proxy-server=socks5://127.0.0.1:1080')  

    driver = webdriver.Chrome(executable_path = chrome_path, chrome_options = co)

    driver.get("https://www.google.com")
    for cookie in load_cookies():
        print('set cookie:%s:%s' %(type(cookie), cookie))
        driver.add_cookie(cookie)
    driver.get("https://www.google.com")

    #print(driver.page_source)
    print(dir(driver))
    for cookie in driver.get_cookies():
        print('get cookie:%s' %(cookie))

    time.sleep(10)

def main():
    get_page()

if '__main__' == __name__:
    main()

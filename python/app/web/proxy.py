# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月24日 肖飞 All rights reserved
#   
#   文件名称：proxy.py
#   创 建 者：肖飞
#   创建日期：2017年12月24日 星期日 12时41分32秒
#   修改日期：2017年12月24日 星期日 12时43分19秒
#   描    述：
#
#================================================================
from selenium import webdriver
from common.pubilc import create_proxyauth_extension
 
proxyauth_plugin_path = create_proxyauth_extension(
    proxy_host="XXXXX.com",
    proxy_port=9020,
    proxy_username="XXXXXXX",
    proxy_password="XXXXXXX"
)
 
 
co = webdriver.ChromeOptions()
# co.add_argument("--start-maximized")
co.add_extension(proxyauth_plugin_path)
 
 
driver = webdriver.Chrome(executable_path="C:\chromedriver.exe", chrome_options=co)
driver.get("http://ip138.com/")
print(driver.page_source)
 
###################
options = webdriver.ChromeOptions()
options.add_argument('--proxy-server=http://ip:port')  
driver = webdriver.Chrome(executable_path="C:\chromedriver.exe", chrome_options=0ptions)
driver.get("http://ip138.com/")
print(driver.page_source)
 

def main():
    pass

if '__main__' == __name__:
    main()

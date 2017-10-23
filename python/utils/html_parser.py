# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月20日 肖飞 All rights reserved
#   
#   文件名称：html_parser.py
#   创 建 者：肖飞
#   创建日期：2017年10月20日 星期五 17时14分10秒
#   修改日期：2017年10月20日 星期五 17时17分05秒
#   描    述：
#
#================================================================
from html.parser import import HTMLParser

class html_parser(HTMLParser):  
    valid = False  
      
    def handle_starttag(self,tag,attr):  
        if tag == 'a':  
            self.valid = True  
            #print (dict(attr))  
              
    def handle_endtag(self,tag):  
        if tag == 'a':  
            self.valid = False  
              
    def handle_data(self,data):  
        if self.valid:  
            print (data)  
def main():
    pass

if '__main__' == __name__:
    main()

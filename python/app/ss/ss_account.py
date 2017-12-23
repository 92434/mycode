# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月23日 肖飞 All rights reserved
#   
#   文件名称：ss_account.py
#   创 建 者：肖飞
#   创建日期：2017年12月23日 星期六 09时21分51秒
#   修改日期：2017年12月23日 星期六 15时05分54秒
#   描    述：
#
#================================================================
import sys
import optparse
import lxml.etree
import os
from prettytable import PrettyTable
try:
    import json
except ImportError:
    import simplejson as json

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

import request

r = request.request()

def get_ss_account():
    list_accounts = []
    url = 'https://global.ishadowx.net/'
    data = r.request.get(url)
    #logger.debug('data:%s' %(data))
    html = lxml.etree.HTML(data)
    node = html.xpath('//div[@class="portfolio-items"]/div/div[@class="portfolio-item"]/div/div[@class="hover-text"]')
    #logger.debug('node:%s' %([(i.items(), i.text) for i in node]))
    for node_account in node:
        node_info = node_account.xpath('./h4')
        account = []
        for node_item in node_info:
            if not node_item.text:
                continue

            item_info = node_item.xpath('./span[1]/text()')
            if not len(item_info):
                continue

            name = '%s' %(node_item.text.strip())
            value = '%s' %(item_info[0].strip())
            #logger.debug('%s%s' %(name, value))
            if name in ['IP Address:', 'Port:', 'Password:']:
                account.append(value)
        list_accounts.append(account)
    #logger.debug('%s' %(list_accounts))
    return list_accounts

def show_list_accounts(list_accounts):
    row_title = ['序列号', 'ip', '端口', '密码']
    table = PrettyTable(row_title)
    table.padding_width = 1
    for i, account in enumerate(list_accounts):
        list_row = [i] + account
        table.add_row(list_row)
    print(table)

def gen_ss_conf(list_accounts):
    index = int(raw_input("请输入要使用的序列号:"))
    if index < 0:
        return
    if index >= len(list_accounts):
        return
    
    #logger.debug('%s' %(list_accounts[index]))
    ip, port, password = list_accounts[index]
    
    dict_conf = {
            'local_address' : '127.0.0.1',
            'local_port' : 1080,
            'timeout' : 300,
            'method' : 'aes-256-cfb',
            'fast_open' : False,
            }
    item = {'server' : ip}
    dict_conf.update(item)
    item = {'server_port' : int(port)}
    dict_conf.update(item)
    item = {'password' : password}
    dict_conf.update(item)
    with open('config.json', 'wb+') as f:
        f.write(json.dumps(dict_conf, sort_keys=False, indent=2))

def main(argv):
    list_accounts = get_ss_account()
    if not len(list_accounts):
        logger.debug('')
        return

    show_list_accounts(list_accounts)
    gen_ss_conf(list_accounts)

if '__main__' == __name__:
    main(sys.argv[1:])

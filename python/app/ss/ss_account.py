# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月23日 肖飞 All rights reserved
#   
#   文件名称：ss_account.py
#   创 建 者：肖飞
#   创建日期：2017年12月23日 星期六 09时21分51秒
#   修改日期：2017年12月28日 星期四 11时13分24秒
#   描    述：
#
#================================================================
import sys
import optparse
import lxml.etree
import os
from prettytable import PrettyTable
import base64
import optparse
import re
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

def select_ss_account(list_accounts):
    index = int(raw_input("请输入要使用的序列号:"))
    if index < 0:
        return
    if index >= len(list_accounts):
        return
    
    #logger.debug('%s' %(list_accounts[index]))
    ip, port, password = list_accounts[index]

    return ip, port, password

def gen_ss_conf(ip, port, password):
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

def b64decode(b64_content):
    content_decoded = None
    count = 0
    while count < 3:
        pad = '=' * count
        try:
            content_decoded = base64.b64decode(b64_content + pad)
            break
        except:
            #logger.exception('')
            count += 1
    return content_decoded

def decode_ssr_link(ssr_link):
    dict_account = None
    content = ssr_link.replace('ssr://', '')
    content_decoded = b64decode(content)
    if not content_decoded:
        logger.debug('')
        return dict_account

    p = '(?P<ip>[^:]+):(?P<port>[^:]+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>[^/]+)/\?(?P<parameter>.*)'
    m = re.match(p, content_decoded)
    if not m:
        logger.debug('')
        return dict_account

    dict_account = m.groupdict()
    password = dict_account.pop('password_base64')
    password = b64decode(password)
    dict_account.update({'password' : password})
    parameter = dict_account.pop('parameter')
    list_parameter = parameter.split('&')
    for parameter_item in list_parameter:
        key, value = parameter_item.split('=')
        value = b64decode(value)
        dict_account.update({key : value})

    logger.debug('dict_account:%s' %(dict_account))
    return dict_account

def gen_ssr_conf(dict_account):
    dict_conf = {
            'server_ipv6': '::',
            'local_address': '127.0.0.1',
            'local_port': 1080,

            #'protocol_param': '',
            #'obfs': 'tls1.2_ticket_auth_compatible',
            #'speed_limit_per_con': 0,
            #'speed_limit_per_user': 0,

            #'additional_ports' : {}, // only works under multi-user mode
            #'additional_ports_only' : False, // only works under multi-user mode
            'timeout': 120,
            'udp_timeout': 60,
            'dns_ipv6': False,
            'connect_verbose_info': 0,
            'redirect': '',
            'fast_open': False,
            }

    ip = dict_account.get('ip')
    if not ip:
        logger.debug('')
        return
    item = {'server' : ip}
    dict_conf.update(item)

    server_port = dict_account.get('port')
    if not server_port:
        logger.debug('')
        return
    item = {'server_port' : int(server_port)}
    dict_conf.update(item)

    protocol = dict_account.get('protocol')
    if not protocol:
        logger.debug('')
        return
    item = {'protocol' : protocol}
    dict_conf.update(item)

    method = dict_account.get('method')
    if not method:
        logger.debug('')
        return
    item = {'method' : method}
    dict_conf.update(item)

    obfs = dict_account.get('obfs')
    if not obfs:
        logger.debug('')
        return
    item = {'obfs' : obfs}
    dict_conf.update(item)

    password = dict_account.get('password')
    if not password:
        logger.debug('')
        return
    item = {'password' : password}
    dict_conf.update(item)

    obfs_param = dict_account.get('obfsparam')
    item = {'obfs_param' : obfs_param}
    dict_conf.update(item)

    with open('config.json', 'wb+') as f:
        f.write(json.dumps(dict_conf, sort_keys=False, indent=2))

def ssr_link_account(ssr_link):
    dict_account = decode_ssr_link(ssr_link)
    gen_ssr_conf(dict_account)

def free_ss_account():
    list_accounts = get_ss_account()
    if not len(list_accounts):
        logger.debug('')
        return

    show_list_accounts(list_accounts)
    ip, port, password = select_ss_account(list_accounts)
    gen_ss_conf(ip, port, password)


def main(argv):
    options = optparse.OptionParser()
    options.add_option('-l', '--ssr_link', dest='ssr_link', help='ssr_link', metavar='URL', default = None)
    options.add_option('-s', '--ss_account', action='store_true', dest='ss_account', help='ss_account', default=False)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))

    if len(args):
        options.print_help()
        return
    if opts.ss_account:
        free_ss_account()
    elif opts.ssr_link:
        ssr_link_account(opts.ssr_link)
    else:
        options.print_help()


if '__main__' == __name__:
    main(sys.argv[1:])

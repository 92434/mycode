# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月23日 肖飞 All rights reserved
#   
#   文件名称：ss_account.py
#   创 建 者：肖飞
#   创建日期：2017年12月23日 星期六 09时21分51秒
#   修改日期：2017年12月30日 星期六 11时16分44秒
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

def ishadowx_account():
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

dict_web_addr_map = {
        'ishadowx' : ishadowx_account,
        }

def select_list_item(list_content):
    while True:
        index = int(raw_input("请输入要使用的序列号:"))
        if index < 0:
            continue
        if index >= len(list_content):
            continue
        
        return list_content[index]

def show_list_web_addr(list_web_addr):
    row_title = ['序列号', '网址']
    table = PrettyTable(row_title)
    table.padding_width = 1
    for i, addr in enumerate(list_web_addr):
        list_row = [i, addr]
        table.add_row(list_row)
    print(table)
    return select_list_item(list_web_addr)

def show_list_accounts(list_accounts):
    row_title = ['序列号', 'ip', '端口', '密码']
    table = PrettyTable(row_title)
    table.padding_width = 1
    for i, account in enumerate(list_accounts):
        list_row = [i] + account
        table.add_row(list_row)
    print(table)
    return select_list_item(list_accounts)


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

def decode_ss_link(link = ''):
    dict_account = {}
    item = {'is_ssr' : False}
    dict_account.update(item)

    if link.startswith('ssr://'):
        item = {'is_ssr' : True}
        dict_account.update(item)
        link = link.replace('ssr://', '')
    elif link.startswith('ss://'):
        link = link.replace('ss://', '')

    logger.debug('link:%s' %(link))
    decoded_link = b64decode(link)
    logger.debug('decoded_link:%s' %(decoded_link))

    parameter_start = decoded_link.find('/?')
    if parameter_start != -1:
        decoded_account = decoded_link[:parameter_start]

        item = {'is_ssr' : True}
        dict_account.update(item)
        parameter_start = parameter_start + len('/?')
        parameter = decoded_link[parameter_start:]
        list_parameter = parameter.split('&')
        for parameter_item in list_parameter:
            key, value = parameter_item.split('=')
            value = b64decode(value)
            item = {key : value}
            dict_account.update(item)
    else:
        decoded_account = decoded_link

    logger.debug('decoded_account:%s' %(decoded_account))
    is_ssr = dict_account.get('is_ssr')
    if is_ssr:
        p = '(?P<server>[^:]+):(?P<server_port>\d+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>[^/]+)'
        m = re.match(p, decoded_account)
        if m:
            dict_matched = m.groupdict()
            password = dict_matched.pop('password_base64')
            password = b64decode(password)
            item = {'password' : password}
            dict_matched.update(item)
            dict_account.update(dict_matched)

            if 'obfsparam' in dict_account:
                obfs_param = dict_account.pop('obfsparam')
                item = {'obfs_param' : obfs_param}
                dict_account.update(item)

            if 'protoparam' in dict_account:
                protocol_param = dict_account.pop('protoparam')
                item = {'protocol_param' : protocol_param}
                dict_account.update(item)

            if 'remarks' in dict_account:
                dict_account.pop('remarks')
            if 'group' in dict_account:
                dict_account.pop('group')

            item = {'redirect' : ''}
            dict_account.update(item)

            item = {'server_ipv6' : '::'}
            dict_account.update(item)
            item = {'dns_ipv6' : False}
            dict_account.update(item)
            item = {'udp_timeout' : 60}
            dict_account.update(item)
            item = {'connect_verbose_info' : 1}
            dict_account.update(item)
    else:
        p = '(?P<method>[^:]+):(?P<password>[^@]+)@(?P<server>[^:]+):(?P<server_port>\d+)'
        m = re.match(p, decoded_account)
        if m:
            dict_matched = m.groupdict()
            dict_account.update(dict_matched)

    item = {'local_address' : '127.0.0.1'}
    dict_account.update(item)
    item = {'local_port' : 1080}
    dict_account.update(item)
    item = {'timeout' : 120}
    dict_account.update(item)
    item = {'fast_open' : False}
    dict_account.update(item)

    dict_account.pop('is_ssr')
    
    return dict_account

def gen_ss_conf(dict_account):
    logger.debug('dict_account:%s' %(dict_account))
    with open('config.json', 'wb+') as f:
        f.write(json.dumps(dict_account, sort_keys=False, indent=2))


def ss_link_account(ss_link):
    dict_account = decode_ss_link(ss_link)
    gen_ss_conf(dict_account)

def free_ss_account():
    list_web = dict_web_addr_map.keys()
    web_addr = show_list_web_addr(list_web)
    account_func = dict_web_addr_map.get(web_addr)
    list_accounts = account_func()

    if not len(list_accounts):
        logger.debug('')
        return

    server, server_port, password = show_list_accounts(list_accounts)
    dict_account = decode_ss_link('')
    item = {'server' : server}
    dict_account.update(item)
    item = {'server_port' : int(server_port)}
    dict_account.update(item)
    item = {'password' : password}
    dict_account.update(item)
    gen_ss_conf(dict_account)


def main(argv):
    options = optparse.OptionParser()
    options.add_option('-l', '--ssr_link', dest='ssr_link', help='ssr_link', metavar='URL', default = None)
    options.add_option('-a', '--ss_account', action='store_true', dest='ss_account', help='ss_account', default=False)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))

    if len(args):
        options.print_help()
        return
    if opts.ss_account:
        free_ss_account()
    elif opts.ssr_link:
        ss_link_account(opts.ssr_link)
    else:
        options.print_help()


if '__main__' == __name__:
    main(sys.argv[1:])

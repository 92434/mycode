# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月23日 肖飞 All rights reserved
#   
#   文件名称：ss_account.py
#   创 建 者：肖飞
#   创建日期：2017年12月23日 星期六 09时21分51秒
#   修改日期：2018年01月07日 星期日 23时02分23秒
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
logging = log.dict_configure(default_log_to_file = True)
logger = logging.getLogger('default')

import request

r = request.request()

def b64decode(b64_content):
    content_decoded = ''
    left = len(b64_content) % 4
    pad = '=' * (4 - left) if left != 0 else ''
    #logger.debug('pad:%s' %(pad))
    try:
        content_decoded = base64.b64decode(b64_content + pad)
    except:
        #logger.exception('')
        pass
    return content_decoded

def b64decode_retry(b64_content):
    content_decoded = ''
    while b64_content:
        content_decoded = b64decode(b64_content)
        if content_decoded:
            return content_decoded
        else:
            b64_content = b64_content[:-1]
    return content_decoded

def decode_utf8_retry(utf8_content):
    while utf8_content:
        try:
            utf8_content = utf8_content.decode('utf8').encode('utf8')
            return utf8_content
        except:
            utf8_content = utf8_content[:-1]
            #logger.exception('')
            continue
    return utf8_content

def filter_ss_link(link):
    filtered_link = ''
    p = '[A-Za-z0-9\+-]'
    for i in link:
        m = re.match(p, i)
        if m:
            filtered_link += i
    return filtered_link

def decode_ss_link(link = ''):
    dict_account = {}
    item = {'is_ssr' : False}
    dict_account.update(item)

    p = 's.*s.*r.*:.*/.*/'
    if re.search(p, link):
        item = {'is_ssr' : True}
        dict_account.update(item)

    p = 's.*s.*r*.*:.*/.*/'
    link = re.sub(p, '', link)

    link = filter_ss_link(link)

    logger.debug('link:%s' %(link))

    decoded_link = b64decode_retry(link)
    logger.debug('decoded_link:%s' %(decoded_link))

    parameter_start = decoded_link.rfind('/?')
    if parameter_start != -1:
        item = {'is_ssr' : True}
        dict_account.update(item)

        decoded_account = decoded_link[:parameter_start]
        parameter_start = parameter_start + len('/?')
        parameter = decoded_link[parameter_start:]
        logger.debug('parameter:%s' %(parameter))

        list_parameter = parameter.split('&')
        for parameter_item in list_parameter:
            try:
                key, value = parameter_item.split('=')
            except:
                continue
            value = b64decode_retry(value)
            value = decode_utf8_retry(value)
            item = {key : value}
            dict_account.update(item)
    else:
        decoded_account = decoded_link

    logger.debug('decoded_account:%s' %(decoded_account))
    is_ssr = dict_account.pop('is_ssr')
    if is_ssr:
        p = '(?P<server>[^:]+):(?P<server_port>\d+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>.+)'
        m = re.match(p, decoded_account)
        if m:
            dict_matched = m.groupdict()
            password = dict_matched.pop('password_base64')
            password = b64decode_retry(password)
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

            #if 'remarks' in dict_account:
            #    dict_account.pop('remarks')
            #if 'group' in dict_account:
            #    dict_account.pop('group')

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
        p = '(?P<method>[^:]+):(?P<password>.+)@(?P<server>[^:]+):(?P<server_port>\d+)'
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

    return dict_account

def select_list_item(list_content):
    while True:
        index = int(raw_input("请输入要使用的序列号:"))
        if index < 0:
            continue
        if index >= len(list_content):
            continue
        
        return list_content[index]

def show_list(list_title, list_content):
    table = PrettyTable(list_title)
    table.padding_width = 1
    for i, list_row in enumerate(list_content):
        if type(list_row) != list:
            list_row = [list_row]
        list_row = [i] + list_row
        table.add_row(list_row)
    print(table)

def ishadowx_list_account():
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

def ishadowx_account():
    list_accounts = ishadowx_list_account()
    show_list(['序列号', 'ip', '端口', '密码'], list_accounts)
    server, server_port, password = select_list_item(list_accounts)
    kwargs = {}
    item = {'server' : server}
    kwargs.update(item)
    item = {'server_port' : int(server_port)}
    kwargs.update(item)
    item = {'password' : password}
    kwargs.update(item)
    ss_parameter_account(**kwargs)

def mu_sccount(url):
    data = r.request.get(url)
    content_decoded = b64decode_retry(data)
    content_decoded = content_decoded.strip()
    list_ssr = content_decoded.splitlines()
    logger.debug('list_ssr:%s' %(list_ssr))
    list_dict_account = []
    #free = '0倍率'
    for ssr_link in list_ssr:
        dict_account = decode_ss_link(ssr_link)
        #if not free in dict_account.pop('remarks'):
        #    continue
        list_dict_account.append(dict_account)

    list_remarks = [account.get('remarks') for account in list_dict_account]
    show_list(['序列号', '帐号'], list_remarks)
    dict_account = select_list_item(list_dict_account)
    gen_ss_conf(dict_account)

def yahaha_account():
    url = 'http://www.yahaha.xyz/link/g7dxNkjdaNNg53Qg?mu=0'
    mu_sccount(url)

def ashin_account():
    url = 'https://ashin.fun/link/0LHX6exDqSREOz1m?mu=0'
    mu_sccount(url)

def doub_account():
    url = 'https://doub.io/sszhfx/'
    proxies = {
        'https' : '127.0.0.1:8087'
            }
    #data = r.request.get(url, headers = r.request.fake_headers, proxies = proxies)
    f = open('test.html', 'rb')
    data = f.read()
    f.close()

    #logger.debug('data:%s' %(data))
    html = lxml.etree.HTML(data)
    node = html.xpath('//table[@width="100%"]/tbody/tr')
    #logger.debug('node:%s' %([(i.items(), i.text) for i in node]))

    if not node:
        return

    header = node[0]
    content = node[1:]

    list_des = header.xpath('./th/text()')
    #logger.debug('list_des:%s' %(list_des))
    list_content = []
    list_ss_link = []
    for item in content:
        list_item = item.xpath('./td')
        link = list_item[-1]
        list_link = link.xpath('./a[@class="dl1" and @href]/@href')
        list_item = [re.sub('[\s]+', ' ', i.xpath('string(.)').strip()) for i in list_item]
        for link in list_link:
            p = 's.*s.*r*://'
            m = re.search(p, link)
            if not m:
                continue
            link = link[m.start():]
            logger.debug('link:%s' %(link))
            list_ss_link.append(link)
            logger.debug('m.group():%s' %(m.group()))
            #logger.debug('list_item:%s' %(list_item))
            list_content.append(list_item[:-1] + [m.group().replace('://', '')])

    show_list(['序列号'] + list_des, list_content)
    ss_link = select_list_item(list_ss_link)
    logger.debug('ss_link:%s' %(ss_link))
    ss_link_account(ss_link)

def gen_ss_conf(dict_account):
    logger.debug('dict_account:%s' %(dict_account))
    with open('config.json', 'wb+') as f:
        f.write(json.dumps(dict_account, sort_keys=False, indent=2))


def ss_link_account(ss_link):
    dict_account = decode_ss_link(ss_link)
    gen_ss_conf(dict_account)

def ss_parameter_account(**kwargs):
    dict_account = decode_ss_link('')
    dict_account.update(kwargs)
    gen_ss_conf(dict_account)

dict_web_addr_map = {
        'ishadowx' : ishadowx_account,
        'yahaha' : yahaha_account,
        'ashin' : ashin_account,
        'doub' : doub_account,
        }

def free_ss_account():
    list_web = dict_web_addr_map.keys()
    show_list(['序列号', '网址'], list_web)
    web_addr = select_list_item(list_web)
    account_func = dict_web_addr_map.get(web_addr)
    account_func()

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

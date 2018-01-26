# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月23日 肖飞 All rights reserved
#   
#   文件名称：ss_account.py
#   创 建 者：肖飞
#   创建日期：2017年12月23日 星期六 09时21分51秒
#   修改日期：2018年01月26日 星期五 12时56分21秒
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


def filter_ss_link(link):
    p = '[^\w\+/=]'
    link_url = link.replace('-', '+')
    link_url = link_url.replace('_', '/')

    filtered_link = re.split(p, link_url)
    if len(filtered_link) == 1:
        return filtered_link

    link_url = link.replace('!', '+')
    link_url = link_url.replace('-', '/')
    filtered_link = re.split(p, link_url)
    if len(filtered_link) == 1:
        return filtered_link
    return None

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
    list_b64_content = filter_ss_link(b64_content)
    logger.debug('list_b64_content:%s' %(list_b64_content))
    b64_content = list_b64_content[0]

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
            logger.exception(utf8_content)
            utf8_content = utf8_content[:-1]
            continue
    return utf8_content

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

    decoded_link = b64decode_retry(link)
    list_decoded_link = decoded_link.split('/?')
    logger.debug('list_decoded_link:%s' %(list_decoded_link))

    if len(list_decoded_link) == 2:
        item = {'is_ssr' : True}
        dict_account.update(item)

        decoded_account, parameter = list_decoded_link

        logger.debug('parameter:%s' %(parameter))
        list_parameter = parameter.split('&')
        for parameter_item in list_parameter:
            try:
                key, value = parameter_item.split('=')
            except:
                continue
            value = b64decode_retry(value)
            value = decode_utf8_retry(value)
            logger.debug('%s:%s' %(key, value))
            item = {key : value}
            dict_account.update(item)

    elif len(list_decoded_link) == 1:
        decoded_account = list_decoded_link[0]

    logger.debug('decoded_account:%s' %(decoded_account))
    is_ssr = dict_account.pop('is_ssr')
    if is_ssr:
        p = '(?P<server>[^:]+):(?P<server_port>\d+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>[\w\+/=\-\!]+)'
        #p_ipv6 = '(?P<server_ipv6>[0-9a-fA-F:]+):(?P<server_port>\d+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>[\w\+/=\-\!]+)'
        p_ipv6 = '(?P<server>[0-9a-fA-F:]+):(?P<server_port>\d+):(?P<protocol>[^:]+):(?P<method>[^:]+):(?P<obfs>[^:]+):(?P<password_base64>[\w\+/=\-\!]+)'
        m = None
        if not m:
            m = re.match(p, decoded_account)
        if not m:
            m = re.match(p_ipv6, decoded_account)
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

            #item = {'dns_ipv6' : False}
            #dict_account.update(item)
            item = {'udp_timeout' : 60}
            dict_account.update(item)
    else:
        p = '(?P<method>[^:]+):(?P<password>.+)@(?P<server>[^:]+):(?P<server_port>\d+)'
        #p_ipv6 = '(?P<method>[^:]+):(?P<password>.+)@(?P<server_ipv6>[0-9a-fA-F:]+):(?P<server_port>\d+)'
        p_ipv6 = '(?P<method>[^:]+):(?P<password>.+)@(?P<server>[0-9a-fA-F:]+):(?P<server_port>\d+)'
        m = None
        if not m:
            m = re.match(p, decoded_account)
        if not m:
            m = re.match(p_ipv6, decoded_account)
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
    item = {'connect_verbose_info' : 1}
    dict_account.update(item)

    #logger.debug('dict_account:%s' %(dict_account))
    return dict_account

def gen_ss_conf(dict_account):
    logger.debug('dict_account:%s' %(dict_account))
    with open('config.json.%s.%s' %(dict_account.get('server'), dict_account.get('server_port')), 'wb+') as f:
        f.write(json.dumps(dict_account, sort_keys=False, indent=2))

def ss_link_account(ss_link):
    dict_account = decode_ss_link(ss_link)
    gen_ss_conf(dict_account)

def ss_parameter_account(**kwargs):
    dict_account = decode_ss_link('')
    dict_account.update(kwargs)
    gen_ss_conf(dict_account)

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

def mu_sccount(url):
    data = r.request.get(url)
    logger.debug('data:%s' %(data))
    content_decoded = b64decode_retry(data)
    logger.debug('content_decoded:%s' %(content_decoded))
    p = '[^\w\+/=\-\!:]'
    content_decoded = re.sub(p, '\n', content_decoded)
    content_decoded = content_decoded.strip()
    logger.debug('content_decoded:%s' %(content_decoded))
    list_ssr = content_decoded.split()
    logger.debug('list_ssr:%s' %(list_ssr))
    list_dict_account = []
    #free = '0倍率'
    for ssr_link in list_ssr:
        ssr_link = ssr_link.strip()
        if not ssr_link:
            continue
        logger.debug('ssr_link:%s' %(ssr_link))
        dict_account = decode_ss_link(ssr_link)
        #if not free in dict_account.pop('remarks'):
        #    continue
        list_dict_account.append(dict_account)

    list_remarks = [account.get('remarks') for account in list_dict_account]
    show_list(['序列号', '帐号'], list_remarks)
    dict_account = select_list_item(list_dict_account)
    gen_ss_conf(dict_account)

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

def rfcvps_club_account():
    url = 'https://free.rfcvps.club/'
    data = r.request.get(url, headers = r.request.fake_headers)
    #logger.debug('data:%s' %(data))
    html = lxml.etree.HTML(data)
    node = html.xpath('//body/div/div[@class="address col-md-12 col-lg-12"]/text()')
    #logger.debug('node:%s' %(node))
    show_list(['序列号', 'ssr'], node)
    ss_link = select_list_item(node)
    #logger.debug('ss_link:%s' %(ss_link))
    ss_link_account(ss_link)

def doub_account():
    #url = 'https://doub.io/sszhfx/'
    #proxies = {
    #        'https' : 'socks5h://127.0.0.1:1080'
    #        }
    #data = r.request.get(url, headers = r.request.fake_headers, proxies = proxies)
    url = 'https://doub.bid/sszhfx/'
    data = r.request.get(url, headers = r.request.fake_headers)
    #f = open('test.html', 'rb')
    #data = f.read()
    #f.close()

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
            #link = link[m.start():]
            logger.debug('link:%s' %(link))
            list_ss_link.append(link)
            #logger.debug('m.group():%s' %(m.group()))
            #logger.debug('list_item:%s' %(list_item))
            list_content.append(list_item[:-1] + [m.group().replace('://', '')])

    show_list(['序列号'] + list_des, list_content)
    ss_link = select_list_item(list_ss_link)
    #logger.debug('ss_link:%s' %(ss_link))
    data = r.request.get(ss_link, headers = r.request.fake_headers)
    html = lxml.etree.HTML(data)
    node = html.xpath('//body/p[@style="border-bottom: solid 1px #ddd;"]/a[@href]/@href')
    ss_link = node[0]
    ss_link_account(ss_link)

def yahaha_account():
    url = 'http://www.yahaha.win/link/g7dxNkjdaNNg53Qg?mu=0'
    mu_sccount(url)

def xiaoheijia_account():
    url = 'http://test.xiaoheijia.top/link/5a4NTg1zCaKtnfoh?mu=0'
    mu_sccount(url)

def luckspeed_account():
    url = 'http://luckyss.ml/link/zv0HLPO55WN8xsbT?mu=0'
    mu_sccount(url)

def ssr_share_account():
    list_url = [
        'https://yzzz.ml/freessr',
        'http://ssr.izdvy.cn/freessr',
        'http://javanet.top/freessr',
        'https://ftp.ssrshare.com/dmsub.txt',
            ]
    show_list(['序列号', 'ssr地址'], list_url)
    url = select_list_item(list_url)
    mu_sccount(url)

def ashin_account():
    url = 'https://ashin.fun/link/0LHX6exDqSREOz1m?mu=0'
    mu_sccount(url)

dict_web_addr_map = {
        'ishadowx' : ishadowx_account,
        'yahaha' : yahaha_account,
        #'ashin' : ashin_account,
        'doub' : doub_account,
        'ssr_share' : ssr_share_account,
        'rfcvps_club' : rfcvps_club_account,
        'xiaoheijia' : xiaoheijia_account,
        'luckspeed' : luckspeed_account,
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

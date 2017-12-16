# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月16日 肖飞 All rights reserved
#   
#   文件名称：691gao_com.py
#   创 建 者：肖飞
#   创建日期：2017年12月16日 星期六 09时19分34秒
#   修改日期：2017年12月16日 星期六 10时45分10秒
#   描    述：
#
#================================================================
import lxml.etree
import sys
import log
import optparse
import os
import request

reload(sys)  
sys.setdefaultencoding('utf-8')

logging = log.dict_configure()
logger = logging.getLogger('default')

r = request.request()

def get_part_urls_from_m3u8(url_m3u8):
    url_files = []

    basename = os.path.basename(url_m3u8)
    p = r.request.urlparse.urlparse(url_m3u8)
    m3u8_domain = '%s://%s' %(p.scheme, p.netloc)
    m3u8_dir = os.path.dirname(p.path)
    head = r.request.urlparse.urljoin(m3u8_domain, m3u8_dir)
    head += '/'
    #logger.debug('head:%s' %(head))

    if url_m3u8.endswith('.m3u8'):
        content = r.request.get(url_m3u8);
        #logger.debug('content:%s' %(content))

        lines = content.splitlines()
        for i in lines:
            line = i.strip()
            if not line.startswith('#'):
                url_files.append(line)

        url_files = map(lambda x : r.request.urlparse.urljoin(head, x), url_files)
        #logger.debug('url_files:%s' %(url_files))

    return url_files

def get_691gao_diao_info(play_url, html):
    urls = []
    output_filename = None

    e_title = html.xpath('//title/text()')
    if not len(e_title):
        return output_filename, urls

    #logger.debug('e_title:%s' %([(i.items(), i.text) for i in e_title]))
    title = e_title[0]
    p =  u'(.*)在线观看.*'
    filetitle = r.request.r(p, title, 1)
    if not filetitle:
        return output_filename, urls

    output_filename = os.path.join('%s' %(filetitle), '%s.mp4' %(filetitle))
    #logger.debug('output_filename:%s' %(output_filename))

    e_player = html.xpath('//*[@type="text/javascript"]/text()')
    if not len(e_player):
        return output_filename, urls

    ck_player_value = e_player[0]
    p = 'video=\["([^"]+)"\]'
    url_m3u8 = r.request.r(p, ck_player_value, 1)
    if not url_m3u8:
        return output_filename, urls

    #logger.debug('url_m3u8:%s' %(url_m3u8))
    urls = get_part_urls_from_m3u8(url_m3u8)

    return output_filename, urls

def get_691gao_list_info(play_url, html):
    urls = []
    output_filename = None

    e_play_url = html.xpath('//div[@class="l"]/a[@class="txt" and @href]/@href')
    if not len(e_play_url):
        #logger.debug('')
        return output_filename, urls
    e_play_url = e_play_url[0]

    p = r.request.urlparse.urlparse(play_url)
    domain = '%s://%s' %(p.scheme, p.netloc)
    play_url = r.request.urlparse.urljoin(domain, e_play_url)
    logger.debug('play_url:%s' %(play_url))

    data = r.request.get(play_url)
    html = lxml.etree.HTML(data)
    e_title = html.xpath('//title/text()')
    logger.debug('e_title:%s' %(e_title))
    if not len(e_title):
        logger.debug('data:%s' %(data))
        return output_filename, urls

    e_title = e_title[0]

    title = e_title
    p =  u'正在播放 (\d+)-(.*)'
    index = r.request.r(p, title, 1)
    filetitle = r.request.r(p, title, 2)
    if not index or not filetitle:
        p =  u'正在播放 (.*)'
        index = 1
        filetitle = r.request.r(p, title, 1)
        if not filetitle:
            logger.debug('data:%s' %(data))
            return output_filename, urls

    output_filename = os.path.join('%s' %(filetitle), '%s-%s.mp4' %(filetitle, index))
    #logger.debug('output_filename:%s' %(output_filename))

    e_player = html.xpath('//*[@type="text/javascript"]/text()')
    logger.debug('e_player:%s' %(e_player))
    if not len(e_player):
        logger.debug('data:%s' %(data))
        return output_filename, urls

    ck_player_value = e_player[0]
    p = 'video=\["([^"]+)->video/mp4"\]'
    url = r.request.r(p, ck_player_value, 1)
    if not url:
        logger.debug('data:%s' %(data))
        return output_filename, urls
    logger.debug('url:%s' %(url))
    if url:
        urls.append(url)

    return output_filename, urls

def parse_url(play_url):
    data = r.request.get(play_url)
    #logger.debug('data:%s' %(data))

    html = lxml.etree.HTML(data)
    output_filename, urls = get_691gao_list_info(play_url, html)
    if output_filename and len(urls):
        return output_filename, urls

    output_filename, urls = get_691gao_diao_info(play_url, html)
    return output_filename, urls

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-p', '--play-url', dest='play_url', help='play_url', metavar='URL', default = None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    output_filename, urls = parse_url(opts.play_url)
    logger.debug('output_filename:%s' %(output_filename))
    logger.debug('urls:%s' %(urls))

if '__main__' == __name__:
    main(sys.argv[1:])

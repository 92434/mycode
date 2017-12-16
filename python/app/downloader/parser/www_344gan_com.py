# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月16日 肖飞 All rights reserved
#   
#   文件名称：www_344gan_com.py
#   创 建 者：肖飞
#   创建日期：2017年12月16日 星期六 12时04分26秒
#   修改日期：2017年12月16日 星期六 12时19分46秒
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

def get_play_url_info(play_url, html):
    urls = []
    output_filename = None

    filetitle = html.xpath('//title/text()')
    if not len(filetitle) == 1:
        return output_filename, urls
    filetitle = filetitle[0]
    p =  u'(.*)在线播放.*'
    filetitle = r.request.r(p, filetitle, 1)
    if not filetitle:
        return output_filename, urls
    logger.debug('filetitle:%s' %(filetitle))
    output_filename = os.path.join('%s' %(filetitle), '%s.mp4' %(filetitle))

    url = html.xpath('//div[@class="film_info clearfix"]/script/@src[contains(., "playdata")]')
    logger.debug('script:%s' %(html.xpath('//div[@class="film_info clearfix"]/script/@src')))
    logger.debug('url:%s' %(url))
    if not len(url) == 1:
        return output_filename, urls
    url = url[0]
    p = r.request.urlparse.urlparse(play_url)
    domain = '%s://%s' %(p.scheme, p.netloc)
    url = r.request.urlparse.urljoin(domain, url)
    logger.debug('url:%s' %(url))
    data = r.request.get(url)
    #logger.debug('data:%s' %(data))
    url = data.split(u'$')
    logger.debug('url:%s' %(url))
    if not len(url) == 3:
        return output_filename, urls
    url_m3u8 = url[1]
    logger.debug('url_m3u8:%s' %(url_m3u8))
    urls = get_part_urls_from_m3u8(url_m3u8)
    return output_filename, urls

def parse_url(play_url):
    data = r.request.get(play_url)
    #logger.debug('data:%s' %(data))

    html = lxml.etree.HTML(data)
    output_filename, urls = get_play_url_info(play_url, html)
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

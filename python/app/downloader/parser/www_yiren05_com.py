# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月16日 肖飞 All rights reserved
#   
#   文件名称：www_yiren05_com.py
#   创 建 者：肖飞
#   创建日期：2017年12月16日 星期六 11时55分26秒
#   修改日期：2017年12月16日 星期六 12时02分02秒
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

def get_yiren05_info(play_url, html):
    urls = []
    output_filename = None

    filetitle = html.xpath('//div[@class="mainAreaBlack"]/*[@class="navBarBlack px19"]/span/font/text()')
    if not len(filetitle) == 1:
        return output_filename, urls
    filetitle = filetitle[0]
    logger.debug('filetitle:%s' %(filetitle))
    output_filename = os.path.join('%s' %(filetitle), '%s.mp4' %(filetitle))
    urls = html.xpath('//video/source/@src')
    logger.debug('url:%s' %(urls))
    #logger.debug('urls:%s' %([(i.tag, i.items(), i.text) for i in urls]))

    return output_filename, urls

def parse_url(play_url):
    data = r.request.get(play_url)
    #logger.debug('data:%s' %(data))

    html = lxml.etree.HTML(data)
    output_filename, urls = get_yiren05_info(play_url, html)
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

# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年12月16日 星期六 11时49分20秒
#   描    述：
#
#================================================================
import sys
import optparse
import importlib

import downloader

import log
import request

reload(sys)  
sys.setdefaultencoding('utf-8')

logging = log.dict_configure()
logger = logging.getLogger('default')

r = request.request()

class ts_downloader(object):
    def __init__(self, jobs, threads, play_url, dry_run):
        self.jobs = jobs
        self.threads = threads
        self.dl = downloader.downloader()
        self.dry_run = dry_run
        self.play_url = play_url
        self.output_filename = None

    def parse_play_url(self):
        ret = False

        p = r.request.urlparse.urlparse(self.play_url)
        module_name = p.netloc.replace('.', '_')

        m = importlib.import_module('.'.join(['parser', module_name]))
        self.output_filename, self.urls = m.parse_url(self.play_url)
        if self.output_filename and len(self.urls):
            ret = True

        return ret

    def download_video(self):
        logger.debug('get %s total_size...' %(self.output_filename))

        pieces_size = downloader.r.request.urls_size(self.urls[:5])
        total_size = pieces_size * len(self.urls) / (len(self.urls[:5]))
        #total_size = self.dl.urls_size(self.urls)
        logger.debug('total_size:%d' %(total_size))

        self.dl.download_urls(self.urls, self.output_filename, total_size, jobs = self.jobs, threads = self.threads, dry_run = self.dry_run)

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default = 4)
    options.add_option('-t', '--threads', type='int', dest='threads', help='threads', default = 4)
    options.add_option('-p', '--play-url', dest='play_url', help='play_url', metavar='URL', default = None)
    options.add_option('-d', '--dry-run', action='store_true', dest='dry_run', help='dry_run', default=False)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.play_url:
        options.print_help()
        return

    dl = ts_downloader(opts.jobs, opts.threads, opts.play_url, opts.dry_run)

    ret = dl.parse_play_url()
    if not ret:
        return

    dl.download_video()

if '__main__' == __name__:
    main(sys.argv[1:])

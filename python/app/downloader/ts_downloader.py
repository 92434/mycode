#!/usr/bin/env python
# -*- coding: utf-8 -*-
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年08月01日 星期二 14时27分12秒
#   描    述：
#
#================================================================
import sys
import os
import downloader
import optparse
import log

logging = log.log().get_logger('debug')

class ts_downloader(object):
    url_m3u8 = ''
    jobs = 0
    output_filename = ''
    curdir = '.'
    dl = None
    dry_run = False

    def __init__(self, url_m3u8, jobs, output_filename, dry_run):
        self.url_m3u8 = url_m3u8
        self.jobs = jobs
        self.output_filename = output_filename
        self.output_dir = os.path.join(os.path.curdir, os.path.splitext(self.output_filename)[0])
        self.dl = downloader.downloader()
        self.dry_run = dry_run

    def parse_m3u8(self):
        url_files = []
        fetched = set()
        head, tail = os.path.split(self.url_m3u8)
        title, ext = os.path.splitext(tail)
        ext = ext[1:]

        total_size = self.dl.urls_size([self.url_m3u8])
        #print('total_size:%d' %(total_size))

        filepath = os.path.join(self.output_dir, tail)

        if filepath.endswith('.m3u8'):
            self.dl.download_urls([self.url_m3u8], title, ext, total_size, jobs = self.jobs, output_dir = self.output_dir, dry_run = self.dry_run)

        if os.path.exists(filepath) and os.path.getsize(filepath):
            with open(filepath, 'r') as f:
                content = f.read().splitlines()
                for i in content:
                    line = i.strip()
                    if not line.startswith('#'):
                        url_files.append(line)

            url_files = map(lambda x : os.path.join(head, x), url_files)

        return url_files

    def download_video(self, url_files):
        logging.debug('get %s total_size...', os.path.join(self.output_dir, self.output_filename))
        total_size = self.dl.urls_size(url_files)
        logging.debug('total_size:%d' %(total_size))
        title, ext = os.path.splitext(self.output_filename)
        ext = ext[1:]
        self.dl.download_urls(url_files, title, ext, total_size, jobs = self.jobs, output_dir = self.output_dir, dry_run = self.dry_run)

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default = 4)
    options.add_option('-o', '--output-filename', dest='output_filename', help='output_filename', default = None)
    #options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default='http://201610.shipinmp4.com/x/20170419/ekdv-478/1/hls/index.m3u8')
    options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default = None)
    options.add_option('-d', '--dry-run', action='store_true', dest='dry_run', help='dry_run', default=False)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args) or not opts.output_filename or not opts.url_m3u8:
        options.print_help()
        return

    dl = ts_downloader(opts.url_m3u8, opts.jobs, opts.output_filename, opts.dry_run)
    url_files = dl.parse_m3u8()
    dl.download_video(url_files)

if '__main__' == __name__:
	main()
#python ts_downloader.py -u http://chyd-sn.wasu.tv/tbvideo/20141108/a5715565-44de-43ff-864d-2e8c5011e361.m3u8 -o '肖飞.mp4'
#http://mp4.146gan.com/20170720/A47ED6/index.m3u8 -o "和服美女在自己卧室被干的高潮连连.mp4"

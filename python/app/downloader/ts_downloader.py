#!/usr/bin/env python
# -*- coding: utf-8 -*-
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年07月31日 星期一 22时51分58秒
#   描    述：
#
#================================================================
import sys
import os

class ts_downloader(object):
    def __init__(self, url_m3u8, jobs, output_filename):
        self.url_m3u8 = url_m3u8
        self.jobs = jobs
        self.output_filename = output_filename

    def parse_m3u8():
        url_files = []
        fetched = set()

        head, tail = os.path.split(self.url_m3u8)
        filepath = os.path.join(os.path.curdir, tail)

        if filepath.endswith('.m3u8'):
            while len(fetched) != len([self.url_m3u8]):
                fetched = self.download_urls([self.url_m3u8], )

        if os.path.exists(filepath):
            with open(filepath, 'r') as f:
                content = f.read().splitlines()
                for i in content:
                    line = i.strip()
                    if not line.startswith('#'):
                        url_files.append(line)

            url_files = map(lambda x : os.path.join(head, x), url_files)
            os.remove(filepath)

        return url_files

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default=6)
    options.add_option('-o', '--output-filename', dest='output_filename', help='output_filename', default='out.mp4')
    #options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default='http://201610.shipinmp4.com/x/20170419/ekdv-478/1/hls/index.m3u8')
    options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default='')
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    ts_downloader = ts_downloader(opts.url_m3u8, opts.jobs, opts.output_filename)

if '__main__' == __name__:
	main()

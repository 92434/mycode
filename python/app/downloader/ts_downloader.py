#!/usr/bin/env python
# -*- coding: utf-8 -*-
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年08月02日 星期三 10时39分01秒
#   描    述：
#
#================================================================
import sys
import os
import downloader
import optparse
import log
import urllib
import re

logging = log.log().get_logger('debug')

class ts_downloader(object):
    url_m3u8 = ''
    jobs = 0
    output_filename = ''
    output_dir = ''
    dl = None
    dry_run = False

    def __init__(self, url_m3u8, jobs, output_filename, dry_run):
        self.url_m3u8 = url_m3u8
        self.jobs = jobs
        self.dl = downloader.downloader()
        self.dry_run = dry_run
        self.set_output_filename(output_filename)

    def set_output_filename(self, output_filename):
        if output_filename:
            self.output_filename = output_filename
            self.output_dir = os.path.join(os.path.curdir, os.path.splitext(self.output_filename)[0])

    def gen_output_dir(self):
        if self.output_filename:
            self.output_dir = os.path.join(os.path.curdir, os.path.splitext(self.output_filename)[0])

    def url_domain(self, play_url):
        proto, rest = urllib.splittype(play_url)
        host, rest = urllib.splithost(rest)

        return '%s://%s' %(proto, host)

    def get_playlist_form_play_url(self, play_url):
        domain = self.url_domain(play_url)
        html = self.dl.get_decoded_html(play_url)

        charset = None
        output_filename = None
        m3u8_url = None

        pattern = r'charset=([\w-]+)'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            charset = m.group(1)
        charset = 'gb18030'
        if charset:
            html = html.decode(charset)

        pattern = u'<title>(.*)在线播放.*</title>'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            output_filename = m.group(1)
        if output_filename:
            output_filename = '%s.mp4' %(output_filename)
            self.set_output_filename(output_filename)

        m3u8_script_url = None
        pattern = u'<script type="text/javascript"\s+src="([^"]+)"'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            m3u8_script = m.group(1)
            m3u8_script_url = urllib.basejoin(domain, m3u8_script)
        if m3u8_script_url:
            html = self.dl.get_decoded_html(m3u8_script_url)
            pattern = u'\[\'([^\$\[]+)\$(?P<m3u8_url>[^\$]+)\$([^\$]+)\'\]'
            r = re.compile(pattern)
            m = r.search(html)
            if m:
                tips = m.group(1).decode('unicode_escape')
                self.url_m3u8 = m.group(2)

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
            self.dl.download_urls([self.url_m3u8], title, ext, total_size, jobs = self.jobs, output_dir = self.output_dir, dry_run = False)

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
        total_size = self.dl.urls_size(url_files[:1])
        total_size = total_size * len(url_files)
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
    options.add_option('-p', '--play-url', dest='play_url', help='play_url', metavar='URL', default = None)
    options.add_option('-d', '--dry-run', action='store_true', dest='dry_run', help='dry_run', default=False)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if (not opts.play_url) and (not opts.output_filename or not opts.url_m3u8):
        options.print_help()
        return

    dl = ts_downloader(opts.url_m3u8, opts.jobs, opts.output_filename, opts.dry_run)
    if opts.play_url:
        dl.get_playlist_form_play_url(opts.play_url)

    url_files = dl.parse_m3u8()
    dl.download_video(url_files)

if '__main__' == __name__:
	main()
#python ts_downloader.py -u http://chyd-sn.wasu.tv/tbvideo/20141108/a5715565-44de-43ff-864d-2e8c5011e361.m3u8 -o '肖飞.mp4'

# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年08月04日 星期五 13时51分53秒
#   描    述：
#
#================================================================
import sys
import os
import downloader
import optparse
import log
import re
import sys  

reload(sys)  
sys.setdefaultencoding('utf-8')

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

    def get_playlist_form_play_url(self, play_url):
        r = downloader.urllib.parse.urlparse(play_url)
        domain = '%s://%s' %(r.scheme, r.netloc)

        html = self.dl.get_decoded_html(play_url)
        #print(html)

        charset = None
        output_filename = None

        pattern = r'charset=\"{0,1}([\w-]+)\"{0,1}'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            charset = m.group(1)
        if charset:
            try:
                html = html.decode(charset)
            except:
                html = html.decode('gb18030')

        pattern_list = [
                u'<title>(.*)在线(播放|观看).*</title>',
                u'<title>正在.*播放(.*)</title>',
        ]

        for pattern in pattern_list:
            r = re.compile(pattern)
            m = r.search(html)
            if m:
                output_filename = m.group(1)
                break;

        if output_filename:
            output_filename = '%s.mp4' %(output_filename)
            self.set_output_filename(output_filename)
        else:
            return
        
        pattern_list = [
                u'a:\'(.*index\.m3u8)\',',
                u'\$([^\$]+index\.m3u8)\$',
        ]
        for pattern in pattern_list:
            r = re.compile(pattern)
            m = r.search(html)
            if m:
                self.url_m3u8 = m.group(1)
                print(self.url_m3u8)
                return

        m3u8_script_url = None
        pattern = u'<script type="text/javascript"\s+src="([^"]+)"'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            m3u8_script = m.group(1)
            m3u8_script_url = downloader.urllib.parse.urljoin(domain, m3u8_script)
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

        filepath = os.path.join(self.output_dir, tail)

        if filepath.endswith('.m3u8'):
            content = self.dl.get_html(self.url_m3u8);

            lines = content.splitlines()
            for i in lines:
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

    if not dl.url_m3u8 or not dl.output_filename or not dl.output_dir:
        return

    url_files = dl.parse_m3u8()
    dl.download_video(url_files)

if '__main__' == __name__:
	main()
#python ts_downloader.py -u http://chyd-sn.wasu.tv/tbvideo/20141108/a5715565-44de-43ff-864d-2e8c5011e361.m3u8 -o '肖飞.mp4'

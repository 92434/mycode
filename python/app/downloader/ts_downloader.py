# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年10月21日 星期六 15时22分30秒
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

logging = log.dict_configure()
logger = logging.getLogger('default')

class ts_downloader(object):
    jobs = 1
    play_url = None
    dl = None
    dry_run = False

    download_urls = []
    output_filename = ''
    output_dir = ''

    def __init__(self, jobs, paly_url, dry_run):
        self.jobs = jobs
        self.dl = downloader.downloader()
        self.dry_run = dry_run
        self.play_url = play_url
        #self.output_dir = os.path.join(os.path.curdir, os.path.splitext(self.output_filename)[0])

    def get_info_play_url(self):
        r = downloader.n.urllib.parse.urlparse(play_url)
        domain = '%s://%s' %(r.scheme, r.netloc)

        html = self.dl.get_content(play_url)
        #logger.debug(html)

        output_filename = None

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
                logger.debug(self.url_m3u8)
                return

        m3u8_script_url = None
        pattern = u'<script type="text/javascript"\s+src="([^"]+)"'
        r = re.compile(pattern)
        m = r.search(html)
        if m:
            m3u8_script = m.group(1)
            m3u8_script_url = downloader.n.urllib.parse.urljoin(domain, m3u8_script)

        if m3u8_script_url:
            html = self.dl.get_content(m3u8_script_url)
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
            content = self.dl.get_content(self.url_m3u8);

            lines = content.splitlines()
            for i in lines:
                line = i.strip()
                if not line.startswith('#'):
                    url_files.append(line)

            url_files = map(lambda x : os.path.join(head, x), url_files)

        return url_files

    def download_video(self, url_files):
        logger.debug('get %s total_size...', os.path.join(self.output_dir, self.output_filename))
        pieces_size = self.dl.urls_size(url_files[:5])
        total_size = pieces_size * len(url_files) / (len(url_files[:5]))
        #total_size = self.dl.urls_size(url_files)
        logger.debug('total_size:%d' %(total_size))
        title, ext = os.path.splitext(self.output_filename)
        ext = ext[1:]
        self.dl.download_urls(url_files, title, ext, total_size, jobs = self.jobs, output_dir = self.output_dir, dry_run = self.dry_run)

def main():

    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default = 6)
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

    dl = ts_downloader(opts.url_m3u8, opts.jobs, opts.output_filename, opts.dry_run)
    if opts.play_url:
        dl.get_playlist_form_play_url(opts.play_url)

    if not dl.url_m3u8 or not dl.output_filename or not dl.output_dir:
        logger.debug('dl.url_m3u8:%s' %(dl.url_m3u8))
        logger.debug('dl.output_filename:%s' %(dl.output_filename))
        logger.debug('dl.output_dir:%s' %(dl.output_dir))
        return

    url_files = dl.parse_m3u8()
    dl.download_video(url_files)

if '__main__' == __name__:
	main()
#python ts_downloader.py -u http://chyd-sn.wasu.tv/tbvideo/20141108/a5715565-44de-43ff-864d-2e8c5011e361.m3u8 -o '肖飞.mp4'

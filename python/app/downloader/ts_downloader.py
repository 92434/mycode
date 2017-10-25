# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：ts_downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 22时35分24秒
#   修改日期：2017年10月25日 星期三 22时59分03秒
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
import lxml.etree

reload(sys)  
sys.setdefaultencoding('utf-8')

logging = log.dict_configure()
logger = logging.getLogger('default')

class ts_downloader(object):
    jobs = 1
    threads = 4
    play_url = None
    dl = None
    dry_run = False

    domain = None
    urls = []
    output_filename = ''
    url_m3u8 = None

    def __init__(self, jobs, threads, play_url, dry_run):
        self.jobs = jobs
        self.threads = threads
        self.dl = downloader.downloader()
        self.dry_run = dry_run
        self.play_url = play_url
        r = downloader.n.urllib.parse.urlparse(self.play_url)
        self.domain = '%s://%s' %(r.scheme, r.netloc)

    def get_part_urls_from_m3u8(self, url_m3u8):
        url_files = []

        basename = os.path.basename(url_m3u8)
        p = downloader.n.urllib.parse.urlparse(url_m3u8)
        m3u8_domain = '%s://%s' %(p.scheme, p.netloc)
        m3u8_dir = os.path.dirname(p.path)
        head = downloader.n.urllib.parse.urljoin(m3u8_domain, m3u8_dir)

        if url_m3u8.endswith('.m3u8'):
            content = self.dl.get_content(url_m3u8);

            lines = content.splitlines()
            for i in lines:
                line = i.strip()
                if not line.startswith('#'):
                    url_files.append(line)

            url_files = map(lambda x : downloader.n.urllib.parse.urljoin(head, x), url_files)
            #logger.debug('url_files:%s' %(url_files))

        return url_files

    def get_691gao_list_info(self, html):
        ret = False
        e_play_url = html.xpath('//div[@class="l"]/a[@class="txt" and @href]')
        #logger.debug('e_play_url:%s' %([(i.items(), i.text) for i in e_play_url]))
        if not len(e_play_url):
            return ret
        self.play_url = downloader.n.urllib.parse.urljoin(self.domain, e_play_url[0].get('href'))
        logger.debug('self.play_url:%s' %(self.play_url))
        data = self.dl.get_content(self.play_url)
        #logger.debug('data:%s' %(data))
        html = lxml.etree.HTML(data)
        #e_player = html.xpath('//*[@id="ckplayer_a1"]')
        e_title = html.xpath('//title')
        if not len(e_title):
            logger.debug('data:%s' %(data))
            return ret
        #logger.debug('e_title:%s' %([(i.items(), i.text) for i in e_title]))
        title = e_title[0].text
        p =  u'\u6b63\u5728\u64ad\u653e (\d+)-(.*)'
        index = downloader.n.r(p, title, 1)
        filetitle = downloader.n.r(p, title, 2)
        if not index or not filetitle:
            logger.debug('data:%s' %(data))
            return ret

        self.output_filename = os.path.join('%s' %(filetitle), '%s-%s.mp4' %(filetitle, index))
        #logger.debug('self.output_filename:%s' %(self.output_filename))

        e_player = html.xpath('//*[@type="text/javascript"]')
        if not len(e_player):
            return ret
        #logger.debug('e_player:%s' %([(i.items(), i.text) for i in e_player]))
        ck_player_value = e_player[3].text
        p = 'video=\["(.*)->video/mp4"\]'
        url = downloader.n.r(p, ck_player_value, 1)
        if not url:
            return ret
        #logger.debug('url:%s' %(url))
        if url:
            self.urls.append(url)
            ret = True

        return ret

    def get_691gao_diao_info(self, html):
        ret = False

        e_title = html.xpath('//title')
        if not len(e_title):
            return ret

        #logger.debug('e_title:%s' %([(i.items(), i.text) for i in e_title]))
        title = e_title[0].text
        p =  u'(.*)\u5728\u7ebf\u89c2\u770b.*'
        filetitle = downloader.n.r(p, title, 1)
        if not filetitle:
            return ret

        self.output_filename = os.path.join('%s' %(filetitle), '%s.mp4' %(filetitle))
        #logger.debug('self.output_filename:%s' %(self.output_filename))

        e_player = html.xpath('//*[@type="text/javascript"]')
        if not len(e_player):
            return ret
        #logger.debug('e_player:%s' %([(i.items(), i.text) for i in e_player]))
        ck_player_value = e_player[5].text
        p = 'video=\["(.*)"\]'
        url_m3u8 = downloader.n.r(p, ck_player_value, 1)
        if not url_m3u8:
            return ret
        #logger.debug('url_m3u8:%s' %(url_m3u8))
        self.urls = self.get_part_urls_from_m3u8(url_m3u8)
        if len(self.urls):
            ret = True

        return ret

    def get_info_form_play_url(self):
        ret = False

        data = self.dl.get_content(self.play_url)
        html = lxml.etree.HTML(data)
        ret = self.get_691gao_list_info(html)
        ret = self.get_691gao_diao_info(html)
        if not ret:
            logger.debug(data)

        return ret

    def download_video(self):
        logger.debug('get %s total_size...' %(self.output_filename))

        pieces_size = self.dl.urls_size(self.urls[:5])
        total_size = pieces_size * len(self.urls) / (len(self.urls[:5]))
        #total_size = self.dl.urls_size(self.urls)
        logger.debug('total_size:%d' %(total_size))

        self.dl.download_urls(self.urls, self.output_filename, total_size, jobs = self.jobs, threads = self.threads, dry_run = self.dry_run)

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default = 1)
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

    ret = dl.get_info_form_play_url()
    if not ret:
        return

    dl.download_video()

if '__main__' == __name__:
	main()

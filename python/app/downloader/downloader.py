# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 13时26分00秒
#   修改日期：2017年10月22日 星期日 19时30分09秒
#   描    述：
#
#================================================================
import network
import socket
import re
import os
import subprocess

import log
import progress_bar

n = network.network()
n.default_init(0)

logging = log.dict_configure()
logger = logging.getLogger('default')

timeout = 3 # in seconds
#socket.setdefaulttimeout(timeout)

try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading


class downloader(object):
    block_size = 2 * 1024 * 1024

    def __init__(self):
        pass

    def urlopen_with_retry(self, *args, **kwargs):
        for i in range(10):
            try:
                return n.urllib.request.urlopen(*args, **kwargs)
            except Exception as e:
                #logger.debug('request attempt %s(%s)' %(str(i + 1), e))
                pass

    def get_content(self, url, headers = None):
        """Gets the content of a URL via sending a HTTP GET request.

        Args:
            url: A URL.
            headers: Request headers used by the client.
            decoded: Whether decode the response body using UTF-8 or the charset specified in Content-Type.

        Returns:
            The content as a string.
        """

        #logger.debug('get_content: %s' % url)
        if not headers:
            headers = n.fake_header

        req = n.urllib.request.Request(url, headers = headers)

        response = self.urlopen_with_retry(req)
        data = response.read()
        data = n.decompresses_response_data(response, data)
        data = n.decode_response_data(response, data)

        return data

    def post_content(self, url, headers = None, post_data = {}):
        """Post the content of a URL via sending a HTTP POST request.

        Args:
            url: A URL.
            headers: Request headers used by the client.
            decoded: Whether decode the response body using UTF-8 or the charset specified in Content-Type.

        Returns:
            The content as a string.
        """

        #logger.debug('post_content: %s \n post_data: %s' % (url, post_data))
        if not headers:
            headers = n.fake_header

        req = n.urllib.request.Request(url, headers = headers)
        post_data = n.urllib.parse.urlencode(post_data)
        post_data_enc = bytes(post_data.encode('utf-8'))
        response = self.urlopen_with_retry(req, data = post_data_enc)
        data = response.read()
        data = n.decompresses_response_data(response, data)
        data = n.decode_response_data(response, data)

        return data

    def get_response_content_length(self, response):
        content_length = response.headers.get('content-length')
        if content_length:
            content_length = int(content_length)
        else:
            content_range = response.headers.get('content-range')
            if content_range:
                range_start = int(content_range[6:].split('/')[0].split('-')[0])
                range_end = int(content_range[6:].split('/')[1])
                content_length = range_end - range_start
            else:
                content_length = float('inf')

        return content_length

    def url_size(self, url, headers = None):
        if not headers:
            headers = n.fake_header

        req = n.urllib.request.Request(url, headers = headers)

        size = float('inf')
        while size == float('inf'):
            response = self.urlopen_with_retry(req)
            size = self.get_response_content_length(response)
        return size

    def urls_size(self, urls, headers = None):
        return sum([self.url_size(url, headers = headers) for url in urls])

    def get_head(self, url, headers = None, get_method = 'HEAD'):
        #logger.debug('get_head: %s' % url)

        if not headers:
            headers = n.fake_header

        req = n.urllib.request.Request(url, headers = headers)
        req.get_method = lambda: get_method
        res = self.urlopen_with_retry(req)
        return dict(res.headers)

    def url_info(self, url, headers = None):
        #logger.debug('url_info: %s' % url)
        if not headers:
            headers = n.fake_header

        req = n.urllib.request.Request(url, headers = headers)
        response = self.urlopen_with_retry(req)

        headers = response.headers

        content_type = headers.get('content-type')
        if content_type == 'image/jpg; charset=UTF-8' or content_type == 'image/jpg':
            content_type = 'audio/mpeg'    #fix for netease

        mapping = {
            'video/3gpp': '3gp',
            'video/f4v': 'flv',
            'video/mp4': 'mp4',
            'video/MP2T': 'ts',
            'video/quicktime': 'mov',
            'video/webm': 'webm',
            'video/x-flv': 'flv',
            'video/x-ms-asf': 'asf',
            'audio/mp4': 'mp4',
            'audio/mpeg': 'mp3',
            'audio/wav': 'wav',
            'audio/x-wav': 'wav',
            'audio/wave': 'wav',
            'image/jpeg': 'jpg',
            'image/png': 'png',
            'image/gif': 'gif',
            'application/pdf': 'pdf',
        }

        if content_type in mapping:
            ext = mapping.get(content_type)
        else:
            content_type = None
            content_disposition = headers.get('content-disposition')
            if content_disposition:
                try:
                    filename = n.urllib.parse.unquote(n.r(r'filename="?([^"]+)"?', content_disposition), 1)
                    if len(filename.split('.')) > 1:
                        ext = filename.split('.')[-1]
                    else:
                        ext = None
                except:
                    ext = None
            else:
                ext = None

        if headers.get('transfer-encoding') != 'chunked':
            size = headers.get('content-length')
            if size:
                size = int(size)
        else:
            size = None

        return content_type, ext, size


    def url_save_part(self, index, url, filepart, file_size, bar, thread_sem = None, force = False, refer = None, headers = None, timeout = None, **kwargs):
        part_size = self.block_size
        base_size = index * self.block_size

        if part_size > file_size - base_size:
            part_size = file_size - base_size

        #When a referer specified with param refer, the key must be 'Referer' for the hack here
        if not headers:
            headers = n.fake_header

        if refer:
            headers.update({'Referer' : refer})

        received = 0
        open_mode  = 'wb'

        if os.path.exists(filepart):
            received = os.path.getsize(filepart)

            if force:
                received = 0
                logger.debug('Overwrite %s: filepart already exists' %(filepart))
            else:
                if received == part_size:
                    logger.debug('Skipping %s: file already exists' %(filepart))
                    if bar:
                        bar.update_received(part_size)
                    if thread_sem:
                        thread_sem.release()
                    return
                elif received < part_size:
                    open_mode = 'ab'
                    if bar:
                        bar.update_received(received)
                else:
                    received = 0
        else:
            try:
                os.mkdir(os.path.dirname(filepart))
            except Exception as e:
                logger.debug('%s' %(e))
                pass

        headers.update({'Range' : 'bytes=' + str(base_size + received) + '-'})
        req = n.urllib.request.Request(url, headers = headers)
        response = self.urlopen_with_retry(req)

        while received != part_size:
            #logger.debug('filepart:%s, open_mode:%s' %(filepart, open_mode))
            output = open(filepart, open_mode)

            data = None

            while received < part_size:
                read_size = part_size - received
                if read_size > 1024 * 256:
                    read_size = 1024 * 256
                try:
                    data = response.read(read_size)
                except Exception as e:
                    #logger.debug('%s' %(e))
                    pass

                if not data:
                    headers.update({'Range' : 'bytes=' + str(base_size + received) + '-'})
                    req = n.urllib.request.Request(url, headers = headers)
                    response = self.urlopen_with_retry(req)
                    continue

                output.write(data)
                received += len(data)
                if bar:
                    bar.update_received(len(data))

            #logger.debug('filepart:%s, received:%d, part_size:%d' %(filepart, received, part_size))

            output.close()

            if os.path.getsize(filepart) == part_size:
                break
            else:
                if bar:
                    bar.received -= received

                logger.debug('download %s failed(received:%d, part_size:%d)! again!' %(filepart, received, part_size))

                os.remove(filepart)

                received = 0
                open_mode = 'wb'
                headers.update({'Range' : 'bytes=' + str(base_size + received) + '-'})
                req = n.urllib.request.Request(url, headers = headers)
                response = self.urlopen_with_retry(req)

        if thread_sem:
            thread_sem.release()

    def url_save(self, url, filepath, bar, threads, jobs_sem = None, force = False, refer = None, multi_urls = False, headers = None, timeout = None, **kwargs):
#When a referer specified with param refer, the key must be 'Referer' for the hack here
        if not headers:
            headers = n.fake_header

        if refer:
            headers.update({'Referer' : refer})

        if os.path.exists(filepath):
            logger.debug('Skipping %s: file already exists' % os.path.basename(filepath))
            file_size = os.path.getsize(filepath)
            if multi_urls:
                if bar:
                    bar.update_received(file_size)
            else:
                if bar:
                    bar.done()

            if jobs_sem:
                jobs_sem.release()
            return
        else:
            try:
                os.mkdir(os.path.dirname(filepath))
            except Exception as e:
                logger.debug('%s' %(e))
                pass

        file_size = self.url_size(url, headers = headers)
        loops = (file_size + (self.block_size - 1)) / self.block_size

        fileparts = []
        thread_sem = _threading.Semaphore(threads)
        threads_set = set()

        for i in range(0, loops, 1):
            thread_sem.acquire()
            filepart = '%s[%d].download' %(filepath, i)
            fileparts.append(filepart)
            local_kwargs = dict(index = i, url = url, filepart = filepart, file_size = file_size, bar = bar, thread_sem = thread_sem, refer = refer, headers = headers)
            local_kwargs.update(kwargs)

            t = _threading.Thread(target = self.url_save_part, kwargs = local_kwargs)
            # Ensure that Ctrl-C will not freeze the process.
            t.daemon = True
            threads_set.add(t)
            t.start()

        for t in threads_set:
            t.join()

        with open(filepath, 'wb') as wfile:
            for i in fileparts:
                with open(i, 'rb') as rfile:
                    wfile.write(rfile.read())
            for i in fileparts:
                os.remove(i)

        if jobs_sem:
            jobs_sem.release()

    def download_urls(self, urls, output_filepath, total_size = 0, jobs = 1, threads = 4, force = False, dry_run = False, refer = None, merge = True, headers = None, **kwargs):
        assert urls
        if dry_run:
            logger.debug('Real URLs:\n%s' % '\n'.join(urls))
            return

        if not force and os.path.exists(output_filepath):
            logger.debug('Skipping %s: file already exists' % output_filepath)
            return
            

        if not total_size:
            try:
                total_size = self.urls_size(urls, headers = headers)
            except:
                import traceback
                traceback.print_exc(file=sys.stdout)
                pass

        logger.debug('Downloading %s ...' %(output_filepath))

        lock = _threading.Lock()
        bar = progress_bar.SimpleProgressBar(total_size, len(urls))

        if len(urls) == 1:
            url = urls[0]
            self.url_save(url = url, filepath = output_filepath, bar = bar, threads = threads, refer = refer, multi_urls = True, headers = headers, **kwargs)
            bar.done()
        else:
            filepieces = []

            jobs_sem = _threading.Semaphore(jobs)
            threads_set = set()

            for i, url in enumerate(urls):
                unixpath, ext = os.path.splitext(output_filepath)
                filepath = '%s[%02d].%s' % (unixpath, i, ext)
                filepieces.append(filepath)
                #logger.debug('Downloading %s [%s/%s]...' % (filepath, i + 1, len(urls)))
                piece = i + 1;
                bar.update_piece(piece)

                jobs_sem.acquire()
                local_kwargs = dict(url = url, filepath = filepath, bar = bar, threads = threads, jobs_sem = jobs_sem, refer = refer, multi_urls = True, headers = headers)
                local_kwargs.update(kwargs)

                t = _threading.Thread(target = self.url_save, kwargs = local_kwargs)
                # Ensure that Ctrl-C will not freeze the process.
                t.daemon = True
                threads_set.add(t)
                t.start()

            for t in threads_set:
                t.join()

            bar.done()

            return filepieces

    def ts2mp4(self, output_filepath, ts_files):
        for i in ts_files:
            if not i.endwith('.ts'):
                return
            if not os.access(i, os.W_OK):
                return

        ts_files_str = '|'.join(ts_files)

        cmd = ['ffmpeg']
        cmd.append('-i')
        cmd.append('concat:%s' %(ts_files_str))
        cmd.append('-acodec')
        cmd.append('copy')
        cmd.append('-vcodec')
        cmd.append('copy')
        cmd.append('-absf')
        cmd.append('aac_adtstoasc')
        cmd.append('%s' %(output_filepath))
        #logger.debug('%s' %(cmd))

        if subprocess.Popen(cmd, cwd=os.path.curdir).wait() != 0:
            raise Exception('merge %s failed!!!', output_filepath)
        else:
            for i in ts_files:
                os.remove(i)

def main():
    dl = downloader()
    #logger.debug(location)
    #html = dl.get_content('http://www.113gan.com/view/index7644.html')
    html = dl.get_content('http://www.113gan.com/playdata/220/7644.js?70568.32')
    logger.debug(html)
    #data = dl.get_content('http://www.113gan.com/view/index7644.html')
    #logger.debug(data)
    data = dl.post_content('http://fanyi.baidu.com/v2transapi', post_data = {'from':'en', 'to':'zh', 'query':'logging', 'transtype':'realtime', 'simple_means_flag':'3'})
    #logger.debug(data)
    size = dl.urls_size(['http://sw.bos.baidu.com/sw-search-sp/software/9a2808964b476/QQ_8.9.3.21169_setup.exe'])
    #logger.debug(size)
    head = dl.get_head('http://www.baidu.com')
    #logger.debug(head)
    info = dl.url_info('http://www.baidu.com')
    #logger.debug(info)

if '__main__' == __name__:
    main()

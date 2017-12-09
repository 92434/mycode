# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 13时26分00秒
#   修改日期：2017年12月09日 星期六 16时36分23秒
#   描    述：
#
#================================================================
import socket
import re
import os
import subprocess

import log
import progress_bar

import request

logging = log.dict_configure()
logger = logging.getLogger('default')

timeout = 30 # in seconds
socket.setdefaulttimeout(timeout)

r = request.request()

try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading


class downloader(object):
    block_size = 1 * 1024 * 1024
    max_read_size = 1024 * 256

    def __init__(self):
        pass

    def url_save_part_thread_wapper(self, *args, **kwargs):
        try:
            self.url_save_part(*args, **kwargs)
        except Exception as e:
            logger.debug('%s' %(e))
        finally:
            thread_sem = kwargs.get('thread_sem')
            if thread_sem:
                thread_sem.release()

    def url_save_part(self, index, url, filepart, file_size, bar, thread_sem = None, force = False, refer = None, headers = None, timeout = None, **kwargs):
        part_size = self.block_size
        base_size = index * self.block_size

        if part_size > file_size - base_size:
            part_size = file_size - base_size

        #When a referer specified with param refer, the key must be 'Referer' for the hack here
        if not headers:
            headers = {}
            headers.update(r.request.fake_headers)

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
                #logger.debug('%s' %(e))
                pass



        while received != part_size:
            #logger.debug('filepart:%s, open_mode:%s' %(filepart, open_mode))
            output = open(filepart, open_mode)

            data = None

            headers.update({'Range' : 'bytes=' + str(base_size + received) + '-'})

            for data in r.request.iter_content(url, chunk_size = self.max_read_size, headers = headers):
                if received + len(data) > part_size:
                    data = data[: (part_size - received)]
                output.write(data)
                received += len(data)
                if bar:
                    bar.update_received(len(data))

                if received == part_size:
                    break

            output.close()

            #logger.debug('filepart:%s, received:%d, part_size:%d' %(filepart, received, part_size))

            if received < part_size:
                open_mode = 'ab'
                continue

            if os.path.getsize(filepart) == part_size:
                break
            else:
                if bar:
                    bar.received -= received

                logger.debug('download %s failed(received:%d, part_size:%d)! again!' %(filepart, received, part_size))

                os.remove(filepart)

                received = 0
                open_mode = 'wb'

    def url_save_thread_wapper(self, *args, **kwargs):
        try:
            self.url_save(*args, **kwargs)
        except Exception as e:
            logger.debug('%s' %(e))
        finally:
            jobs_sem = kwargs.get('jobs_sem')
            if jobs_sem:
                jobs_sem.release()

    def url_save(self, url, filepath, bar, threads, jobs_sem = None, force = False, refer = None, multi_urls = False, headers = None, timeout = None, **kwargs):
#When a referer specified with param refer, the key must be 'Referer' for the hack here
        if not headers:
            headers = {}
            headers.update(r.request.fake_headers)

        if refer:
            headers.update({'Referer' : refer})

        if os.path.exists(filepath):
            logger.debug('Skipping %s: file already exists' % os.path.basename(filepath))
            file_size = os.path.getsize(filepath)
            if multi_urls:
                if bar:
                    bar.update_received(file_size)

            return
        else:
            try:
                os.makedirs(os.path.dirname(filepath))
            except Exception as e:
                #logger.debug('%s' %(e))
                pass

        file_size = r.request.url_size(url, headers = headers)
        loops = (file_size + (self.block_size - 1)) / self.block_size

        fileparts = []
        thread_sem = _threading.Semaphore(threads)
        threads_set = set()

        for i in range(0, loops, 1):
            import time
            thread_sem.acquire()
            time.sleep(1)
            filepart = '%s.part%d' %(filepath, i)
            fileparts.append(filepart)
            local_kwargs = dict(index = i, url = url, filepart = filepart, file_size = file_size, bar = bar, thread_sem = thread_sem, refer = refer, headers = headers)
            local_kwargs.update(kwargs)

            t = _threading.Thread(target = self.url_save_part_thread_wapper, kwargs = local_kwargs)
            # Ensure that Ctrl-C will not freeze the process.
            t.setDaemon(True)
            threads_set.add(t)
            t.start()

        for t in threads_set:
            t.join()

        for i in fileparts:
            if not os.access(i, os.F_OK):
                return

        wfile = open(filepath, 'wb')
        for i in fileparts:
            rfile = open(i, 'rb')
            wfile.write(rfile.read())
            rfile.close()
        wfile.close()

        for i in fileparts:
            os.remove(i)

    def download_urls(self, urls, output_filepath, total_size = 0, jobs = 1, threads = 4, force = False, dry_run = False, refer = None, merge = True, headers = None, **kwargs):
        assert urls
        if dry_run:
            logger.debug('Real URLs:\n%s' % '\n'.join(urls))
            return

        if not force and os.path.exists(output_filepath):
            logger.debug('Skipping %s: file already exists' % output_filepath)
            return
            

        if not total_size:
            total_size = r.request.urls_size(urls, headers = headers)

        logger.debug('Downloading %s ...' %(output_filepath))

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
                jobs_sem.acquire()
                unixpath, _ = os.path.splitext(output_filepath)
                p = r.request.urlparse.urlparse(url)
                _, ext = os.path.splitext(p.path)
                filepath = '%s[%02d]%s' % (unixpath, i, ext)
                filepieces.append(filepath)
                #logger.debug('Downloading %s [%s/%s]...' % (filepath, i + 1, len(urls)))
                piece = i + 1;
                bar.update_piece(piece)

                local_kwargs = dict(url = url, filepath = filepath, bar = bar, threads = threads, jobs_sem = jobs_sem, refer = refer, multi_urls = True, headers = headers)
                local_kwargs.update(kwargs)

                t = _threading.Thread(target = self.url_save_thread_wapper, kwargs = local_kwargs)
                # Ensure that Ctrl-C will not freeze the process.
                t.setDaemon(True)
                threads_set.add(t)
                t.start()

            for t in threads_set:
                t.join()

            bar.done()

            if merge:
                import merge_ts
                ret = merge_ts.merge_ts(output_filepath, filepieces)

def main():
    import optparse
    import sys
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-t', '--threads', type='int', dest='threads', help='threads', default = 3)
    options.add_option('-u', '--url', dest='url', help='url', default=None)
    options.add_option('-o', '--output-path', dest='output_path', help='output_path', default=None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.url:
        return

    urls = [opts.url, opts.url]

    dl = downloader()
    p = r.request.urlparse.urlparse(opts.url)
    output_path = None
    if not opts.output_path:
        output_path = os.path.join(os.curdir, p.path[1:])
    else:
        output_path = opts.output_path
    dl.download_urls(urls, output_path, threads = opts.threads, merge = False)

if '__main__' == __name__:
    main()

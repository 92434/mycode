# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：downloader.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 13时26分00秒
#   修改日期：2017年08月02日 星期三 10时32分24秒
#   描    述：
#
#================================================================
import network
import log
import re
import socket
import os
import sys
import time
import subprocess

urllib = network.network().default_init(0)
logging = log.log().get_logger('debug')

try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading

class SimpleProgressBar:
    def __init__(self, total_size, total_pieces = 1):
        self.displayed = False
        self.total_size = total_size
        self.total_pieces = total_pieces
        self.current_piece = 1
        self.received = 0
        self.speed = ''
        self.last_updated = time.time()

        total_pieces_len = len(str(total_pieces))
        # 38 is the size of all statically known size in self.bar
        total_str = '%5s' % round(self.total_size / 1048576, 1)
        total_str_width = max(len(total_str), 5)
        self.bar_size = self.get_terminal_size()[1] - 27 - 2*total_pieces_len - 2*total_str_width
        #self.bar = '{:>4}%% ({:>%s}/%sMB) ├{:─<%s}┤[{:>%s}/{:>%s}] {}' % (total_str_width, total_str, self.bar_size, total_pieces_len, total_pieces_len)
        self.bar = '{:>4}%% ({:>%s}/%sMB) ├{:-<%s}┤[{:>%s}/{:>%s}] {}' % (total_str_width, total_str, self.bar_size, total_pieces_len, total_pieces_len)

    def get_terminal_size(self):
        """Get (width, height) of the current terminal."""
        try:
            import fcntl, termios, struct # fcntl module only available on Unix
            return struct.unpack('hh', fcntl.ioctl(1, termios.TIOCGWINSZ, '1234'))
        except:
            return (40, 80)

    def update(self):
        self.displayed = True
        bar_size = self.bar_size
        percent = round(self.received * 100 / self.total_size, 1)
        if percent >= 100:
            percent = 100
        dots = bar_size * int(percent) // 100
        plus = int(percent) - dots // bar_size * 100
        if plus > 0.8:
            #plus = '█'
            plus = '|'
        elif plus > 0.4:
            plus = '>'
        else:
            plus = ''
        #bar = '█' * dots + plus
        bar = '|' * dots + plus
        #print('self.bar:\'%s\'' %(self.bar))
        #print('percent:\'%f\'' %(percent))
        #print('round(self.received / 1048576, 1):\'%f\'' %(round(self.received / 1048576, 1)))
        #print('bar:\'%s\'' %(bar))
        #print('self.current_piece:\'%d\'' %(self.current_piece))
        #print('self.total_pieces:\'%d\'' %(self.total_pieces))
        #print('self.speed:\'%s\'' %(self.speed))
        bar = self.bar.format(percent, round(self.received / 1048576, 1), bar, self.current_piece, self.total_pieces, self.speed)
        sys.stdout.write('\r' + bar)
        sys.stdout.flush()

    def update_received(self, n):
        self.received += n
        time_diff = time.time() - self.last_updated
        bytes_ps = n / time_diff if time_diff else 0
        if bytes_ps >= 1024 ** 3:
            self.speed = '{:4.0f} GB/s'.format(bytes_ps / 1024 ** 3)
        elif bytes_ps >= 1024 ** 2:
            self.speed = '{:4.0f} MB/s'.format(bytes_ps / 1024 ** 2)
        elif bytes_ps >= 1024:
            self.speed = '{:4.0f} kB/s'.format(bytes_ps / 1024)
        else:
            self.speed = '{:4.0f}  B/s'.format(bytes_ps)
        self.last_updated = time.time()
        self.update()

    def update_piece(self, n):
        self.current_piece = n

    def done(self):
        if self.displayed:
            print()
            self.displayed = False

class PiecesProgressBar:
    def __init__(self, total_size, total_pieces = 1):
        self.displayed = False
        self.total_size = total_size
        self.total_pieces = total_pieces
        self.current_piece = 1
        self.received = 0

    def update(self):
        self.displayed = True
        bar = '{0:>5}%[{1:<40}] {2}/{3}'.format('', '=' * 40, self.current_piece, self.total_pieces)
        sys.stdout.write('\r' + bar)
        sys.stdout.flush()

    def update_received(self, n):
        self.received += n
        self.update()

    def update_piece(self, n):
        self.current_piece = n

    def done(self):
        if self.displayed:
            print()
            self.displayed = False

class DummyProgressBar:
    def __init__(self, *args):
        pass
    def update_received(self, n):
        pass
    def update_piece(self, n):
        pass
    def done(self):
        pass


class downloader(object):
    fake_headers = {
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
        'Accept-Charset': 'UTF-8,*;q=0.5',
        'Accept-Encoding': 'gzip,deflate,sdch',
        'Accept-Language': 'en-US,en;q=0.8',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0'
    }

    def __init__(self):
        pass

    def r1(self, pattern, text):
        m = re.search(pattern, text)
        if m:
            return m.group(1)
    def tr(self, s):
        return s

    def ungzip(self, data):
        """Decompresses data for Content-Encoding: gzip.
        """
        from io import BytesIO
        import gzip
        buffer = BytesIO(data)
        f = gzip.GzipFile(fileobj = buffer)
        return f.read()

    def undeflate(self, data):
        """Decompresses data for Content-Encoding: deflate.
        (the zlib compression is used.)
        """
        import zlib
        decompressobj = zlib.decompressobj(-zlib.MAX_WBITS)
        return decompressobj.decompress(data)+decompressobj.flush()

    # DEPRECATED in favor of get_content()
    def get_response(self, url, faker = False):
        #logging.debug('get_response: %s' % url)

        if faker:
            response = urllib.request.urlopen(urllib.request.Request(url, headers = self.fake_headers), None)
        else:
            response = urllib.request.urlopen(url)

        data = response.read()
        if response.info().get('Content-Encoding') == 'gzip':
            data = self.ungzip(data)
        elif response.info().get('Content-Encoding') == 'deflate':
            data = self.undeflate(data)
        response.data = data
        return response

    # DEPRECATED in favor of get_content()
    def get_html(self, url, encoding = None, faker = False):
        response = self.get_response(url, faker)
        content = response.data
        return str(content, 'utf-8', 'ignore')


    # DEPRECATED in favor of get_content()
    def get_decoded_html(self, url, faker = False):
        response = self.get_response(url, faker)
        content = response.data
        charset = self.r1(r'charset=([\w-]+)', response.headers['content-type'])
        if charset:
            return content.decode(charset, 'ignore')
        else:
            return content

    def get_location(self, url):
        #logging.debug('get_location: %s' % url)

        response = urllib.request.urlopen(url)
        # urllib will follow redirections and it's too much code to tell urllib
        # not to do that
        return response.geturl()

    def urlopen_with_retry(self, *args, **kwargs):
        for i in range(10):
            try:
                return urllib.request.urlopen(*args, **kwargs)
            except socket.timeout:
                logging.debug('request attempt %s timeout' % str(i + 1))

    def get_content(self, url, headers = {}, decoded = True):
        """Gets the content of a URL via sending a HTTP GET request.

        Args:
            url: A URL.
            headers: Request headers used by the client.
            decoded: Whether decode the response body using UTF-8 or the charset specified in Content-Type.

        Returns:
            The content as a string.
        """

        #logging.debug('get_content: %s' % url)

        req = urllib.request.Request(url, headers = headers)

        response = self.urlopen_with_retry(req)
        data = response.read()

        # Handle HTTP compression for gzip and deflate (zlib)
        try:
            content_encoding = response.headers['Content-Encoding']
        except:
            content_encoding = None
        if content_encoding == 'gzip':
            data = self.ungzip(data)
        elif content_encoding == 'deflate':
            data = self.undeflate(data)

        # Decode the response body
        if decoded:
            charset = self.r1(r'charset=([\w-]+)', response.headers['Content-Type'])
            if charset is not None:
                data = data.decode(charset)
            else:
                data = data.decode('utf-8', 'ignore')

        return data

    def post_content(self, url, headers = {}, post_data = {}, decoded = True):
        """Post the content of a URL via sending a HTTP POST request.

        Args:
            url: A URL.
            headers: Request headers used by the client.
            decoded: Whether decode the response body using UTF-8 or the charset specified in Content-Type.

        Returns:
            The content as a string.
        """

        #logging.debug('post_content: %s \n post_data: %s' % (url, post_data))

        req = urllib.request.Request(url, headers = headers)
        post_data_enc = bytes(urllib.parse.urlencode(post_data).encode('utf-8'))
        response = self.urlopen_with_retry(req, data = post_data_enc)
        data = response.read()

        # Handle HTTP compression for gzip and deflate (zlib)
        try:
            content_encoding = response.headers('Content-Encoding')
        except:
            content_encoding = None

        if content_encoding == 'gzip':
            data = self.ungzip(data)
        elif content_encoding == 'deflate':
            data = self.undeflate(data)

        # Decode the response body
        if decoded:
            try:
                charset = self.r1(r'charset=([\w-]+)', response.headers('Content-Type'))
            except:
                charset = None
            if charset is not None:
                data = data.decode(charset)
            else:
                data = data.decode('utf-8')

        return data

    def url_size(self, url, faker = False, headers = {}):
        if faker:
            response = self.urlopen_with_retry(urllib.request.Request(url, headers = fake_headers))
        elif headers:
            response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers))
        else:
            response = self.urlopen_with_retry(url)

        size = None
        if 'content-length' in response.headers:
            size = response.headers['content-length']
        return int(size) if size != None else float('inf')

    def urls_size(self, urls, faker = False, headers = {}):
        return sum([self.url_size(url, faker = faker, headers = headers) for url in urls])

    def get_head(self, url, headers = {}, get_method = 'HEAD'):
        #logging.debug('get_head: %s' % url)

        if headers:
            req = urllib.request.Request(url, headers = headers)
        else:
            req = urllib.request.Request(url)
        req.get_method = lambda: get_method
        res = self.urlopen_with_retry(req)
        return dict(res.headers)

    def url_info(self, url, faker = False, headers = {}):
        #logging.debug('url_info: %s' % url)

        if faker:
            response = self.urlopen_with_retry(urllib.request.Request(url, headers = fake_headers))
        elif headers:
            response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers))
        else:
            response = self.urlopen_with_retry(urllib.request.Request(url))

        headers = response.headers

        type = headers['content-type']
        if type == 'image/jpg; charset=UTF-8' or type == 'image/jpg' : type = 'audio/mpeg'    #fix for netease

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

        if type in mapping:
            ext = mapping[type]
        else:
            type = None
            if 'content-disposition' in dir(headers):
                try:
                    filename = urllib.parse.unquote(r1(r'filename="?([^"]+)"?', headers['content-disposition']))
                    if len(filename.split('.')) > 1:
                        ext = filename.split('.')[-1]
                    else:
                        ext = None
                except:
                    ext = None
            else:
                ext = None

        if headers['transfer-encoding'] != 'chunked':
            size = headers['content-length'] and int(headers['content-length'])
        else:
            size = None

        return type, ext, size

    def url_locations(self, urls, faker = False, headers = {}):
        locations = []
        for url in urls:
            #logging.debug('url_locations: %s' % url)

            if faker:
                response = self.urlopen_with_retry(urllib.request.Request(url, headers = fake_headers))
            elif headers:
                response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers))
            else:
                response = self.urlopen_with_retry(urllib.request.Request(url))

            locations.append(response.url)
        return locations

    def url_save(self, url, filepath, bar, piece = None, sem = None, lock = None, force = False, refer = None, is_part = False, faker = False, headers = {}, timeout = None, **kwargs):
#When a referer specified with param refer, the key must be 'Referer' for the hack here
        if refer is not None:
            headers['Referer'] = refer
        file_size = self.url_size(url, faker = faker, headers = headers)

        if os.path.exists(filepath):
            if not force and file_size == os.path.getsize(filepath):
                if not is_part:
                    if bar:
                        if lock:
                            lock.acquire()
                        bar.done()
                        if lock:
                            lock.release()
                    print('Skipping %s: file already exists' % self.tr(os.path.basename(filepath)))
                else:
                    if bar:
                        if lock:
                            lock.acquire()
                        bar.update_received(file_size)
                        if lock:
                            lock.release()
                if sem:
                    sem.release()
                return
            else:
                if not is_part:
                    if bar:
                        if lock:
                            lock.acquire()
                        bar.done()
                        if lock:
                            lock.release()
                    print('Overwriting %s' % self.tr(os.path.basename(filepath)), '...')
        elif not os.path.exists(os.path.dirname(filepath)):
            os.mkdir(os.path.dirname(filepath))

        temp_filepath = filepath + '.download' if file_size != float('inf') else filepath
        received = 0
        if not force:
            open_mode = 'ab'

            if os.path.exists(temp_filepath):
                received += os.path.getsize(temp_filepath)

                if bar:
                    if lock:
                        lock.acquire()
                    bar.update_received(os.path.getsize(temp_filepath))
                    if lock:
                        lock.release()
        else:
            open_mode = 'wb'

        if received < file_size:
            if faker:
                headers = fake_headers
            elif headers:
                headers = headers
            else:
                headers = {}
            if received:
                headers['Range'] = 'bytes=' + str(received) + '-'
            if refer:
                headers['Referer'] = refer

            if timeout:
                response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers), timeout = timeout)
            else:
                response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers))
            try:
                range_start = int(response.headers['content-range'][6:].split('/')[0].split('-')[0])
                end_length = int(response.headers['content-range'][6:].split('/')[1])
                range_length = end_length - range_start
            except:
                content_length = response.headers['content-length']
                range_length = int(content_length) if content_length!=None else float('inf')

            if file_size != received + range_length:
                received = 0
                open_mode = 'wb'

                if bar:
                    if lock:
                        lock.acquire()
                    bar.received = 0
                    if lock:
                        lock.release()

            with open(temp_filepath, open_mode) as output:
                while True:
                    buffer = response.read(1024 * 256)
                    if not buffer:
                        if received == file_size: # Download finished
                            break
                        else: # Unexpected termination. Retry request
                            headers['Range'] = 'bytes=' + str(received) + '-'
                            response = self.urlopen_with_retry(urllib.request.Request(url, headers = headers))
                    output.write(buffer)
                    received += len(buffer)
                    if bar:
                        if lock:
                            lock.acquire()
                        bar.update_received(len(buffer))
                        if lock:
                            lock.release()

        assert received == os.path.getsize(temp_filepath), '%s == %s == %s' % (received, os.path.getsize(temp_filepath), temp_filepath)

        if os.access(filepath, os.W_OK):
            os.remove(filepath) # on Windows rename could fail if destination filepath exists
        os.rename(temp_filepath, filepath)
        if sem:
            sem.release()


    def ts2mp4(self, output_filepath, ts_files):
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
        #logging.debug('%s' %(cmd))

        if subprocess.Popen(cmd, cwd=os.path.curdir).wait() != 0:
            raise Exception('%s')

    def download_urls(self, urls, title, ext, total_size = 0, jobs = 1, force = False, output_dir = '.', dry_run = False, refer = None, merge = True, faker = False, headers = {}, **kwargs):
        assert urls
        if dry_run:
            print('Real URLs:\n%s' % '\n'.join(urls))
            return

        if not total_size:
            try:
                total_size = self.urls_size(urls, faker=faker, headers=headers)
            except:
                import traceback
                traceback.print_exc(file=sys.stdout)
                pass

        output_filename = '%s.%s' %(title, ext)
        output_filepath = os.path.join(output_dir, output_filename)

        lock = _threading.Lock()
        if total_size:
            if not force and os.path.exists(output_filepath) and os.path.getsize(output_filepath) >= total_size * 0.9:
                print('Skipping %s: file already exists' % output_filepath)
                return
            bar = SimpleProgressBar(total_size, len(urls))
        else:
            bar = PiecesProgressBar(total_size, len(urls))

        if len(urls) == 1:
            url = urls[0]
            print('Downloading %s ...' % self.tr(output_filename))
            bar.update()
            self.url_save(url, output_filepath, bar, refer = refer, faker = faker, headers = headers, **kwargs)
            bar.done()
        else:
            parts = []
            print('Downloading %s.%s ...' % (self.tr(title), ext))
            bar.update()

            sem = _threading.Semaphore(jobs)
            threads = set()

            for i, url in enumerate(urls):
                filename = '%s[%02d].%s' % (title, i, ext)
                filepath = os.path.join(output_dir, filename)
                parts.append(filepath)
                #print 'Downloading %s [%s/%s]...' % (self.tr(filename), i + 1, len(urls))
                bar.update_piece(i + 1)
                piece = i + 1;

                sem.acquire()
                local_kwargs = dict(url = url, filepath = filepath, bar = bar, piece = piece, sem = sem, lock = lock, refer = refer, is_part = True, faker = faker, headers = headers)
                local_kwargs.update(kwargs)

                t = _threading.Thread(target = self.url_save, kwargs = local_kwargs)
                # Ensure that Ctrl-C will not freeze the process.
                t.daemon = True
                threads.add(t)
                t.start()

            for t in threads:
                t.join()

            bar.done()

            if not merge:
                print()
                return

            if ext == "mp4":
                try:
                    self.ts2mp4(output_filepath, parts)
                    print('Merged into %s' % output_filename)
                except:
                    raise
                else:
                    for part in parts:
                        os.remove(part)

            else:
                print("Can't merge %s files" % ext)

def main():
    dl = downloader()
    location = dl.get_location('http://www.baidu.com')
    #print(location)
    #html = dl.get_decoded_html('http://www.113gan.com/view/index7644.html')
    html = dl.get_decoded_html('http://www.113gan.com/playdata/220/7644.js?70568.32')
    print(html)
    #data = dl.get_content('http://www.113gan.com/view/index7644.html')
    #print(data)
    data = dl.post_content('http://fanyi.baidu.com/v2transapi', post_data = {'from':'en', 'to':'zh', 'query':'logging', 'transtype':'realtime', 'simple_means_flag':'3'})
    #print(data)
    size = dl.urls_size(['http://sw.bos.baidu.com/sw-search-sp/software/9a2808964b476/QQ_8.9.3.21169_setup.exe'])
    #print(size)
    head = dl.get_head('http://www.baidu.com')
    #print(head)
    info = dl.url_info('http://www.baidu.com')
    #print(info)

if '__main__' == __name__:
    main()

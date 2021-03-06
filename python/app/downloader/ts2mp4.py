#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import optparse
import subprocess
import socket
from time import time

try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading

if sys.version_info[0] == 3:
    import urllib.request
    import urllib.error
else:
    import imp
    import urllib2
    urllib = imp.new_module('urllib')
    urllib.request = urllib2
    urllib.error = urllib2

_NOT_TTY = not os.isatty(2)

def _print(*objects, **kwargs):
    sep = kwargs.get('sep', ' ')
    end = kwargs.get('end', '\n')
    out = kwargs.get('file', sys.stdout)
    out.write(sep.join(objects) + end)

class _Options(object):
    help = False


class Progress(object):
    def __init__(self, title, total=0, units=''):
        self._title = title
        self._total = total
        self._done = 0
        self._lastp = -1
        self._start = time()
        self._show = False
        self._units = units
        self._index = 0
        self._task_info = {}
        self._task_info_old = {}
        self.t = None

    def show_task_info(self):
        self.t = _threading.Timer(1, self.show_task_info)
        self.t.start()

        if not self._task_info:
            self.t.cancel()
            self.t = None
            return

        items = self._task_info.items()
        length = len(items)
        index = self._index % length
        self._index += 1

        cur_url, cur_info = items[index]
        old_info = self._task_info_old.get(cur_url, None)
        d = {cur_url : cur_info}
        self._task_info_old.update(d)

        if not old_info:
            return

        cur_pos, cur_total = cur_info
        old_pos, old_total = old_info
        duration = length

        speed = ((cur_pos - old_pos) / 1024.0) / duration
        percent = cur_pos * 100.0 / cur_total

        info = '\rurl:%s %20d/%-20d %5dK/S %3.2f%%    ' %(cur_url, cur_pos, cur_total, speed, percent)
        self.update(inc = 0, info = info)

    def add_task_info(self, url, pos, total):
        cur_info = (pos, total)
        d = {url : cur_info}
        self._task_info.update(d)

        if _NOT_TTY:
            return

        if not self.t:
            self.t = _threading.Timer(1, self.show_task_info)
            self.t.start()

    def remove_task_info(self, url):
        self._task_info.pop(url, None)
        self._task_info_old.pop(url, None)

    def update(self, inc=1, info = ''):
        self._done += inc

        if _NOT_TTY:
            return

        if not self._show:
            if 0.5 <= time() - self._start:
                self._show = True
            else:
                return

        if self._total <= 0:
            sys.stderr.write('\r%s: %d, ' % (
                self._title,
                self._done))
            sys.stderr.flush()
        else:
            p = (100 * self._done) / self._total

            if self._lastp != p:
                self._lastp = p
                sys.stderr.write('\r%s: %3d%% (%d%s/%d%s)   ' % (
                    self._title,
                    p,
                    self._done, self._units,
                    self._total, self._units))
                sys.stderr.flush()
        if info:
            sys.stderr.write('\r%s' %(info))
            sys.stderr.flush()

    def end(self):
        if _NOT_TTY or not self._show:
            return

        self.t.cancel()

        if self._total <= 0:
            sys.stderr.write('\r%s: %d, done.\n' % (
                self._title,
                self._done))
            sys.stderr.flush()
        else:
            p = (100 * self._done) / self._total
            sys.stderr.write('\r%s: %3d%% (%d%s/%d%s), done.\n' % (
                self._title,
                p,
                self._done, self._units,
                self._total, self._units))
            sys.stderr.flush()

class ts2mp4(object):
    def __init__(self, force_broken, url_m3u8, jobs, out_file):
        self.force_broken = force_broken
        self.url_m3u8 = url_m3u8
        self.jobs = jobs
        self.out_file = out_file
        self.error_task = {}

    def _InitHttp(self):
        timeout = 30 # in seconds
        socket.setdefaulttimeout(timeout)
        
        handlers = []
        mgr = urllib.request.HTTPPasswordMgrWithDefaultRealm()
        try:
            import netrc
            n = netrc.netrc()
            for host in n.hosts:
                p = n.hosts[host]
                mgr.add_password(p[1], 'http://%s/' % host, p[0], p[2])
                mgr.add_password(p[1], 'https://%s/' % host, p[0], p[2])
        except:  # pylint: disable=bare-except
            pass
        handlers.append(urllib.request.HTTPBasicAuthHandler(mgr))
        handlers.append(urllib.request.HTTPDigestAuthHandler(mgr))

        if 'http_proxy' in os.environ:
            url = os.environ['http_proxy']
            handlers.append(urllib.request.ProxyHandler({'http': url, 'https': url}))
        if 'REPO_CURL_VERBOSE' in os.environ:
            handlers.append(urllib.request.HTTPHandler(debuglevel=1))
            handlers.append(urllib.request.HTTPSHandler(debuglevel=1))
            urllib.request.install_opener(urllib.request.build_opener(*handlers))

    def download_file_by_url(self, url, dest_file, pm):
        BLOCK_SIZE = 8192
        current_size = 0

        if os.path.exists(dest_file):
            current_size = os.path.getsize(dest_file)
            dest = open(dest_file, 'r+b')

            dest.seek(0, os.SEEK_END)
        else:
            dest = open(dest_file, 'w+b')

        try:
            try:
                request = urllib.request.Request(url, None)
                request.add_header('Range', 'bytes=%d-' %(current_size))
                r = urllib.request.urlopen(request)
                FILE_SIZE = int(r.headers.getheader("Content-Length"))

                _print('Get %s' %(url), file=sys.stderr)

            except urllib.error.HTTPError as e:
                if e.code in [401, 403, 404, 501]:
                    return False
                _print('fatal: Cannot get %s' % url, file=sys.stderr)
                _print('fatal: HTTP error %s' % e.code, file=sys.stderr)
                return False
            except urllib.error.URLError as e:
                _print('fatal: Cannot get %s' % url, file=sys.stderr)
                _print('fatal: error %s' % e.reason, file=sys.stderr)
                return False
            except Exception as e:
                _print('fatal: Cannot get %s' % url, file=sys.stderr)
                _print('fatal: error %s' % e, file=sys.stderr)
                return False

            try:
                while True:
                    buf = r.read(BLOCK_SIZE)
                    if buf == '' and current_size == FILE_SIZE:
                        return True

                    dest.write(buf)
                    current_size += len(buf)
                    pm.add_task_info(url, current_size, FILE_SIZE)

                    if current_size > FILE_SIZE:
                        return False
            except Exception as e:
                _print('fatal: error %s' % e, file=sys.stderr)
                _print('download %s Error!!!' % url, file=sys.stderr)
                return False
            finally:
                pm.remove_task_info(url)
                r.close()
        finally:
            dest.close()

    def download_file(self, url_file, lock, fetched, pm, sem, err_event):
        head, tail = os.path.split(url_file)
        dest_file = os.path.join(os.path.curdir, tail)
        dest_file_suffix = '.part'
        dest_file_part = dest_file + dest_file_suffix
        count = self.error_task.get(url_file, 0)
        if os.path.exists(dest_file) or self.download_file_by_url(url_file, dest_file_part, pm):
            if os.path.exists(dest_file_part):
                os.rename(dest_file_part, dest_file_part.rstrip(dest_file_suffix))
            lock.acquire()
            fetched.add(url_file)
            pm.update()
            lock.release()
        else:
            count += 1
            if count >= 5:
                os.remove(dest_file_part)
                count = 0
            err_event.set()

        if count == 0:
            self.error_task.pop(url_file, 0)
        else:
            self.error_task.update({url_file : count})

        sem.release()

    def download_files(self, url_files):
        lock = _threading.Lock()
        err_event = _threading.Event()
        sem = _threading.Semaphore(self.jobs)
        fetched = set()
        threads = set()
        pm = Progress('download files', len(url_files))

        for i in url_files:
            if err_event.isSet() and not self.force_broken:
                break

            sem.acquire()
            kwargs = dict(url_file=i, lock=lock, fetched=fetched, pm=pm, sem=sem, err_event=err_event)
            t = _threading.Thread(target = self.download_file, kwargs = kwargs)
            # Ensure that Ctrl-C will not freeze the process.
            t.daemon = True
            threads.add(t)
            t.start()

        for t in threads:
            t.join()

        # If we saw an error, exit with code 1 so that other scripts can check.
        if err_event.isSet():
            _print('error: Exited download_files due to errors', file=sys.stderr)
        pm.end()

        return fetched
    
    def parse_m3u8(self):
        url_files = []
        fetched = set()

        head, tail = os.path.split(self.url_m3u8)
        filename = os.path.join(os.path.curdir, tail)

        if filename.endswith('.m3u8'):
            while len(fetched) != len([self.url_m3u8]):
                fetched = self.download_files([self.url_m3u8])

        if os.path.exists(filename):
            with open(filename, 'r') as f:
                content = f.read().splitlines()
                for i in content:
                    line = i.strip()
                    if not line.startswith('#'):
                        url_files.append(line)

            url_files = map(lambda x : os.path.join(head, x), url_files)
            os.remove(filename)

        return url_files

    def ts2mp4(self, url_files):
        ts_files = map(lambda x : os.path.join(os.path.curdir, os.path.split(x)[1]), url_files)
        sskip = len('/index')
        eskip = len('.ts')
        ts_files = sorted(ts_files, key = lambda x:int(x[x.rfind('/') + sskip : -eskip]))
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
        cmd.append('%s' %(self.out_file))
        _print('%s' %(cmd), file=sys.stderr)

        if subprocess.Popen(cmd, cwd=os.path.curdir).wait() != 0:
            _print('failed!', file=sys.stderr)
        else:
            for i in ts_files:
                os.remove(i)


def main(argv):
    _print('argv:%s' %(argv), file=sys.stderr)
    options = optparse.OptionParser()
    options.add_option('-b', '--force-broken', action='store_true', dest='force_broken', help='force_broken', default=False)
    options.add_option('-j', '--jobs', type='int', dest='jobs', help='jobs', default=6)
    options.add_option('-o', '--out-filename', dest='out_file', help='out_file', default='out.mp4')
    #options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default='http://201610.shipinmp4.com/x/20170419/ekdv-478/1/hls/index.m3u8')
    options.add_option('-u', '--m3u8-url', dest='url_m3u8', help='url_m3u8', metavar='URL', default='')
    opts, args = options.parse_args(argv)
    _print('opts:%s' %(opts), file=sys.stderr)
    _print('args:%s' %(args), file=sys.stderr)
    if not opts.url_m3u8:
        options.print_help()
        return
    ts2mp4_inst = ts2mp4(opts.force_broken, opts.url_m3u8, opts.jobs, opts.out_file)
    ts2mp4_inst._InitHttp()
    url_files = ts2mp4_inst.parse_m3u8()
    #_print('url_files:%s' %(url_files), file=sys.stderr)

    fetched = set()
    while len(fetched) != len(url_files):
        fetched = ts2mp4_inst.download_files(url_files)
    ts2mp4_inst.ts2mp4(url_files)
    
    

if __name__ == '__main__':
    main(sys.argv[1:])

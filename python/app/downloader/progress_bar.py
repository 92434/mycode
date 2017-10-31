# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月21日 肖飞 All rights reserved
#   
#   文件名称：progress_bar.py
#   创 建 者：肖飞
#   创建日期：2017年10月21日 星期六 09时32分54秒
#   修改日期：2017年10月31日 星期二 09时27分53秒
#   描    述：
#
#================================================================
import sys
import time

try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading

class SimpleProgressBar:
    def __init__(self, total_size, total_pieces = 1):
        self.lock = _threading.Lock()
        self.lock.acquire()

        self.displayed = False
        self.total_size = total_size
        self.total_pieces = total_pieces
        self.current_piece = 1
        self.received = 0
        self.last_received = 0
        self.speed = ''
        self.last_updated = time.time()

        total_pieces_len = len(str(total_pieces))
        # 38 is the size of all statically known size in self.bar
        total_str = '%5s' % round(self.total_size / 1048576, 1)
        total_str_width = max(len(total_str), 5)
        self.bar_size = self.get_terminal_size()[1] - 27 - 2 * total_pieces_len - 2 * total_str_width
        #self.bar = '{:>4}%% ({:>%s}/%sMB) ├{:─<%s}┤[{:>%s}/{:>%s}] {}' % (total_str_width, total_str, self.bar_size, total_pieces_len, total_pieces_len)
        self.bar = '{:>4}%% ({:>%s}/%sMB) ├{:-<%s}┤[{:>%s}/{:>%s}] {}' % (total_str_width, total_str, self.bar_size, total_pieces_len, total_pieces_len)
        self.lock.release()

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
            #plus = '░'
            plus = '|'
        elif plus > 0.4:
            plus = '>'
        else:
            plus = ''

        #bar = '█' * dots + plus
        bar = '|' * dots + plus

        bar = self.bar.format(percent, round(self.received / 1048576, 1), bar, self.current_piece, self.total_pieces, self.speed)
        sys.stdout.write('\r' + bar)
        sys.stdout.flush()

    def update_received(self, n):
        self.lock.acquire()

        self.received += n

        time_diff = time.time() - self.last_updated
        if time_diff > 1.0:
            bytes_diff = self.received - self.last_received
            bytes_ps = bytes_diff / time_diff
            if bytes_ps >= 1024 ** 3:
                self.speed = '{:4.0f} GB/s'.format(bytes_ps / 1024 ** 3)
            elif bytes_ps >= 1024 ** 2:
                self.speed = '{:4.0f} MB/s'.format(bytes_ps / 1024 ** 2)
            elif bytes_ps >= 1024:
                self.speed = '{:4.0f} kB/s'.format(bytes_ps / 1024)
            else:
                self.speed = '{:4.0f}  B/s'.format(bytes_ps)

            self.last_received = self.received
            self.last_updated = time.time()

            self.update()

        self.lock.release()

    def update_piece(self, n):
        self.lock.acquire()
        self.current_piece = n
        self.update()
        self.lock.release()

    def done(self):
        self.lock.acquire()
        if self.displayed:
            sys.stdout.write('\n')
            sys.stdout.flush()
            self.displayed = False
        self.lock.release()

class PiecesProgressBar:
    def __init__(self, total_size, total_pieces = 1):
        self.lock = _threading.Lock()

        self.lock.acquire()
        self.displayed = False
        self.total_size = total_size
        self.total_pieces = total_pieces
        self.current_piece = 1
        self.received = 0
        self.lock.release()

    def update(self):
        self.displayed = True
        bar = '{0:>5}%[{1:<40}] {2}/{3}'.format('', '=' * 40, self.current_piece, self.total_pieces)
        sys.stdout.write('\r' + bar)
        sys.stdout.flush()

    def update_received(self, n):
        self.lock.acquire()
        self.received += n
        self.update()
        self.lock.release()

    def update_piece(self, n):
        self.lock.acquire()
        self.current_piece = n
        self.update()
        self.lock.release()

    def done(self):
        self.lock.acquire()
        if self.displayed:
            sys.stdout.write('\n')
            sys.stdout.flush()
            self.displayed = False
        self.lock.release()

class DummyProgressBar:
    def __init__(self, *args):
        self.lock = _threading.Lock()
        pass
    def update_received(self, n):
        self.lock.acquire()
        self.lock.release()
        pass
    def update_piece(self, n):
        self.lock.acquire()
        self.lock.release()
        pass
    def done(self):
        self.lock.acquire()
        self.lock.release()
        pass

def main():
    import log
    logging = log.dict_configure()
    logger = logging.getLogger('default')

    bar = SimpleProgressBar(1000, 1000)
    for i in range(0, 1000, 1):
        bar.update_piece(i + 1)
        bar.update_received(1)
        time.sleep(1)
    bar.done()

if '__main__' == __name__:
    main()

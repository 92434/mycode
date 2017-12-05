# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月05日 肖飞 All rights reserved
#   
#   文件名称：console_io.py
#   创 建 者：肖飞
#   创建日期：2017年12月05日 星期二 12时00分25秒
#   修改日期：2017年12月05日 星期二 12时20分42秒
#   描    述：
#
#================================================================
import log
logging = log.dict_configure()
logger = logging.getLogger('default')

class abstract_con(object):
    def __init__(self):
        pass

    def getch():
        logger.debug('not valid')

class windows_con(abstract_con):
    def __init__(self):
        import msvcrt

    def getch(self):
        import msvcrt
        return msvcrt.getch()

class unix_con(abstract_con):
    def __init__(self):
        import tty, sys

    def getch(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno(), termios.TCSANOW)
            ch = sys.stdin.read(1)
            #sys.stdout.write(ch)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

class console_io(object):
    def __init__(self):
        try:
            self.con = windows_con()
        except:
            self.con = unix_con()

    def getch(self):
        return self.con.getch()


def main():
    import sys
    io = console_io()
    ch = 0
    while ch != 'q':
        ch = io.getch()
        sys.stdout.write(hex(ord(ch)))

if '__main__' == __name__:
    main()

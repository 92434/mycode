# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月19日 肖飞 All rights reserved
#   
#   文件名称：daemon.py
#   创 建 者：肖飞
#   创建日期：2017年12月19日 星期二 11时15分00秒
#   修改日期：2017年12月19日 星期二 14时25分58秒
#   描    述：
#
#================================================================
import os
import sys
import time

import log
logging = log.dict_configure(default_log_to_file = True)
logger = logging.getLogger('default')

def start_daemon(*args, **kwargs):
    pid = os.fork()  
    if pid > 0:
        sys.exit(0)

    #os.chdir('/')
    os.umask(0)
    os.setsid()

    pid = os.fork()
    if pid > 0:
        sys.exit(0)

    if not 'server' in kwargs.keys():
        sys.exit(0)

    server = kwargs.pop('server')
    server.run(args = args, kwargs = kwargs)


class server(object):
    def __init__(self):
        pass

    def run(self, *argv, **kwargs):
        logger.debug('%s' %(kwargs))

def main(argv):
    s = server()
    d = {}
    item = {'server' : s}
    d.update(item)
    start_daemon(*argv, **d)

if '__main__' == __name__:
    main(sys.argv[1:])

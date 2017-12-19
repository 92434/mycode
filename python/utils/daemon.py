# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月19日 肖飞 All rights reserved
#   
#   文件名称：daemon.py
#   创 建 者：肖飞
#   创建日期：2017年12月19日 星期二 11时15分00秒
#   修改日期：2017年12月19日 星期二 11时44分16秒
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
    logger.debug('')
    pid = os.fork()  
    if pid > 0:
        sys.exit(0)

    logger.debug('')

    #os.chdir('/')
    os.umask(0)
    os.setsid()

    logger.debug('')
    pid = os.fork()
    if pid > 0:
        sys.exit(0)

    logger.debug('%s' %(kwargs))
    if not 'proc' in kwargs.keys():
        sys.exit(0)

    proc = kwargs.pop('proc')
    proc(args = args, kwargs = kwargs)
    

def test_proc(*args, **kwargs):
    logger.debug('%s' %(kwargs))
    tick = 0
    while True:
        time.sleep(1)
        logger.debug('tick:%s' %(tick))
        tick += 1

def main():
    d = {}
    item = {'proc' : test_proc}
    d.update(item)
    start_daemon(*sys.argv, **d)

if '__main__' == __name__:
    main()

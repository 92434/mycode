# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月19日 肖飞 All rights reserved
#   
#   文件名称：unix_socket_client.py
#   创 建 者：肖飞
#   创建日期：2017年12月19日 星期二 12时14分30秒
#   修改日期：2017年12月19日 星期二 13时23分44秒
#   描    述：
#
#================================================================
import socket  
import sys  
import io

import log
logging = log.dict_configure()
logger = logging.getLogger('default')
  
def create_client(address):
    sock = None
    if not address:
        return sock
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.settimeout(10)
    try:
        sock.connect(address)
    except:
        sock = None
        return sock

    return sock

def run_client(sock, message):
    if not sock:
        return

    if not message:
        return

    f = io.BytesIO(message)
    size = 0

    try:
        while True:
            try:
                data = f.read(16)
                size += len(data)
                if data:
                    #logger.debug('data:%s' %(data))
                    sock.sendall(data)
                    data = sock.recv(16)
                    if data:
                        #logger.debug('recv:%s' %(data))
                        pass
                    else:
                        break
                else:
                    break
            except socket.timeout, e:
                pass
    finally:
        logger.debug('send size:%s' %(size))
        sock.close()

def main():
    s = 'hello, test message!' * 1024 * 1024
    run_client(create_client('.unix_socket'), s)

if '__main__' == __name__:
    main()

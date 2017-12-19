# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月19日 肖飞 All rights reserved
#   
#   文件名称：unix_socket_server.py
#   创 建 者：肖飞
#   创建日期：2017年12月19日 星期二 11时56分36秒
#   修改日期：2017年12月19日 星期二 14时52分36秒
#   描    述：
#
#================================================================
import socket  
import os  
import time
  
import log
logging = log.dict_configure()
logger = logging.getLogger('default')

def create_server(address):
    if not address:
        logger.debug('')
        return

    if os.access(address, os.F_OK):
        os.unlink(address)
        logger.debug('')
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.settimeout(10)
    sock.bind(address)
    sock.listen(1)
    return sock

def run_server(sock):
    if not sock:
        return
    while True:
        connection = None
        address = None
        try:
            connection, address = sock.accept()
            logger.debug('%s:%s' %(connection, address))
            size = 0

            while True:
                data = connection.recv(16)
                size += len(data)
                if data:
                    #logger.debug('recv:%s' %(data))
                    connection.sendall(data)
                else:
                    break
        except socket.timeout, e:
            #logger.exception('')
            pass
        except socket.error, e:
            #logger.exception('')
            pass
        finally:
            if connection:
                logger.debug('recv size:%s' %(size))
                connection.close()

def main():
    run_server(create_server('.unix_socket'))

if '__main__' == __name__:
    main()

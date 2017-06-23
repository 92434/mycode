#!/usr/bin/env python
# -*- coding: utf-8 -*-
#================================================================
#   Copyright (C) 2017年06月23日 肖飞 All rights reserved
#   
#   文件名称：tcp_server.py
#   创 建 者：肖飞
#   创建日期：2017年06月23日 星期五 16时03分20秒
#   修改日期：2017年06月23日 星期五 17时37分15秒
#   描  述：
#
#================================================================
import socket
import threading
import SocketServer
import time

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request.recv(1024)
        cur_thread = threading.current_thread()
        print(type(data))
        for i in range(0, len(data)):
            print(hex(ord(data[i])))

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

def main():
    HOST, PORT = "localhost", 5558

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address
    print("ip:%s port:%d" %(ip, port))
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()

    print("Server loop running in thread:", server_thread.name)

    while True:
        time.sleep(1);

if '__main__' == __name__:
    main()

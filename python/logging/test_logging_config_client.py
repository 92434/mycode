#!/usr/bin/env python
#$0 testconf
 
import socket
import sys
import struct
 
HOST = 'localhost'
PORT = 9999
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print "connecting..."
s.connect((HOST, PORT))
print "connected..."
data_to_send = open(sys.argv[1], "r").read()
print "sending length..."
s.send(struct.pack(">L", len(data_to_send)))
print "sending data..."
s.send(data_to_send)
print "closing..."
s.close()

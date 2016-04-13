# -*- coding: utf-8 -*-

import struct
def float_to_hex(f):
	f = float(f)
	return struct.pack("!f", f).encode('hex')

def hex_to_float(h):
	h = hex(int(h, 16))[2 : ]
	return struct.unpack('!f', h.decode('hex'))[0]

import ctypes
def c_float_to_hex(f):
	f = float(f)
	fp = ctypes.pointer(ctypes.c_float(f))
	ip = ctypes.cast(fp, ctypes.POINTER(ctypes.c_int))
	return ip.contents.value.__hex__()

def c_hex_to_float(h):
	i = int(h, 16)
	ip = ctypes.pointer(ctypes.c_int(i))
	fp = ctypes.cast(ip, ctypes.POINTER(ctypes.c_float))
	return fp.contents.value

if '__main__' == __name__:
	f = '1.2345'
	print "float_to_hex(%s):%s" %(f, hex(int(float_to_hex(f), 16)))
	print "c_float_to_hex(%s):%s" %(f, hex(int(c_float_to_hex(f), 16)))

	h = '0x3f9e0419'
	print "hex_to_float(%s):%s" %(h, hex_to_float(h))
	print "c_hex_to_float(%s):%s" %(h, c_hex_to_float(h))

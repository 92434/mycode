import xmlrpclib
import os
proxy = xmlrpclib.ServerProxy('http://localhost:8000/')
 
print proxy.file_reader_xf22001(os.path.abspath('test_SimpleXMLRPCServer.py'))
print proxy

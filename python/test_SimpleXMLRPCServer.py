from SimpleXMLRPCServer import SimpleXMLRPCServer
 
def file_reader_xf22001(file_name):
 
	with open(file_name, 'r') as f:
		return f.read()
 
server = SimpleXMLRPCServer(('0.0.0.0', 8000))
server.register_introspection_functions()
 
server.register_function(file_reader_xf22001)
 
server.serve_forever()

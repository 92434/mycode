import SocketServer
import logging
import logging.handlers

class MyHandler(SocketServer.StreamRequestHandler):
	def handle(request):
		addr = request.client_address

		print 'connection from:', addr

		data = request.rfile.read(1024)

		print 'data:', data

server = SocketServer.ThreadingTCPServer(('localhost', logging.handlers.DEFAULT_TCP_LOGGING_PORT), MyHandler)

server.serve_forever()

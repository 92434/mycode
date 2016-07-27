import SocketServer
import logging.handlers
import cPickle
import struct

from log import *

#rd, wr, ex = select.select([self.socket.fileno()],  [], [],  self.timeout)
#if rd:
#	self.handle_request()  

class MyHandler(SocketServer.StreamRequestHandler):
	def unPickle(self, data):
		return cPickle.loads(data)

	def handleLogRecord(self, record):
		# if a name is specified, we use the named logger rather than the one
		# implied by the record.
		logname = getattr(self.server, 'logname', None)
		if logname:
			name = logname
		else:
			name = record.name
		#print "name:%s" %(name)
		logger = get_logger(name)
		#print "logger:%s" %(logger)
		# N.B. EVERY record gets logged. This is because Logger.handle
		# is normally called AFTER logger-level filtering. If you want
		# to do filtering, do it at the client end to save wasting
		# cycles and network bandwidth!
		#logger.handle(record)
		for i in logger.handlers:
			if i.name == 'file':
				print i.format(record)
				i.handle(record)

	def handle(self):
		addr = self.client_address

		print 'connection from:', addr

		while True:
			data = self.connection.recv(4)
			if len(data) < 4:
				return

			data_len = struct.unpack(">L", data)[0]
			#print 'data_len:%d' %(data_len)
			data = self.connection.recv(data_len)
			#print 'data:%s' %(data)
			while len(data) < data_len:
				data = data + self.connection.recv(data_len - len(data))
			raw_data = self.unPickle(data)
			#print 'raw_data:%s' %(raw_data)
			record = logging.makeLogRecord(raw_data)
			#print 'record:%s' %(record)
			self.handleLogRecord(record)

def log_server(ip = 'localhost', port = logging.handlers.DEFAULT_TCP_LOGGING_PORT):
	server = SocketServer.ThreadingTCPServer((ip, logging.handlers.DEFAULT_TCP_LOGGING_PORT), MyHandler)
	server.serve_forever()


if __name__ == '__main__':
	log_server()

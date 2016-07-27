import SocketServer
from threading import Timer, Thread
import time
import sys
import os
import socket
import signal

HOST, PORT = "localhost", 9999

class MyUDPHandler(SocketServer.BaseRequestHandler):
	"""
	This class works similar to the TCP handler class, except that
	self.request consists of a pair of data and client socket, and since
	there is no connection the client address must be given explicitly
	when sending data back via sendto().
	"""

	def handle(self):
		data = self.request[0].strip()
		socket = self.request[1]
		print("{} wrote:".format(self.client_address[0]))
		print(data)
		socket.sendto(data.upper(), self.client_address)

def timer_func():
	data = " ".join(sys.argv[1:])

# SOCK_DGRAM is the socket type to use for UDP sockets
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# As you can see, there is no connect() call; UDP has no connections.
# Instead, data is directly sent to the recipient via sendto().
	sock.sendto(bytes(data + "\n"), (HOST, PORT))
	received = str(sock.recv(1024))

	print("Sent:     {}".format(data))
	print("Received: {}".format(received))
	Timer(1, timer_func).start()
def cc_cb(signo, frame):
	print(dir(frame))
	exit(0)
	


if __name__ == "__main__":
	server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
	Timer(1, timer_func).start()
	t = Thread(target = server.serve_forever)
	t.setDaemon(True)
	signal.signal(signal.SIGINT, cc_cb)
	t.start()
	
	while True:
		time.sleep(1)

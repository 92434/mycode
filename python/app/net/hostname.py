# -*- coding: utf-8 -*-

import socket

def main():
	address_family = socket.AF_INET
	socket_type = socket.SOCK_STREAM
	request_queue_size = 5
	allow_reuse_address = False
	server_address = ('localhost', 8000)

	s = socket.socket(address_family, socket_type)
	s.bind(server_address)
	server_address = s.getsockname()
	print server_address

	fileno = s.fileno()
	
	print fileno

	print socket.gethostname()
	print socket.getfqdn(server_address[0])
	

if __name__ == '__main__':
	main()

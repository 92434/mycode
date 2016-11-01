# -*- coding: utf-8 -*-
import logging
import logging.config
import time
import os

file_config_content = """
[loggers]
keys=root,simpleExample
 
[handlers]
keys=consoleHandler
 
[formatters]
keys=simpleFormatter
 
[logger_root]
level=DEBUG
handlers=consoleHandler
 
[logger_simpleExample]
level=DEBUG
handlers=consoleHandler
qualname=simpleExample
propagate=0
 
[handler_consoleHandler]
class=StreamHandler
level=DEBUG
formatter=simpleFormatter
args=(sys.stdout,)
 
[formatter_simpleFormatter]
format=%(levelname)s %(asctime)s %(module)s %(process)d %(thread)d %(message)s
datefmt=
"""
file_config_filepath = '/tmp/file_config.conf'

class my_filter(logging.Filter):
	def __init__(self, param=None):
		self.param = param

	def filter(self, record):
		if not self.param:
			return True

		if self.param in record.msg:
			record.msg.replace(self.param, '!' * 20)
			return True
		return True

def gen_debug_file():
	tm = time.localtime()
	cwd = os.getcwd()
	dirname = os.path.join(os.getcwd(), '.log', )
	filename = '%04d-%02d-%02d_%02d%02d%02d_%s' %(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 'debug.log')
	debug_file = os.path.join(dirname, filename)
	if not os.path.exists(dirname):
		os.makedirs(dirname)
	return debug_file
#debug_file = gen_debug_file()
debug_file = ''

dict_config = {
	'version': 1,
	'disable_existing_loggers': False,
	'formatters': {
		'verbose': {
			'format': '%(levelname)s %(asctime)s %(module)s %(process)d %(thread)d %(message)s'
		},
		'simple': {
			'format': '%(levelname)s %(message)s'
		},
		'myformatter': {
			'format': '[%(asctime)s|%(pathname)s|%(funcName)s|%(lineno)d] %(message)s'
		},
	},
	'filters': {
		'special': {
			'()': my_filter,
			'param': 'noshow',
		}
	},
	'handlers': {
		'null': {
			'level': 'DEBUG',
			'class': 'logging.NullHandler',
		},
		#'file': {
		#	'level': 'DEBUG',
		#	'class': 'logging.FileHandler',
		#	#'filename': os.path.join(os.path.dirname(os.path.abspath(__file__)), 'debug.log'),
		#	'filename': debug_file,
		#	'formatter': 'myformatter',
		#	'filters': ['special']
		#},
		'tcp': {
			'level': 'DEBUG',
			'class': 'logging.handlers.SocketHandler',
			'host': 'localhost',
			'port': logging.handlers.DEFAULT_TCP_LOGGING_PORT,
			'formatter': 'myformatter',
			'filters': ['special']
		},
		'udp': {
			'level': 'DEBUG',
			'class': 'logging.handlers.DatagramHandler',
			'host': 'localhost',
			'port': logging.handlers.DEFAULT_UDP_LOGGING_PORT,
			'formatter': 'myformatter',
			'filters': ['special']
		},
		'console': {
			'level': 'DEBUG',
			'class': 'logging.StreamHandler',
			'formatter': 'myformatter',
			'filters': ['special'],
		},
	},

	'loggers': {
		'my_logger': {
			#'handlers': ['file', 'console', 'tcp', 'udp'],
			'handlers': ['console', 'tcp', 'udp'],
			'level': 'DEBUG',
			'propagate': True,
		},
	},
}

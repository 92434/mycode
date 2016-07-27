# -*- coding: utf-8 -*-
import os
import logging
import logging.config

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

my_config = {
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
		'file': {
			'level': 'DEBUG',
			'class': 'logging.FileHandler',
			'filename': os.path.join(os.path.dirname(os.path.abspath(__file__)), 'debug.log'),
			'formatter': 'myformatter',
			'filters': ['special']
		},
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
			'level': 'ERROR',
			'class': 'logging.StreamHandler',
			'formatter': 'simple',
			'filters': ['special'],
		},
	},

	'loggers': {
		'my_logger': {
			'handlers': ['file', 'console', 'tcp', 'udp'],
			'level': 'DEBUG',
			'propagate': True,
		},
	},
}

logging.config.dictConfig(my_config)

#create logger
logger = logging.getLogger("my_logger")

class base(object):
	def __init__(self, *args, **kwargs):
		txt = "\nself.__class__:%s\nargs:%s\nkwargs:%s\n" %(
			self.__class__,
			args,
			kwargs
		)
		logger.debug(txt)


class derived(base):
	def __init__(self, *args, **kwargs):
		txt = str(super(derived, self))
		logger.debug(txt)
		super(derived, self).__init__(*args, **kwargs)
		txt = "\nself.__class__:%s\nargs:%s\nkwargs:%s\n" %(
			self.__class__,
			args,
			kwargs
		)
		logger.debug(txt)

if __name__ == "__main__":
	d = derived(1, 2, {3 : 4})

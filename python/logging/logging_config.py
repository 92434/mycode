import logging
import logging.config
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

config_filepath = 'logging.conf'

def test_logging_file_config():
	with open(config_filepath, 'w+') as f:
		f.write(file_config_content)
	
	logging.config.fileConfig("logging.conf")

	os.remove(config_filepath)

	#create logger
	logger = logging.getLogger("simpleExample")
	 
	#"application" code
	logger.debug("debug message")
	logger.info("info message")
	logger.warn("warn message")
	logger.error("error message")
	logger.critical("critical message")

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

def test_logging_dict_config():
	logging.config.dictConfig(my_config)

	#create logger
	logger = logging.getLogger("my_logger")
	 
	#"application" code
	logger.debug("debug message")
	logger.info("info message")
	logger.warn("warn message")
	logger.error("error message")
	logger.critical("critical message")

def test_logging_simple_config():
	# create logger with "spam_application"
	logger = logging.getLogger("spam_application")
	logger.setLevel(logging.DEBUG)
	# create file handler which logs even debug messages
	fh = logging.FileHandler(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'debug.log'))
	fh.setLevel(logging.DEBUG)
	# create console handler with a higher log level
	ch = logging.StreamHandler()
	ch.setLevel(logging.ERROR)
	# create formatter and add it to the handlers
	fh_formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
	fh.setFormatter(fh_formatter)
	ch_formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
	ch.setFormatter(ch_formatter)
	# add the handlers to the logger
	logger.addHandler(ch)
	logger.addHandler(fh)

	#"application" code
	logger.debug("debug message")
	logger.info("info message")
	logger.warn("warn message")
	logger.error("error message")
	logger.critical("critical message")


if __name__ == '__main__':
	print '*' * 100
	#test_logging_file_config()
	print '*' * 100
	test_logging_dict_config()
	print '*' * 100
	#test_logging_simple_config()

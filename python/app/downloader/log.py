# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月31日 肖飞 All rights reserved
#   
#   文件名称：log.py
#   创 建 者：肖飞
#   创建日期：2017年07月31日 星期一 14时02分27秒
#   修改日期：2017年07月31日 星期一 15时10分04秒
#   描    述：
#
#================================================================
import logging
import logging.handlers
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


class log(object):
    dict_config = {
        'version': 1,
        'disable_existing_loggers': False,
        'formatters': {
            'verbose': {
                'format': '%(levelname)s %(asctime)s %(module)s %(process)d %(thread)d %(message)s'
            },
            'simple': {
                'format': '%(message)s'
            },
            'xiaofei-debug': {
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
            #	'formatter': 'xiaofei-debug',
            #	'filters': ['special']
            #},
            'tcp': {
                'level': 'DEBUG',
                'class': 'logging.handlers.SocketHandler',
                'host': 'localhost',
                'port': logging.handlers.DEFAULT_TCP_LOGGING_PORT,
                'formatter': 'xiaofei-debug',
                'filters': ['special']
            },
            'udp': {
                'level': 'DEBUG',
                'class': 'logging.handlers.DatagramHandler',
                'host': 'localhost',
                'port': logging.handlers.DEFAULT_UDP_LOGGING_PORT,
                'formatter': 'xiaofei-debug',
                'filters': ['special']
            },
            'debug': {
                'level': 'DEBUG',
                'class': 'logging.StreamHandler',
                'formatter': 'xiaofei-debug',
                'filters': ['special'],
            },
            'simple': {
                'level': 'DEBUG',
                'class': 'logging.StreamHandler',
                'formatter': 'simple',
                'filters': ['special'],
            },
        },

        'loggers': {
            'debug': {
                'handlers': ['debug', 'tcp', 'udp'],
                'level': 'DEBUG',
                'propagate': True,
            },
            'simple': {
                'handlers': ['simple', 'tcp', 'udp'],
                'level': 'DEBUG',
                'propagate': True,
            },
        },
    }

    def __init__(self):
        logging.config.dictConfig(self.dict_config)

    def get_logger(self, logger_name = 'debug'):
        logger = logging.getLogger(logger_name)
        return logger

def main():
    logger = log().get_logger('debug')
    logger.debug('xiaofei')
    logger.warn('xiaofei')
    logger = log().get_logger('simple')
    logger.debug('xiaofei')
    logger.warn('xiaofei')

if '__main__' == __name__:
    main()

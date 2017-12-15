# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月20日 肖飞 All rights reserved
#   
#   文件名称：configure.py
#   创 建 者：肖飞
#   创建日期：2017年10月20日 星期五 10时05分24秒
#   修改日期：2017年12月15日 星期五 15时08分16秒
#   描    述：
#
#================================================================
import logging
import logging.config
import time
import os

def simple_logger():
    # create logger with "spam_application"
    logger = logging.getLogger("spam_application")
    logger.setLevel(logging.DEBUG)
    # create file handler which logs even debug messages
    fh = logging.FileHandler(debug_file)
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

    return logger

default_file_config_content = """
[loggers]
keys=root,default
 
[handlers]
keys=consoleHandler
 
[formatters]
keys=simpleFormatter
 
[logger_root]
level=DEBUG
handlers=consoleHandler
 
[logger_default]
level=DEBUG
handlers=consoleHandler
qualname=default
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
def file_configure(config_file = None):
    if not config_file:
        config_file = '/tmp/default_config_file.conf'
        with open(config_file, 'w+') as f:
            f.write(default_file_config_content)
    logging.config.fileConfig(filename)
    return logging

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

default_config_dict = {
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
        #   'level': 'DEBUG',
        #   'class': 'logging.FileHandler',
        #   'filename': None,
        #   'formatter': 'myformatter',
        #   'filters': ['special']
        #},
        #'tcp': {
        #    'level': 'DEBUG',
        #    'class': 'logging.handlers.SocketHandler',
        #    'host': 'localhost',
        #    'port': logging.handlers.DEFAULT_TCP_LOGGING_PORT,
        #    'formatter': 'myformatter',
        #    'filters': ['special']
        #},
        #'udp': {
        #    'level': 'DEBUG',
        #    'class': 'logging.handlers.DatagramHandler',
        #    'host': 'localhost',
        #    'port': logging.handlers.DEFAULT_UDP_LOGGING_PORT,
        #    'formatter': 'myformatter',
        #    'filters': ['special']
        #},
        'console': {
            'level': 'DEBUG',
            'class': 'logging.StreamHandler',
            'formatter': 'myformatter',
            'filters': ['special'],
        },
    },

    'loggers': {
        'default': {
            #'handlers': ['file', 'console', 'tcp', 'udp'],
            'handlers': ['console', 'tcp', 'udp'],
            'level': 'DEBUG',
            'propagate': True,
        },
    },
}

def gen_debug_file(filename = None):
    tm = time.localtime()
    cwd = os.getcwd()
    dirname = os.path.join(os.getcwd(), '.log', )
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    if not filename:
        filename = '%04d-%02d-%02d_%02d%02d%02d_%s' %(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 'debug.log')
    debug_file = os.path.join(dirname, filename)
    return debug_file

def dict_configure(config_dict = None, default_log_to_file = False):
    if not config_dict:
        config_dict = default_config_dict

    if default_log_to_file:
        try:
            file_handler = {
                'file': {
                    'level': 'DEBUG',
                    'class': 'logging.FileHandler',
                    'filename': '%s' %(gen_debug_file()),
                    'formatter': 'myformatter',
                    'filters': ['special']
                    },
                }
            handlers = config_dict.get('handlers')
            handlers.update(file_handler)
            loggers = config_dict.get('loggers')
            default = loggers.get('default')
            handlers = default.get('handlers')
            handlers.append('file')
        except:
            raise Exception()

    logging.config.dictConfig(config_dict)
    return logging

def main():
    pass

if '__main__' == __name__:
    main()

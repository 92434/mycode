# -*- coding: utf-8 -*-

import logging
from config.configure import file_config_content, file_config_filepath, gen_debug_file, debug_file, dict_config

__all__ = ['get_logger', 'get_log', 'logger', 'color']

# Color escape string
COLOR_RED='\033[1;31m'
COLOR_GREEN='\033[1;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_BLUE='\033[1;34m'
COLOR_PURPLE='\033[1;35m'
COLOR_CYAN='\033[1;36m'
COLOR_GRAY='\033[1;37m'
COLOR_WHITE='\033[1;38m'
COLOR_RESET='\033[1;0m'

# Define log color
LOG_COLORS = {
	'DEBUG': COLOR_BLUE + '%s' + COLOR_RESET,
	'INFO': COLOR_GREEN + '%s' + COLOR_RESET,
	'WARNING': COLOR_YELLOW + '%s' + COLOR_RESET,
	'ERROR': COLOR_RED + '%s' + COLOR_RESET,
	'CRITICAL': COLOR_RED + '%s' + COLOR_RESET,
	'EXCEPTION': COLOR_RED + '%s' + COLOR_RESET,
}

def color(txt = '', level = 'DEBUG'):
	return LOG_COLORS.get(level) %(txt)

def file_configure(filename_user):
	filename = filename_user
	if not filename:
		filename = config.configure.file_config_filepath
		with open(filename, 'w+') as f:
			f.write(config.configure.file_config_content)
	logging.config.fileConfig(filename)
	return logging

def dict_configure(dict_config_user):
	if not dict_config_user:
		dict_config_user = dict_config
	logging.config.dictConfig(dict_config)
	return logging

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


def get_log(configure = None):
	if isinstance(configure, str):
		return file_configure(configure)
	elif isinstance(configure, dict):
		return dict_configure(configure)
	else:
		return dict_configure(None)

def get_logger(logger_name = 'my_logger'):
	if logger_name:
		return get_log().getLogger(logger_name)
	else:
		return simple_logger()

logger = get_logger(None)

if __name__ == '__main__':
	logger.debug("debug message")
	logger.info("info message")
	logger.warn("warn message")
	logger.error("error message")
	logger.critical("critical message")
	color('xiaofei', 'DEBUG')

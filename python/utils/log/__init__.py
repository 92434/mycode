# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月20日 肖飞 All rights reserved
#   
#   文件名称：__init__.py
#   创 建 者：肖飞
#   创建日期：2017年10月20日 星期五 10时34分43秒
#   修改日期：2017年10月20日 星期五 10时40分42秒
#   描    述：
#
#================================================================
import logging
from config.configure import simple_logger, file_configure, dict_configure

__all__ = ['simple_logger', 'file_configure', 'dict_configure', 'color']

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

def main():
    logging = dict_configure()
    logger = logging.getLogger('default')

    logger.debug("debug message")
    logger.info("info message")
    logger.warn("warn message")
    logger.error("error message")
    logger.critical("critical message")
    logger.debug(color('xiaofei', 'DEBUG'))

if '__main__' == __name__:
    main()

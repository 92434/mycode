# -*- coding: utf-8 -*-
import os
import sys

sys.path.append('/home/xiaofei/workspace/code/mycode/python/utils')
import log
logger = log.get_logger()

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

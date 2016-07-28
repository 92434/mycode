# -*- coding: utf-8 -*-
import os
from log import *

class base(object):
	def __new__(self, *args, **kwargs):
		obj = super(base, self).__new__(self, *args, **kwargs)
		txt = "\ncls:%s\nargs:%s\nkwargs:%s\n" %(
			self,
			args,
			kwargs
		)
		logger.debug(txt)
		return obj
	def __init__(self, *args, **kwargs):
		txt = "\nself.__class__:%s\nargs:%s\nkwargs:%s\n" %(
			self.__class__,
			args,
			kwargs
		)
		logger.debug(txt)


class derived(base):
	def __new__(self, *args, **kwargs):
		obj = super(derived, self).__new__(self, *args, **kwargs)
		txt = "\ncls:%s\nargs:%s\nkwargs:%s\n" %(
			self,
			args,
			kwargs
		)
		logger.debug(txt)
		return obj
	def __init__(self, *args, **kwargs):
		super(derived, self).__init__(*args, **kwargs)
		txt = "\nself.__class__:%s\nargs:%s\nkwargs:%s\n" %(
			self.__class__,
			args,
			kwargs
		)
		logger.debug(txt)

if __name__ == "__main__":
	d = derived(1, 2, {3 : 4})

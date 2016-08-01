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
	objects = 'base'


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
	objects = 'derived'

if __name__ == "__main__":
	d = derived(1, 2, {3 : 4})
	d.objects = 'xiaofei'
	txt = d.__class__.objects
	logger.debug(txt)
	txt = dir(d.__class__)
	logger.debug(txt)
	txt = derived.objects
	logger.debug(txt)
	txt = base.objects
	logger.debug(txt)

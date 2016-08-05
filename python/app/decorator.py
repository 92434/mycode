# -*- coding: utf-8 -*-

#import traceback
from log import *

def decorator(*args, **kwargs):
	#print traceback.extract_stack()[-1][:-1]
	txt = 'args:%s, kwargs:%s' %(str(args), str(kwargs))
	logger.debug(txt)
	return args[0]

def decorator_proxy(*args, **kwargs):
	#print traceback.extract_stack()[-1][:-1]
	txt = 'args:%s, kwargs:%s' %(str(args), str(kwargs))
	logger.debug(txt)
	return args[0]

@decorator(decorator_proxy)
def test_decorator(*args):
	#print traceback.extract_stack()[-1][:-1]
	txt = 'args:%s' %(str(args))
	logger.debug(txt)


if __name__ == '__main__':
	test_decorator(1,2)

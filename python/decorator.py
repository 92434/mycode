# -*- coding: utf-8 -*-

import traceback


def decorator(*args, **kwargs):
	print traceback.extract_stack()[-1][:-1]
	print 'args', args
	print 'kwargs', kwargs
	return args[0]

def decorator_proxy(*args, **kwargs):
	print traceback.extract_stack()[-1][:-1]
	print 'args', args
	print 'kwargs', kwargs
	return args[0]

@decorator(decorator_proxy)
def test_decorator(*args):
	print traceback.extract_stack()[-1][:-1]
	print 'args', args


if __name__ == '__main__':
	test_decorator(1,2)

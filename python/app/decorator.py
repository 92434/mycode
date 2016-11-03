# -*- coding: utf-8 -*-

#import traceback
from log import *

def decorator(*args, **kwargs):
    #print traceback.extract_stack()[-1][:-1]
    txt = 'args:%s, kwargs:%s' %(args, kwargs)
    logger.debug(txt)

    def wapper(*args, **kwargs):
        txt = 'args:%s, kwargs:%s' %(args, kwargs)
        logger.debug(txt)
        return args[0]
    return wapper(*args, **kwargs)


def decorator_proxy(*args, **kwargs):
    #print traceback.extract_stack()[-1][:-1]
    txt = 'args:%s, kwargs:%s' %(args, kwargs)
    logger.debug(txt)

    def wapper(*args, **kwargs):
        txt = 'args:%s, kwargs:%s' %(args, kwargs)
        logger.debug(txt)
        a = args[0]
        def func(*args, **kwargs):
            txt = 'args:%s, kwargs:%s' %(args, kwargs)
            logger.debug(txt)
            a(args, kwargs)
        return func
    return wapper(*args, **kwargs)

@decorator(decorator_proxy)
def test_decorator(*args, **kwargs):
    #print traceback.extract_stack()[-1][:-1]
    txt = 'args:%s, kwargs:%s' %(args, kwargs)
    logger.debug(txt)


if __name__ == '__main__':
    test_decorator(1,2)

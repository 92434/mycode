# -*- coding: utf-8 -*-

from log import *

class C(object):
    def __init__(self):
        txt = 'self:%s' %(str(self))
        logger.debug(txt)
        self.__dict__["content"] = {}
    
    def __getattribute__(self, name):
        txt = 'self:%s name:%s' %(str(self), str(name))
        logger.debug(txt)
        return super(C, self).__getattribute__(name)

    def __setattr__(self, name, value):
        txt = 'self:%s name:%s value:%s' %(str(self), str(name), str(value))
        self.content[name] = value
        logger.debug(txt)

    def __getattr__(self, name):
        txt = 'self:%s name:%s' %(str(self), str(name))
        logger.debug(txt)
        return self.content[name]

    def __get__(self, instance, owner):
        txt = 'self:%s instance:%s owner:%s' %(str(self), str(instance), str(owner))
        logger.debug(txt)
        return self

    def __set__(self, instance, value):
        txt = 'self:%s instance:%s value:%s' %(str(self), str(instance), str(value))
        logger.debug(txt)

    def __delete__(self, instance):
        txt = 'self:%s instance:%s' %(str(self), str(instance))
        logger.debug(txt)


def main():
    t = C()
    t.xiaofei = 'xiaofei'
    v = t.xiaofei
    t.xiaofei1 = 'xiaofei1'
    v = t.xiaofei1

if __name__ == '__main__':
    main()

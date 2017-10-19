#!/usr/bin/python

import sys
import importlib

class utils(object):
    def __init__(self):
        pass
    def lobject(self, o):
        print o
        for i in dir(o):
            print "%30s %s" %("<%s>:" %(i), type(eval("o." + i)))

def get_mod(str_mod):
    mod=None
    try:
        mod = eval(str_mod)
    except:
        mod = None
    mod = importlib.import_module(str_mod, mod)
    if(mod):
        return mod
    try:
        #mod = importlib.import_module(str_mod, mod)
        mod = __import__(str_mod, globals(), locals(), [], 0)
    except:
        mod=None
    return mod

import unittest
class xxx(unittest.TestCase):
    pass

def other():
    m = __import__('__main__')
    for i in dir(m):
        o = getattr(m, i)
        if isinstance(o, type) and issubclass(o, unittest.case.TestCase):
            print o
            print dir(o)


def main():
    h = utils()
    h.lobject(get_mod(sys.argv[1]) if len(sys.argv) >= 2 else None)

if __name__ == '__main__':
    main()
    print('%s' %(sys.executable))
    print('%s' %(sys.argv))
    #other()

    #import contextlib
    #import urllib
    #with contextlib.closing(urllib.urlopen("http://www.python.org/")) as front_page:
    #   for line in front_page:
    #       print line

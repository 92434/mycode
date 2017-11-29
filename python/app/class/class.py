# -*- coding: utf-8 -*-
import os
import sys
import types

class base(object):
    def __new__(cls, *args, **kwargs):
        obj = super(base, cls).__new__(cls, *args, **kwargs)
        txt = "cls:%s args:%s kwargs:%s" %(cls, args, kwargs)
        logger.debug(txt)
        return obj

    def __init__(self, *args, **kwargs):
        txt = "self:%s args:%s kwargs:%s" %(self, args, kwargs)
        logger.debug(txt)

        self.__dict__['content'] = {}

    def __getattribute__(self, name):
        txt = 'self:%s name:%s' %(self, name)
        logger.debug(txt)

        return super(base, self).__getattribute__(name)

    def __setattr__(self, name, value):
        txt = 'self:%s name:%s value:%s' %(self, name, value)
        logger.debug(txt)

        self.content[name] = value

    def __getattr__(self, name):
        txt = 'self:%s name:%s' %(self, name)
        logger.debug(txt)

        return self.content[name]

    def __get__(self, instance, owner):
        txt = 'self:%s instance:%s owner:%s' %(self, instance, owner)
        logger.debug(txt)

        return self

    def __set__(self, instance, value):
        txt = 'self:%s instance:%s value:%s' %(self, instance, value)
        logger.debug(txt)
        txt = 'instance.__class__.objects:%s' %(instance.__class__.objects)
        logger.debug(txt)
        self.objects = value

    def __delete__(self, instance):
        txt = 'self:%s instance:%s' %(self, instance)
        logger.debug(txt)

    def func(self, *args, **kwargs):
        txt = "self:%s args:%s kwargs:%s" %(self, args, kwargs)
        logger.debug(txt)

    def __call__(self, *args, **kwargs):
        txt = 'self:%s args:%s kwargs:%s' %(self, args, kwargs)
        logger.debug(txt)

    objects = 'base'

def test_attr():
    b = base(1, 2, {3 : 4})
    b.xiaofei = 'xiaofei'
    b.xiaofei = 'xiaofei1'
    
    txt = 'base.__dict__:%s' %(base.__dict__)
    logger.debug(txt)

    txt = 'base.func:%s' %(base.func)
    logger.debug(txt)

    txt = 'base.__dict__.func:%s' %(base.__dict__['func'])
    logger.debug(txt)

    txt = 'b.func:%s' %(b.func)
    logger.debug(txt)

    base.func(b)
    b.func()
    b(1, 2, {3 : 4}, v = 5)


class derived(base):
    def __new__(cls, *args, **kwargs):
        obj = super(derived, cls).__new__(cls, *args, **kwargs)
        txt = "cls:%s args:%s kwargs:%s" %(cls, args, kwargs)
        logger.debug(txt)
        return obj

    def __init__(self, *args, **kwargs):
        super(derived, self).__init__(*args, **kwargs)
        txt = "self:%s args:%s kwargs:%s" %(self, args, kwargs)
        logger.debug(txt)

    objects = 'derived'

def test_derived():
    d = derived(1, 2, {3 : 4})
    d.objects = 'xiaofei'

    txt = 'derived.objects:%s' %(derived.objects)
    logger.debug(txt)

    txt = 'base.objects:%s' %(base.objects)
    logger.debug(txt)

    txt = 'd.objects:%s' %(d.objects)
    logger.debug(txt)

class attr_wrapper(object):
    def __new__(cls, *args, **kwargs):
        obj = super(attr_wrapper, cls).__new__(cls, *args, **kwargs)
        txt = "cls:%s args:%s kwargs:%s" %(cls, args, kwargs)
        logger.debug(txt)
        return obj

    def __init__(self, *args, **kwargs):
        super(attr_wrapper, self).__init__(*args, **kwargs)
        txt = "self:%s args:%s kwargs:%s" %(self, args, kwargs)
        logger.debug(txt)

    a = base(1, 2, {3 : 4})

    objects = 'attr_wrapper'

def test_attr_wapper():
    a = attr_wrapper(1, 2, {3 : 4})
    a.a
    a.a = 'xiaofei'

    delattr(a, 'a')

    txt = 'a.a.content:%s' %(str(a.a.content))
    logger.debug(txt)


class _LazyDescr(object):

    def __init__(self, name):
        txt = 'self:%s name:%s' %(self, name)
        logger.debug(txt)
        self.name = name

    def __get__(self, instance, owner):
        txt = 'self:%s instance:%s owner:%s' %(self, instance, owner)
        logger.debug(txt)

        #txt = 'dir(instance):%s' %(dir(instance))
        #logger.debug(txt)

        txt = 'dir(instance.__class__):%s' %(dir(instance.__class__))
        logger.debug(txt)

        result = self._resolve()
        setattr(instance, self.name, result) # Invokes __set__.

        #This is a bit ugly, but it avoids running this again.
        #txt = 'instance.__class__:%s self.name:%s' %(instance.__class__, self.name)
        #logger.debug(txt)
        delattr(instance.__class__, self.name)

        return result

def _import_module(name):
    """Import module, returning the module after the last dot."""
    __import__(name)
    return sys.modules[name]

class MovedAttribute(_LazyDescr):
    def __init__(self, name, old_mod, new_mod, old_attr=None, new_attr=None):
        super(MovedAttribute, self).__init__(name)
        PY2 = sys.version_info[0] == 2
        PY3 = sys.version_info[0] == 3

        if PY3:
            if new_mod is None:
                new_mod = name
            self.mod = new_mod
            if new_attr is None:
                if old_attr is None:
                    new_attr = name
                else:
                    new_attr = old_attr
            self.attr = new_attr
        else:
            self.mod = old_mod
            if old_attr is None:
                old_attr = name
            self.attr = old_attr

    def _resolve(self):
        module = _import_module(self.mod)
        return getattr(module, self.attr)


class _LazyModule(types.ModuleType):

    def __init__(self, name):
        super(_LazyModule, self).__init__(name)
        self.__doc__ = self.__class__.__doc__

    def __getattribute__(self, name):
        txt = 'self:%s name:%s' %(self, name)
        logger.debug(txt)

        return super(_LazyModule, self).__getattribute__(name)

    def __setattr__(self, name, value):
        txt = 'self:%s name:%s value:%s' %(self, name, value)
        logger.debug(txt)
        super(_LazyModule, self).__setattr__(name, value)

    def __getattr__(self, name):
        txt = 'self:%s name:%s' %(self, name)
        logger.debug(txt)
        return super(_LazyModule, self).__getattr__(name)

class attr_owner(_LazyModule):
    pass

def test_ma():
    m_a_list = [
        MovedAttribute("urlopen", "urllib2", "urllib.request"),
        MovedAttribute("install_opener", "urllib2", "urllib.request"),
    ]

    for i in m_a_list:
        txt = 'i.name:%s' %(i.name)
        logger.debug(txt)
        txt = 'i:%s' %(i)
        logger.debug(txt)

        setattr(attr_owner, i.name, i)

    logger.debug('')

    sys.modules['urllib.request'] = ao = attr_owner('urllib.request')

    logger.debug('')

    txt = 'ao.urlopen:%s' %(ao.urlopen)
    logger.debug(txt)

    txt = 'ao.install_opener:%s' %(ao.install_opener)
    logger.debug(txt)

    txt = 'ao.urlopen:%s' %(ao.urlopen)
    logger.debug(txt)

    txt = 'ao.install_opener:%s' %(ao.install_opener)
    logger.debug(txt)

    try:
        txt = 'ao.urlopen1:%s' %(ao.urlopen1)
        logger.debug(txt)
    except Exception, e:
        logger.debug(e)

    try:
        txt = 'ao.install_opener1:%s' %(ao.install_opener1)
        logger.debug(txt)
    except Exception, e:
        logger.debug(e)

    try:
        txt = 'attr_owner.urlopen:%s' %(attr_owner.urlopen)
        logger.debug(txt)
    except Exception, e:
        logger.debug(e)

    txt = 'dir(ao):%s' %(dir(ao))
    logger.debug(txt)

if __name__ == "__main__":
    test_attr_wapper()
    #test_derived()
    #test_attr()
    #test_ma()

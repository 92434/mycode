# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月17日 肖飞 All rights reserved
#   
#   文件名称：__init__.py
#   创 建 者：肖飞
#   创建日期：2017年12月17日 星期日 11时14分36秒
#   修改日期：2017年12月17日 星期日 11时20分57秒
#   描    述：
#
#================================================================
import importlib

DICT = {
        }

def get_module(module_name):
    module = DICT.get(module_name)
    if module:
        m = importlib.import_module(module)
    else:
        m = importlib.import_module(module_name)

    return m

def main():
    pass

if '__main__' == __name__:
    main()

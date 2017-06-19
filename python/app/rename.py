#!/bin/python

# -*- coding: utf-8 -*-

import os

def get_filelist(dirname, ext_list):

    filelist = []
    for dir_name, sub_dir_list, sub_filelist in os.walk(dirname):
        #sub_filelist_ = map(lambda x : os.path.join(dir_name, x), sub_filelist)
        #sub_dirs_ = map(lambda x : os.path.join(dir_name, x), sub_dir_list)
        sub_filelist_ = [os.path.join(dirname, i) for i in sub_filelist if os.path.splitext(i)[1] in ext_list]
        sub_dirs_ = [os.path.join(dirname, i) for i in sub_dir_list]

        filelist.extend(sub_filelist_)

        for sub_dir in sub_dirs_:
            filelist.extend(get_filelist(sub_dir, ext_list));

    return filelist


if __name__ == '__main__':
    filelist = get_filelist(os.curdir, ['.bmp'])
    for i in filelist:
        print('name:%s' %(os.path.split(i)[-1]))
        print('dir:%s' %os.path.split(i)[0])

#!/bin/python

# -*- coding: utf-8 -*-

import os

def get_filelist(dirname, ext_list):

    filelist = []
    for dir_name, sub_dir_list, sub_filelist in os.walk(dirname):
        sub_filelist = [os.path.join(dir_name, i) for i in sub_filelist if os.path.splitext(i)[1] in ext_list]

        filelist.extend(sub_filelist)

    return filelist


if __name__ == '__main__':
    filelist = get_filelist(os.curdir, ['.bmp'])
    for i in filelist:
        print('name:%s' %(os.path.split(i)[-1]))
        print('dir:%s' %os.path.split(i)[0])

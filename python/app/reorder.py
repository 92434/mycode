#!/usr/bin/env python
# -*- coding: utf-8 -*-
#================================================================
#   Copyright (C) 2017年07月15日 肖飞 All rights reserved
#   
#   文件名称：reorder.py
#   创 建 者：肖飞
#   创建日期：2017年07月15日 星期六 16时54分21秒
#   修改日期：2017年07月16日 星期日 12时00分36秒
#   描    述：
#
#================================================================

import sys
import optparse
import os
import shutil

def create_ordered_file(dirname, person_id, state, index, line):
    if not os.access(line, os.F_OK):
        return
    
    basename = os.path.basename(line)
    new_filename = os.path.join(dirname, '%s' %(person_id), state, '%d_' %(index) + basename)
    if not os.access(os.path.dirname(new_filename), os.F_OK):
        os.makedirs(os.path.dirname(new_filename))
    shutil.copy2(line, new_filename)

def reorder_from_list(filelist):
    content = ''
    with open(filelist) as f:
        content = f.read()
    lines = content.splitlines()
    dirname = 'reorder'

    if os.access(dirname, os.F_OK):
        print('%s exists!' %(dirname))
        return

    enroll_person_id = 0
    verify_person_id = 0
    person_id = 0
    index = 0
    state = ''
    for line in lines:
        if '[enroll]' == line.strip():
            enroll_person_id += 1
            person_id = enroll_person_id
            state = 'enroll'
        elif '[verify]' == line.strip():
            verify_person_id += 1
            person_id = verify_person_id
            state = 'verify'
        elif len(line.strip()):
            print('state:%s, person_id:%d, index:%d' %(state, person_id, index))
            create_ordered_file(dirname, person_id, state, index, line.strip())
            index += 1


def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-f', '--filelist', action='store', dest='filelist', help='assign ordered filelist', default="index.txt")
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    reorder_from_list(opts.filelist)

if '__main__' == __name__:
	main()

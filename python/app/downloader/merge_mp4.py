# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年11月04日 肖飞 All rights reserved
#   
#   文件名称：merge_mp4.py
#   创 建 者：肖飞
#   创建日期：2017年11月04日 星期六 22时39分22秒
#   修改日期：2017年11月05日 星期日 15时40分34秒
#   描    述：
#
#================================================================
import os
import sys
import optparse
import subprocess
import re

import filesystem

import log
logging = log.dict_configure()
logger = logging.getLogger('default')

def merge_mp4(output_filepath, mp4_files):
    for i in mp4_files:
        if not i.endswith('.mp4'):
            return
        if not os.access(i, os.W_OK):
            return

    ts_files_str = '|'.join(mp4_files)

    cmd = ['ffmpeg']
    cmd.append('-i')
    cmd.append('concat:%s' %(ts_files_str))
    cmd.append('-acodec')
    cmd.append('copy')
    cmd.append('-vcodec')
    cmd.append('copy')
    cmd.append('-absf')
    cmd.append('aac_adtstoasc')
    cmd.append('%s' %(output_filepath))
    #logger.debug('%s' %(cmd))

    if subprocess.Popen(cmd, cwd = os.path.curdir).wait() != 0:
        raise Exception('merge %s failed!!!' %(output_filepath))
    else:
        for i in mp4_files:
            os.remove(i)

def mp4_part_key(path):
    p = '.*-(\d+).mp4'
    m = re.search(p, path)
    if m:
        return int(m.group(1))
    else:
        raise Exception('mp4 file name %s error!!!' %(path))

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-d', '--dir', dest='dir', help='dir', default=None)
    options.add_option('-o', '--output-path', dest='output_path', help='output_path', default=None)
    opts, args = options.parse_args(argv)
    logger.debug('opts:%s' %(opts))
    logger.debug('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.dir:
        return

    if not os.path.isdir(opts.dir):
        return

    fs = filesystem.filesystem()
    filelist = fs.get_filelist('.', ['.mp4'])
    filelist = sorted(filelist, key = lambda path : (mp4_part_key(path)))

    output_path = ''
    if not opts.output_path:
        output_path = os.path.join(opts.dir, '%s.mp4' %(os.path.basename(opts.dir)))
    else:
        output_path = opts.output_path

    #logger.debug('output_path:%s' %(filelist))
    #for i in filelist:
    #    logger.debug('add %s' %(i))
    merge_mp4(output_path, filelist)

if '__main__' == __name__:
    main()

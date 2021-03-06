# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年11月04日 肖飞 All rights reserved
#   
#   文件名称：merge_mp4.py
#   创 建 者：肖飞
#   创建日期：2017年11月04日 星期六 22时39分22秒
#   修改日期：2017年11月18日 星期六 16时10分46秒
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

def mp4_to_ts(mp4_file):
    ret = False
    cmd = ['ffmpeg']
    cmd.append('-i')
    cmd.append('%s' %(mp4_file))
    #cmd.append('-sameq')
    #cmd.append('%s.mpg' %(mp4_file))
    cmd.append('-acodec')
    cmd.append('copy')
    cmd.append('-vcodec')
    cmd.append('copy')
    cmd.append('-vbsf')
    cmd.append('h264_mp4toannexb')
    cmd.append('%s.ts' %(mp4_file))
    #logger.debug('%s' %(cmd))

    if subprocess.Popen(cmd, cwd = os.path.curdir).wait() != 0:
        raise Exception('merge %s failed!!!' %(output_filepath))
    else:
        ret = True

    return ret


def merge_mp4(output_filepath, mp4_files):
    ret = False
    for i in mp4_files:
        if not i.endswith('.mp4'):
            return ret
        if not os.access(i, os.W_OK):
            return ret

    for i in mp4_files:
        if not mp4_to_ts(i):
            return ret

    merge_list_file = 'filelist.txt'
    with open(merge_list_file, 'w+') as f:
        for i in mp4_files:
            f.write('file \'%s.ts\'\n' %(i))
        f.flush()

    cmd = ['ffmpeg']
    cmd.append('-f')
    cmd.append('concat')
    cmd.append('-safe')
    cmd.append('0')
    cmd.append('-i')
    cmd.append('%s' %(merge_list_file))
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
            os.remove('%s.ts' %(i))

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
    filelist = fs.get_filelist(opts.dir, ['.mp4'])
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

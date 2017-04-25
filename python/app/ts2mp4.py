#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

def find_ts_files(top):
    ts_files = []
    for i in os.listdir(top):
        if os.path.isfile(i) and i.endswith('.ts'):
            ts_files.append(i)
    return ts_files


def main(argv):
    out_file = argv[0]
    out_file.decode('utf-8')
    if not out_file.endswith('.mp4'):
        print('%s not a mp4 file!' %(out_file))
        return

    ts_files = find_ts_files('.')
    sskip = len('index')
    eskip = len('.ts')
    ts_files = sorted(ts_files, key = lambda x:int(x[sskip:-eskip]))
    ts_files_str = '|'.join(ts_files)

    cmd = ['ffmpeg']
    cmd.append('-i')
    cmd.append('concat:%s' %(ts_files_str))
    cmd.append('-acodec')
    cmd.append('copy')
    cmd.append('-vcodec')
    cmd.append('copy')
    cmd.append('-absf')
    cmd.append('aac_adtstoasc')
    cmd.append('%s' %(out_file))
    print(cmd)

    if subprocess.Popen(cmd, cwd=os.path.curdir).wait() != 0:
        print('failed!')
    else:
        for i in ts_files:
            os.remove(i)

if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1:])

# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月20日 肖飞 All rights reserved
#   
#   文件名称：filesystem.py
#   创 建 者：肖飞
#   创建日期：2017年10月20日 星期五 15时17分50秒
#   修改日期：2017年10月20日 星期五 15时24分24秒
#   描    述：
#
#================================================================
import os
import log
logging = log.dict_configure()
logger = logging.getLogger('default')

class filesystem(object):
    def __init__(self):
        pass

    def get_filelist(self, dirname, ext_list):

        filelist = []
        for dir_name, sub_dir_list, sub_filelist in os.walk(dirname):
            sub_filelist = [os.path.join(dir_name, i) for i in sub_filelist if os.path.splitext(i)[1] in ext_list]

            filelist.extend(sub_filelist)

        return filelist

def main():
    fs = filesystem()
    l = fs.get_filelist('.', ['.py'])
    logger.debug('l:%s' %(l))
    

if '__main__' == __name__:
    main()

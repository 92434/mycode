# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月09日 肖飞 All rights reserved
#   
#   文件名称：watcher.py
#   创 建 者：肖飞
#   创建日期：2017年12月09日 星期六 21时42分17秒
#   修改日期：2017年12月09日 星期六 21时46分20秒
#   描    述：
#
#================================================================
import os
import sys
import signal

class watcher():  
    def __init__(self):  
        self.child = os.fork()  
        if self.child == 0:  
            return  
        else:  
            self.watch()  
  
    def watch(self):  
        try:  
            os.wait()  
        except KeyboardInterrupt:  
            self.kill()  
        sys.exit()  
  
    def kill(self):  
        try:  
            os.kill(self.child, signal.SIGKILL)  
        except OSError:  
            pass  

def main():
    pass

if '__main__' == __name__:
    main()

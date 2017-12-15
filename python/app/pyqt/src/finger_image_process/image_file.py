# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月13日 肖飞 All rights reserved
#   
#   文件名称：image_file.py
#   创 建 者：肖飞
#   创建日期：2017年12月13日 星期三 14时39分55秒
#   修改日期：2017年12月13日 星期三 14时52分45秒
#   描    述：
#
#================================================================
import time

class image_file():
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self.header1 = [
                0x42, 0x4d,  # //file type
                0x0, 0x0, 0x0, 0x00,  # //file size***
                0x00, 0x00,  # //reserved
                0x00, 0x00,  # //reserved
                0x36, 0x4, 0x00, 0x00,  # //head byte***
                ##//infoheader
                0x28, 0x00, 0x00, 0x00,  # //struct size
            ]

        self.header2 = [
                0x01, 0x00,  # //must be 1
                0x08, 0x00,  # //color count
                0x00, 0x00, 0x00, 0x00,  # //compression
                0x00, 0x00, 0x00, 0x00,  # //data size***
                0x00, 0x00, 0x00, 0x00,  # //dpix
                0x00, 0x00, 0x00, 0x00,  # //dpiy
                0x00, 0x00, 0x00, 0x00,  # //color used
                0x00, 0x00, 0x00, 0x00,  # //color important
            ]


    def size_info(self):
        buf = []
        for i in range(4):
            buf.append((self.width >> (8 * i)) & 0xFF)
        for i in range(4):
            buf.append((self.height >> (8 * i)) & 0xFF)
        return buf

    def lut_info(self):
        buf = [] 
        for i in range(256):
            buf.append(i)# r
            buf.append(i)# g
            buf.append(i)# b
            buf.append(0)# a
        return buf

    def write_bmp(self, filename, buf):
        img = []
        img += self.header1
        img += self.size_info()
        img += self.header2
        img += self.lut_info()

        for b in buf:
            img.append(ord(b))

        filepath = "%s.bmp" %(filename)
        s = ''
        with open(filepath, 'wb+') as f:
            for v in img:
                s += chr(v)
            f.write(s)

def main():
    f = image_file(16, 16)
    s = ''
    for i in range(16):
        for j in range(16):
            s += chr(i * 16 + j)

    f.write_bmp('test', s)

if '__main__' == __name__:
    main()

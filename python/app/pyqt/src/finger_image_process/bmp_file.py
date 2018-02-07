# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月13日 肖飞 All rights reserved
#   
#   文件名称：bmp_file.py
#   创 建 者：肖飞
#   创建日期：2017年12月13日 星期三 14时39分55秒
#   修改日期：2018年02月07日 星期三 13时40分17秒
#   描    述：
#
#================================================================
import time
import struct

class bmp_file():
    #struct BMP_FILEHDR {                     // BMP file header
    #	char   bfType[2];                   // "BM"
    #	int  bfSize;                      // size of file
    #	short  bfReserved1;
    #	short  bfReserved2;
    #	int  bfOffBits;                   // pointer to the pixmap bits
    #};
    ##pragma pack()
    #
    #struct BMP_INFOHDR {                     // BMP information header
    #	int  biSize;                      // size of this struct
    #	int  biWidth;                     // pixmap width
    #	int  biHeight;                    // pixmap height
    #	short  biPlanes;                    // should be 1
    #	short  biBitCount;                  // number of bits per pixel
    #	int  biCompression;               // compression method
    #	int  biSizeImage;                 // size of image
    #	int  biXPelsPerMeter;             // horizontal resolution
    #	int  biYPelsPerMeter;             // vertical resolution
    #	int  biClrUsed;                   // number of colors used
    #	int  biClrImportant;              // number of important colors
    #};
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.format = 'BM'
        self.unused_short = 0
        self.bpp = 8
        self.compress_method = 0
        self.number_of_color = 0
        self.number_of_important_color = 0

        self.bmp_des = '<'

        self.bmp_des_bmpfile_hdr = '2s' #format (BM)
        self.bmp_des_bmpfile_hdr += 'i' #file size
        self.bmp_des_bmpfile_hdr += 'h' #unused
        self.bmp_des_bmpfile_hdr += 'h' #unused
        self.bmp_des_bmpfile_hdr += 'i' #pointer to pixmap bits (1078)
        self.bmp_des += self.bmp_des_bmpfile_hdr

        self.bmp_des_bmpinfo_hdr = 'i' #size of this struct (40)
        self.bmp_des_bmpinfo_hdr += 'i' #pixmap width (w)
        self.bmp_des_bmpinfo_hdr += 'i' #pixmap height (h)
        self.bmp_des_bmpinfo_hdr += 'h' #should be 1 (1)
        self.bmp_des_bmpinfo_hdr += 'h' #number of bits per pixel (8)
        self.bmp_des_bmpinfo_hdr += 'i' #compression method (0)
        self.bmp_des_bmpinfo_hdr += 'i' #size of image (w * h)
        self.bmp_des_bmpinfo_hdr += 'i' #horizontal resolution (w)
        self.bmp_des_bmpinfo_hdr += 'i' #vertical resolution (h)
        self.bmp_des_bmpinfo_hdr += 'i' #number of colors used (0)
        self.bmp_des_bmpinfo_hdr += 'i' #number of important colors (0)

        self.bmp_des_bmpinfo_hdr_size = struct.calcsize(self.bmp_des_bmpinfo_hdr)

        self.bmp_des += self.bmp_des_bmpinfo_hdr

        self.lut = self.gen_lut()
        self.bmp_des += '1024s' #256 * (r,g,b,a)

        self.bmp_des_info = self.bmp_des

        self.bmp_data_offset = struct.calcsize(self.bmp_des)

        #!!!慎用
        self._width = self.width
        self._height = self.height
        self.size_of_image = ((self.bpp + 7) / 8) * self.width * self.height

        self.bmp_des_data = '%ss' %(self.size_of_image)
        self.bmp_des += self.bmp_des_data

        self.filesize = struct.calcsize(self.bmp_des)

    def gen_lut(self):
        lut = ''
        for i in range(256):
            lut += struct.pack('<4c', chr(i), chr(i), chr(i), chr(0))
        return lut

    def list_buf_to_data(self, list_buf):
        data = ''
        for i in list_buf:
            data += struct.pack('<c', i)
        return data

    def write_bmp(self, filename, buf):
        self.data = buf
        bmp_data = struct.pack(
                self.bmp_des,
                self.format,
                self.filesize,
                self.unused_short,
                self.unused_short,
                self.bmp_data_offset,
                self.bmp_des_bmpinfo_hdr_size,
                self.width,
                self.height,
                1,
                self.bpp,
                self.compress_method,
                self.size_of_image,
                self._width,
                self._height,
                self.number_of_color,
                self.number_of_important_color,
                self.lut,
                self.data
                )
        filepath = "%s.bmp" %(filename)
        with open(filepath, 'wb+') as f:
            f.write(bmp_data)

    def read_bmp(self, filename):
        f = open(filename, 'rb')

        data = f.read(self.bmp_data_offset)
        self.format, self.filesize, self.unused_short, self.unused_short, self.bmp_data_offset, self.bmp_des_bmpinfo_hdr_size, self.width, self.height, _, self.bpp, self.compress_method, self.size_of_image, self._width, self._height, self.number_of_color, self.number_of_important_color, self.lut = struct.unpack(self.bmp_des_info, data)
        self.size_of_image = ((self.bpp + 7) / 8) * self.width * self.height

        self.bmp_des_data = '<%ss' %(self.size_of_image)

        self.data = f.read(self.size_of_image)

        f.close()

def main():
    f = bmp_file(16, 16)
    s = ''
    for i in range(16):
        for j in range(16):
            s += chr(i * 16 + j)

    f.write_bmp('test', s)
    f.read_bmp('test.bmp')

if '__main__' == __name__:
    main()

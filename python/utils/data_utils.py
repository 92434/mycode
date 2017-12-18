# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月18日 肖飞 All rights reserved
#   
#   文件名称：data_utils.py
#   创 建 者：肖飞
#   创建日期：2017年12月18日 星期一 10时43分46秒
#   修改日期：2017年12月18日 星期一 11时54分05秒
#   描    述：
#
#================================================================
import io

class abstract_archive(object):
    def __init__(self):
        self.block_size = 1024 * 1024 * 2

    def compress(self, in_data):
        raise Exception('not valid!')
    
    def decompress(self, in_data):
        raise Exception('not valid!')

class zlib_archive(abstract_archive):
    def __init__(self):
        import zlib
        super(zlib_archive, self).__init__()
        self.tool = zlib

    def compress(self, in_data):
        obj = self.tool.compressobj()
        bi = io.BytesIO(in_data)
        out_data = ''
        #print('in_data length:%s' %(len(in_data)))

        while True:
            content = bi.read(self.block_size)
            if not content:
                break

            out_data += obj.compress(content)
        out_data += obj.flush()

        #print('out_data length:%s' %(len(out_data)))
        return out_data

    def decompress(self, in_data):
        obj = self.tool.decompressobj()
        bi = io.BytesIO(in_data)
        out_data = ''
        #print('in_data length:%s' %(len(in_data)))

        while True:
            content = bi.read(self.block_size)
            if not content:
                break

            out_data += obj.decompress(content)
        out_data += obj.flush()

        #print('out_data length:%s' %(len(out_data)))
        return out_data

class lzma_archive(abstract_archive):
    def __init__(self):
        import lzma
        super(lzma_archive, self).__init__()
        self.tool = lzma

    def compress(self, in_data):
        bi = io.BytesIO(in_data)
        out_data = ''
        #print('in_data length:%s' %(len(in_data)))

        while True:
            content = bi.read(self.block_size)
            if not content:
                break

            out_data += self.tool.compress(content)

        #print('out_data length:%s' %(len(out_data)))
        return out_data

    def decompress(self, in_data):
        bi = io.BytesIO(in_data)
        out_data = ''
        #print('in_data length:%s' %(len(in_data)))

        while True:
            content = bi.read(self.block_size)
            if not content:
                break

            out_data += self.tool.decompress(content)

        #print('out_data length:%s' %(len(out_data)))
        return out_data

class archive(object):
    def __init__(self):
        self.archive = zlib_archive()
        #self.archive = lzma_archive()

def main():
    a = archive()
    data = 'hello word, 00000000000000000000000000000000' * 100
    data = a.archive.compress(data)
    data = a.archive.decompress(data)
    #print('%s' %(data))

if '__main__' == __name__:
    main()

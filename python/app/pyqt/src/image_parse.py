# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月13日 肖飞 All rights reserved
#   
#   文件名称：image_parse.py
#   创 建 者：肖飞
#   创建日期：2017年12月13日 星期三 13时14分07秒
#   修改日期：2017年12月13日 星期三 18时36分41秒
#   描    述：
#
#================================================================
import ConfigParser
import optparse
import os
import sys

class image_database(object):
    def __init__(self):
        self.data_size = 22181

        self.conf_file = os.path.join(os.path.dirname(__file__), 'default.ini')
        self.conf = ConfigParser.ConfigParser()
        if not os.path.exists(self.conf_file):
            for i in range(0, self.data_size):
                self.conf.add_section('%d' %(i))
            self.save()
        else:
            self.conf.read(self.conf_file)
        print('load %s successful!!' %(self.conf_file))

    def save(self):
        with open(self.conf_file, 'w+') as f:
            self.conf.write(f)

    
    def get_conf_section(self, section):
        ret = None
        if type(section) == int:
            section = '%d' %(section)

        if not self.conf.has_section(section):
            return ret

        items = self.conf.items(section)
        if len(items) == 0:
            return ret

        items = sorted(items, key = lambda x : int(x[1]))
        value, count = items[0]

        if int(count) == 1:
            value = None

        ret = value

        return ret
    
    def get_conf_section_item(self, section, item):
        ret = None

        if type(section) == int:
            section = '%d' %(section)
        if type(item) == int:
            item = '%02x' %(item)

        if not self.conf.has_section(section):
            return ret

        if not self.conf.has_option(section, item):
            return ret

        return self.conf.get(section, item)

    def update_conf_setction_item(self, section, item):
        if type(section) == int:
            section = '%d' %(section)

        if type(item) == int:
            item = '%02x' %(item)

        value = self.get_conf_section_item(section, item)
        if not value:
            value = '%d' %(0)

        value = '%d' %(int(value) + 1)

        #print('update section:%s, item:%s, value:%s' %(section, item, value))

        self.conf.set(section, item, value)

    def parse_csv_file(self, csv_file):
        d = {}
        with open(csv_file) as f:
            for line in f:
                t, pid, odata, idata = line.strip().split(',')
                l_data = d.get(pid)
                if not l_data:
                    l_data = []
                data = (odata, idata)
                l_data.append(data)
                d.update({pid : l_data})
        return d

    def update_image_info_bitmap(self, csv_file):
        d = self.parse_csv_file(csv_file)

        for pid in d:
            l_data = d.get(pid)
            if len(l_data) != self.data_size:
                continue
            print('add pid %s' %(pid))
            for section, item in enumerate(l_data):
                self.update_conf_setction_item(section, int(item[1], 16))
        self.save()

    def gen_pixel_bytes(self, csv_file, pid_select, index):
        d = self.parse_csv_file(csv_file)
        for pid in d:
            l_data = d.get(pid)
            if len(l_data) != self.data_size:
                continue
            if pid != pid_select:
                print('skip pid %s' %(pid))
                continue

            for section in range(1, self.data_size - 4, 3):
                byte1 = self.get_conf_section(section + 0)
                byte2 = self.get_conf_section(section + 1)
                byte3 = self.get_conf_section(section + 2)
                #if not byte1:
                #    print('pos %s not found' %(section + 0))
                #if not byte2:
                #    print('pos %s not found' %(section + 1))
                #if not byte3:
                #    print('pos %s not found' %(section + 2))

                byte1 = byte1 if byte1 else l_data[section + 0][1]
                byte2 = byte2 if byte2 else l_data[section + 1][1]
                byte3 = byte3 if byte3 else l_data[section + 2][1]

                #print('%s' %(byte1))
                #print('%s' %(byte2))
                #print('%s' %(byte3))

                #xor = 0x55
                #xor = 0xaa
                #xor = 0xff
                xor = 0x00
                byte1 = chr(int(byte1, 16) ^ xor)
                byte2 = chr(int(byte2, 16) ^ xor)
                byte3 = chr(int(byte3, 16) ^ xor)

                byte1_h = chr((ord(byte1) & 0xf0) >> 4)
                byte1_l = chr((ord(byte1) & 0x0f))

                byte2_h = chr((ord(byte2) & 0xf0) >> 4)
                byte2_l = chr((ord(byte2) & 0x0f))

                byte3_h = chr((ord(byte3) & 0xf0) >> 4)
                byte3_l = chr((ord(byte3) & 0x0f))

                halfbytes = [byte1_l, byte1_h, byte2_l, byte2_h, byte3_l, byte3_h]

                if index == 6:
                    #pixel1 = chr(ord(byte1_h) + (ord(byte1_l) << 4))
                    #pixel2 = chr(ord(byte3_l) + (ord(byte2_h) << 4))
                    pixel1 = chr((ord(byte1_l) + (0 << 4)) ^ 0x0f)
                    pixel2 = chr((0 + (ord(byte2_h) << 4) ^ 0xf0))
                else:
                    pixel1 = chr((ord(halfbytes[index]) << 4))
                    pixel2 = chr((ord(halfbytes[index]) << 4))

                #print('%s %s' %(hex(ord(d1)), hex(ord(d2))))

                yield pixel1, pixel2

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-f', '--file', dest='file', help='file', default=None)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    database = image_database()
    database.update_image_info_bitmap(opts.file)

    for i in range(22181):
        print('%08d:%s' %(i, database.get_conf_section(i)))

if '__main__' == __name__:
    main(sys.argv[1:])

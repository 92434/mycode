# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月13日 肖飞 All rights reserved
#   
#   文件名称：image_parse.py
#   创 建 者：肖飞
#   创建日期：2017年12月13日 星期三 13时14分07秒
#   修改日期：2018年02月08日 星期四 17时36分08秒
#   描    述：
#
#================================================================
import ConfigParser
import optparse
import os
import sys

import bmp_file

class image_database(object):
    def __init__(self, remove_conf_data = True):
        self.conf_file = None
        self.conf = None
        self.load_conf(remove_conf_data)

    def load_conf(self, remove_conf_data = True):
        self.conf_file = os.path.join(os.path.dirname(__file__), 'default.ini')
        self.conf = ConfigParser.ConfigParser()

        if os.path.exists(self.conf_file):
            if remove_conf_data:
                self.save()
        else:
            self.save()

        self.conf.read(self.conf_file)
        print('load %s!!' %(self.conf_file))

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
    
    def set_section_item_value(self, section, item, value):
        if not self.conf.has_section(section):
            self.conf.add_section(section)
        self.conf.set(section, item, value)

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

        self.set_section_item_value(section, item, value)

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

    def update_image_info_bitmap(self, data):
        for section, item in enumerate(data):
            self.update_conf_setction_item(section, ord(item))
        self.save()

    def get_valid_data(self, data, skip_s, skip_k, width, height, step):
        valid_data = ''

        data = data[skip_s:]
        data = data[:len(data) - skip_k]
        for h in range(height):
            line = ''
            for w in range(step):
                c = data[h * step + w]
                c = int(c[1], 16)
                #print('c:%s' %(repr(c)))

                if w >= width:
                    if not c == 0:
                        raise Exception('!')
                    c = '%02x' %(c)
                    line += c
                else:
                    valid_data += chr(c)

            print('%03d:%s' %(h, line))
        return valid_data

    def gen_sort_combine(self, l_head, l_data):
        result = []

        if len(l_data) <= 1:
            result.append(l_head + l_data)
        else:
            for i in l_data:
                l_head_sub = l_head[:]
                l_data_sub = l_data[:]

                l_head_sub.append(i)
                l_data_sub.remove(i)
                result += self.gen_sort_combine(l_head_sub, l_data_sub)

        return result
            
    def gen_pixel_bytes(self, data, index, bulk, pixel_per_bulk, slice_per_byte):
        pixels = ''
        for section in range(0, len(self.conf.sections()), bulk):
            byte_bulk = ''
            for i in range(bulk):
                byte = self.get_conf_section(section + i)
                byte = byte if byte else hex(ord(data[section + i]))
                #print('%s' %(byte))

                #xor = 0x55
                #xor = 0xaa
                #xor = 0xff
                xor = 0x00
                byte = chr(int(byte, 16) ^ xor)
                byte_bulk += byte

            list_slice = []

            mask_width = 8 / slice_per_byte
            mask = 0
            for i in range(mask_width):
                mask += (1 << i)

            for i in range(bulk * slice_per_byte):
                byte_index = i / slice_per_byte
                byte_start = i % slice_per_byte
                byte = byte_bulk[byte_index]
                byte_slice = chr((ord(byte) >> (mask_width * byte_start)) & mask)
                #print('%s' %(hex(ord(byte_slice))))
                list_slice.append(byte_slice)

            if index == -1:
                list_pixel_order = [
                        (3, 0),
                        (2, 7),
                        (11, 10),
                        (8, 9),

                        #(0, 2),
                        #(7, 3),
                        #(10, 8),
                        #(9, 11),

                        #(1, 0),
                        #(4, 7),
                        #(5, 10),
                        #(6, 9),
                        ]
                for pixel_index in range(pixel_per_bulk):
                    pixel = 0
                    pixel_order = list_pixel_order[pixel_index]
                    for order in range(len(pixel_order)):
                        slice_index = pixel_order[order]
                        if slice_index == -1:
                            c = 0
                        else:
                            c = ord(list_slice[slice_index])
                        pixel += (c << (order * mask_width))

                    pixels += chr(pixel)
                continue

            for pixel_index in range(pixel_per_bulk):
                pixels += chr(ord(list_slice[index]) << (8 - mask_width))


        return pixels

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-f', '--file', dest='file', help='file', default=None)
    options.add_option('-p', '--pids', dest='pids', help='pids', default='9')
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return
    database = image_database()

    #halfbytes1 = ['byte1_l', 'byte4_h', 'byte5_h', 'byte6_l']
    #halfbytes2 = ['byte2_h', 'byte3_h', 'byte4_l', 'byte5_l']
    #halfbytes3 = ['byte1_h', 'byte2_l', 'byte3_l', 'byte6_h']
    #
    #pixel_bit_11_8_list = database.gen_sort_combine([], halfbytes1)
    #pixel_bit_9_4_list = database.gen_sort_combine([], halfbytes2)


    #for i in range(4 * 3 * 2 * 1 * 4 * 3 * 2 * 1):
    #    if not i in [168 ,170 ,174 ,176 ,180 ,182 ,24 ,26 ,30 ,32 ,36 ,38]:
    #        continue
    #    index_high = i / len(pixel_bit_11_8_list)
    #    index_low = i % len(pixel_bit_9_4_list)

    #    halfbytes_h = pixel_bit_11_8_list[index_high]
    #    halfbytes_l = pixel_bit_9_4_list[index_low]

    #    pixel1 = '%s|%s' %(halfbytes_h[0], halfbytes_l[0])
    #    pixel2 = '%s|%s' %(halfbytes_h[1], halfbytes_l[1])
    #    pixel3 = '%s|%s' %(halfbytes_h[2], halfbytes_l[2])
    #    pixel4 = '%s|%s' %(halfbytes_h[3], halfbytes_l[3])
    #    print('[%04s] %s, %s, %s, %s' %(i, pixel1, pixel2, pixel3, pixel4))

    if not opts.file:
        options.print_help()
        return

    d = database.parse_csv_file(opts.file)
    for pid, data in d.items():
        print('%s:%s' %(pid, len(data)))

    if not opts.pids:
        return

    pids = opts.pids.split(',')

    for pid in pids:
        if not pid in d.keys():
            continue

        data = d.get(pid)

        data = database.get_valid_data(data, 1, 4, 96, 80, 132)
        database.update_image_info_bitmap(data)

        bulk = 6
        pixel_per_bulk = 4
        slice_per_byte = 2
        count = bulk * slice_per_byte
        for i in range(count):
            pixels = database.gen_pixel_bytes(data, i, bulk, pixel_per_bulk, slice_per_byte)
            bmp = bmp_file.bmp_file(64, 80)
            bmp.write_bmp('%03d.bmp' %(i), pixels)

        pixels = database.gen_pixel_bytes(data, -1, bulk, pixel_per_bulk, slice_per_byte)
        bmp = bmp_file.bmp_file(64, 80)
        bmp.write_bmp('%s.bmp' %('test'), pixels)

if '__main__' == __name__:
    main(sys.argv[1:])

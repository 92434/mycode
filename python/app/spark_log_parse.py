# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月26日 肖飞 All rights reserved
#   
#   文件名称：log_parse.py
#   创 建 者：肖飞
#   创建日期：2017年07月26日 星期三 09时11分14秒
#   修改日期：2017年08月31日 星期四 19时15分00秒
#   描    述：
#
#================================================================
import sys
import datetime
import xlwt
import os
import optparse
import shutil

class result_info(object):
    matched_state_changed = False
    image_catagory = ''
    image_id = ''
    image_serial_no = ''
    new_catagory = ''
    new_id = ''
    path = ''
    update_catagory = ''
    update_id = ''
    enroll_list = []

    def __init__(self, matched_state_changed, image_catagory, image_id, image_serial_no, new_catagory, new_id, update_catagory, update_id, path, enroll_list):
        self.matched_state_changed = matched_state_changed
        self.image_catagory = image_catagory
        self.image_id = image_id
        self.image_serial_no = image_serial_no
        self.new_catagory = new_catagory
        self.new_id = new_id
        self.path = path
        self.update_catagory = update_catagory
        self.update_id = update_id
        self.enroll_list = enroll_list[:]

def copy_file_from_far(bmp_path, new_dir_name, fa_catagory, fa_id, catagory, id):
    bmp_basename = os.path.basename(bmp_path)
    new_bmp_path = os.path.join(new_dir_name, 'images', fa_catagory, fa_id, catagory, id, bmp_basename)
    if not os.access(os.path.dirname(new_bmp_path), os.F_OK):
        os.makedirs(os.path.dirname(new_bmp_path))
    shutil.copy2(bmp_path, new_bmp_path);

def parse_log_list(filelist):
    content = []
    fr_result = []
    fa_result = []
    map_enroll_catagory_map_id_path_list = {}
    mode = ''
    for i in filelist:
        with open(i) as f:
            for line in f:
                line = line.strip()
                fields = line.split(',')
                if len(fields) == 6:
                    current_enroll_id_info, catagory_info, id_info, serial_no_info, path_info, ret_info = fields
                    if mode != current_enroll_id_info.split(':')[0]:
                        mode = current_enroll_id_info.split(':')[0]
                        map_enroll_catagory_map_id_path_list = {}
                    map_id_path_list = map_enroll_catagory_map_id_path_list.get(catagory_info.split(':')[1], None)
                    if not map_id_path_list:
                        map_id_path_list = {id_info.split(':')[1] : [path_info.split(':')[1]]}
                        map_enroll_catagory_map_id_path_list.update({catagory_info.split(':')[1] : map_id_path_list})
                    else:
                        path_list = map_id_path_list.get(id_info.split(':')[1], None)
                        if not path_list:
                            path_list = [path_info.split(':')[1]]
                            map_id_path_list.update({id_info.split(':')[1] : path_list})
                        else:
                            path_list.append(path_info.split(':')[1])


                elif len(fields) == 11:
                    catagory_info, id_info, serial_no_info, match_state_info, new_match_state_info, new_matched_catagory_info, new_matched_id_info, update_template_catagory_info,  update_template_id_info, path_info, ret_info = fields
                    matched_state_changed = False
                    if (match_state_info.split(':')[1] != 'UNKNOW' and new_match_state_info.split(':')[1] != 'UNKNOW') or (match_state_info.split(':')[1] == 'UNKNOW'):
                        if match_state_info.split(':')[1] != new_match_state_info.split(':')[1]:
                            matched_state_changed = True

                    image_catagory = catagory_info.split(':')[2]
                    image_id = id_info.split(':')[1]
                    image_serial_no = serial_no_info.split(':')[1]
                    new_catagory = new_matched_catagory_info.split(':')[1]
                    new_id = new_matched_id_info.split(':')[1]
                    update_catagory = update_template_catagory_info.split(':')[1]
                    update_id = update_template_id_info.split(':')[1]
                    path = path_info.split(':')[1]

                    if mode != catagory_info.split(':')[0]:
                        mode = catagory_info.split(':')[0]

                    if mode == 'fa':
                        if new_match_state_info.split(':')[1] == 'MATCHED':
                            if image_catagory == new_catagory and image_id == new_id:
                                pass
                            else:
                                enroll_list = map_enroll_catagory_map_id_path_list.get(new_catagory).get(new_id)
                                fa_result_item = result_info(matched_state_changed, image_catagory, image_id, image_serial_no, new_catagory, new_id, update_catagory, update_id, path, enroll_list)
                                fa_result.append(fa_result_item)
                    if mode == 'fr':
                        enroll_list = []
                        fr_result_item = result_info(matched_state_changed, image_catagory, image_id, image_serial_no, new_catagory, new_id, update_catagory, update_id, path, enroll_list)
                        fr_result.append(fr_result_item)
    return fr_result, fa_result

''' 
设置单元格样式 
'''
    
def style(fontname, height, highlight, bold = False): 
    xlstyle = xlwt.XFStyle() # 初始化样式 
    
    font = xlwt.Font() # 为样式创建字体 
    font.name = fontname # 'Times New Roman' 
    font.bold = bold 
    font.colour_index = 4
    font.height = height 
    xlstyle.font = font 

    #print(xlwt.Style.colour_map)
    if highlight:
        pattern = xlwt.Pattern() # Create the Pattern
        pattern.pattern = xlwt.Pattern.SOLID_PATTERN # May be: NO_PATTERN, SOLID_PATTERN, or 0x00 through 0x12
        pattern.pattern_fore_colour = 2 # May be: 8 through 63. 0 = Black, 1 = White, 2 = Red, 3 = Green, 4 = Blue, 5 = Yellow, 6 = Magenta, 7 = Cyan, 16 = Maroon, 17 = Dark Green, 18 = Dark Blue, 19 = Dark Yellow , almost brown), 20 = Dark Magenta, 21 = Teal, 22 = Light Gray, 23 = Dark Gray, the list goes on...
        xlstyle.pattern = pattern # Add Pattern to Style
    
    # borders= xlwt.Borders() 
    # borders.left= 6 
    # borders.right= 6 
    # borders.top= 6 
    # borders.bottom= 6 
    
    # xlstyle.borders = borders 
    
    return xlstyle 

def gen_xls(fr_result, fa_result, output_filename):
    header_style = style('Times New Roman', 220, False, True)
    normal_style = style('Times New Roman', 220, False, False)
    highlight_style = style('Times New Roman', 220, True, False)
    use_style = None
    f = xlwt.Workbook() #创建工作簿 
    fa_header = [u'catagory', u'id', u'serial_no', u'match_catagory', u'match_id', u'path', u'enroll_path']
    sheet = f.add_sheet(u'far test', cell_overwrite_ok=True) #创建sheet 
    row = 0
    col = 0
    for i in fa_header:
        sheet.write(row, col, fa_header[col], header_style)
        col += 1

    row += 1
    col = 0

    pre_line = None
    for i in fa_result:
        #copy_file_from_far(i.path, os.path.splitext(output_filename)[0], i.new_catagory, i.new_id, i.image_catagory, i.image_id)
        l = [i.image_catagory, i.image_id, i.image_serial_no, i.new_catagory, i.new_id, i.path]
        l.extend(i.enroll_list)
        for value in l:
            highlight = False
            if pre_line:
                if len(l) > col and len(pre_line) > col:
                    if l[col] != pre_line[col]:
                        highlight = True
            if highlight:
                use_style = highlight_style
            else:
                use_style = normal_style
            sheet.write(row, col, value.decode('utf-8'), use_style)
            col += 1
        row += 1
        col = 0
        pre_line = l

    fr_header = [u'catagory', u'id', u'serial_no', u'match_catagory', u'match_id', u'update_catagory', u'update_id', u'path']
    sheet = f.add_sheet(u'frr test', cell_overwrite_ok=True) #创建sheet 
    row = 0
    col = 0
    for i in fr_header:
        sheet.write(row, col, fr_header[col], header_style)
        col += 1

    row += 1
    col = 0

    for i in fr_result:
        l = [i.image_catagory, i.image_id, i.image_serial_no, i.new_catagory, i.new_id, i.update_catagory, i.update_id, i.path]
        for value in l:
            highlight = False

            if col == 0 or col == 1 or col == 3 or col == 4:
                if l[0] == l[3] and l[1] == l[4]:
                    pass
                else:
                    if l[3] and l[4]:
                        highlight = True
                    else:
                        if i.matched_state_changed:
                            highlight = True

            if col == 5 or col == 6:
                if l[5] and l[6]:
                    highlight = True

            if highlight:
                use_style = highlight_style
            else:
                use_style = normal_style
            sheet.write(row, col, value.decode('utf-8'), use_style)
            col += 1
        row += 1
        col = 0
    if not output_filename:
        now = datetime.datetime.now()
        output_filename = 'parse_result_%04d%02d%02d%02d%02d%02d_%06d.xls' %(now.year, now.month, now.day, now.hour, now.minute, now.second, now.microsecond)
    f.save(output_filename) #保存文件 

def get_filelist(dirname, ext_list):
    filelist = []
    for dir_name, sub_dir_list, sub_filelist in os.walk(dirname):
        for i in sub_filelist:
            if len(ext_list):
                if os.path.splitext(i)[1] in ext_list:
                    filelist.append(os.path.join(dir_name, i))
            else:
                filelist.append(os.path.join(dir_name, i))

    return filelist

def list_far_top_by_catagory(fa_result):
    print('top count:')
    number = sys.stdin.readline()
    number = number.strip()

    try:
        number = int(number)
    except:
        number = None

    map_catagory_farcount = {}
    for i in fa_result:
        catagory = i.image_catagory
        count = map_catagory_farcount.get(catagory)
        if not count:
            count = 1
        else:
            count += 1
        map_catagory_farcount.update({catagory : count})

    list_catagory_count = []
    for i in map_catagory_farcount.items():
        list_catagory_count.append(i)

    def list_catagory_count_key(x):
        return x[1]
    list_catagory_count = sorted(list_catagory_count, key = lambda x : list_catagory_count_key(x))

    list_catagory_count.reverse()

    catagory_width = 0
    count_width = 0
    max_count = 0
    for i in list_catagory_count[:number]:
        if max_count < i[1]:
            max_count = i[1]

        width = len(i[0].decode('utf-8'))
        width *= 2
        if catagory_width < width:
            catagory_width = width

        width = len(str(i[1]))
        if count_width < width:
            count_width = width

    bar_width = 60
    for i in list_catagory_count[:number]:
        filed_size = bar_width * i[1] / max_count
        bar = '█' * filed_size
        bar += ' ' * (bar_width - filed_size)
        catagory = i[0]
        count = i[1]

        print('%*d %s %s' %(count_width, count, bar, catagory))

def list_far_top_by_id_for_catagory(fa_result, args):
    print(args)

def interactive_mode(fr_result, fa_result):
    need_xls = False
    tips = ''' 
functions:
0.exit
1.create_xls_and_exit
2.list_far_top_by_catagory
3.list_far_top_by_id_for_catagory
>>>'''

    while True:
        sys.stdout.write(tips)
        line = sys.stdin.readline()
        line = line.strip()
        command = line.split()
        if not len(command):
            continue

        cmd = command[0]

        if cmd == '0':
            break
        elif cmd == '1':
            need_xls = True
            break
        elif cmd == '2':
            list_far_top_by_catagory(fa_result)
        elif cmd == '3':
            list_far_top_by_id_for_catagory(fa_result)
    
    return need_xls

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-d', '--dir', action='store', dest='report_directory', help='report directory', default=os.curdir)
    options.add_option('-o', '--output-filename', action='store', dest='output_filename', help='output_filename', default=None)
    options.add_option('-i', '--interactive', action='store_false', dest='interactive', help='enable interactive mode', default=True)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args) or not os.access(opts.report_directory, os.F_OK):
        options.print_help()
        return

    filelist = get_filelist(opts.report_directory, ['.log'])
    fr_result, fa_result = parse_log_list(filelist)

    need_xls = True
    if opts.interactive:
        need_xls = interactive_mode(fr_result, fa_result)
   
    if need_xls:
        def fa_result_key(x):
            image_catagory = x.image_catagory
            image_id = x.image_id
            image_serial_no = x.image_serial_no
            new_catagory = x.new_catagory
            new_id = x.new_id
            return new_catagory, int(new_id), image_catagory, int(image_id), int(image_serial_no)

        fa_result = sorted(fa_result, key = lambda x : fa_result_key(x))
        gen_xls(fr_result, fa_result, opts.output_filename)

if '__main__' == __name__:
    main()

# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月26日 肖飞 All rights reserved
#   
#   文件名称：log_parse.py
#   创 建 者：肖飞
#   创建日期：2017年07月26日 星期三 09时11分14秒
#   修改日期：2017年07月26日 星期三 12时02分27秒
#   描    述：
#
#================================================================
import sys
import datetime
import xlwt

class result_info(object):
    image_catagory = ''
    image_id = ''
    image_serial_no = ''
    new_catagory = ''
    new_id = ''
    path = ''
    enroll_list = []

    def __init__(self, image_catagory, image_id, image_serial_no, new_catagory, new_id, path, enroll_list):
        self.image_catagory = image_catagory
        self.image_id = image_id
        self.image_serial_no = image_serial_no
        self.new_catagory = new_catagory
        self.new_id = new_id
        self.path = path
        self.enroll_list = enroll_list[:]

def parse_log_list(filelist):
    content = []
    fr_result = []
    fa_result = []
    map_enroll_catagory_map_id_path_list = {}
    mode = ''
    for i in filelist:
        with open(i) as f:
            content = f.read().splitlines()
        for line in content:
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
                catagory_info, id_info, serial_no_info, match_state_info, new_match_state_info, new_matched_catagory_info, new_matched_id_info, update_template_catagory_info,  update_template_if_info, path_info, ret_info = fields
                if mode != catagory_info.split(':')[0]:
                    mode = catagory_info.split(':')[0]
                if mode == 'fa':
                    if new_match_state_info.split(':')[1] == 'MATCHED':
                        image_catagory = catagory_info.split(':')[2]
                        image_id = id_info.split(':')[1]
                        image_serial_no = serial_no_info.split(':')[1]
                        new_catagory = new_matched_catagory_info.split(':')[1]
                        new_id = new_matched_id_info.split(':')[1]
                        path = path_info.split(':')[1]
                        enroll_list = map_enroll_catagory_map_id_path_list.get(new_catagory).get(new_id)
                        fa_result_item = result_info(image_catagory, image_id, image_serial_no, new_catagory, new_id, path, enroll_list)
                        fa_result.append(fa_result_item)
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

def gen_xls(fr_result, fa_result):
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
    now = datetime.datetime.now()
    f.save('parse_result_%04d%02d%02d%02d%02d%02d_%06d.xls' %(now.year, now.month, now.day, now.hour, now.minute, now.second, now.microsecond)) #保存文件 

def main():
    argv = sys.argv[1:]
    fr_result, fa_result = parse_log_list(argv)
    gen_xls(fr_result, fa_result)

if '__main__' == __name__:
	main()

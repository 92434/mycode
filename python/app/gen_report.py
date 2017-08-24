# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年07月21日 肖飞 All rights reserved
#   
#   文件名称：gen_report.py
#   创 建 者：肖飞
#   创建日期：2017年07月21日 星期五 11时49分06秒
#   修改日期：2017年08月23日 星期三 14时28分52秒
#   描    述：
#
#================================================================
import os
import sys
import optparse
import xlwt

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

def build_database_for_report(directory):
    filelist = get_filelist(directory, ['.log'])
    map_database_list = []
    try:
        filelist.sort(key = lambda x : os.path.basename(os.path.dirname(x)))
    except:
        filelist.sort()
    for i in filelist:
        content_list = []
        if os.path.basename(i) == "report.log":
            with open(i) as f:
                for j in f:
                    map_database = {}
                    key_value_list = j.strip().split(',')
                    for key_value in key_value_list:
                        l = key_value.strip().split(':')
                        if len(l) == 2:
                            map_database.update({l[0] : l[1]})
                    map_database_list.append(map_database)
    return map_database_list

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

def gen_xls_from_map_database_list(map_database_list):
    f = xlwt.Workbook() #创建工作簿 
    header = [u'configuration_name', u'位置', u'图片目录', u'允许录入的手指数', u'允许录入人数', u'fr比对选择类型', u'fa比对选择类型', u'模拟手机数', u'结果严格接id计入', u'pb 库版本', u'固件版本', u'ft库版本', u'debug_switch', u'algorithm_mode', u'enroll_max_templates', u'algorithm_max_templates', u'spa_enable', u'spa_ratio', u'algorithm_far_level', u'update_template_far_level', u'update_template_threshold', u'verify_quickly_enable', u'update_template_outside_enable', u'image_quality_score', u'verify_image_quality_score', u'enroll_duplicate_area_check_enable', u'valid_area_scale', u'enrollment_tips_enable', u'enrollment_tips_parameter1', u'enrollment_tips_parameter2', u'enrollment_tips_parameter3', u'verify_improve_enable', u'verify_improve_level', u'mcu_image_bit', u'mcu_interrupt_mode', u'mcu_state_check_mode', u'repeat_get_image_count', u'template_buffer_enable', u'transfer_bytes_max', u'config_debuginfo_switch', u'use_dyn_far', u'fr result', u'frr', u'fa result', u'far']
    keys = ['configuration_name', 'log_dirname', 'pictures_directory', 'max_number_of_id_per_proc', 'max_number_of_catagory_per_proc', 'fr_select_type', 'fa_select_type', 'max_proc_number', 'strict_identify_mode', 'pb_library_version', 'firmware_version', 'ft_lib_verision', 'debug_switch', 'algorithm_mode', 'enroll_max_templates', 'algorithm_max_templates', 'spa_enable', 'spa_ratio', 'algorithm_far_level', 'update_template_far_level', 'update_template_threshold', 'verify_quickly_enable', 'update_template_outside_enable', 'image_quality_score', 'verify_image_quality_score', 'enroll_duplicate_area_check_enable', 'valid_area_scale', 'enrollment_tips_enable', 'enrollment_tips_parameter1', 'enrollment_tips_parameter2', 'enrollment_tips_parameter3', 'verify_improve_enable', 'verify_improve_level', 'mcu_image_bit', 'mcu_interrupt_mode', 'mcu_state_check_mode', 'repeat_get_image_count', 'template_buffer_enable', 'transfer_bytes_max', 'config_debuginfo_switch', 'use_dyn_far', 'fr_result', 'frr', 'fa_result', 'far']
    map_sheet = {}
    map_sheet_info_list = {}
    map_pre_line_list = {}
    use_style = None
    header_style = style('Times New Roman', 220, False, True)
    normal_style = style('Times New Roman', 220, False, False)
    highlight_style = style('Times New Roman', 220, True, False)
    for map_database in map_database_list:
        test_type = map_database.get('test_type', None)
        if test_type:
            index = 0

            info_list = map_sheet_info_list.get(test_type, None)
            if not info_list:
                info_list = [0, 0]
                map_sheet_info_list.update({test_type:info_list})

            row = info_list[0]
            col = info_list[1]

            pre_line_list = map_pre_line_list.get(test_type, None)
            if not pre_line_list:
                pre_line_list = []
                
            sheet = map_sheet.get(test_type, None)
            if not sheet:
                sheet = f.add_sheet(test_type.decode('utf-8'), cell_overwrite_ok=True) #创建sheet 
                map_sheet.update({test_type : sheet})

                index = 0
                for i in header:
                    sheet.write(row + index, col, header[index], header_style)
                    index += 1
                row = 0
                col += 1

            index = 0
            current_line_list = []
            for key in keys:
                highlight = False
                value = map_database.get(key, u'N/A').decode('utf-8')
                current_line_list.append(value)
                if len(pre_line_list):
                    pre_value = pre_line_list[index]
                    if value != pre_value:
                        highlight = True
                if highlight:
                    use_style = highlight_style
                else:
                    use_style = normal_style

                sheet.write(row + index, col, value, use_style)
                index += 1
            row = 0
            col += 1
            info_list = [row, col]
            map_sheet_info_list.update({test_type:info_list})
            map_pre_line_list.update({test_type:current_line_list})
    f.save('report.xls') #保存文件 

def main():
    argv = sys.argv[1:]
    options = optparse.OptionParser()
    options.add_option('-d', '--dir', action='store', dest='report_directory', help='report directory', default=os.curdir)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    map_database_list = build_database_for_report(opts.report_directory)
    gen_xls_from_map_database_list(map_database_list)

if '__main__' == __name__:
	main()

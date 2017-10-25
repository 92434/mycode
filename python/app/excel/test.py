# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年10月24日 肖飞 All rights reserved
#   
#   文件名称：test.py
#   创 建 者：肖飞
#   创建日期：2017年10月24日 星期二 11时12分05秒
#   修改日期：2017年10月24日 星期二 11时34分59秒
#   描    述：
#
#================================================================
from collections import OrderedDict  
from pyexcel_xls import get_data  
from pyexcel_xls import save_data  
import log
logging = log.dict_configure()
logger = logging.getLogger('default')
  
def read_xls_file():  
    xls_data = get_data(unicode(r"1.xls", "utf-8"))  
    for sheet_n in xls_data.keys():  
        level = 0
        logger.debug("%ssheet:%s content:%s", '\t' * level, sheet_n, xls_data[sheet_n])   
        for i in xls_data[sheet_n]:
            level = 1
            logger.debug("%si:%s", '\t' * level, i)   
            for j in i:
                level = 2
                logger.debug("%sj:%s", '\t' * level, j)   
                if not isinstance(j, list):
                    continue
                for k in j:
                    level = 3
                    logger.debug("%sk:%s", '\t' * level, k)   

    return xls_data  
  
  
# 写Excel数据, xls格式  
def save_xls_file():  
    data = OrderedDict()  
    # sheet表的数据  
    sheet_1 = []  
    row_1_data = [u"ID", u"昵称", u"等级"]   # 每一行的数据  
    row_2_data = [4, 5, 6]  
    # 逐条添加数据  
    sheet_1.append(row_1_data)  
    sheet_1.append(row_2_data)  
    # 添加sheet表  
    data.update({u"这是XX表": sheet_1})  
  
    # 保存成xls文件  
    save_data("1.xls", data)  

def main():
    read_xls_file()

if '__main__' == __name__:
    main()

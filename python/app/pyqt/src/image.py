# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月07日 肖飞 All rights reserved
#   
#   文件名称：image.py
#   创 建 者：肖飞
#   创建日期：2017年12月07日 星期四 18时09分36秒
#   修改日期：2017年12月08日 星期五 16时01分25秒
#   描    述：
#
#================================================================
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QListWidget, QListWidgetItem
import sys

def get_csv_data(index):
    d = {}
    with open('睡眠唤醒OK.csv') as f:
        for i in f:
            t, pid, odata, idata = i.strip().split(',')
            l = d.get(pid)
            if not l:
                l = []
            data = (odata, idata)
            l.append(data)
            d.update({pid : l})
    for i in d:
        l = d.get(i)
        #print('pid:%s, len:%s' %(i, len(l)))
        if len(l) > 1024:
            print('pid:%s, len:%s' %(i, len(l)))
            if i == '208':
                l = l[1 : -4]
                print('len:%s' %(len(l)))
                #for j in range(0, len(l), 6):
                #    da, db, dc, dd, de, df = l[j][1], l[j+1][1], l[j+2][1], l[j+3][1], l[j+4][1], l[j+5][1]
                #    da = chr(int(da, 16))
                #    db = chr(int(db, 16))
                #    dc = chr(int(dc, 16))
                #    dd = chr(int(dd, 16))
                #    de = chr(int(de, 16))
                #    df = chr(int(df, 16))

                #    d1 = da
                #    d2 = dc
                #    d3 = de
                #    d4 = chr(((ord(db) & 0xf0) + ((ord(dd) & 0xf0) << 4)) & 0xff)
                #    yield d1, d2, d3, d4


                for j in range(0, len(l), 3):
                    da, db, dc = l[j][1], l[j+1][1], l[j+2][1]
                    
                    xor = 0x55
                    #xor = 0xaa
                    da = chr(int(da, 16) ^ xor)
                    db = chr(int(db, 16) ^ xor)
                    dc = chr(int(dc, 16) ^ xor)

                    da_h = chr((ord(da) & 0xf0) >> 4)
                    da_l = chr((ord(da) & 0x0f))

                    db_h = chr((ord(db) & 0xf0) >> 4)
                    db_l = chr((ord(db) & 0x0f))

                    dc_h = chr((ord(dc) & 0xf0) >> 4)
                    dc_l = chr((ord(dc) & 0x0f))

                    ld = [da_l, da_h, db_l, db_h, dc_l, dc_h]

                    d1 = chr(ord(da_l) + (ord(da_h) << 4))
                    d2 = chr(ord(db_h) + (ord(dc_l) << 4))

                    #d1 = chr((ord(ld[index]) << 4))
                    #d2 = chr((ord(ld[index]) << 4))

                    #print('%s %s' %(hex(ord(d1)), hex(ord(d2))))

                    yield d1, d2

def main():
    app = QApplication(sys.argv)
    list_widget = QListWidget()

    for i in range(0, 6):
        s = ''
        for i1, i2 in get_csv_data(i):
            s += i1 + i2
        print('len:%d', len(s))

        #item = QListWidgetItem(list_widget)
        #image = QImage(s, 112, 132, QImage.Format_Grayscale8)
        #widget = QWidget()
        #layout = QVBoxLayout()
        #label = QLabel()
        #label.setPixmap(QPixmap.fromImage(image))
        #layout.addWidget(label)
        #widget.setLayout(layout)
        #item.setSizeHint(widget.sizeHint());
        #list_widget.setItemWidget(item, widget);

        item = QListWidgetItem(list_widget)
        image = QImage(s, 132, 112, QImage.Format_Grayscale8)
        widget = QWidget()
        layout = QVBoxLayout()
        label = QLabel()
        label.setText(str(i))
        layout.addWidget(label)

        label = QLabel()
        label.setPixmap(QPixmap.fromImage(image))
        layout.addWidget(label)
        widget.setLayout(layout)
        item.setSizeHint(widget.sizeHint());
        list_widget.setItemWidget(item, widget);

    list_widget.show()
    app.exec_()

if '__main__' == __name__:
    main()

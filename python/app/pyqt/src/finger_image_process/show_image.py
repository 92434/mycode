# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年02月07日 肖飞 All rights reserved
#   
#   文件名称：show_image.py
#   创 建 者：肖飞
#   创建日期：2018年02月07日 星期三 10时19分05秒
#   修改日期：2018年02月07日 星期三 14时23分44秒
#   描    述：
#
#================================================================
import io
import os
import optparse
import sys

from PyQt5.QtGui import QFont, QBrush, QColor, QImage, QPixmap
from PyQt5.QtWidgets import QApplication, QListWidget, QTableWidget, QListWidgetItem, QTableWidgetItem, QWidget, QHBoxLayout, QVBoxLayout, QLabel

import bmp_file

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-l', '--list_file', dest='list_file', help='list_file', default=None)
    options.add_option('-p', '--position', type='int', dest='position', help='position', default=None)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return
    if not opts.list_file:
        options.print_help()
        return
    if opts.position == None:
        options.print_help()
        return
    
    list_file = []
    f = open(opts.list_file, 'r')
    for i in f:
        filepath = i.strip()
        if filepath:
            list_file.append(filepath)
    f.close()

    app = QApplication(argv)
    list_widget = QListWidget()

    for filepath in list_file:
        if not os.path.exists(filepath):
            continue

        bmp = bmp_file.bmp_file(96, 96)
        bmp.read_bmp(filepath)

        #xor1 = 0x9f
        #xor2 = 0x70
        xor1 = 0x00
        xor2 = 0x00
        data = ''
        f = io.BytesIO(bmp.data)
        for h in range(0, bmp.height):
            for w in range(0, bmp.width):
                c = f.read(1)
                if w % 2 == 0:
                    xor = xor1
                else:
                    xor = xor2
                c = chr(ord(c) ^ xor)

                c_8 = chr(0)
                c_7 = chr((ord(c) >> 7) & 0x01)
                c_6 = chr((ord(c) >> 6) & 0x01)
                c_5 = chr((ord(c) >> 5) & 0x01)
                c_4 = chr((ord(c) >> 4) & 0x01)
                c_3 = chr((ord(c) >> 3) & 0x01)
                c_2 = chr((ord(c) >> 2) & 0x01)
                c_1 = chr((ord(c) >> 1) & 0x01)
                c_0 = chr((ord(c) >> 0) & 0x01)

                l = [
                    c_0,
                    c_1,
                    c_2,
                    c_3,
                    c_4,
                    c_5,
                    c_6,
                    c_7,
                    c_8,
                ]

                #c = chr(
                #        (ord(l[opts.position]) << 7) +
                #        (ord(l[opts.position]) << 6) +
                #        (ord(l[opts.position]) << 5) +
                #        (ord(l[opts.position]) << 4) +
                #        (ord(l[opts.position]) << 3) +
                #        (ord(l[opts.position]) << 2) +
                #        (ord(l[opts.position]) << 1) +
                #        (ord(l[opts.position]) << 0)
                #        )

                #c = chr(
                #        (ord(c_0) << 7) +
                #        (ord(c_2) << 6) +
                #        (ord(c_3) << 5) +
                #        (ord(c_1) << 4) +
                #        (ord(c_4) << 3) +
                #        (ord(c_6) << 2) +
                #        (ord(c_7) << 1) +
                #        (ord(c_5) << 0)
                #        )

                c = chr(
                        (ord(c_0) << 7) +
                        (ord(c_2) << 6) +
                        (ord(c_3) << 5) +
                        (ord(c_1) << 4) +
                        (ord(c_5) << 3) +
                        (ord(c_7) << 2) +
                        (ord(c_6) << 1) +
                        (ord(c_4) << 0)
                        )

                data += c
        f.close()

        filepath = '%s_decode' %(os.path.splitext(filepath)[0])
        bmp.write_bmp(filepath, data)

        table = QTableWidget(bmp.height, bmp.width)
        font = QFont()
        font.setFamily("Calibri")
        font.setPointSize(6)
        table.setFont(font)

        table.setEditTriggers(QTableWidget.NoEditTriggers) 
        table.verticalHeader().setVisible(False)
        table.horizontalHeader().setVisible(False)

        f = io.BytesIO(data)
        for h in range(0, bmp.height):
            for w in range(0, bmp.width):
                c = f.read(1)

                item = QTableWidgetItem()
                item.setText('%02x' %(ord(c)))
                item.setBackground(QBrush(QColor(ord(c), ord(c), ord(c))))
                table.setItem(h, w, item)
        f.close()

        for c in range(0, bmp.width):
            table.setColumnWidth(c, 18)

        for r in range(0, bmp.height):
            table.setRowHeight(r, 18)

        item = QListWidgetItem(list_widget)
        item.setSizeHint(table.sizeHint());
        list_widget.setItemWidget(item, table);

        item = QListWidgetItem(list_widget)
        image = QImage(data, bmp.width, bmp.height, QImage.Format_Grayscale8)
        pixmap = QPixmap.fromImage(image)
        widget = QWidget()
        layout = QVBoxLayout()

        label = QLabel()
        label.setText(filepath)
        layout.addWidget(label)

        label = QLabel()
        label.setPixmap(pixmap)
        layout.addWidget(label)
        widget.setLayout(layout)
        item.setSizeHint(widget.sizeHint());
        list_widget.setItemWidget(item, widget);

    list_widget.show()
    app.exec_()

if '__main__' == __name__:
    main(sys.argv[1:])

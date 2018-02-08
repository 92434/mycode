# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年02月07日 肖飞 All rights reserved
#   
#   文件名称：show_image.py
#   创 建 者：肖飞
#   创建日期：2018年02月07日 星期三 10时19分05秒
#   修改日期：2018年02月08日 星期四 15时22分58秒
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

def get_image_data(filepath):
    bmp = bmp_file.bmp_file()
    bmp.read_bmp(filepath)
    return bmp

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-l', '--list_file', dest='list_file', help='list_file', default=None)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return
    if not opts.list_file:
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

        bmp = get_image_data(filepath)
        data = bmp.data

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

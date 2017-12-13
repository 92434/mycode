# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月07日 肖飞 All rights reserved
#   
#   文件名称：image.py
#   创 建 者：肖飞
#   创建日期：2017年12月07日 星期四 18时09分36秒
#   修改日期：2017年12月13日 星期三 16时50分52秒
#   描    述：
#
#================================================================
from PyQt5.QtCore import QSize
from PyQt5.QtGui import QImage, QPixmap, QBrush, QColor, QPalette, QFont
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QHBoxLayout, QListWidget, QListWidgetItem, QTableWidget, QTableWidgetItem, QHeaderView

import io
import optparse
import sys

import image_file
import image_parse

def main(argv):
    options = optparse.OptionParser()
    options.add_option('-f', '--file', dest='file', help='file', default=None)
    options.add_option('-p', '--pid', dest='pid', help='file', default=None)
    opts, args = options.parse_args(argv)
    print('opts:%s' %(opts))
    print('args:%s' %(args))
    if len(args):
        options.print_help()
        return

    if not opts.file:
        options.print_help()
        return

    app = QApplication(argv)
    list_widget = QListWidget()
    database = image_parse.image_database()

    for i in range(0, 7):
        if i == 6:
            table_column = 132
        else:
            table_column = 132 / 2

        s = ''
        for i1, i2 in database.gen_pixel_bytes(opts.file, opts.pid, i):
            s += i1 + i2

        print('len(s):%s' %(len(s)))

        if len(s) != 14784:
            continue

        table = QTableWidget(112, table_column)
        font = QFont()
        font.setFamily("Calibri")
        font.setPointSize(6)
        table.setFont(font)

        table.setEditTriggers(QTableWidget.NoEditTriggers) 
        #table.verticalHeader().setVisible(False)
        #table.horizontalHeader().setVisible(False)

        #widget = QWidget()
        #vlayout = QVBoxLayout()
        f = io.BytesIO(s)
        #bf = open('%s.csv' %(opts.pid), 'wb+')
        for h in range(0, 112):
            #hlayout = QHBoxLayout()
            bf_line = ''
            for w in range(0, table_column):
                if i == 6:
                    c = f.read(1)
                else:
                    c = f.read(1)
                    c = f.read(1)
                item = QTableWidgetItem()
                if i == 6:
                    item.setText('%02x' %(ord(c)))
                else:
                    if i % 2 == 0:
                        item.setText('%02x' %((ord(c) & 0xf0) >> 4))
                    else:
                        item.setText('%02x' %((ord(c) & 0xf0)))

                #item.setSizeHint(QSize(25, 25));
                item.setBackground(QBrush(QColor(ord(c), ord(c), ord(c))))
                table.setItem(h, w, item)

                #item = QLabel()
                #item.setText('%02x' %(ord(c)))
                #pe = QPalette()  
                #pe.setColor(QPalette.Background, QColor(ord(c), ord(c), ord(c)))  
                #item.setPalette(pe)
                #item.setAutoFillBackground(True)
                #hlayout.addWidget(item)

                #bf_line += hex(ord(c)) + ','


            #vlayout.addItem(hlayout)

            #bf_line += '\n'
            #bf.write(bf_line)

        #widget.setLayout(vlayout)
        #widget.show()

        #bf.close()
        for c in range(0, table_column):
            table.setColumnWidth(c, 18)

        for r in range(0, 112):
            table.setRowHeight(r, 18)

        #table.resizeColumnsToContents()
        #table.resizeRowsToContents()
        #table.show()
        item = QListWidgetItem(list_widget)
        item.setSizeHint(table.sizeHint());
        list_widget.setItemWidget(item, table);

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
        pixmap = QPixmap.fromImage(image)
        #pixmap.save('%d.bmp' %(i))
        img = image_file.image_file(132, 112)
        img.write_bmp('%s-%s-%s' %(opts.file, opts.pid, i), s)
        widget = QWidget()
        layout = QVBoxLayout()
        label = QLabel()
        label.setText(str(i))
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

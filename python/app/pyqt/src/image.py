# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月07日 肖飞 All rights reserved
#   
#   文件名称：image.py
#   创 建 者：肖飞
#   创建日期：2017年12月07日 星期四 18时09分36秒
#   修改日期：2017年12月12日 星期二 19时39分42秒
#   描    述：
#
#================================================================
from PyQt5.QtCore import QSize
from PyQt5.QtGui import QImage, QPixmap, QBrush, QColor, QPalette, QFont
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QHBoxLayout, QListWidget, QListWidgetItem, QTableWidget, QTableWidgetItem

import io
import optparse
import sys
import time


header1 = \
	[
		0x42, 0x4d,  # //file type
		0x0, 0x0, 0x0, 0x00,  # //file size***
		0x00, 0x00,  # //reserved
		0x00, 0x00,  # //reserved
		0x36, 0x4, 0x00, 0x00,  # //head byte***
		##//infoheader
		0x28, 0x00, 0x00, 0x00,  # //struct size
	]

header2 = \
	[
		0x01, 0x00,  # //must be 1
		0x08, 0x00,  # //color count
		0x00, 0x00, 0x00, 0x00,  # //compression
		0x00, 0x00, 0x00, 0x00,  # //data size***
		0x00, 0x00, 0x00, 0x00,  # //dpix
		0x00, 0x00, 0x00, 0x00,  # //dpiy
		0x00, 0x00, 0x00, 0x00,  # //color used
		0x00, 0x00, 0x00, 0x00,  # //color important
	]

class FingerprintImageFile():
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def addSizeInfo2FileHeader(self,buf):
        for i in range(4):
            buf.append((self.width >> (8 * i)) & 0xFF)
        for i in range(4):
            buf.append((self.height >> (8 * i)) & 0xFF)
        return buf

    def addCLUT2FileHeader(self,buf):
        for i in range(256):
            buf.append(i)  # r
            buf.append(i)  # g
            buf.append(i)  # b
            buf.append(0)  # a
        return buf

    def generateBmp(self,buf):
        img_buf = header1[:]
        print('bmp buffer len:%d' %(len(buf)))
        # add size
        img_buf = self.addSizeInfo2FileHeader(img_buf)
        print img_buf

        img_buf = img_buf + header2

        # fill color look-up table
        img_buf = self.addCLUT2FileHeader(img_buf)
        
        # add data
        for v in buf:
            img_buf.append(ord(v))

        return img_buf

    def writeFile2System(self,buf, index):
        img_buf = self.generateBmp(buf)
        self.writeFile2FileSystem(img_buf, index)
    
    def writeFile2FileSystem(self,img_buf, index):
        curTime = time.localtime(time.time())
        fileName = time.strftime('%Y-%m-%d-%H-%M-%S',curTime)

        fileName = "%s.bmp" %(index)

        fBmp = open(fileName, 'wb+')
        strBmp = ''
        for v in img_buf:
            strBmp += chr(v)
        print ord(strBmp[1098])

        fBmp.write(strBmp)
        fBmp.close()    

def get_csv_data(csv_file, index, package_id = None):
    d = {}
    with open(csv_file) as f:
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
        if len(l) > 1024:
            print('pid:%s, len:%s' %(i, len(l)))
            if package_id and i == package_id:
                l = l[1 : -4]
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
                    #xor = 0xff
                    #xor = 0x00
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
                    #d1 = chr(ord(da_l) + (ord(da_h) << 4))
                    #d2 = chr(ord(dc_h) + (ord(db_l) << 4))

                    #d1 = chr((ord(ld[index]) << 4))
                    #d2 = chr((ord(ld[index]) << 4))

                    #print('%s %s' %(hex(ord(d1)), hex(ord(d2))))

                    yield d1, d2

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

    for i in range(0, 1):
        s = ''
        for i1, i2 in get_csv_data(opts.file, i, opts.pid):
            s += i1 + i2

        print('len(s):%s' %(len(s)))

        if len(s) != 14784:
            continue

        #table = QTableWidget(132, 112)
        #font = QFont()
        #font.setFamily("Calibri")
        #font.setPointSize(6)
        #table.setFont(font)

        #table.setEditTriggers(QTableWidget.NoEditTriggers) 

        #widget = QWidget()
        #vlayout = QVBoxLayout()
        f = io.BytesIO(s)
        with open('%s.csv' %(opts.pid), 'wb+') as bf:
            for h in range(0, 112):
                #hlayout = QHBoxLayout()
                bf_line = ''
                for w in range(0, 132):
                    c = f.read(1)
                    #item = QTableWidgetItem('%02x' %(ord(c)))
                    #item.setSizeHint(QSize(5, 5));
                    #item.setBackground(QBrush(QColor(ord(c), ord(c), ord(c))))

                    #table.setItem(h, w, item)
                    #item = QLabel()
                    #item.setText('%02x' %(ord(c)))
                    #pe = QPalette()  
                    #pe.setColor(QPalette.Background, QColor(ord(c), ord(c), ord(c)))  
                    #item.setPalette(pe)
                    #item.setAutoFillBackground(True)
                    #hlayout.addWidget(item)

                    bf_line += hex(ord(c)) + ','
                #vlayout.addItem(hlayout)

                bf_line += '\n'
                bf.write(bf_line)
        #table.resizeColumnsToContents()
        #table.resizeRowsToContents()
        #table.show()
        #widget.setLayout(vlayout)
        #widget.show()

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
        img = FingerprintImageFile(132, 112)
        img.writeFile2System(s, opts.pid)
        #pixmap.save('%d.bmp' %(i))
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

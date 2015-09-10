# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created: Thu Jul  2 11:47:28 2015
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(332, 202)
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.pushButton = QtGui.QPushButton(self.centralWidget)
        self.pushButton.setGeometry(QtCore.QRect(110, 50, 98, 31))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 332, 27))
        self.menuBar.setObjectName(_fromUtf8("menuBar"))
        self.menuMenu = QtGui.QMenu(self.menuBar)
        self.menuMenu.setObjectName(_fromUtf8("menuMenu"))
        self.menuSubmenu = QtGui.QMenu(self.menuMenu)
        font = QtGui.QFont()
        font.setBold(False)
        font.setWeight(50)
        self.menuSubmenu.setFont(font)
        #self.menuSubmenu.setToolTipsVisible(True)
        self.menuSubmenu.setObjectName(_fromUtf8("menuSubmenu"))
        MainWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QtGui.QToolBar(MainWindow)
        self.mainToolBar.setObjectName(_fromUtf8("mainToolBar"))
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QtGui.QStatusBar(MainWindow)
        self.statusBar.setObjectName(_fromUtf8("statusBar"))
        MainWindow.setStatusBar(self.statusBar)
        self.actionItem1 = QtGui.QAction(MainWindow)
        self.actionItem1.setObjectName(_fromUtf8("actionItem1"))
        self.menuMenu.addAction(self.menuSubmenu.menuAction())
        self.menuMenu.addAction(self.actionItem1)
        self.menuBar.addAction(self.menuMenu.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.pushButton.setText(_translate("MainWindow", "b1", None))
        self.menuMenu.setTitle(_translate("MainWindow", "menu", None))
        self.menuSubmenu.setTitle(_translate("MainWindow", "submenu", None))
        self.actionItem1.setText(_translate("MainWindow", "item1", None))


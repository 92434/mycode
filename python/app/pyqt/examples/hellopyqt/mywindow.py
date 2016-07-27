from PyQt4 import QtGui, QtCore
# -*- coding: utf-8 -*-

from window import Ui_MainWindow
class Widget(QtGui.QMainWindow, Ui_MainWindow):
    """QtGui.QMainWindow和界面设计时选择的类型一致"""
    def __init__(self, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.setupUi(self) # Ui_Form.setupUi

    def on_pushButton_clicked(self):
        print('button clicked')


if __name__ == '__main__':
    import sys
    app = QtGui.QApplication(sys.argv)
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())

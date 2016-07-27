#coding=utf-8
import sys
from PyQt4 import QtGui, QtCore
class Window( QtGui.QWidget ):
    def __init__( self ):
        super( Window, self ).__init__()
        self.setWindowTitle( "hello" )
        self.resize( 500, 500 )
         
        gridlayout = QtGui.QGridLayout()
         
        self.button1 = QtGui.QPushButton( "File" )
        self.button2 = QtGui.QPushButton( "Font" )
        self.button3 = QtGui.QPushButton( "Color" )
        gridlayout.addWidget( self.button1 )
        gridlayout.addWidget( self.button2 )
        gridlayout.addWidget( self.button3 )
        spacer = QtGui.QSpacerItem( 200, 80 )
        gridlayout.addItem( spacer, 3, 1, 1, 3 )
        self.setLayout( gridlayout )
         
        self.connect( self.button1, QtCore.SIGNAL( 'clicked()' ), self.OnButton1 )
        self.connect( self.button2, QtCore.SIGNAL( 'clicked()' ), self.OnButton2 )
        self.connect( self.button3, QtCore.SIGNAL( 'clicked()' ), self.OnButton3 )
         
         
    def OnButton1( self ):
        fileName = QtGui.QFileDialog.getOpenFileName( self, 'Open' )
        if not fileName.isEmpty():
            self.setWindowTitle( fileName )    
         
    def OnButton2( self ):
        font, ok = QtGui.QFontDialog.getFont()
        if ok:
            self.setWindowTitle( font.key() )
     
    def OnButton3( self ):
        color = QtGui.QColorDialog.getColor()
        if color.isValid():
            self.setWindowTitle( color.name() )
         
app = QtGui.QApplication( sys.argv )
win = Window()
win.show()
app.exec_()

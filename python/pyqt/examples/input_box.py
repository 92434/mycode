#coding=utf-8
import sys
from PyQt4 import QtGui, QtCore
 
class MyDialog( QtGui.QDialog ):
    def __init__( self ):
        super( MyDialog, self ).__init__()
         
        self.setWindowTitle( "Dialog" )
        self.gridlayout = QtGui.QGridLayout()
         
        self.label = QtGui.QLabel( "Please Input:" )
        self.textField = QtGui.QLineEdit()
        self.okButton = QtGui.QPushButton( "OK" )
        self.cancalButton = QtGui.QPushButton( "Cancel" )
         
        self.gridlayout.addWidget( self.label , 0, 0 )
        self.gridlayout.addWidget( self.textField , 0, 1 )
        self.gridlayout.addWidget( self.cancalButton , 0, 2 )
        self.gridlayout.addWidget( self.okButton , 0, 3 )
         
        self.connect( self.okButton, QtCore.SIGNAL( 'clicked()' ), self.OnOk )
        self.connect( self.cancalButton, QtCore.SIGNAL( 'clicked()' ), self.OnCancel )
         
        self.setLayout( self.gridlayout )
         
    def OnOk( self ):
        self.text = self.textField.text()
        self.done( 1 )
    def OnCancel( self ):
        self.done( 0 )
         
 
class Window( QtGui.QWidget ):
    def __init__( self ):
        super( Window, self ).__init__()
        self.setWindowTitle( "hello" )
        self.resize( 400, 300 )
         
        hboxlayout = QtGui.QGridLayout()
        self.creatDialogButton = QtGui.QPushButton( "Create a new Dialog" )
        hboxlayout.addWidget( self.creatDialogButton )
        self.setLayout( hboxlayout )
         
        self.connect( self.creatDialogButton, QtCore.SIGNAL( 'clicked()' ), self.OnButton )
         
    def OnButton( self ):
        dialog = MyDialog()
        r = dialog.exec_()
        if r:
            self.creatDialogButton.setText( dialog.text )
         
         
app = QtGui.QApplication( sys.argv )
win = Window()
win.show()
app.exec_()

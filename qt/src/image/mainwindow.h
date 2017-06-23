#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "console.h"

#include <QFileInfoList>
#include <QMainWindow>
#include <QtWidgets>
#include <QTcpSocket>

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Console *log;

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	QToolBar *fileToolBar;
	QToolBar *viewToolBar;

	QTableView *info_view;
	QStandardItemModel *info_model;

	QLineEdit *enroll_edit;
	QPushButton *enroll_btn;
	QListWidget *enroll_list;

	QLineEdit *idnentify_edit;
	QPushButton *idnentify_btn;
	QListWidget *identify_list;

	QTcpSocket tcpClient;


private:
	void createActions();
	void createStatusBar();
	void createDockWindows();
	void about();
	void createEnrollLists(QDockWidget *dock);
	void createIdentifyLists(QDockWidget *dock);
	QFileInfoList getFileList(QString path, QStringList filters = QStringList());
	void update_list_view(QString path, QListWidget *list_widget);
	int set_label_bmp(QString filename, QLabel *label);
	void createLog(QDockWidget *dock);
	void createCentralWindow();
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void get_enroll_dir();
	void update_enroll_list_view();
	void get_idnentify_dir();
	void update_idnentify_list_view();
	void enroll_list_clicked(int row);
	void startTransfer();
	void displayError(QAbstractSocket::SocketError socketError);
};

#endif // MAINWINDOW_H

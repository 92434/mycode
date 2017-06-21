#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "console.h"

#include <QFileInfoList>
#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Console *console;
	Console *log;

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	QToolBar *fileToolBar;
	QToolBar *viewToolBar;

	QLineEdit *enroll_edit;
	QPushButton *enroll_btn;
	QListWidget *enroll_list;

	QLineEdit *idnentify_edit;
	QPushButton *idnentify_btn;
	QListWidget *identify_list;

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
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void get_enroll_dir();
	void update_enroll_list_view();
	void get_idnentify_dir();
	void update_idnentify_list_view();
};

#endif // MAINWINDOW_H

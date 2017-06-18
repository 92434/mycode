#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "console.h"

#include <QMainWindow>
#include <qlistwidget.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Console *console;

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;

	QToolBar *fileToolBar;
	QToolBar *viewToolBar;

	QListWidget *enroll_list;
	QListWidget *identify_list;

private:
	void createActions();
	void createStatusBar();
	void createDockWindows();
	void about();
	void createEnrollLists(QDockWidget *dock);
	void createIdentifyLists(QDockWidget *dock);
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
};

#endif // MAINWINDOW_H

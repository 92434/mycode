#include <QtWidgets>

#include "mainwindow.h"

#include <qmenubar.h>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	console = new Console;
	setCentralWidget(console);

	//console->setEnabled(false);
	//console->setLocalEchoEnabled(true);

	createActions();
	createStatusBar();
	createDockWindows();

	setWindowTitle(tr("image"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About image"), tr("The <b>image</b> is created by xiaofei!!!"));
}

void MainWindow::createActions()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	viewMenu = menuBar()->addMenu(tr("&View"));
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));


	fileToolBar = addToolBar(tr("File"));
	viewToolBar = addToolBar(tr("View"));


	fileMenu->addSeparator();

	QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));
	fileToolBar->addAction(quitAct);

	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));

	QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}
void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createEnrollLists(QDockWidget *dock)
{
	enroll_list = new QListWidget(dock);
	enroll_list->addItems(QStringList()
						  << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
						  << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
						  << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
						  << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
						  << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
						  << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
	dock->setWidget(enroll_list);
}

void MainWindow::createIdentifyLists(QDockWidget *dock)
{
	identify_list = new QListWidget(dock);
	identify_list->addItems(QStringList()
						  << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
						  << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
						  << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
						  << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
						  << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
						  << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
	dock->setWidget(identify_list);
}

void MainWindow::createDockWindows()
{
	QDockWidget *dock;

	dock = new QDockWidget(tr("enroll"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
	viewToolBar->addAction(dock->toggleViewAction());
	createEnrollLists(dock);

	dock = new QDockWidget(tr("identify"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
	viewToolBar->addAction(dock->toggleViewAction());
	createIdentifyLists(dock);
}


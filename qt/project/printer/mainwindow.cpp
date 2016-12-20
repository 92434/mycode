#include <QtWidgets/qdesktopwidget.h>
#include <qdatetime.h>
#include <QPalette>

#include "xiaofei_debug.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QDesktopWidget *pDesk = QApplication::desktop();

	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowTitle("Kingly printer");
	setWindowIcon(QIcon(":/resource/MainForm_64_64.ico"));

	move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);

	m_timerId = startTimer(1000);

	initDatetime();
	initFrameShowCom();
}

void MainWindow::initDatetime()
{
	QPalette palette = QPalette();

	palette.setColor(QPalette::Background, Qt::blue);
	palette.setColor(QPalette::Foreground, Qt::white);

	ui->label->setAutoFillBackground(true);
	ui->label->setPalette(palette);

	ui->label->setText(QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss")));

	connect(this, SIGNAL(timeout(QTimerEvent *)), this, SLOT(datetimeUpdateTimeout(QTimerEvent *)));
}

void MainWindow::initFrameShowCom()
{
	layout_show_com = new QHBoxLayout();
	scene_show_com = new DiagramScene(this);
	//scene_show_com->setSceneRect(QRectF(0, 0, 448, 192));
	scene_show_com->setSceneRect(QRectF(0, 0, 5000, 154));
	scene_show_com->setBackgroundBrush(Qt::lightGray);
	connect(scene_show_com, SIGNAL(itemInserted(DiagramItem *)), this, SLOT(itemInserted(DiagramItem *)));
	connect(scene_show_com, SIGNAL(textInserted(QGraphicsTextItem *)), this, SLOT(textInserted(QGraphicsTextItem *)));
	connect(scene_show_com, SIGNAL(itemSelected(QGraphicsItem *)), this, SLOT(itemSelected(QGraphicsItem *)));
	view_show_com = new QGraphicsView(scene_show_com);
	//view_show_com->setParent(ui->frame_show_com);
	layout_show_com->addWidget(view_show_com);
	ui->frame_show_com->setLayout(layout_show_com);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	emit this->timeout(event);
}

void MainWindow::datetimeUpdateTimeout(QTimerEvent *event)
{
	if (event->timerId() == m_timerId) {
		QString datetime = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss"));

		//printf("datetime = %s\n",qPrintable(datetime));

		ui->label->setText(datetime);
	}
}

MainWindow::~MainWindow()
{
	delete layout_show_com;
	delete scene_show_com;
	delete view_show_com;
	delete ui;
}

void MainWindow::on_pushButton_exit_clicked()
{
	close();
}

void MainWindow::on_pushButton_minimize_clicked()
{
	lower();
}

void MainWindow::itemInserted(DiagramItem *item)
{
	scene_show_com->setMode(DiagramScene::InsertItem);
	item = item;
}

void MainWindow::textInserted(QGraphicsTextItem *item)
{
	scene_show_com->setMode(DiagramScene::InsertText);
	item = item;
}


void MainWindow::itemSelected(QGraphicsItem *item)
{
	item = item;
}

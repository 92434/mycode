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
	QPalette palette = QPalette();

	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowTitle("Kingly printer");
	setWindowIcon(QIcon(":/resource/MainForm_64_64.ico"));

	move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);

	ui->label->setAutoFillBackground(true);
	palette.setColor(QPalette::Background, Qt::blue);
	palette.setColor(QPalette::Foreground, Qt::white);
	ui->label->setPalette(palette);

	ui->label->setText(QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss")));

	m_timerId = startTimer(1000);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_timerId) {
		QString datetime = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss"));

		//printf("datetime = %s\n",qPrintable(datetime));

		ui->label->setText(datetime);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_8_clicked()
{
	close();
}

void MainWindow::on_pushButton_7_clicked()
{
	lower();
}

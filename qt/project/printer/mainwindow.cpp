#include <QtWidgets/qdesktopwidget.h>
#include <qdatetime.h>
#include <QPalette>

#include "../../utils/xiaofei_debug.h"
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

	mStartPrint = false;

	initFrameShowCom();

	initDatetime();
	initStatusThumbnail();

	mTimerIdTime = startTimer(1000);
	mTimerIdStatus = startTimer(100);
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

void MainWindow::initStatusThumbnail()
{
	updateStatusThumbnail();
	connect(this, SIGNAL(timeout(QTimerEvent *)), this, SLOT(statusThumbnailTimeout(QTimerEvent *)));
}

void MainWindow::initFrameShowCom()
{
	scene_show_com = new GraphicsScene(this);
	scene_show_com->setSceneRect(QRectF(0, 0, 426, 169));
	scene_show_com->setBackgroundBrush(Qt::lightGray);
	connect(scene_show_com, SIGNAL(itemInserted(QGraphicsItem *)), this, SLOT(itemInserted(QGraphicsItem *)));
	//connect(scene_show_com, SIGNAL(textInserted(QGraphicsTextItem *)), this, SLOT(textInserted(QGraphicsTextItem *)));
	//connect(scene_show_com, SIGNAL(itemSelected(QGraphicsItem *)), this, SLOT(itemSelected(QGraphicsItem *)));
	//ui->graphicsView_show_com->setScene(scene_show_com);
	//ui->graphicsView_show_com->setMouseTracking(true);
	view_show_com = new QGraphicsView(scene_show_com);
	view_show_com->setMouseTracking(true);
	view_show_com->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view_show_com->setParent(ui->frame_show_com);
	layout_show_com = new QHBoxLayout();
	layout_show_com->addWidget(view_show_com);
	ui->frame_show_com->setLayout(layout_show_com);
	ui->pushButton_graphicsitem_up->setAutoRepeat(true);
	ui->pushButton_graphicsitem_down->setAutoRepeat(true);
	ui->pushButton_graphicsitem_left->setAutoRepeat(true);
	ui->pushButton_graphicsitem_right->setAutoRepeat(true);
	ui->pushButton_dec_print_length->setAutoRepeat(true);
	ui->pushButton_inc_print_length->setAutoRepeat(true);

	//ui->label_status_thumbnails->setScaledContents(true);
	//ui->label_status_thumbnails->setAlignment(Qt::AlignCenter);

	updateSceneSizeInfo();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	emit this->timeout(event);
}

void MainWindow::datetimeUpdateTimeout(QTimerEvent *event)
{
	if (event->timerId() == mTimerIdTime) {
		QString datetime = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss"));

		//printf("datetime = %s\n",qPrintable(datetime));

		ui->label->setText(datetime);
	}
}

void MainWindow::updateStatusThumbnail()
{
	int w = scene_show_com->visualRect().width();
	int h = scene_show_com->visualRect().height();

	QRectF targetRect = QRectF(0, 0, w, h);
	QRectF sourceRect = QRectF(scene_show_com->visualRect());
	QImage image = QImage(w, h, QImage::Format_Mono);

	QPainter painter(&image);

	if(mStartPrint) {
		scene_show_com->render(&painter, targetRect, sourceRect, Qt::KeepAspectRatio);
	} else {
		painter.fillRect(targetRect, Qt::white);
	}

	painter.end();

	image = image.scaled(ui->label_status_thumbnails->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	ui->label_status_thumbnails->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::statusThumbnailTimeout(QTimerEvent *event)
{
	if (event->timerId() == mTimerIdStatus) {
		updateStatusThumbnail();
	}
}

MainWindow::~MainWindow()
{
	killTimer(mTimerIdTime);
	killTimer(mTimerIdStatus);
	delete layout_show_com;
	delete view_show_com;
	delete scene_show_com;
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

void MainWindow::itemInserted(QGraphicsItem *item)
{
	scene_show_com->setInsertMode(GraphicsScene::Esc);
	item = item;
}

//void MainWindow::itemSelected(QGraphicsItem *item)
//{
//	scene_show_com->setInsertMode(GraphicsScene::Move);
//	item = item;
//}

void MainWindow::updateSceneSizeInfo()
{
	QRectF rect = scene_show_com->sceneRect();

	if(scene_show_com->printXMax() * scene_show_com->zoom() > 426) {
		rect.setWidth(scene_show_com->printXMax() * scene_show_com->zoom());
	} else {
		rect.setWidth(426);
	}

	scene_show_com->setSceneRect(rect);

	ui->lineEdit_print_length->setText(QString::number(scene_show_com->printXMax()));
}

void MainWindow::on_pushButton_dec_print_length_clicked()
{
	if(scene_show_com->printXMax() > 0) {
		scene_show_com->setPrintXMax(scene_show_com->printXMax() - 1);
	}

	updateSceneSizeInfo();
}

void MainWindow::on_pushButton_inc_print_length_clicked()
{
	if(scene_show_com->printXMax() < 2000) {
		scene_show_com->setPrintXMax(scene_show_com->printXMax() + 1);
	}

	updateSceneSizeInfo();
}

void MainWindow::on_lineEdit_print_length_editingFinished()
{
	int max = ui->lineEdit_print_length->text().toInt();

	if(max >= 0 && max <= 2000) {
		scene_show_com->setPrintXMax(max);
	}

	updateSceneSizeInfo();
}

void MainWindow::on_pushButton_QR_clicked()
{
    QRSetttingDialog *dialog = new QRSetttingDialog;
    dialog->exec();
	scene_show_com->setInsertMode(GraphicsScene::InsertGraphicsQRItem);
}

void MainWindow::on_pushButton_Text_clicked()
{
    TextSettingDialog *dialog = new TextSettingDialog;
    dialog->exec();
    scene_show_com->setInsertMode(GraphicsScene::InsertGraphicsTextItem);
}

void MainWindow::on_pushButton_graphicsitem_up_clicked()
{
	scene_show_com->moveSelectedItems(0);
}

void MainWindow::on_pushButton_graphicsitem_down_clicked()
{
	scene_show_com->moveSelectedItems(1);
}

void MainWindow::on_pushButton_graphicsitem_left_clicked()
{
	scene_show_com->moveSelectedItems(2);
}

void MainWindow::on_pushButton_graphicsitem_right_clicked()
{
	scene_show_com->moveSelectedItems(3);
}

void MainWindow::on_pushButton_deselect_items_clicked()
{
	scene_show_com->deselectItems();
}

void MainWindow::on_pushButton_delete_items_clicked()
{
	scene_show_com->deleteSelectedItems();
}

void MainWindow::on_pushButton_start_print_clicked()
{
	mStartPrint = !mStartPrint;
	scene_show_com->setInPrinting(mStartPrint);
	scene_show_com->deselectItems();
	updateStatusThumbnail();
}

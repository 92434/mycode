#include "mainwindow.h"
#include <QHostAddress>


struct BMP_FILEHDR {                     // BMP file header
	qint8   bfType[2];                   // "BM"
	qint32  bfSize;                      // size of file
	qint16  bfReserved1;
	qint16  bfReserved2;
	qint32  bfOffBits;                   // pointer to the pixmap bits
};

struct BMP_INFOHDR {                     // BMP information header
	qint32  biSize;                      // size of this struct
	qint32  biWidth;                     // pixmap width
	qint32  biHeight;                    // pixmap height
	qint16  biPlanes;                    // should be 1
	qint16  biBitCount;                  // number of bits per pixel
	qint32  biCompression;               // compression method
	qint32  biSizeImage;                 // size of image
	qint32  biXPelsPerMeter;             // horizontal resolution
	qint32  biYPelsPerMeter;             // vertical resolution
	qint32  biClrUsed;                   // number of colors used
	qint32  biClrImportant;              // number of important colors
};


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	createActions();
	createStatusBar();
	createCentralWindow();
	createDockWindows();

	setWindowTitle(tr("image"));

	connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
	connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
	tcpClient.connectToHost(QHostAddress::LocalHost, 5558);
}

void MainWindow::startTransfer()
{
	typedef struct _temp {
		int i;
		int j;
		char k;
		short k1;
		int l;
	} temp_t;
	temp_t temp = {
		.i = 1,
		.j = 2,
		.k = 3,
		.k1 = 4,
		.l = 5,
	};
	// called when the TCP client connected to the loopback server
	tcpClient.write((char *)&temp, sizeof(temp_t));
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
	if (socketError == QTcpSocket::RemoteHostClosedError) {
		return;
	}

	QMessageBox::information(this, tr("Network error"),
							 tr("The following error occurred: %1.")
							 .arg(tcpClient.errorString()));

	tcpClient.close();
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

void MainWindow::createCentralWindow()
{
	info_view = new QTableView(this);

	info_model = new QStandardItemModel(0, 2, info_view);
	info_model->setHeaderData(0, Qt::Horizontal, tr("Label"));
	info_model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));
	info_view->setModel(info_model);

	setCentralWidget(info_view);
}

int MainWindow::set_label_bmp(QString filename, QLabel *label)
{
#define add_debug_info(x, fmt) do { \
    log->putData(QString::asprintf(#x fmt, x, x).toLatin1()); \
} while(0)

	int ret = 0;

	BMP_FILEHDR bmp_file_header;
	BMP_INFOHDR bmp_info_header;

	int len;
	unsigned char *buffer;

	QFile f(filename);

	if (f.open(QIODevice::ReadOnly) == 0) {
		log->putData("File can not be opened!");
		ret = -1;
		return ret;
	}

	QDataStream in(&f);
	//in.setVersion(QDataStream::Qt_4_0);

	in.setByteOrder(QDataStream::LittleEndian);

	in >> bmp_file_header.bfType[0];
	in >> bmp_file_header.bfType[1];
	in >> bmp_file_header.bfSize;
	in >> bmp_file_header.bfReserved1;
	in >> bmp_file_header.bfReserved2;
	in >> bmp_file_header.bfOffBits;

	in >> bmp_info_header.biSize;
	in >> bmp_info_header.biWidth;
	in >> bmp_info_header.biHeight;
	in >> bmp_info_header.biPlanes;
	in >> bmp_info_header.biBitCount;
	in >> bmp_info_header.biCompression;
	in >> bmp_info_header.biSizeImage;
	in >> bmp_info_header.biXPelsPerMeter;
	in >> bmp_info_header.biYPelsPerMeter;
	in >> bmp_info_header.biClrUsed;
	in >> bmp_info_header.biClrImportant;

	//add_debug_info(bmp_file_header.bfType[0], ":%02x(%c)\n");
	//add_debug_info(bmp_file_header.bfType[1], ":%02x(%c)\n");
	//add_debug_info(bmp_file_header.bfSize, ":%08x(%d)\n");
	//add_debug_info(bmp_file_header.bfReserved1, ":%04x(%d)\n");
	//add_debug_info(bmp_file_header.bfReserved2, ":%04x(%d)\n");
	//add_debug_info(bmp_file_header.bfOffBits, ":%08x(%d)\n");

	//add_debug_info(bmp_info_header.biSize, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biWidth, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biHeight, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biPlanes, ":%04x(%d)\n");
	//add_debug_info(bmp_info_header.biBitCount, ":%04x(%d)\n");
	//add_debug_info(bmp_info_header.biCompression, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biSizeImage, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biXPelsPerMeter, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biYPelsPerMeter, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biClrUsed, ":%08x(%d)\n");
	//add_debug_info(bmp_info_header.biClrImportant, ":%08x(%d)\n");

	f.seek(bmp_file_header.bfOffBits);

	len = bmp_info_header.biWidth * bmp_info_header.biHeight;
	buffer = new unsigned char[len];

	in.readRawData((char *)buffer, len);

	f.close();
	QImage image = QImage(buffer, bmp_info_header.biWidth, bmp_info_header.biHeight, QImage::Format_Grayscale8);

	image = image.mirrored();

	delete buffer;

	label->setPixmap(QPixmap::fromImage(image));


	return ret;
}

void MainWindow::update_list_view(QString path, QListWidget *list_widget)
{
	QStringList filters;
	filters << "*.bmp";
	QFileInfoList file_info_list = getFileList(path, filters);

	foreach (QFileInfo info, file_info_list) {
		QWidget *widget = new QWidget;

		log->putData(info.absoluteFilePath().toLatin1());
		log->putData("\n");

		QVBoxLayout *layout = new QVBoxLayout();
		QLabel *lab1 = new QLabel;
		QLabel *lab2 = new QLabel;

		set_label_bmp(info.absoluteFilePath(), lab1);
		lab1->setAlignment(Qt::AlignCenter);

		lab2->setText(info.baseName());
		lab2->setAlignment(Qt::AlignCenter);

		layout->addWidget(lab1);
		layout->addWidget(lab2);
		widget->setLayout(layout);

		QListWidgetItem *item = new QListWidgetItem(list_widget);
		item->setSizeHint(widget->sizeHint());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		list_widget->setItemWidget(item, widget);
	}
}

void MainWindow::update_enroll_list_view()
{
	enroll_list->clear();
	update_list_view(enroll_edit->text(), enroll_list);
}

void MainWindow::get_enroll_dir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose enroll dir"), enroll_edit->text());

	if (dir.isEmpty()) {
		return;
	}

	enroll_edit->setText(dir);

	update_enroll_list_view();
}

void MainWindow::createLog(QDockWidget *dock)
{
	log = new Console;
	//setCentralWidget(log);
	//log->setEnabled(false);
	//log->setLocalEchoEnabled(true);

	dock->setWidget(log);
}

void MainWindow::enroll_list_clicked(int row)
{
	log->putData(QString::asprintf("enroll_list %d clicked!\n", row).toLatin1());
}

void MainWindow::createEnrollLists(QDockWidget *dock)
{

	QWidget *dock_widget = new QWidget(dock);

	QHBoxLayout *hbox = new QHBoxLayout(dock);
	enroll_edit = new QLineEdit();
	enroll_edit->setText(QFileInfo(".").absolutePath());
	hbox->addWidget(enroll_edit);
	enroll_btn = new QPushButton(tr("enroll path"));
	hbox->addWidget(enroll_btn);

	QVBoxLayout *vbox = new QVBoxLayout(dock);
	vbox->addLayout(hbox);

	enroll_list = new QListWidget();
	enroll_list->setMovement(QListView::Static);
	vbox->addWidget(enroll_list);

	dock_widget->setLayout(vbox);
	dock->setWidget(dock_widget);

	//update_enroll_list_view();
	connect(enroll_edit, SIGNAL(returnPressed()), this, SLOT(update_enroll_list_view()));
	connect(enroll_btn, SIGNAL(clicked()), this, SLOT(get_enroll_dir()));
	connect(enroll_list, SIGNAL(currentRowChanged(int)), this, SLOT(enroll_list_clicked(int)));
}

void MainWindow::update_idnentify_list_view()
{
	identify_list->clear();
	update_list_view(idnentify_edit->text(), identify_list);
}

void MainWindow::get_idnentify_dir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose idnentify dir"), idnentify_edit->text());

	if (dir.isEmpty()) {
		return;
	}

	idnentify_edit->setText(dir);

	update_idnentify_list_view();
}

void MainWindow::createIdentifyLists(QDockWidget *dock)
{
	QWidget *dock_widget = new QWidget(dock);

	QHBoxLayout *hbox = new QHBoxLayout(dock);
	idnentify_edit = new QLineEdit();
	idnentify_edit->setText(QFileInfo(".").absolutePath());
	hbox->addWidget(idnentify_edit);
	idnentify_btn = new QPushButton(tr("idnetify path"));
	hbox->addWidget(idnentify_btn);

	QVBoxLayout *vbox = new QVBoxLayout(dock);
	vbox->addLayout(hbox);

	identify_list = new QListWidget();
	identify_list->setMovement(QListView::Static);
	vbox->addWidget(identify_list);

	dock_widget->setLayout(vbox);
	dock->setWidget(dock_widget);

	//update_idnentify_list_view();
	connect(idnentify_edit, SIGNAL(returnPressed()), this, SLOT(update_idnentify_list_view()));
	connect(idnentify_btn, SIGNAL(clicked()), this, SLOT(get_idnentify_dir()));
}

void MainWindow::createDockWindows()
{
	QDockWidget *dock;


	dock = new QDockWidget(tr("log"), this);
	dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	viewMenu->addAction(dock->toggleViewAction());
	viewToolBar->addAction(dock->toggleViewAction());
	createLog(dock);

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

QFileInfoList MainWindow::getFileList(QString path, QStringList filters)
{
	QDir dir_file(path);
	QDir dir_dir(path);

	if(filters.size() != 0) {
		dir_file.setNameFilters(filters);
	}

	QFileInfoList file_list = dir_file.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir_dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	for(int i = 0; i != folder_list.size(); i++) {
		QString name = folder_list.at(i).absoluteFilePath();
		QFileInfoList child_file_list = getFileList(name, filters);

		file_list.append(child_file_list);
	}

	return file_list;
}

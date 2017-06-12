#include "dialog.h"

#include <qlayout.h>
#include <qdebug.h>
#include <QtGui/qimage.h>
#include  <qendian.h>
//#include <qbytearray.h>

QT_USE_NAMESPACE


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

Dialog::Dialog(Application *a, QWidget *parent)
	: QDialog(parent)
{
	label = new QLabel(this);
	label->setGeometry(0, 0, 64, 128);

	//read_bmp(":/image/ft_enroll_error_TestVer_id0_7.bmp", label);
	read_bmp(":/image/test.bmp", label);

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(label, 0, 0);

	setLayout(mainLayout);
	setWindowTitle(tr("readimg"));
}

int Dialog::read_bmp(QString filename, QLabel *label)
{
#define add_debug_info(x, fmt) do { \
    str_debug.append(QString::asprintf(#x fmt, x, x)); \
} while(0)

	int ret = 0;
	QString str_debug;

	BMP_FILEHDR bmp_file_header;
	BMP_INFOHDR bmp_info_header;

	int len;
	unsigned char *buffer;
	int i;

    //QFile f(":/image/test.bmp");

	QFile f(filename);

	if (f.open(QIODevice::ReadOnly) == 0) {
		str_debug.append("File can not be opened!");
		qDebug() << qPrintable(str_debug);
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

	add_debug_info(bmp_file_header.bfType[0], ":%02x(%c)\n");
	add_debug_info(bmp_file_header.bfType[1], ":%02x(%c)\n");
	add_debug_info(bmp_file_header.bfSize, ":%08x(%d)\n");
	add_debug_info(bmp_file_header.bfReserved1, ":%04x(%d)\n");
	add_debug_info(bmp_file_header.bfReserved2, ":%04x(%d)\n");
	add_debug_info(bmp_file_header.bfOffBits, ":%08x(%d)\n");
	str_debug.append("\n");

	add_debug_info(bmp_info_header.biSize, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biWidth, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biHeight, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biPlanes, ":%04x(%d)\n");
	add_debug_info(bmp_info_header.biBitCount, ":%04x(%d)\n");
	add_debug_info(bmp_info_header.biCompression, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biSizeImage, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biXPelsPerMeter, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biYPelsPerMeter, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biClrUsed, ":%08x(%d)\n");
	add_debug_info(bmp_info_header.biClrImportant, ":%08x(%d)\n");
	qDebug() << qPrintable(str_debug);

	f.seek(bmp_file_header.bfOffBits);

	len = bmp_info_header.biWidth * bmp_info_header.biHeight;
	buffer = new unsigned char[len];

	//QByteArray *ba = new QByteArray(len, 0);
	//str_debug = QString::asprintf("ba->size():%d\n", ba->size());
	//qDebug() << qPrintable(str_debug);

	//in.readBytes(ba, len);

	in.readRawData((char *)buffer, len);

	str_debug = QString::asprintf("bmp data:\n");
	for(i = 0; i < len; i++) {
		if((i != 0) && (i % 64 == 0)) {
			str_debug.append("\n");
		}

		str_debug.append(QString::asprintf("%02X ", buffer[i]));
	}
	qDebug() << qPrintable(str_debug);

	QImage image = QImage(buffer, bmp_info_header.biWidth, bmp_info_header.biHeight, QImage::Format_Grayscale8);

    image = image.mirrored();
	label->setPixmap(QPixmap::fromImage(image));

	f.close();

	str_debug = QString::asprintf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	qDebug() << qPrintable(str_debug);

	return ret;
}

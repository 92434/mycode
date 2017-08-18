const long _RADIUS = 10;
const long _IMAGES = 2;

//////////////////////////////////////////////////////////////////////////

#include <QDebug>
#include <ctime>

#define CHECK_TIME(text, ...) \
{ \
    clock_t t1 = std::clock(); \
    { __VA_ARGS__; } \
    clock_t t2 = std::clock(); \
    qDebug() << #__VA_ARGS__ << text << "->" << (long)((double)(t2 - t1) / (double)(CLOCKS_PER_SEC) * 1000.0); \
}

//////////////////////////////////////////////////////////////////////////

#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
	loadImage("ground.bmp");
}

Widget::~Widget() {}

//////////////////////////////////////////////////////////////////////////

#include <QImage>
#include "filter.h"

QImage _image[_IMAGES];

void Widget::loadImage(const QString &path)
{
	if (!_IMAGES) {
		return;
	}

	_image[0].load(path);

	if (_image[0].isNull()) {
		return;
	}

	for(int i = 1; i < _IMAGES; ++i) {
		if (i == 1) {
			_image[1] = _image[0].convertToFormat(QImage::Format_RGB888);
		} else {
			_image[i] = _image[1].copy();
		}
	}

	static filter::pair_t pair[] = {
		{ filter::Gauss, filter::Blur1D },
		{ filter::Gauss, filter::Blur2D }
	};
	filter::bitmap_t bmp;

	for(int i = 1; i < _IMAGES; ++i) {
		bmp.set((filter::bitmap_t::pixel_t *)_image[i].bits(),
				_image[i].width(), _image[i].height());
		CHECK_TIME(i, filter::Filter(pair[i - 1], bmp, _RADIUS))
	}

	resize(_image[0].width() * _IMAGES, _image[0].height());
}

//////////////////////////////////////////////////////////////////////////

#include <QPainter>

void Widget::paintEvent(QPaintEvent *)
{
	if (!_IMAGES) {
		return;
	}

	if (_image[0].isNull()) {
		return;
	}

	QPainter painter(this);

	for(int i = 0, w = 0; i < _IMAGES; w += _image[i].width(), ++i) {
		painter.drawImage(w, 0, _image[i]);
	}
}

#include <QFileDialog>

void Widget::mouseDoubleClickEvent(QMouseEvent *)
{
	if (!_IMAGES) {
		return;
	}

	QString filename = QFileDialog::getOpenFileName(
						   this,
						   "Open Image",
						   QDir::currentPath(),
						   "Image files (*.bmp *.jpg *.png);;All files(*.*)");

	if (!filename.isNull()) {
		loadImage(filename);
	}
}

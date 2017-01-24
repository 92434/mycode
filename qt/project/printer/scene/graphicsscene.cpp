#include "graphicsscene.h"
#include "graphicsarrowitem.h"
#include "xiaofei_debug.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

GraphicsScene::GraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	mRuleHeight = 30;
	mGridMargin = 10;

	setZoom(3);
	setPrintXMax(25);
	setHeight(34);

	setItemWidth(17);
	setItemHeight(17);
	setInPrinting(false);

	mVisualRect = QRectF(0, mRuleHeight, mPrintXMax * mZoom, mHeight * mZoom);

	mCurrentPoint = QPointF(0, 0);
}

void GraphicsScene::setInsertMode(sceneOpMode mode)
{
	mSceneOpMode = mode;
}

GraphicsQRItem *GraphicsScene::prepareQRItem(GraphicsQRItem *originItem, bool allocItem)
{
	GraphicsQRItem *item = 0;
	QRSettingDialog *dialog = new QRSettingDialog;

	if(originItem != 0) {
		//printf("originItem->height():%d, originItem->rotate():%d, originItem->mirrorHorizontal():%d, originItem->mirrorVertical():%d, originItem->lock():%d, originItem->symbol():%d\n", originItem->height(), originItem->rotate(), originItem->mirrorHorizontal(), originItem->mirrorVertical(), originItem->lock(), originItem->symbol());
		dialog->setHeight(originItem->height());
		dialog->setRotate(originItem->rotate());
		dialog->setMirrorHorizontal(originItem->mirrorHorizontal());
		dialog->setMirrorVertical(originItem->mirrorVertical());
		dialog->setLock(originItem->lock());
		dialog->setSymbol(originItem->symbol());
	}

	if(allocItem) {
		item = new GraphicsQRItem;
	} else {
		item = originItem;
	}

	if(item != 0) {
		if(dialog->exec() == QDialog::Accepted) {
			//printf("dialog->height():%d, dialog->rotate():%d, dialog->mirrorHorizontal():%d, dialog->mirrorVertical():%d, dialog->lock():%d, dialog->symbol():%d\n", dialog->height(), dialog->rotate(), dialog->mirrorHorizontal(), dialog->mirrorVertical(), dialog->lock(), dialog->symbol());
			item->setHeight(dialog->height());
			item->setRotate(dialog->rotate());
			item->setMirrorHorizontal(dialog->mirrorHorizontal());
			item->setMirrorVertical(dialog->mirrorVertical());
			item->setLock(dialog->lock());
			item->setSymbol(dialog->symbol());
		}
	}

	delete dialog;

	return item;
}

GraphicsTextItem *GraphicsScene::prepareTextItem(GraphicsTextItem *originItem, bool allocItem)
{
	GraphicsTextItem *item = 0;
	TextSettingDialog *dialog = new TextSettingDialog;

	if(originItem != 0) {
		//printf("originItem->text():%s, originItem->font():%s, originItem->rotate():%d, originItem->mirrorHorizontal():%d, originItem->mirrorVertical():%d, originItem->lock():%d\n", originItem->text().toLatin1().data(), originItem->font().toString().toLocal8Bit().data(), originItem->rotate(), originItem->mirrorHorizontal(), originItem->mirrorVertical(), originItem->lock());
		dialog->setText(originItem->text());
		dialog->setFont(originItem->font());
		dialog->setRotate(originItem->rotate());
		dialog->setMirrorHorizontal(originItem->mirrorHorizontal());
		dialog->setMirrorVertical(originItem->mirrorVertical());
		dialog->setLock(originItem->lock());
	}

	if(allocItem) {
		item = new GraphicsTextItem;
	} else {
		item = originItem;
	}

	if(item != 0) {
		if(dialog->exec() == QDialog::Accepted) {
			//printf("dialog->text():%s, dialog->font():%s, dialog->rotate():%d, dialog->mirrorHorizontal():%d, dialog->mirrorVertical():%d, dialog->lock():%d\n", dialog->text().toLatin1().data(), dialog->font().toString().toLocal8Bit().data(), dialog->rotate(), dialog->mirrorHorizontal(), dialog->mirrorVertical(), dialog->lock());
			item->setText(dialog->text());
			item->setFont(dialog->font());
			item->setRotate(dialog->rotate());
			item->setMirrorHorizontal(dialog->mirrorHorizontal());
			item->setMirrorVertical(dialog->mirrorVertical());
			item->setLock(dialog->lock());
		}
	}

	delete dialog;

	return item;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF pos = mouseEvent->scenePos();

	if (mouseEvent->button() != Qt::LeftButton) {
		return;
	}

	pos.setX((int)pos.x() / mZoom * mZoom);
	pos.setY((int)pos.y() / mZoom * mZoom);

	if(!mVisualRect.contains(pos)) {
		return;
	}

	//qDebug() << pos;


	switch (mSceneOpMode) {
		case InsertGraphicsQRItem: {
			mGraphicsQRItem = prepareQRItem();

			if(mGraphicsQRItem != 0) {
				mGraphicsQRItem->updateItem();
				mGraphicsQRItem->setPos(pos);
				addItem(mGraphicsQRItem);
				emit itemInserted(mGraphicsQRItem);
			}
		}

		break;
		case InsertGraphicsTextItem: {
			mGraphicsTextItem = prepareTextItem();

			if(mGraphicsTextItem != 0) {
				mGraphicsTextItem->updateItem();
				mGraphicsTextItem->setPos(pos);
				addItem(mGraphicsTextItem);
				emit itemInserted(mGraphicsTextItem);
			}
		}
		break;
		default:
			break;
	}

	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	foreach (QGraphicsItem * item, selectedItems()) {
		switch(item->type()) {
			case GraphicsQRItem::Type: {
				mGraphicsQRItem = prepareQRItem((GraphicsQRItem *)item, false);

				if(mGraphicsQRItem != 0) {
					mGraphicsQRItem->updateItem();
				}
			}
			break;
			case GraphicsTextItem::Type: {
				mGraphicsTextItem = prepareTextItem((GraphicsTextItem *)item, false);

				if(mGraphicsTextItem != 0) {
					mGraphicsTextItem->updateItem();
				}
			}
			break;
			default:
				break;
		}
	}

	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//qDebug() << mouseEvent->scenePos();

	if(!mVisualRect.contains(mouseEvent->scenePos())) {
		return;
	}

	mCurrentPoint = mouseEvent->scenePos();

	QGraphicsScene::mouseMoveEvent(mouseEvent);

	update();
}


void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicsScene::drawBG(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, mRuleHeight, mPrintXMax * mZoom, mHeight * mZoom);
	//validRect &= rect;

	if(rect.isNull()) {
		return;
	}

	if(validRect.isNull()) {
		return;
	}

	//printf("");
	//qDebug() << validRect;

	QColor colorGridBG(Qt::white);
	QColor colorMargin(Qt::black);

	painter->fillRect(validRect.x(), validRect.y(), validRect.width(), validRect.height(), colorGridBG);
	//painter->fillRect(validRect.x(), vsetPosalidRect.y() + validRect.height(), validRect.width(), mGridMargin, colorMargin);
}



void GraphicsScene::drawBGGrid(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, mRuleHeight, mPrintXMax * mZoom, mHeight * mZoom);
	//validRect &= rect;

	if(rect.isNull()) {
		return;
	}


	if(validRect.isNull()) {
		return;
	}

	int x = validRect.x();
	int y = validRect.y();
	int width = validRect.width();
	int height = validRect.height() - 1;

	QColor colorStep2(Qt::lightGray);
	QColor colorStep10(Qt::blue);

	painter->save();
	painter->setBrush(Qt::NoBrush);

	for(int i = y; i < y + height; i++) {
		if((i - y) % (10 * mZoom) == 0) {
			painter->setPen(colorStep10);
			painter->drawLine(x, i, x + width, i);
		} else if((i - y) % (2 * mZoom) == 0) {
			painter->setPen(colorStep2);
			painter->drawLine(x, i, x + width, i);
		}
	}

	for(int i = x; i < x + width; i++) {
		if((i - x) % (10 * mZoom) == 0) {
			painter->setPen(colorStep10);
			painter->drawLine(i, y, i, y + height);
		} else if((i - x) % (2 * mZoom) == 0) {
			painter->setPen(colorStep2);
			painter->drawLine(i, y, i, y + height);
		}
	}

	painter->restore();
}

void GraphicsScene::drawBGRule(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, 0, mPrintXMax * mZoom, mRuleHeight);
	//validRect &= rect;

	if(rect.isNull()) {
		return;
	}


	if(validRect.isNull()) {
		return;
	}

	int x = validRect.x();
	int y = validRect.y();
	int width = validRect.width();
	int height = validRect.height() - 1;

	int y0_1 = y + height - 10;
	int y0_10 = y + height - 20;
	int y1 = y + height;

	QColor colorRule(Qt::white);
	QColor colorNumber(Qt::black);

	//int fontHeight = painter->fontMetrics().height();

	painter->save();
	painter->setBrush(Qt::NoBrush);

	painter->setPen(colorRule);

	for(int i = x; i < x + width; i++) {
		if(i % (10 * mZoom) == 0) {
			painter->drawLine(i, y0_10, i, y1);
		} else if(i % mZoom == 0) {
			painter->drawLine(i, y0_1, i, y1);
		}
	}

	painter->setPen(colorNumber);

	for(int i = x; i < x + width; i++) {
		if(i % (10 * mZoom) == 0) {
			//printf("");
			//qDebug() << "i:" << i;
			//QRect rectText = QRect(i, y0_1 - fontHeight, 10 * mZoom, fontHeight);
			QString number = QString::number(i / mZoom);
			//painter->drawText(rectText, Qt::AlignLeft, number);
			painter->drawText(i, y0_1, number);
		}
	}

	painter->restore();
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	//printf("");
	//qDebug() << validRect;

	QGraphicsScene::drawBackground(painter, rect);
	drawBG(painter, rect);

	if(mInPrinting) {
		return;
	}

	drawBGGrid(painter, rect);
	drawBGRule(painter, rect);
}

void GraphicsScene::drawFGPos(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, 0, mPrintXMax * mZoom, mRuleHeight);
	//validRect &= rect;

	if(rect.isNull()) {
		return;
	}

	if(validRect.isNull()) {
		return;
	}

	int x = validRect.x();
	int y = validRect.y();
	int width = validRect.width();
	int height = validRect.height();

	painter->save();
	painter->setBrush(Qt::NoBrush);

	QColor colorCurrentPos(Qt::black);

	painter->setPen(colorCurrentPos);
	int currentPointX = (mCurrentPoint.x() <= x + width) ? mCurrentPoint.x() : x + width;
	currentPointX = currentPointX / mZoom * mZoom;
	painter->drawLine(currentPointX, y, currentPointX, y + height);

	painter->restore();
}

void GraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	QGraphicsScene::drawForeground(painter, rect);

	if(mInPrinting) {
		return;
	}

	drawFGPos(painter, rect);
}

void GraphicsScene::moveSelectedItems(int type)
{
	int dx = 0;
	int dy = 0;

	switch(type) {
		case 0:
			dy = -1 * mZoom;
			break;
		case 1:
			dy = 1 * mZoom;
			break;
		case 2:
			dx = -1 * mZoom;
			break;
		case 3:
			dx = 1 * mZoom;
			break;
		default:
			break;
	}

	foreach (QGraphicsItem * item, selectedItems()) {
		QPointF pos = item->pos();

		if(!mVisualRect.contains(pos + QPointF(dx, dy))) {
			return;
		}
	}

	foreach (QGraphicsItem * item, selectedItems()) {
		item->moveBy(dx, dy);
	}
}

void GraphicsScene::deselectItems()
{
	clearSelection();
}

void GraphicsScene::deleteSelectedItems()
{
	foreach (QGraphicsItem * item, selectedItems()) {
		delete item;
	}
}

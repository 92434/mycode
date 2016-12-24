#include "graphicsscene.h"
#include "graphicsarrowitem.h"
#include "../../../utils/xiaofei_debug.h"

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

	mCurrentPoint = QPointF(0, 0);
	mSceneOpMode = InsertGraphicsPolygonItem;
	mGraphicsPolygonItemType = GraphicsPolygonItem::Step;
	mGraphicsLineItem = 0;
	mTextGraphicsItem = 0;
	mItemColor = Qt::white;
	mTextColor = Qt::black;
	mLineColor = Qt::black;
}

void GraphicsScene::setLineColor(const QColor &color)
{
	bool isColorChanged = false;
	mLineColor = color;

	foreach (QGraphicsItem * item, selectedItems()) {
		if (item->type() == GraphicsArrowItem::Type) {
			GraphicsArrowItem *arrow = qgraphicsitem_cast<GraphicsArrowItem *>(item);
			arrow->setColor(mLineColor);
			isColorChanged = true;
		}
	}

	if(isColorChanged) {
		update();
	}
}

void GraphicsScene::setTextColor(const QColor &color)
{
	mTextColor = color;

	foreach (QGraphicsItem * item, selectedItems()) {
		if (item->type() == GraphicsTextItem::Type) {
			(qgraphicsitem_cast<GraphicsTextItem *>(item))->setDefaultTextColor(mTextColor);
		}
	}
}

void GraphicsScene::setItemColor(const QColor &color)
{
	mItemColor = color;

	foreach (QGraphicsItem * item, selectedItems()) {
		if (item->type() == GraphicsPolygonItem::Type) {
			GraphicsPolygonItem *graphicsPolygonItem = qgraphicsitem_cast<GraphicsPolygonItem *>(item);
			graphicsPolygonItem->setBrush(mItemColor);
		}
	}
}

void GraphicsScene::setFont(const QFont &font)
{
	mFont = font;

	foreach (QGraphicsItem * item, selectedItems()) {
		QGraphicsTextItem *graphicsTextItem = qgraphicsitem_cast<GraphicsTextItem *>(item);
		graphicsTextItem->setFont(mFont);
	}
}

void GraphicsScene::setInsertMode(sceneOpMode mode)
{
	mSceneOpMode = mode;
}

void GraphicsScene::setGraphicsPolygonItemType(GraphicsPolygonItem::GraphicsPolygonType type)
{
	mGraphicsPolygonItemType = type;
}

void GraphicsScene::textItemLostFocus(GraphicsTextItem *item)
{
	QTextCursor cursor = item->textCursor();
	cursor.clearSelection();
	item->setTextCursor(cursor);

	if (item->toPlainText().isEmpty()) {
		removeItem(item);
		item->deleteLater();
	}
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton) {
		return;
	}

	GraphicsPolygonItem *item;

	switch (mSceneOpMode) {
		case InsertGraphicsPolygonItem:
			item = new GraphicsPolygonItem(mGraphicsPolygonItemType);
			item->setBrush(mItemColor);
			addItem(item);
			item->setPos(mouseEvent->scenePos());
			emit itemInserted(item);
			break;
		case InsertQGraphicsLineItem:
			mGraphicsLineItem = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
			mGraphicsLineItem->setPen(QPen(mLineColor, 2));
			addItem(mGraphicsLineItem);
			break;
		case InsertTextGraphicsItem:
			mTextGraphicsItem = new GraphicsTextItem();
			mTextGraphicsItem->setFont(mFont);
			mTextGraphicsItem->setTextInteractionFlags(Qt::TextEditorInteraction);
			mTextGraphicsItem->setZValue(1000.0);
			connect(mTextGraphicsItem, SIGNAL(lostFocus(GraphicsTextItem *)), this, SLOT(textItemLostFocus(GraphicsTextItem *)));
			connect(mTextGraphicsItem, SIGNAL(selectedChange(QGraphicsItem *)), this, SIGNAL(itemSelected(QGraphicsItem *)));
			addItem(mTextGraphicsItem);
			mTextGraphicsItem->setDefaultTextColor(mTextColor);
			mTextGraphicsItem->setPos(mouseEvent->scenePos());
			emit textInserted(mTextGraphicsItem);
		default:
			break;
	}

	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	switch (mSceneOpMode) {
		case InsertQGraphicsLineItem:

			if(mGraphicsLineItem != 0) {
				QLineF newLine(mGraphicsLineItem->line().p1(), mouseEvent->scenePos());
				mGraphicsLineItem->setLine(newLine);
			}

			break;
		default:
			QGraphicsScene::mouseMoveEvent(mouseEvent);
			break;
	}

	mCurrentPoint = mouseEvent->scenePos();
	update();

	//qDebug() << mouseEvent->scenePos();
}


void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicsScene::drawBG(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, mRuleHeight, mPrintXMax * mZoom, mHeight * mZoom);
	//validRect &= rect;

	if(validRect.isNull()) {
		return;
	}

	//printf("");
	//qDebug() << validRect;

	QColor colorGridBG(Qt::white);
	QColor colorMargin(Qt::black);

	painter->fillRect(validRect.x(), validRect.y(), validRect.width(), validRect.height(), colorGridBG);
	painter->fillRect(validRect.x(), validRect.y() + validRect.height(), validRect.width(), mGridMargin, colorMargin);
}

void GraphicsScene::drawBGGrid(QPainter *painter, const QRectF &rect)
{
	QRectF validRect(0, mRuleHeight, mPrintXMax * mZoom, mHeight * mZoom);
	//validRect &= rect;

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
		} else if((i - x) % (2 * mZoom) == 0){
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
	drawBGGrid(painter, rect);
	drawBGRule(painter, rect);
}

void GraphicsScene::drawFGPos(QPainter *painter, const QRectF &rect) {
	QRectF validRect(0, 0, mPrintXMax * mZoom, mRuleHeight);
	validRect &= rect;

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
	//QGraphicsScene::drawForeground(painter, rect);
	drawFGPos(painter, rect);
}

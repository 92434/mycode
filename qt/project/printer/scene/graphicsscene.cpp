#include "graphicsscene.h"
#include "graphicsarrowitem.h"
#include "../../../utils/xiaofei_debug.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

GraphicsScene::GraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	mZoom = 4;
	mHeight = 34;
	mRuleHeight = 40;
	mGridMargin = 10;
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

	update();
}

void GraphicsScene::drawBGGrid(QPainter *painter, const QRectF &rect, int size, QColor color)
{
	int x = rect.x();
	int y = rect.y() + mRuleHeight;
	int width = rect.width();
	int height = mHeight * mZoom - mRuleHeight;

	int step = size * mZoom;


	//printf("");

	painter->save();
	painter->setPen(color);
	painter->setBrush(Qt::NoBrush);

	for(int i = y; i < y + height; i++) {
		if(i % step == 0) {
			painter->drawLine(x, i, x + width, i);
			//qDebug() << rect.x() << ',' << rect.y() << ' ' << rect.width() << 'x' << rect.height();
			//qDebug() << "x1:" << x << " y1:" << i << " x2:" << x + width << " y2:" << i;
		}
	}

	for(int i = x; i < x + width; i++) {
		if(i % step == 0) {
			painter->drawLine(i, y, i, y + height);
			//qDebug() << rect.x() << ',' << rect.y() << ' ' << rect.width() << 'x' << rect.height();
			//qDebug() << "x1:" << i << " y1:" << y << " x2:" << i << " y2:" << y + height;
		}
	}

	painter->restore();
}

void GraphicsScene::drawBG(QPainter *painter, const QRectF &rect)
{
	QColor colorGridBG(Qt::white);
	QColor colorMargin(Qt::black);

	painter->fillRect(rect.x(), rect.y() + mRuleHeight, rect.width(), mHeight * mZoom - mRuleHeight, colorGridBG);
	painter->fillRect(rect.x(), mHeight * mZoom, rect.width(), mGridMargin, colorMargin);
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsScene::drawBackground(painter, rect);
	drawBG(painter, rect);
	drawBGGrid(painter, rect, 2, QColor(Qt::lightGray));
	drawBGGrid(painter, rect, 10, QColor(Qt::cyan));
}


void GraphicsScene::drawFGRule(QPainter *painter, const QRectF &rect)
{
	int x = rect.x();
	int y = rect.y();
	int width = rect.width();
	int height = mRuleHeight;

	int y0_1 = y + height - 10;
	int y0_10 = y + height - 30;
	int y1 = y + height;

	int step = mZoom;
	QColor colorRule(Qt::gray);
	QColor colorCurrentPos(Qt::black);

	int fontHeight = painter->fontMetrics().height();

	painter->save();
	painter->setBrush(Qt::NoBrush);

	painter->setPen(colorRule);

	for(int i = x; i < x + width; i++) {
		if(i % (10 * step) == 0) {
			painter->drawLine(i, y0_10, i, y1);
		} else if(i % step == 0) {
			painter->drawLine(i, y0_1, i, y1);
		}
	}

	painter->setPen(colorCurrentPos);
	painter->drawLine(mCurrentPoint.x(), rect.y(), mCurrentPoint.x(), y1);

	for(int i = (x - 10 * step >= 0) ? (x - 10 * step) : x; i < x + width; i++) {
		if(i % (10 * step) == 0) {
			//printf("");
			//qDebug() << "i:" << i;
			//QRect rectText = QRect(i, y0_1 - fontHeight, 10 * step, fontHeight);
			QString number = QString::number(i / step);
			//painter->drawText(rectText, Qt::AlignLeft, number);
			painter->drawText(i, y0_1, number);
		}
	}

	painter->restore();
}

void GraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	//printf("");
	//QGraphicsScene::drawForeground(painter, rect);
	drawFGRule(painter, rect);
}

#include "graphicsscene.h"
#include "graphicsarrowitem.h"
#include "../../../utils/xiaofei_debug.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

GraphicsScene::GraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
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
	if (mSceneOpMode == InsertQGraphicsLineItem) {
		if(mGraphicsLineItem != 0) {
			QLineF newLine(mGraphicsLineItem->line().p1(), mouseEvent->scenePos());
			mGraphicsLineItem->setLine(newLine);
		}
	} else if (mSceneOpMode == Move) {
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}


void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool GraphicsScene::isSelectedItemTypeMatch(int type)
{
	foreach (QGraphicsItem * item, selectedItems()) {
		if (item->type() == type) {
			return true;
		}
	}
	return false;
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsScene::drawBackground(painter, rect);
	printf("");
	qDebug() << rect.x() << ',' << rect.y() << ' ' << rect.width() << 'x' << rect.height();
}


void GraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	QGraphicsScene::drawForeground(painter, rect);
	printf("");
	qDebug() << rect.x() << ',' << rect.y() << ' ' << rect.width() << 'x' << rect.height();
}

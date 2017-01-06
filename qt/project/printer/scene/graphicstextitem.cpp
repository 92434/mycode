#include "graphicstextitem.h"
#include "graphicsscene.h"


QString GraphicsTextItem::mText = QString("xiaofei");
QColor GraphicsTextItem::mFgColor = QColor(Qt::black);
QColor GraphicsTextItem::mBgColor = QColor(Qt::white);
QFont GraphicsTextItem::mFont = QFont();

GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
	//setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setTextInteractionFlags(Qt::NoTextInteraction);
}

//QVariant GraphicsTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//	if (change == QGraphicsItem::ItemSelectedHasChanged) {
//		emit selectedChange(this);
//	}
//
//	return value;
//}
//
//void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
//{
//	setTextInteractionFlags(Qt::NoTextInteraction);
//	emit lostFocus(this);
//	QGraphicsTextItem::focusOutEvent(event);
//}
//
//void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
//	if (textInteractionFlags() == Qt::NoTextInteraction) {
//		setTextInteractionFlags(Qt::TextEditorInteraction);
//	}
//
//	QGraphicsTextItem::mouseDoubleClickEvent(event);
//}

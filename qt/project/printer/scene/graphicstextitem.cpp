#include "graphicstextitem.h"
#include "graphicsscene.h"

GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
	setText("http://www.baidu.com");
	setRotate(0);
	setMirrorHorizontal(false);
	setMirrorVertical(false);
	setLock(true);

	setColor(QColor(Qt::black));

	updateItem();
}

void GraphicsTextItem::updateItem()
{
	setPlainText(mText);

	resetMatrix();

	if(mMirrorHorizontal) {
		setMatrix(QMatrix(-1, 0, 0, 1, 0, 0), true);
	}

	if(mMirrorvertical) {
		setMatrix(QMatrix(1, 0, 0, -1, 0, 0), true);
	}

	setRotation(mRotate);

	setFlag(QGraphicsItem::ItemIsMovable, !mLock);

	setDefaultTextColor(mColor);

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

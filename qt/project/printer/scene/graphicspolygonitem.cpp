#include "graphicspolygonitem.h"
#include "graphicsarrowitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

//! [0]
GraphicsPolygonItem::GraphicsPolygonItem(GraphicsPolygonType graphicsPolygonType, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
	mGraphicsPolygonType = graphicsPolygonType;

	QPainterPath path;

	switch (mGraphicsPolygonType) {
		case StartEnd:
			path.moveTo(200, 50);
			path.arcTo(150, 0, 50, 50, 0, 90);
			path.arcTo(50, 0, 50, 50, 90, 90);
			path.arcTo(50, 50, 50, 50, 180, 90);
			path.arcTo(150, 50, 50, 50, 270, 90);
			path.lineTo(200, 25);
			mPolygon = path.toFillPolygon();
			break;
		case Conditional:
			mPolygon << QPointF(-100, 0) << QPointF(0, 100)
					 << QPointF(100, 0) << QPointF(0, -100)
					 << QPointF(-100, 0);
			break;
		case Step:
			mPolygon << QPointF(-100, -100) << QPointF(100, -100)
					 << QPointF(100, 100) << QPointF(-100, 100)
					 << QPointF(-100, -100);
			break;
		default:
			mPolygon << QPointF(-120, -80) << QPointF(-70, 80)
					 << QPointF(120, 80) << QPointF(70, -80)
					 << QPointF(-120, -80);
			break;
	}

	setPolygon(mPolygon);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
//! [0]

//! [1]
void GraphicsPolygonItem::removeArrow(GraphicsArrowItem *arrow)
{
	int index = listArrows.indexOf(arrow);

	if (index != -1) {
		listArrows.removeAt(index);
	}
}
//! [1]

//! [2]
void GraphicsPolygonItem::removeArrows()
{
	foreach (GraphicsArrowItem * arrow, listArrows) {
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		scene()->removeItem(arrow);
		delete arrow;
	}
}
//! [2]

//! [3]
void GraphicsPolygonItem::addArrow(GraphicsArrowItem *arrow)
{
	listArrows.append(arrow);
}
//! [3]

//! [4]
QPixmap GraphicsPolygonItem::image() const
{
	QPixmap pixmap(250, 250);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setPen(QPen(Qt::black, 8));
	painter.translate(125, 125);
	painter.drawPolyline(mPolygon);

	return pixmap;
}
//! [4]

//! [5]
void GraphicsPolygonItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	event = event;
	scene()->clearSelection();
	setSelected(true);
}
//! [5]

//! [6]
QVariant GraphicsPolygonItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) {
		foreach (GraphicsArrowItem * arrow, listArrows) {
			arrow->updatePosition();
		}
	}

	return value;
}
//! [6]

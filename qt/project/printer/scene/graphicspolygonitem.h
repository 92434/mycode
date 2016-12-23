#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class GraphicsArrowItem;

//! [0]
class GraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
	enum { Type = UserType + 15 };
	enum GraphicsPolygonType { Step, Conditional, StartEnd, Io };

	explicit GraphicsPolygonItem(GraphicsPolygonType graphicsPolygonType, QGraphicsItem *parent = 0);

	void removeArrow(GraphicsArrowItem *arrow);
	void removeArrows();
	GraphicsPolygonType getGraphicsPolygonType() const {
		return mGraphicsPolygonType;
	}
	QPolygonF getPolygon() const {
		return mPolygon;
	}
	void addArrow(GraphicsArrowItem *arrow);

	QPixmap image() const;
	int type() const Q_DECL_OVERRIDE {
		return Type;
	}

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
	GraphicsPolygonType mGraphicsPolygonType;
	QPolygonF mPolygon;
	QList<GraphicsArrowItem *> listArrows;
};
//! [0]

#endif // GRAPHICSITEM_H

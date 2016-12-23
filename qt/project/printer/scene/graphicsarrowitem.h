#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "graphicspolygonitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

//! [0]
class GraphicsArrowItem : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    GraphicsArrowItem(GraphicsPolygonItem *startItem, GraphicsPolygonItem *endItem, QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void setColor(const QColor &color) { mColor = color; }
    GraphicsPolygonItem *startItem() const { return mGraphicsPolygonItemStart; }
    GraphicsPolygonItem *endItem() const { return mGraphicsPolygonItemEnd; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    GraphicsPolygonItem *mGraphicsPolygonItemStart;
    GraphicsPolygonItem *mGraphicsPolygonItemEnd;
    QColor mColor;
    QPolygonF mQPolygonF;
};
//! [0]

#endif // ARROW_H

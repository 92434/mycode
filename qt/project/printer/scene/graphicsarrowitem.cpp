#include "graphicsarrowitem.h"

#include <math.h>

#include <QPen>
#include <QPainter>

const qreal Pi = 3.14;

//! [0]
GraphicsArrowItem::GraphicsArrowItem(GraphicsPolygonItem *startItem, GraphicsPolygonItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    mGraphicsPolygonItemStart = startItem;
    mGraphicsPolygonItemEnd = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    mColor = Qt::black;
    setPen(QPen(mColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF GraphicsArrowItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath GraphicsArrowItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(mQPolygonF);
    return path;
}
//! [2]

//! [3]
void GraphicsArrowItem::updatePosition()
{
    QLineF line(mapFromItem(mGraphicsPolygonItemStart, 0, 0), mapFromItem(mGraphicsPolygonItemEnd, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void GraphicsArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (mGraphicsPolygonItemStart->collidesWithItem(mGraphicsPolygonItemEnd))
        return;

    QPen myPen = pen();
    myPen.setColor(mColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(mColor);
//! [4] //! [5]

    QLineF centerLine(mGraphicsPolygonItemStart->pos(), mGraphicsPolygonItemEnd->pos());
    QPolygonF endPolygon = mGraphicsPolygonItemEnd->getPolygon();
    QPointF p1 = endPolygon.first() + mGraphicsPolygonItemEnd->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i) + mGraphicsPolygonItemEnd->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, mGraphicsPolygonItemStart->pos()));
//! [5] //! [6]

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                    cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                    cos(angle + Pi - Pi / 3) * arrowSize);

    mQPolygonF.clear();
    mQPolygonF << line().p1() << arrowP1 << arrowP2;
//! [6] //! [7]
    painter->drawLine(line());
    painter->drawPolygon(mQPolygonF);
    if (isSelected()) {
        painter->setPen(QPen(mColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
//! [7]

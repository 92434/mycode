#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "graphicspolygonitem.h"
#include "graphicstextitem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum sceneOpMode {
		Move = 0,
		Delete,
		Esc,
		InsertGraphicsPolygonItem,
		InsertQGraphicsLineItem,
		InsertTextGraphicsItem
	};

	explicit GraphicsScene(QObject *parent = 0);

	QFont font() const {
		return mFont;
	}

	QColor textColor() const {
		return mTextColor;
	}

	QColor itemColor() const {
		return mItemColor;
	}

	QColor lineColor() const {
		return mLineColor;
	}

	void setLineColor(const QColor &color);
	void setTextColor(const QColor &color);
	void setItemColor(const QColor &color);
	void setFont(const QFont &font);

public slots:
	void setInsertMode(sceneOpMode mode);
	void setGraphicsPolygonItemType(GraphicsPolygonItem::GraphicsPolygonType type);
	void textItemLostFocus(GraphicsTextItem *item);

signals:
	void itemInserted(GraphicsPolygonItem *item);
	void textInserted(QGraphicsTextItem *item);
	void itemSelected(QGraphicsItem *item);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void drawBackground(QPainter *painter, const QRectF &rect);
	void drawForeground(QPainter *painter, const QRectF &rect);

private:

	QFont mFont;
	QColor mTextColor;
	QColor mItemColor;
	QColor mLineColor;

	GraphicsPolygonItem::GraphicsPolygonType mGraphicsPolygonItemType;
	sceneOpMode mSceneOpMode;
	QGraphicsLineItem *mGraphicsLineItem;
	GraphicsTextItem *mTextGraphicsItem;
	bool bLeftButtonDown;
	bool isSelectedItemTypeMatch(int type);
};
//! [0]

#endif // GRAPHICSSCENE_H

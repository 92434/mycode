#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "graphicspolygonitem.h"
#include "graphicstextitem.h"

#include <QGraphicsScene>
#include <QPainter>

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

	qreal printXMax() {
		return mPrintXMax;
	}

	void setPrintXMax(qreal printXMax) {
		mPrintXMax = printXMax;
		update();
	}

	int zoom() {
		return mZoom;
	}

	void setZoom(int zoom) {
		mZoom = zoom;
	}

	int height() {
		return mHeight;
	}

	void setHeight(int height) {
		mHeight = height;
	}

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
	void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
	void drawForeground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:

	QFont mFont;
	QColor mTextColor;
	QColor mItemColor;
	QColor mLineColor;

	GraphicsPolygonItem::GraphicsPolygonType mGraphicsPolygonItemType;
	sceneOpMode mSceneOpMode;
	QGraphicsLineItem *mGraphicsLineItem;
	GraphicsTextItem *mTextGraphicsItem;

	int mZoom;
	int mHeight;
	int mRuleHeight;
	int mGridMargin;
	qreal mPrintXMax;
	QPointF mCurrentPoint;

	void drawBGGrid(QPainter *painter, const QRectF &rect);
	void drawBG(QPainter *painter, const QRectF &rect);
	void drawBGRule(QPainter *painter, const QRectF &rect);
	void drawFGPos(QPainter *painter, const QRectF &rect);
};
//! [0]

#endif // GRAPHICSSCENE_H

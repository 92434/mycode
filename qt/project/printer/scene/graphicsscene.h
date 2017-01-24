#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

//#include "graphicspolygonitem.h"
#include <QApplication>
#include "graphicstextitem.h"
#include "graphicsqritem.h"

#include <QGraphicsScene>
#include <QPainter>


#include "qrsetttingdialog.h"
#include "graphicsqritem.h"
#include "graphicstextitem.h"

class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum sceneOpMode {
		Move = 0,
		Delete,
		Esc,
		InsertGraphicsQRItem,
		InsertGraphicsTextItem
	};

	explicit GraphicsScene(QObject *parent = 0);

	qreal printXMax() {
		return mPrintXMax;
	}

	void setPrintXMax(qreal printXMax) {
		mPrintXMax = printXMax;
		mVisualRect.setWidth(mPrintXMax * mZoom);
		update();
	}

	int zoom() {
		return mZoom;
	}

	void setZoom(int zoom) {
		mZoom = zoom;
		update();
	}

	int height() {
		return mHeight;
	}

	void setHeight(int height) {
		mHeight = height;
		mVisualRect.setHeight(mHeight * mZoom);
		update();
	}

	int itemHeight() {
		return mItemHeight;
	}

	void setItemHeight(int height) {
		mItemHeight = height;
	}

	int itemWidth() {
		return mItemWidth;
	}

	void setItemWidth(int width) {
		mItemWidth = width;
	}

	bool isInPrinting() {
		return mInPrinting;
	}

	void setInPrinting(bool inPrinting) {
		mInPrinting = inPrinting;
		update();
	}

	QRectF visualRect() {
		return mVisualRect;
	}

	void moveSelectedItems(int type);
	void deselectItems();
	void deleteSelectedItems();

public slots:
	void setInsertMode(sceneOpMode mode);

signals:
	void itemInserted(QGraphicsItem *item);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
	void drawForeground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:
	sceneOpMode mSceneOpMode;


	//QRSetttingDialog *mQRSettingDialog;

	GraphicsQRItem *mGraphicsQRItem;
	GraphicsTextItem *mGraphicsTextItem;

	int mZoom;
	int mHeight;
	int mRuleHeight;
	int mGridMargin;

	QPointF mCurrentPoint;

	qreal mPrintXMax;
	QRectF mVisualRect;

	bool mInPrinting;

	int mItemWidth;
	int mItemHeight;

	void drawBGGrid(QPainter *painter, const QRectF &rect);
	void drawBG(QPainter *painter, const QRectF &rect);
	void drawBGRule(QPainter *painter, const QRectF &rect);
	void drawFGPos(QPainter *painter, const QRectF &rect);

	void insertItem(QPointF pos);
	GraphicsQRItem *prepareQRItem(GraphicsQRItem *originItem = 0, bool allocItem = true);
	GraphicsTextItem *prepareTextItem(GraphicsTextItem *originItem = 0, bool allocItem = true);
};

#endif // GRAPHICSSCENE_H

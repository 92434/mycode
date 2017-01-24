#ifndef GRAPHICSQRITEM_H
#define GRAPHICSQRITEM_H
#include <QGraphicsItem>
#include <qzint.h>
#include <QComboBox>

class GraphicsQRItem : public QGraphicsItem
{

public:
	enum {
		Type = UserType + 1,
	};

	GraphicsQRItem();
	~GraphicsQRItem() {}

	int type() const Q_DECL_OVERRIDE {
		return Type;
	}

	QRectF boundingRect() const Q_DECL_OVERRIDE;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	int width() {
		return mWidth;
	}
	void setWidth(int w) {
		mWidth = w;
	}

	int height() {
		return mHeight;
	}

	void setHeight(int h) {
		mHeight = h;

		switch(mSymbol) {
			case BARCODE_QRCODE:
				setWidth(mHeight);
				break;
			default:
				break;
		}
	}

	int zoom() {
		return mZoom;
	}

	void setZoom(int zoom) {
		mZoom = zoom;
	}

	int rotate() {
		return mRotate;
	}

	void setRotate(int rotate) {
		mRotate = rotate;
	}

	bool mirrorHorizontal() {
		return mMirrorHorizontal;
	}

	void setMirrorHorizontal(bool b) {
		mMirrorHorizontal = b;
	}

	bool mirrorVertical() {
		return mMirrorvertical;
	}

	void setMirrorVertical(bool b) {
		mMirrorvertical = b;
	}

	bool lock() {
		return mLock;
	}

	void setLock(bool b) {
		mLock = b;
	}

	int symbol() {
		return mSymbol;
	}

	void setSymbol(int symbol) {
		mSymbol = symbol;
	}

	QColor FGColor() {
		return mFgcolor;
	}

	void setFGColor(QColor color) {
		mFgcolor = color;
	}

	QColor BGColor() {
		return mBgcolor;
	}

	void setBGColor(QColor color) {
		mBgcolor = color;
	}

	void updateItem();

signals:
	//void lostFocus(GraphicsTextItem *item);
	//void selectedChange(QGraphicsItem *item);

protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	//void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
	//void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
	int mWidth;
	int mHeight;
	int mZoom;
	int mRotate;
	bool mMirrorHorizontal;
	bool mMirrorvertical;
	bool mLock;
	int mSymbol;

	QColor mFgcolor;
	QColor mBgcolor;

	Zint::QZint mZint;
	Zint::QZint::AspectRatioMode mZintAspectRationMode;

	void setZint();
};
#endif

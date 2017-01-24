#ifndef TEXTGRAPHICSITEM_H
#define TEXTGRAPHICSITEM_H

#include <QGraphicsTextItem>
#include <QPen>
//#include <QFont>

#include "../settingdialogs/textsettingdialog.h"
#include "../utils/xiaofei_debug.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class GraphicsTextItem : public QGraphicsTextItem
{
	Q_OBJECT

public:
	enum { Type = UserType + 3 };

	GraphicsTextItem(QGraphicsItem *parent = 0);

	int type() const Q_DECL_OVERRIDE {
		return Type;
	}

	QString text() {
		return mText;
	}

	void setText(QString text) {
		mText = text;
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

	QColor color() {
		return mColor;
	}

	void setColor(QColor color) {
		mColor = color;
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
	QString mText;
	int mRotate;
	bool mMirrorHorizontal;
	bool mMirrorvertical;
	bool mLock;
	QColor mColor;
};

#endif // TEXTGRAPHICSITEM_H

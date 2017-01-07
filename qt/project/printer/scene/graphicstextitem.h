#ifndef TEXTGRAPHICSITEM_H
#define TEXTGRAPHICSITEM_H

#include <QGraphicsTextItem>
#include <QPen>
#include <QFont>

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

	static QString text() {
		return mText;
	}

	static void setText(QString text) {
		mText = text;
	}

	static int rotate() {
		return mRotate;
	}

	static void setRotate(int rotate) {
		mRotate = rotate;
	}

	static bool mirrorHorizontal() {
		return mMirrorHorizontal;
	}

	static void setMirrorHorizontal(bool b) {
		mMirrorHorizontal = b;
	}

	static bool mirrorVertical() {
		return mMirrorvertical;
	}

	static void setMirrorVertical(bool b) {
		mMirrorvertical = b;
	}

	static bool lock() {
		return mLock;
	}

	static void setLock(bool b) {
		mLock = b;
	}

	static QColor color() {
		return mColor;
	}

	static void setColor(QColor color) {
		mColor = color;
	}

	static QFont font() {
		return mFont;
	}

	static void setFont(QFont font) {
		mFont = font;
	}

signals:
	//void lostFocus(GraphicsTextItem *item);
	//void selectedChange(QGraphicsItem *item);

protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	//void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
	static QString mText;
	static int mRotate;
	static bool mMirrorHorizontal;
	static bool mMirrorvertical;
	static bool mLock;
	static QColor mColor;
	static QFont mFont;
	void updateGraphicsTextItem();
	void initGraphicsTextItem();
};

#endif // TEXTGRAPHICSITEM_H

#ifndef TEXTGRAPHICSITEM_H
#define TEXTGRAPHICSITEM_H

#include <QGraphicsTextItem>
#include <QPen>
#include <QFont>

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

	QColor fgColor() {
		return mFgColor;
	}

	void setFgColor(QColor color) {
		mFgColor = color;
	}

	QColor bgColor() {
		return mBgColor;
	}

	void setBgColor(QColor color) {
		mBgColor = color;
	}

	QFont font() {
		return mFont;
	}

	void setFont(QFont font) {
		mFont = font;
	}

signals:
	//void lostFocus(GraphicsTextItem *item);
	//void selectedChange(QGraphicsItem *item);

protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	//void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
	//void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
	static QString mText;
	static QColor mFgColor;
	static QColor mBgColor;
	static QFont mFont;
};

#endif // TEXTGRAPHICSITEM_H

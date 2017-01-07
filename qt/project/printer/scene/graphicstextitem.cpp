#include "graphicstextitem.h"
#include "graphicsscene.h"


QString GraphicsTextItem::mText = QString("xiaofei");
int GraphicsTextItem::mRotate = 0;
bool GraphicsTextItem::mMirrorHorizontal = false;
bool GraphicsTextItem::mMirrorvertical = false;
bool GraphicsTextItem::mLock = true;
QColor GraphicsTextItem::mColor = QColor(Qt::black);
QFont GraphicsTextItem::mFont = QFont();

GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
	updateGraphicsTextItem();
	initGraphicsTextItem();
}

void GraphicsTextItem::updateGraphicsTextItem()
{
	TextSettingDialog *dialog = new TextSettingDialog;

    dialog->setText(text());
    dialog->setRotate(rotate());
    dialog->setMirrorHorizontal(mirrorHorizontal());
    dialog->setMirrorVertical(mirrorVertical());
    dialog->setLock(lock());

    if(font().family() != QString()) {
        dialog->setFont(font());
	}

	if(dialog->exec() == QDialog::Accepted) {
        setText(dialog->text());
        setRotate(dialog->rotate());
        setMirrorHorizontal(dialog->mirrorHorizontal());
        setMirrorVertical(dialog->mirrorVertical());
        setLock(dialog->lock());
        setFont(dialog->font());
	}
}

void GraphicsTextItem::initGraphicsTextItem()
{

	setPlainText(mText);

	resetMatrix();

	if(mMirrorHorizontal) {
		setMatrix(QMatrix(-1, 0, 0, 1, 0, 0), true);
	}

	if(mMirrorvertical) {
		setMatrix(QMatrix(1, 0, 0, -1, 0, 0), true);
	}

	setRotation(mRotate);

	setFlag(QGraphicsItem::ItemIsMovable, !mLock);

	QGraphicsTextItem::setFont(mFont);

	setDefaultTextColor(mColor);

	setFlag(QGraphicsItem::ItemIsSelectable);

	setTextInteractionFlags(Qt::NoTextInteraction);
}

void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event = event;
	updateGraphicsTextItem();
	initGraphicsTextItem();
}

//QVariant GraphicsTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//	if (change == QGraphicsItem::ItemSelectedHasChanged) {
//		emit selectedChange(this);
//	}
//
//	return value;
//}
//
//void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
//{
//	setTextInteractionFlags(Qt::NoTextInteraction);
//	emit lostFocus(this);
//	QGraphicsTextItem::focusOutEvent(event);
//}
//
//void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
//	if (textInteractionFlags() == Qt::NoTextInteraction) {
//		setTextInteractionFlags(Qt::TextEditorInteraction);
//	}
//
//	QGraphicsTextItem::mouseDoubleClickEvent(event);
//}

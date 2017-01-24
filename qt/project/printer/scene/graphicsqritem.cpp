#include "graphicsqritem.h"
#include "xiaofei_debug.h"

GraphicsQRItem::GraphicsQRItem() : QGraphicsItem()
{
	mWidth = 17;
	mHeight = 17;
	mZoom = 3;
	mRotate = 0;
	mMirrorHorizontal = false;
	mMirrorvertical = false;
	mLock = true;
	mFgcolor = QColor(Qt::black);
	mBgcolor = QColor(Qt::white);
	mSymbol = BARCODE_QRCODE;

	updateItem();
}

void GraphicsQRItem::updateItem()
{
	resetMatrix();

	if(mMirrorHorizontal) {
		setMatrix(QMatrix(-1, 0, 0, 1, 0, 0), true);
	}

	if(mMirrorvertical) {
		setMatrix(QMatrix(1, 0, 0, -1, 0, 0), true);
	}

	setRotation(mRotate);

	setFlag(QGraphicsItem::ItemIsMovable, !mLock);

	setFlag(QGraphicsItem::ItemIsSelectable, true);

	setZint();

	printf("mMirrorHorizontal:%d, mMirrorvertical:%d, mRotate:%d, mLock:%d\n", mMirrorHorizontal, mMirrorvertical, mRotate, mLock);
}

QRectF GraphicsQRItem::boundingRect() const
{
	return QRectF(0, 0, mWidth * mZoom, mHeight * mZoom);
}

void GraphicsQRItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
	if(isSelected()) {
		mZint.setFgColor(mBgcolor);
		mZint.setBgColor(mFgcolor);
	} else {
		mZint.setFgColor(mFgcolor);
		mZint.setBgColor(mBgcolor);
	}

	mZint.render(*painter, boundingRect(), mZintAspectRationMode);
}

void GraphicsQRItem::setZint()
{
	mZintAspectRationMode = (Zint::QZint::AspectRatioMode)1;

	//if(chkComposite->isChecked() == true) {
	//	bc.setPrimaryMessage(txtData->text());
	//	bc.setText(txtComposite->toPlainText());
	//} else {
	//	bc.setText(txtData->text());
	//	/*bc.setPrimaryMessage(txtComposite->text());*/
	//}
	mZint.setText("https://www.baidu.com");

	mZint.setSecurityLevel(0);
	mZint.setWidth(0);
	mZint.setInputMode(UNICODE_MODE);
	mZint.setHideText(false);

	//if(chkHRTHide->isChecked() == false) {
	//	bc.setHideText(true);
	//}

	mZint.setSymbol(mSymbol);

	//if(chkComposite->isChecked()) {
	//	bc.setSecurityLevel(cmbCompType->currentIndex());
	//}

	//bc.setBorderType((Zint::QZint::BorderType)(btype->currentIndex() * 2));
	mZint.setBorderType((Zint::QZint::BorderType)(0 * 2));
	//bc.setBorderWidth(bwidth->value());
	mZint.setBorderWidth(0);
	//bc.setHeight(heightb->value());
	mZint.setHeight(50);
	//bc.setWhitespace(spnWhitespace->value());
	mZint.setWhitespace(0);
	mZint.setFgColor(mFgcolor);
	mZint.setBgColor(mBgcolor);
}

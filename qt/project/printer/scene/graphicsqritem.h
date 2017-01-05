#ifndef GRAPHICSQRITEM_H
#define GRAPHICSQRITEM_H
#include <QGraphicsItem>
#include <qzint.h>

typedef struct _type_name {
	int type;
	QString name;
} style_name_t;

enum BarcodeTypes {
	NAME_TYPE_BARCODE_AUSREDIRECT = 68,
	NAME_TYPE_BARCODE_AUSREPLY = 66,
	NAME_TYPE_BARCODE_AUSROUTE = 67,
	NAME_TYPE_BARCODE_AUSPOST = 63,
	NAME_TYPE_BARCODE_AZTEC = 92,
	NAME_TYPE_BARCODE_AZRUNE = 128,
	NAME_TYPE_BARCODE_CHANNEL = 140,
	NAME_TYPE_BARCODE_CODABAR = 18,
	NAME_TYPE_BARCODE_CODE11 = 1,
	NAME_TYPE_BARCODE_CODE128 = 20,
	NAME_TYPE_BARCODE_CODE16K = 23,
	NAME_TYPE_BARCODE_C25LOGIC = 6,
	NAME_TYPE_BARCODE_C25IATA = 4,
	NAME_TYPE_BARCODE_C25IND = 7,
	NAME_TYPE_BARCODE_C25INTER = 3,
	NAME_TYPE_BARCODE_C25MATRIX = 2,
	NAME_TYPE_BARCODE_CODE32 = 129,
	NAME_TYPE_BARCODE_CODE39 = 8,
	NAME_TYPE_BARCODE_EXCODE39 = 9,
	NAME_TYPE_BARCODE_CODE49 = 24,
	NAME_TYPE_BARCODE_CODE93 = 25,
	NAME_TYPE_BARCODE_CODE_ONE = 141,
	NAME_TYPE_BARCODE_RSS14 = 29,
	NAME_TYPE_BARCODE_RSS_EXP = 31,
	NAME_TYPE_BARCODE_RSS_EXPSTACK = 81,
	NAME_TYPE_BARCODE_RSS_LTD = 30,
	NAME_TYPE_BARCODE_RSS14STACK = 79,
	NAME_TYPE_BARCODE_RSS14STACK_OMNI = 80,
	NAME_TYPE_BARCODE_DATAMATRIX = 71,
	NAME_TYPE_BARCODE_DPIDENT = 22,
	NAME_TYPE_BARCODE_DPLEIT = 21,
	NAME_TYPE_BARCODE_KIX = 90,
	NAME_TYPE_BARCODE_EAN14 = 72,
	NAME_TYPE_BARCODE_EANX = 13,
	NAME_TYPE_BARCODE_FIM = 49,
	NAME_TYPE_BARCODE_FLAT = 28,
	NAME_TYPE_BARCODE_GRIDMATRIX = 142,
	NAME_TYPE_BARCODE_ITF14 = 89,
	NAME_TYPE_BARCODE_ISBNX = 69,
	NAME_TYPE_BARCODE_JAPANPOST = 76,
	NAME_TYPE_BARCODE_KOREAPOST = 77,
	NAME_TYPE_BARCODE_LOGMARS = 50,
	NAME_TYPE_BARCODE_MAXICODE = 57,
	NAME_TYPE_BARCODE_MICROPDF417 = 84,
	NAME_TYPE_BARCODE_MICROQR = 97,
	NAME_TYPE_BARCODE_MSI_PLESSEY = 47,
	NAME_TYPE_BARCODE_NVE18 = 75,
	NAME_TYPE_BARCODE_PDF417 = 55,
	NAME_TYPE_BARCODE_PHARMA = 51,
	NAME_TYPE_BARCODE_PHARMA_TWO = 53,
	NAME_TYPE_BARCODE_PZN = 52,
	NAME_TYPE_BARCODE_PLANET = 82,
	NAME_TYPE_BARCODE_POSTNET = 40,
	NAME_TYPE_BARCODE_QRCODE = 58,
	NAME_TYPE_BARCODE_RM4SCC = 70,
	NAME_TYPE_BARCODE_TELEPEN = 32,
	NAME_TYPE_BARCODE_TELEPEN_NUM = 87,
	NAME_TYPE_BARCODE_PLESSEY = 86,
	NAME_TYPE_BARCODE_UPCA = 34,
	NAME_TYPE_BARCODE_UPCE = 37,
	NAME_TYPE_BARCODE_ONECODE = 85
};

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
	int getWidth() {
		return mW;
	}
	void setWidth(int w) {
		mW = w;
	}

	int getHeight() {
		return mH;
	}

	void setHeight(int h) {
		mH = h;
	}

	int zoom() {
		return mZoom;
	}

	void setZoom(int zoom) {
		mZoom = zoom;
	}

signals:
	//void lostFocus(GraphicsTextItem *item);
	//void selectedChange(QGraphicsItem *item);

protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	//void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
	//void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
	Zint::QZint bc;
	Zint::QZint::AspectRatioMode ar;
	int mW;
	int mH;
	int mZoom;
	std::vector<style_name_t> vecStyleName;
	int currentType;

	QColor m_fgcolor;
	QColor m_bgcolor;

	void init();
};
#endif

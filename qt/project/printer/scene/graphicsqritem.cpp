#include "graphicsqritem.h"
#include "xiaofei_debug.h"

GraphicsQRItem::GraphicsQRItem() : QGraphicsItem()
{
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);

	w = 45;
	h = 45;
	currentType = 53;
	m_fgcolor = QColor(Qt::black);
	m_bgcolor = QColor(Qt::white);

	init();
}


QRectF GraphicsQRItem::boundingRect() const
{
	return QRectF(0, 0, w, h);
}

void GraphicsQRItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
	if(isSelected()) {
		//printf("\n");
	} else {
		//printf("\n");
	}

	bc.render(*painter, boundingRect(), ar);
}

void GraphicsQRItem::init()
{
	style_name_t styleNames[] = {
		{NAME_TYPE_BARCODE_AUSREDIRECT, "Australia Post Redirect Code"},
		{NAME_TYPE_BARCODE_AUSREPLY, "Australia Post Reply-Paid"},
		{NAME_TYPE_BARCODE_AUSROUTE, "Australia Post Routing Code"},
		{NAME_TYPE_BARCODE_AUSPOST, "Australia Post Standard Customer"},
		{NAME_TYPE_BARCODE_AZTEC, "Aztec Code (ISO 24778)"},
		{NAME_TYPE_BARCODE_AZRUNE, "Aztec Runes"},
		{NAME_TYPE_BARCODE_CHANNEL, "Channel Code"},
		{NAME_TYPE_BARCODE_CODABAR, "Codabar"},
		{NAME_TYPE_BARCODE_CODE11, "Code 11"},
		{NAME_TYPE_BARCODE_CODE128, "Code 128 (ISO 15417)"},
		{NAME_TYPE_BARCODE_CODE16K, "Code 16k"},
		{NAME_TYPE_BARCODE_C25LOGIC, "Code 2 of 5 Data Logic"},
		{NAME_TYPE_BARCODE_C25IATA, "Code 2 of 5 IATA"},
		{NAME_TYPE_BARCODE_C25IND, "Code 2 of 5 Industrial"},
		{NAME_TYPE_BARCODE_C25INTER, "Code 2 of 5 Interleaved"},
		{NAME_TYPE_BARCODE_C25MATRIX, "Code 2 of 5 Matrix"},
		{NAME_TYPE_BARCODE_CODE32, "Code 32 (Italian Pharmacode)"},
		{NAME_TYPE_BARCODE_CODE39, "Code 39 (ISO 16388)"},
		{NAME_TYPE_BARCODE_EXCODE39, "Code 39 Extended"},
		{NAME_TYPE_BARCODE_CODE49, "Code 49"},
		{NAME_TYPE_BARCODE_CODE93, "Code 93"},
		{NAME_TYPE_BARCODE_CODE_ONE, "Code One"},
		{NAME_TYPE_BARCODE_RSS14, "Databar"},
		{NAME_TYPE_BARCODE_RSS_EXP, "Databar Expanded"},
		{NAME_TYPE_BARCODE_RSS_EXPSTACK, "Databar Expanded Stacked"},
		{NAME_TYPE_BARCODE_RSS_LTD, "Databar Limited"},
		{NAME_TYPE_BARCODE_RSS14STACK, "Databar Stacked"},
		{NAME_TYPE_BARCODE_RSS14STACK_OMNI, "Databar Stacked Omnidirectional"},
		{NAME_TYPE_BARCODE_DATAMATRIX, "Data Matrix (ISO 16022)"},
		{NAME_TYPE_BARCODE_DPIDENT, "Deutsche Post Identcode"},
		{NAME_TYPE_BARCODE_DPLEIT, "Deutsche Post Leitcode"},
		{NAME_TYPE_BARCODE_KIX, "Dutch Post KIX"},
		{NAME_TYPE_BARCODE_EAN14, "EAN-14"},
		{NAME_TYPE_BARCODE_EANX, "European Article Number (EAN)"},
		{NAME_TYPE_BARCODE_FIM, "Facing Identification Mark (FIM)"},
		{NAME_TYPE_BARCODE_FLAT, "Flattermarken"},
		{NAME_TYPE_BARCODE_GRIDMATRIX, "Grid Matrix"},
		{NAME_TYPE_BARCODE_ITF14, "ITF-14"},
		{NAME_TYPE_BARCODE_ISBNX, "International Standard Book Number (ISBN)"},
		{NAME_TYPE_BARCODE_JAPANPOST, "Japanese Postal Barcode"},
		{NAME_TYPE_BARCODE_KOREAPOST, "Korean Postal Barcode"},
		{NAME_TYPE_BARCODE_LOGMARS, "LOGMARS"},
		{NAME_TYPE_BARCODE_MAXICODE, "Maxicode (ISO 16023)"},
		{NAME_TYPE_BARCODE_MICROPDF417, "MicroPDF417 (ISO 24728)"},
		{NAME_TYPE_BARCODE_MICROQR, "Micro QR Code"},
		{NAME_TYPE_BARCODE_MSI_PLESSEY, "MSI Plessey"},
		{NAME_TYPE_BARCODE_NVE18, "NVE-18"},
		{NAME_TYPE_BARCODE_PDF417, "PDF417 (ISO 15438)"},
		{NAME_TYPE_BARCODE_PHARMA, "Pharmacode"},
		{NAME_TYPE_BARCODE_PHARMA_TWO, "Pharmacode 2-track"},
		{NAME_TYPE_BARCODE_PZN, "Pharma Zentralnummer (PZN)"},
		{NAME_TYPE_BARCODE_PLANET, "PLANET"},
		{NAME_TYPE_BARCODE_POSTNET, "Postnet"},
		{NAME_TYPE_BARCODE_QRCODE, "QR Code (ISO 18004)"},
		{NAME_TYPE_BARCODE_RM4SCC, "Royal Mail 4-state Barcode"},
		{NAME_TYPE_BARCODE_TELEPEN, "Telepen"},
		{NAME_TYPE_BARCODE_TELEPEN_NUM, "Telepen Numeric"},
		{NAME_TYPE_BARCODE_PLESSEY, "UK Plessey"},
		{NAME_TYPE_BARCODE_UPCA, "Universal Product Code (UPC-A)"},
		{NAME_TYPE_BARCODE_UPCE, "Universal Product Code (UPC-E)"},
		{NAME_TYPE_BARCODE_ONECODE, "USPS Intelligent Mail"},
	};


	for (unsigned int i = 0; i < sizeof(styleNames) / sizeof(style_name_t); i++) {
		vecStyleName.push_back(styleNames[i]);
	}

	ar = (Zint::QZint::AspectRatioMode)1;

	//if(chkComposite->isChecked() == true) {
	//	bc.setPrimaryMessage(txtData->text());
	//	bc.setText(txtComposite->toPlainText());
	//} else {
	//	bc.setText(txtData->text());
	//	/*bc.setPrimaryMessage(txtComposite->text());*/
	//}
	bc.setText("https://www.baidu.com");

	bc.setSecurityLevel(0);
	bc.setWidth(0);
	bc.setInputMode(UNICODE_MODE);
	bc.setHideText(false);

	//if(chkHRTHide->isChecked() == false) {
	//	bc.setHideText(true);
	//}

	switch(vecStyleName.at(currentType).type) {
		case NAME_TYPE_BARCODE_CODE128:

			//if(m_optionWidget->findChild<QRadioButton *>("radC128Stand")->isChecked()) {
			//	bc.setSymbol(BARCODE_CODE128);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radC128CSup")->isChecked()) {
			//	bc.setSymbol(BARCODE_CODE128B);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radC128EAN")->isChecked()) {
			//	if(chkComposite->isChecked()) {
			//		bc.setSymbol(BARCODE_EAN128_CC);
			//	} else {
			//		bc.setSymbol(BARCODE_EAN128);
			//	}
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radC128HIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_128);
			//}

			break;

		case NAME_TYPE_BARCODE_EANX:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_EANX_CC);
			//} else {
			//	bc.setSymbol(BARCODE_EANX);
			//}

			break;

		case NAME_TYPE_BARCODE_UPCA:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_UPCA_CC);
			//} else {
			//	bc.setSymbol(BARCODE_UPCA);
			//}

			break;

		case NAME_TYPE_BARCODE_UPCE:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_UPCE_CC);
			//} else {
			//	bc.setSymbol(BARCODE_UPCE);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS14:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS14_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS14);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS_LTD:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS_LTD_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS_LTD);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS_EXP:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS_EXP_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS_EXP);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS14STACK:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS14STACK_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS14STACK);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS14STACK_OMNI:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS14_OMNI_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS14STACK_OMNI);
			//}

			break;

		case NAME_TYPE_BARCODE_RSS_EXPSTACK:

			//if(chkComposite->isChecked()) {
			//	bc.setSymbol(BARCODE_RSS_EXPSTACK_CC);
			//} else {
			//	bc.setSymbol(BARCODE_RSS_EXPSTACK);
			//}

			//if(m_optionWidget->findChild<QComboBox *>("cmbCols")->currentIndex() != 0) {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbCols")->currentIndex());
			//}

			break;

		case NAME_TYPE_BARCODE_PDF417:
			//bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbPDFCols")->currentIndex());
			//bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbPDFECC")->currentIndex() - 1);
			//bc.setPdf417CodeWords(m_optionWidget->findChild<QSpinBox *>("codewords")->value());

			//if(m_optionWidget->findChild<QRadioButton *>("radPDFStand")->isChecked()) {
			//	bc.setSymbol(BARCODE_PDF417);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radPDFTruncated")->isChecked()) {
			//	bc.setSymbol(BARCODE_PDF417TRUNC);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radPDFHIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_PDF);
			//}

			break;

		case NAME_TYPE_BARCODE_MICROPDF417:
			//bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbMPDFCols")->currentIndex());

			//if(m_optionWidget->findChild<QRadioButton *>("radMPDFStand")->isChecked()) {
			//	bc.setSymbol(BARCODE_MICROPDF417);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radMPDFHIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_MICPDF);
			//}

			break;

		case NAME_TYPE_BARCODE_AZTEC:
			//bc.setSymbol(BARCODE_AZTEC);

			//if(m_optionWidget->findChild<QRadioButton *>("radAztecSize")->isChecked()) {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbAztecSize")->currentIndex() + 1);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radAztecECC")->isChecked()) {
			//	bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbAztecECC")->currentIndex() + 1);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radAztecGS1")->isChecked()) {
			//	bc.setInputMode(GS1_MODE);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radAztecHIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_AZTEC);
			//}

			break;

		case NAME_TYPE_BARCODE_MSI_PLESSEY:
			//bc.setSymbol(BARCODE_MSI_PLESSEY);
			//bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbMSICheck")->currentIndex());
			break;

		case NAME_TYPE_BARCODE_CODE39:

			//if(m_optionWidget->findChild<QRadioButton *>("radC39HIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_39);
			//} else {
			//	bc.setSymbol(BARCODE_CODE39);

			//	if(m_optionWidget->findChild<QRadioButton *>("radC39Check")->isChecked()) {
			//		bc.setWidth(1);
			//	}
			//}

			break;

		case NAME_TYPE_BARCODE_EXCODE39:
			//bc.setSymbol(BARCODE_EXCODE39);

			//if(m_optionWidget->findChild<QRadioButton *>("radC39Check")->isChecked()) {
			//	bc.setWidth(1);
			//}

			break;
		case NAME_TYPE_BARCODE_CODE16K:
			//bc.setSymbol(BARCODE_CODE16K);

			//if(m_optionWidget->findChild<QRadioButton *>("radC16kStand")->isChecked()) {
			//	bc.setInputMode(UNICODE_MODE);
			//} else {
			//	bc.setInputMode(GS1_MODE);
			//}

			break;

		case NAME_TYPE_BARCODE_DATAMATRIX:
			//bc.setSecurityLevel(1);

			//if(m_optionWidget->findChild<QRadioButton *>("radDM200HIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_DM);
			//} else {
			//	bc.setSymbol(BARCODE_DATAMATRIX);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radDM200GS1")->isChecked()) {
			//	bc.setInputMode(GS1_MODE);
			//}

			//bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbDM200Size")->currentIndex());

			//if(m_optionWidget->findChild<QCheckBox *>("chkDMRectangle")->isChecked()) {
			//	bc.setOption3(DM_SQUARE);
			//} else {
			//	bc.setOption3(0);
			//}

			break;

		case NAME_TYPE_BARCODE_QRCODE:
			bc.setSymbol(BARCODE_QRCODE);

			//if(m_optionWidget->findChild<QRadioButton *>("radQRHIBC")->isChecked()) {
			//	bc.setSymbol(BARCODE_HIBC_QR);
			//} else {
			//	bc.setSymbol(BARCODE_QRCODE);
			//}
			bc.setSymbol(BARCODE_QRCODE);

			//if(m_optionWidget->findChild<QRadioButton *>("radQRGS1")->isChecked()) {
			//	bc.setInputMode(GS1_MODE);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radQRSize")->isChecked()) {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbQRSize")->currentIndex() + 1);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radQRECC")->isChecked()) {
			//	bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbQRECC")->currentIndex() + 1);
			//}

			break;

		case NAME_TYPE_BARCODE_MICROQR:
			//bc.setSymbol(BARCODE_MICROQR);

			//if(m_optionWidget->findChild<QRadioButton *>("radMQRSize")->isChecked()) {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbMQRSize")->currentIndex());
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radMQRECC")->isChecked()) {
			//	bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbMQRECC")->currentIndex() + 1);
			//}

			break;

		case NAME_TYPE_BARCODE_GRIDMATRIX:
			//bc.setSymbol(BARCODE_GRIDMATRIX);

			//if(m_optionWidget->findChild<QRadioButton *>("radGridSize")->isChecked()) {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbGridSize")->currentIndex() + 1);
			//}

			//if(m_optionWidget->findChild<QRadioButton *>("radGridECC")->isChecked()) {
			//	bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbGridECC")->currentIndex() + 1);
			//}

			break;

		case NAME_TYPE_BARCODE_MAXICODE:
			//bc.setSymbol(BARCODE_MAXICODE);

			//if(m_optionWidget->findChild<QComboBox *>("cmbMaxiMode")->currentIndex() == 0) {
			//	bc.setSecurityLevel(2);
			//	bc.setPrimaryMessage(m_optionWidget->findChild<QLineEdit *>("txtMaxiPrimary")->text());
			//} else {
			//	bc.setSecurityLevel(m_optionWidget->findChild<QComboBox *>("cmbMaxiMode")->currentIndex() + 3);
			//}

			break;

		case NAME_TYPE_BARCODE_CHANNEL:
			//bc.setSymbol(BARCODE_CHANNEL);

			//if(m_optionWidget->findChild<QComboBox *>("cmbChannel")->currentIndex() == 0) {
			//	bc.setWidth(0);
			//} else {
			//	bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbChannel")->currentIndex() + 2);
			//}

			break;

		case NAME_TYPE_BARCODE_CODE_ONE:
			//bc.setSymbol(BARCODE_CODEONE);

			//if(m_optionWidget->findChild<QRadioButton *>("radC1GS1")->isChecked()) {
			//	bc.setInputMode(GS1_MODE);
			//}

			//bc.setWidth(m_optionWidget->findChild<QComboBox *>("cmbC1Size")->currentIndex());

			break;

		case NAME_TYPE_BARCODE_CODE49:
			//bc.setSymbol(BARCODE_CODE49);

			//if(m_optionWidget->findChild<QRadioButton *>("radC49GS1")->isChecked()) {
			//	bc.setInputMode(GS1_MODE);
			//}

			break;

		default:
			bc.setSymbol(vecStyleName.at(currentType).type);
			break;
	}

	//if(chkComposite->isChecked()) {
	//	bc.setSecurityLevel(cmbCompType->currentIndex());
	//}

	//bc.setBorderType((Zint::QZint::BorderType)(btype->currentIndex() * 2));
	bc.setBorderType((Zint::QZint::BorderType)(0 * 2));
	//bc.setBorderWidth(bwidth->value());
	bc.setBorderWidth(0);
	//bc.setHeight(heightb->value());
	bc.setHeight(50);
	//bc.setWhitespace(spnWhitespace->value());
	bc.setWhitespace(0);
	bc.setFgColor(m_fgcolor);
	bc.setBgColor(m_bgcolor);
}

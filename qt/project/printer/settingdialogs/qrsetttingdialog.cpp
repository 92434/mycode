#include "qrsetttingdialog.h"
#include "ui_qrsetttingdialog.h"
#include "xiaofei_debug.h"

QRSetttingDialog::QRSetttingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::QRSetttingDialog)
{
	ui->setupUi(this);
}

QRSetttingDialog::~QRSetttingDialog()
{
	delete ui;
}

void QRSetttingDialog::setHeight(int height)
{
	mHeight = height;
	ui->lineEdit_height->setText(QString::number(mHeight));
}


void QRSetttingDialog::setRotate(int rotate)
{
	mRotate = rotate;

	switch(mRotate) {
		case 0:
			ui->radioButton_no_totate->setChecked(true);
			break;
		case 90:
			ui->radioButton_rotate_90->setChecked(true);
			break;
		case 180:
			ui->radioButton_rotate_180->setChecked(true);
			break;
		case 270:
			ui->radioButton_rorate_270->setChecked(true);
			break;
		default:
			break;
	}
}

void QRSetttingDialog::setMirrorHorizontal(bool b)
{
	mMirrorHorizontal = b;
	ui->checkBox_horizontal->setChecked(mMirrorHorizontal);
}

void QRSetttingDialog::setMirrorVertical(bool b)
{
	mMirrorVertical = b;
	ui->checkBox_vertical->setChecked(mMirrorVertical);
}

void QRSetttingDialog::setLock(bool b)
{
	mLock = b;
	ui->checkBox_lock->setChecked(mLock);
}

void QRSetttingDialog::on_pushButton_ok_clicked()
{
	accept();
}

void QRSetttingDialog::on_pushButton_cancel_clicked()
{
	reject();
}

void QRSetttingDialog::on_radioButton_no_totate_clicked()
{
	mRotate = 0;
}

void QRSetttingDialog::on_radioButton_rotate_90_clicked()
{
	mRotate = 90;
}

void QRSetttingDialog::on_radioButton_rotate_180_clicked()
{
	mRotate = 180;
}

void QRSetttingDialog::on_radioButton_rorate_270_clicked()
{
	mRotate = 270;
}

void QRSetttingDialog::on_checkBox_horizontal_clicked()
{
	mMirrorHorizontal = ui->checkBox_horizontal->isChecked();
}

void QRSetttingDialog::on_checkBox_vertical_clicked()
{
	mMirrorVertical = ui->checkBox_vertical->isChecked();
}

void QRSetttingDialog::on_checkBox_lock_clicked()
{
	mLock = ui->checkBox_lock->isChecked();
}

void QRSetttingDialog::setSymbol(int symbol)
{
    for (unsigned int i = 0; i < mVectorStyleName.size(); i++) {
        if(mVectorStyleName.at(i).type == symbol) {
			ui->comboBox_zint_type->setCurrentIndex(i);
			return;
		}
	}
}

void QRSetttingDialog::on_comboBox_zint_type_currentIndexChanged(int index)
{
	updateSymbol(index);
}

void QRSetttingDialog::on_lineEdit_height_textChanged(const QString &arg1)
{
	int height = ui->lineEdit_height->text().toInt();

	if(height != 0) {
		mHeight = height;
	}
}

void QRSetttingDialog::addZintType()
{
	style_name_t styleNames[] = {
		{BARCODE_AUSREDIRECT, "Australia Post Redirect Code"},
		{BARCODE_AUSREPLY, "Australia Post Reply-Paid"},
		{BARCODE_AUSROUTE, "Australia Post Routing Code"},
		{BARCODE_AUSPOST, "Australia Post Standard Customer"},
		{BARCODE_AZTEC, "Aztec Code (ISO 24778)"},
		{BARCODE_AZRUNE, "Aztec Runes"},
		{BARCODE_CHANNEL, "Channel Code"},
		{BARCODE_CODABAR, "Codabar"},
		{BARCODE_CODE11, "Code 11"},
		{BARCODE_CODE128, "Code 128 (ISO 15417)"},
		{BARCODE_CODE16K, "Code 16k"},
        {BARCODE_C25LOGIC, "Code 2 of 5 Data Logic"},
		{BARCODE_C25IATA, "Code 2 of 5 IATA"},
		{BARCODE_C25IND, "Code 2 of 5 Industrial"},
		{BARCODE_C25INTER, "Code 2 of 5 Interleaved"},
		{BARCODE_C25MATRIX, "Code 2 of 5 Matrix"},
		{BARCODE_CODE32, "Code 32 (Italian Pharmacode)"},
		{BARCODE_CODE39, "Code 39 (ISO 16388)"},
		{BARCODE_EXCODE39, "Code 39 Extended"},
		{BARCODE_CODE49, "Code 49"},
		{BARCODE_CODE93, "Code 93"},
        {BARCODE_CODEONE, "Code One"},
		{BARCODE_RSS14, "Databar"},
		{BARCODE_RSS_EXP, "Databar Expanded"},
		{BARCODE_RSS_EXPSTACK, "Databar Expanded Stacked"},
		{BARCODE_RSS_LTD, "Databar Limited"},
		{BARCODE_RSS14STACK, "Databar Stacked"},
		{BARCODE_RSS14STACK_OMNI, "Databar Stacked Omnidirectional"},
		{BARCODE_DATAMATRIX, "Data Matrix (ISO 16022)"},
		{BARCODE_DPIDENT, "Deutsche Post Identcode"},
		{BARCODE_DPLEIT, "Deutsche Post Leitcode"},
		{BARCODE_KIX, "Dutch Post KIX"},
		{BARCODE_EAN14, "EAN-14"},
		{BARCODE_EANX, "European Article Number (EAN)"},
		{BARCODE_FIM, "Facing Identification Mark (FIM)"},
		{BARCODE_FLAT, "Flattermarken"},
		{BARCODE_GRIDMATRIX, "Grid Matrix"},
		{BARCODE_ITF14, "ITF-14"},
		{BARCODE_ISBNX, "International Standard Book Number (ISBN)"},
		{BARCODE_JAPANPOST, "Japanese Postal Barcode"},
		{BARCODE_KOREAPOST, "Korean Postal Barcode"},
		{BARCODE_LOGMARS, "LOGMARS"},
		{BARCODE_MAXICODE, "Maxicode (ISO 16023)"},
		{BARCODE_MICROPDF417, "MicroPDF417 (ISO 24728)"},
		{BARCODE_MICROQR, "Micro QR Code"},
		{BARCODE_MSI_PLESSEY, "MSI Plessey"},
		{BARCODE_NVE18, "NVE-18"},
		{BARCODE_PDF417, "PDF417 (ISO 15438)"},
		{BARCODE_PHARMA, "Pharmacode"},
		{BARCODE_PHARMA_TWO, "Pharmacode 2-track"},
		{BARCODE_PZN, "Pharma Zentralnummer (PZN)"},
		{BARCODE_PLANET, "PLANET"},
		{BARCODE_POSTNET, "Postnet"},
		{BARCODE_QRCODE, "QR Code (ISO 18004)"},
		{BARCODE_RM4SCC, "Royal Mail 4-state Barcode"},
		{BARCODE_TELEPEN, "Telepen"},
		{BARCODE_TELEPEN_NUM, "Telepen Numeric"},
		{BARCODE_PLESSEY, "UK Plessey"},
		{BARCODE_UPCA, "Universal Product Code (UPC-A)"},
		{BARCODE_UPCE, "Universal Product Code (UPC-E)"},
		{BARCODE_ONECODE, "USPS Intelligent Mail"},
	};

	for (unsigned int i = 0; i < sizeof(styleNames) / sizeof(style_name_t); i++) {
		mVectorStyleName.push_back(styleNames[i]);
		ui->comboBox_zint_type->addItem(styleNames[i].name);
	}
}


void QRSetttingDialog::updateSymbol(int index)
{
	switch(mVectorStyleName.at(index).type) {
		case BARCODE_CODE128:
			mSymbol = BARCODE_CODE128;
			//mSymbol = BARCODE_CODE128B;
			//mSymbol = BARCODE_EAN128_CC;
			//mSymbol = BARCODE_EAN128;
			//mSymbol = BARCODE_HIBC_128;

			break;

		case BARCODE_EANX:

			mSymbol = BARCODE_EANX;
			//mSymbol = BARCODE_EANX_CC;

			break;

		case BARCODE_UPCA:
			mSymbol = BARCODE_UPCA;
			//mSymbol = BARCODE_UPCA_CC;

			break;

		case BARCODE_UPCE:
			mSymbol = BARCODE_UPCE;
			//mSymbol = BARCODE_UPCE_CC;

			break;

		case BARCODE_RSS14:
			mSymbol = BARCODE_RSS14;
			//mSymbol = BARCODE_RSS14_CC;

			break;

		case BARCODE_RSS_LTD:
			mSymbol = BARCODE_RSS_LTD;
			//mSymbol = BARCODE_RSS_LTD_CC;

			break;

		case BARCODE_RSS_EXP:
			mSymbol = BARCODE_RSS_EXP;
			//mSymbol = BARCODE_RSS_EXP_CC;

			break;

		case BARCODE_RSS14STACK:
			mSymbol = BARCODE_RSS14STACK;
			//mSymbol = BARCODE_RSS14STACK_CC;

			break;

		case BARCODE_RSS14STACK_OMNI:
			mSymbol = BARCODE_RSS14STACK_OMNI;
			//mSymbol = BARCODE_RSS14_OMNI_CC;

			break;

		case BARCODE_RSS_EXPSTACK:
			mSymbol = BARCODE_RSS_EXPSTACK;
			//mSymbol = BARCODE_RSS_EXPSTACK_CC;

			break;

		case BARCODE_PDF417:
			mSymbol = BARCODE_PDF417;
			//mSymbol = BARCODE_PDF417TRUNC;
			//mSymbol = BARCODE_HIBC_PDF;

			break;

		case BARCODE_MICROPDF417:
			mSymbol = BARCODE_MICROPDF417;
			//mSymbol = BARCODE_HIBC_MICPDF;

			break;

		case BARCODE_AZTEC:
			mSymbol = BARCODE_AZTEC;
			//mSymbol = BARCODE_HIBC_AZTEC;

			break;

		case BARCODE_MSI_PLESSEY:
			mSymbol = BARCODE_MSI_PLESSEY;
			break;

		case BARCODE_CODE39:
			mSymbol = BARCODE_CODE39;
			//mSymbol = BARCODE_HIBC_39;

			break;

		case BARCODE_EXCODE39:
			mSymbol = BARCODE_EXCODE39;

			break;
		case BARCODE_CODE16K:
			mSymbol = BARCODE_CODE16K;

			break;

		case BARCODE_DATAMATRIX:
			mSymbol = BARCODE_HIBC_DM;
			//mSymbol = BARCODE_DATAMATRIX;

			break;

		case BARCODE_QRCODE:
			mSymbol = BARCODE_QRCODE;
			//mSymbol = BARCODE_HIBC_QR;

			break;

		case BARCODE_MICROQR:
			mSymbol = BARCODE_MICROQR;

			break;

		case BARCODE_GRIDMATRIX:
			mSymbol = BARCODE_GRIDMATRIX;

			break;

		case BARCODE_MAXICODE:
			mSymbol = BARCODE_MAXICODE;

			break;

		case BARCODE_CHANNEL:
			mSymbol = BARCODE_CHANNEL;

			break;

        case BARCODE_CODEONE:
			mSymbol = BARCODE_CODEONE;

			break;

		case BARCODE_CODE49:
			mSymbol = BARCODE_CODE49;

			break;

		default:
			mSymbol = BARCODE_CODE49;
			break;
	}
}

#include "textsettingdialog.h"
#include "ui_textsettingdialog.h"

TextSettingDialog::TextSettingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TextSettingDialog)
{
	ui->setupUi(this);
	ui->checkBox_diy_font->setCheckable(false);
	mFont = QApplication::font();
}

TextSettingDialog::~TextSettingDialog()
{
	delete ui;
}

QString TextSettingDialog::text()
{
	return mText;
}

void TextSettingDialog::setText(QString text)
{
	mText = text;
	ui->textEdit_text->setPlainText(mText);
}

int TextSettingDialog::rotate()
{
	return mRotate;
}

void TextSettingDialog::setRotate(int rotate)
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

bool TextSettingDialog::mirrorHorizontal()
{
	return mMirrorHorizontal;
}

void TextSettingDialog::setMirrorHorizontal(bool b)
{
	mMirrorHorizontal = b;
	ui->checkBox_horizontal->setChecked(mMirrorHorizontal);
}

bool TextSettingDialog::mirrorVertical()
{
	return mMirrorVertical;
}

void TextSettingDialog::setMirrorVertical(bool b)
{
	mMirrorVertical = b;
	ui->checkBox_vertical->setChecked(mMirrorVertical);
}

bool TextSettingDialog::lock()
{
	return mLock;
}

void TextSettingDialog::setLock(bool b)
{
	mLock = b;
	ui->checkBox_lock->setChecked(mLock);
}

void TextSettingDialog::setFont(QFont font)
{
	mFont = font;
}

QFont TextSettingDialog::font()
{
	return mFont;
}


void TextSettingDialog::on_pushButton_ok_clicked()
{
	accept();
}

void TextSettingDialog::on_pushButton_cancel_clicked()
{
	reject();
}

void TextSettingDialog::on_radioButton_no_totate_clicked()
{
	mRotate = 0;
}

void TextSettingDialog::on_radioButton_rotate_90_clicked()
{
	mRotate = 90;
}

void TextSettingDialog::on_radioButton_rotate_180_clicked()
{
	mRotate = 180;
}

void TextSettingDialog::on_radioButton_rorate_270_clicked()
{
	mRotate = 270;
}

void TextSettingDialog::on_textEdit_text_textChanged()
{
	mText = ui->textEdit_text->toPlainText();
}

void TextSettingDialog::on_checkBox_horizontal_clicked()
{
	mMirrorHorizontal = ui->checkBox_horizontal->isChecked();
}

void TextSettingDialog::on_checkBox_vertical_clicked()
{
	mMirrorVertical = ui->checkBox_vertical->isChecked();
}

void TextSettingDialog::on_checkBox_lock_clicked()
{
	mLock = ui->checkBox_lock->isChecked();
}

void TextSettingDialog::on_pushButton_font_clicked()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, mFont, this, tr("字体对话框"));

	if(ok) {
		mFont = font;
	}
}

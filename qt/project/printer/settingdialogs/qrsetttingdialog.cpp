#include "qrsetttingdialog.h"
#include "ui_qrsetttingdialog.h"

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

int QRSetttingDialog::rotate()
{
    return mRotate;
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

bool QRSetttingDialog::mirrorHorizontal()
{
    return mMirrorHorizontal;
}

void QRSetttingDialog::setMirrorHorizontal(bool b)
{
    mMirrorHorizontal = b;
    ui->checkBox_horizontal->setChecked(mMirrorHorizontal);
}

bool QRSetttingDialog::mirrorVertical()
{
    return mMirrorVertical;
}

void QRSetttingDialog::setMirrorVertical(bool b)
{
    mMirrorVertical = b;
    ui->checkBox_vertical->setChecked(mMirrorVertical);
}

bool QRSetttingDialog::lock()
{
    return mLock;
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

QComboBox *QRSetttingDialog::zintTypeBox() {
    return ui->comboBox_zint_type;
}

void QRSetttingDialog::on_comboBox_zint_type_currentIndexChanged(int index)
{
    mZintTypeIndex = index;
}

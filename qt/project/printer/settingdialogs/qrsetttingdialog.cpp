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

void QRSetttingDialog::on_pushButton_exit_clicked()
{
    close();
}

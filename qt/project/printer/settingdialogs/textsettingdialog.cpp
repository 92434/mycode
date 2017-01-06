#include "textsettingdialog.h"
#include "ui_textsettingdialog.h"

TextSettingDialog::TextSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextSettingDialog)
{
    ui->setupUi(this);
}

TextSettingDialog::~TextSettingDialog()
{
    delete ui;
}

void TextSettingDialog::on_pushButton_exit_clicked()
{
    close();
}

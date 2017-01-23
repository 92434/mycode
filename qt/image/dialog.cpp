#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Application *a, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QImage image;
    image.load( ":/640x480_test1.jpeg");
    ui->label->setPixmap(QPixmap::fromImage(image));
}

Dialog::~Dialog()
{
    delete ui;
}

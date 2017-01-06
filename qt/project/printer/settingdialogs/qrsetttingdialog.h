#ifndef QRSETTTINGDIALOG_H
#define QRSETTTINGDIALOG_H

#include <QDialog>

namespace Ui {
class QRSetttingDialog;
}

class QRSetttingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRSetttingDialog(QWidget *parent = 0);
    ~QRSetttingDialog();

private slots:
    void on_pushButton_exit_clicked();

private:
    Ui::QRSetttingDialog *ui;
};

#endif // QRSETTTINGDIALOG_H

#ifndef QRSETTTINGDIALOG_H
#define QRSETTTINGDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class QRSetttingDialog;
}

class QRSetttingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRSetttingDialog(QWidget *parent = 0);
    ~QRSetttingDialog();

    int rotate();
    void setRotate(int rotate);
    bool mirrorHorizontal();
    void setMirrorHorizontal(bool b);
    bool mirrorVertical();
    void setMirrorVertical(bool b);
    bool lock();
    void setLock(bool b);
    int zintTypeIndex() {
        return mZintTypeIndex;
    }

    QComboBox *zintTypeBox();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_radioButton_no_totate_clicked();

    void on_radioButton_rotate_90_clicked();

    void on_radioButton_rotate_180_clicked();

    void on_radioButton_rorate_270_clicked();

    void on_checkBox_horizontal_clicked();

    void on_checkBox_vertical_clicked();

    void on_checkBox_lock_clicked();

    void on_comboBox_zint_type_currentIndexChanged(int index);

private:
    Ui::QRSetttingDialog *ui;
    int mRotate;
    bool mMirrorHorizontal;
    bool mMirrorVertical;
    bool mLock;
    int mZintTypeIndex;
};

#endif // QRSETTTINGDIALOG_H

#ifndef TEXTSETTINGDIALOG_H
#define TEXTSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class TextSettingDialog;
}

class TextSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextSettingDialog(QWidget *parent = 0);
    ~TextSettingDialog();
    QString text() {
        return mText;
    }

    bool mirrorHorizontal() {
        return mMirrorHorizontal;
    }

    bool mirrorVertical() {
        return mMirrorVertical;
    }

    bool lock() {
        return mLock;
    }

    QFont font() {
        return mFont;
    }


private slots:
    void on_pushButton_exit_clicked();

private:
    Ui::TextSettingDialog *ui;
    QString mText;
    int mRotate;
    bool mMirrorHorizontal;
    bool mMirrorVertical;
    bool mLock;
    QFont mFont;
};

#endif // TEXTSETTINGDIALOG_H

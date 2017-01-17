#ifndef QRSETTTINGDIALOG_H
#define QRSETTTINGDIALOG_H

#include <QDialog>
#include <QComboBox>

#include "qzint.h"

namespace Ui
{
	class QRSetttingDialog;
}

typedef struct _type_name {
	int type;
	QString name;
} style_name_t;

class QRSetttingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit QRSetttingDialog(QWidget *parent = 0);
	~QRSetttingDialog();

	int rotate() {
		return mRotate;
	}
	void setRotate(int rotate);

	bool mirrorHorizontal() {
		return mMirrorHorizontal;
	}
	void setMirrorHorizontal(bool b);

	bool mirrorVertical() {
		return mMirrorVertical;
	}
	void setMirrorVertical(bool b);

	bool lock() {
		return mLock;
	}
	void setLock(bool b);

	int height() {
		return mHeight;
	}
	void setHeight(int height);

	int symbol() {
		return mSymbol;
	}
	void setSymbol(int symbol);

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

	void on_lineEdit_height_textChanged(const QString &arg1);

private:
	Ui::QRSetttingDialog *ui;
	int mHeight;
	int mRotate;
	bool mMirrorHorizontal;
	bool mMirrorVertical;
	bool mLock;
	int mSymbol;

	std::vector<style_name_t> mVectorStyleName;

	void updateSymbol(int index);
	void addZintType();
};

#endif // QRSETTTINGDIALOG_H

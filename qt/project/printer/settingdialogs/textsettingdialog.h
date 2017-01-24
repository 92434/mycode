#ifndef TEXTSETTINGDIALOG_H
#define TEXTSETTINGDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QFontDialog>
#include <QDebug>

namespace Ui
{
	class TextSettingDialog;
}

class TextSettingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TextSettingDialog(QWidget *parent = 0);
	~TextSettingDialog();
	QString text();
	void setText(QString text);
	int rotate();
	void setRotate(int rotate);
	bool mirrorHorizontal();
	void setMirrorHorizontal(bool b);
	bool mirrorVertical();
	void setMirrorVertical(bool b);
	bool lock();
	void setLock(bool b);
	QFont font();
	void setFont(QFont font);
private slots:
	void on_pushButton_ok_clicked();

	void on_pushButton_cancel_clicked();

	void on_radioButton_no_totate_clicked();

	void on_radioButton_rotate_90_clicked();

	void on_radioButton_rotate_180_clicked();

	void on_radioButton_rorate_270_clicked();

	void on_textEdit_text_textChanged();

	void on_checkBox_horizontal_clicked();

	void on_checkBox_vertical_clicked();

	void on_checkBox_lock_clicked();

	void on_pushButton_font_clicked();

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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QApplication>
#include <QFileDialog>
#include <QFontDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPageSetupDialog>
#include <QProgressDialog>

#include "../app/single_application.h"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(single_application *a, QWidget *parent = 0);

private slots:
	void changedFont(const QFont &);

Q_SIGNALS:
	void currentFontChanged(const QFont &);

private:
	QLabel *label;
	QApplication *a;

private:
};

#endif // DIALOG_H

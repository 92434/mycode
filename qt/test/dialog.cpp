#include "dialog.h"


QT_USE_NAMESPACE

Dialog::Dialog(single_application *a, QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;

	label = new QLabel("用此标签查看字体效果", this);
	label->setGeometry(0, 0, 200, 30);
	connect(this, SIGNAL(currentFontChanged(QFont)), this, SLOT(changedFont(QFont)));

	mainLayout->addWidget(label, 0, 0);


	setLayout(mainLayout);
	setWindowTitle(tr("test"));


	QPalette palette = label->palette(); //获取文本编辑的调色板对象
	//QColor color = QColorDialog::getColor(QPalette::Base, this);
	//颜色对话框的初始颜色值为文本编辑的背景色
	QColor color = QColorDialog::getColor(palette.color(QPalette::Base), this);

	//如果用户在颜色对话框点击取消的话，得到的color是无效的
	if(color.isValid()) {
		//QPalette::Base 通常用于背景色
		label->setAutoFillBackground(true);
		palette.setColor(QPalette::Base, color);
		palette.setColor(QPalette::Background, color);
		label->setPalette(palette);
	}

	QString FileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "./", tr("字库文件(*.ttf)"));
	a->addFont(FileName);

	bool ok;
	QFont font = QFontDialog::getFont(&ok, label->font(), this, tr("字体对话框"));

	if(ok) {
		label->setFont(font);
	}

	QPrinter printer;
	QPageSetupDialog pageDlg(&printer, this);
	pageDlg.setWindowTitle(tr("页面设置对话框"));

	if(pageDlg.exec() == QDialog::Accepted) {
		//
	}

	QProgressDialog progressDlg(tr("正在复制文件"), tr("取消"), 0, 10000, this);
	progressDlg.setWindowTitle(tr("进度条对话框"));
	//设置为模态对话框
	progressDlg.setWindowModality(Qt::WindowModal);
	//如果这里使用exec 进度对话框会卡主
	progressDlg.show();

	for(int i = 0; i < 10000; i++) {
		progressDlg.setValue(i);
		qApp->processEvents();

		if(progressDlg.wasCanceled()) {
			break;
		}
	}

	progressDlg.setValue(10000);
}

void Dialog::changedFont(const QFont &f)
{
	label->setFont(f);
}

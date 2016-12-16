#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;

private slots:
	void on_pushButton_8_clicked();

	void on_pushButton_7_clicked();

private:
	Ui::MainWindow *ui;
	int m_timerId;
};

#endif // MAINWINDOW_H

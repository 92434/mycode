#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QGraphicsView>
#include <QBoxLayout>

#include "scene/graphicsarrowitem.h"
#include "scene/graphicspolygonitem.h"
#include "scene/graphicsscene.h"
#include "scene/graphicstextitem.h"

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

Q_SIGNALS:
	void timeout(QTimerEvent *event);

protected:
	void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;

private slots:
	void datetimeUpdateTimeout(QTimerEvent *event);
	void itemInserted(GraphicsPolygonItem *item);
	void textInserted(QGraphicsTextItem *);
	void itemSelected(QGraphicsItem *item);

	void on_pushButton_exit_clicked();

	void on_pushButton_minimize_clicked();

	void on_pushButton_dec_print_length_clicked();

	void on_pushButton_inc_print_length_clicked();

	void on_lineEdit_print_length_editingFinished();

private:
	Ui::MainWindow *ui;
	int m_timerId1;

	GraphicsScene *scene_show_com;
	QHBoxLayout *layout_show_com;
	QGraphicsView *view_show_com;

	void initDatetime();
	void initFrameShowCom();
	void updateSceneSizeInfo();
};

#endif // MAINWINDOW_H

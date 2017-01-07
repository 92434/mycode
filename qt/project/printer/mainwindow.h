#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QGraphicsView>
#include <QBoxLayout>

#include "scene/graphicsarrowitem.h"
#include "scene/graphicspolygonitem.h"
#include "scene/graphicsscene.h"
#include "scene/graphicstextitem.h"
#include "settingdialogs/qrsetttingdialog.h"

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
	void statusThumbnailTimeout(QTimerEvent *event);

	void itemInserted(QGraphicsItem *item);

	void on_pushButton_exit_clicked();

	void on_pushButton_minimize_clicked();

	void on_pushButton_dec_print_length_clicked();

	void on_pushButton_inc_print_length_clicked();

	void on_lineEdit_print_length_editingFinished();

	void on_pushButton_QR_clicked();

	void on_pushButton_Text_clicked();

	void on_pushButton_graphicsitem_up_clicked();

	void on_pushButton_graphicsitem_down_clicked();

	void on_pushButton_graphicsitem_left_clicked();

	void on_pushButton_graphicsitem_right_clicked();

	void on_pushButton_deselect_items_clicked();

	void on_pushButton_delete_items_clicked();

	void on_pushButton_start_print_clicked();

private:
	Ui::MainWindow *ui;

	void initDatetime();
	void initFrameShowCom();
	void initStatusThumbnail();
	void updateSceneSizeInfo();
	void updateStatusThumbnail();

	GraphicsScene *scene_show_com;
	QHBoxLayout *layout_show_com;
	QGraphicsView *view_show_com;
	int mTimerIdTime;
	int mTimerIdStatus;
	bool mStartPrint;
};

#endif // MAINWINDOW_H

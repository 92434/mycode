#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include "mainwindow.h"

class single_application : public QApplication
{
	Q_OBJECT
public:
	MainWindow w;

	single_application(int &argc, char **argv);
	virtual ~single_application();
	bool init();

private slots:
	void newLocalSocketConnection();

private:
	QLocalSocket socket;
	QLocalServer server;
};

#endif

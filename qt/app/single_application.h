#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QMainWindow>
#include <QFontDatabase>
#include <qfile.h>
#include <qdir.h>

#include "../utils/xiaofei_debug.h"

class single_application : public QApplication
{
	Q_OBJECT
public:

	single_application(int &argc, char **argv);
	virtual ~single_application();
	bool init(QMainWindow *w);

private slots:
	void newLocalSocketConnection();

private:
	QLocalSocket socket;
	QLocalServer server;
	QMainWindow *w;

	void addFont(const QString &fileName);
};

#endif

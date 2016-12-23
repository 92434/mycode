#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QMainWindow>
#include <QFontDatabase>
#include <qfile.h>
#include <qdir.h>

#include "../utils/xiaofei_debug.h"

class Application : public QApplication
{
	Q_OBJECT
public:

	Application(int &argc, char **argv);
	virtual ~Application();
	bool setSingleWindow(QWidget *w = 0);

	bool addFont(const QString &fileName);

private slots:
	void newLocalSocketConnection();

private:
	QLocalSocket socket;
	QLocalServer server;
	QWidget *w;
};

#endif

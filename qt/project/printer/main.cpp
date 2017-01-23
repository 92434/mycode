#include "../../app/application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);

	qDebug() << a.font().rawName();
	qDebug() << a.font().family();
	qDebug() << a.font().defaultFamily();
	qDebug() << a.font().styleName();
	qDebug() << a.font().toString();
	qDebug() << a.font().key();

	MainWindow w;

	if(!a.setSingleWindow(&w)) {
		return 0;
	}

	w.show();

	return a.exec();
}

#include "../../app/application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);

	//qDebug() << a.font().rawName();
	//qDebug() << a.font().family();
	//qDebug() << a.font().defaultFamily();
	//qDebug() << a.font().styleName();
	//qDebug() << a.font().toString();
	//qDebug() << a.font().key();
	//a.addFont(":resource/小米兰亭字体.ttf");
	QFont font;

	if(font.fromString("Sans Serif,9,-1,5,50,0,0,0,0,0")) {
		a.setFont(font);
	}

	MainWindow w;

	if(!a.setSingleWindow(&w)) {
		return -1;
	}

	w.show();

	return a.exec();
}

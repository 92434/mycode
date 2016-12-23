#include "../../app/application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);

	MainWindow w;

	if(!a.setSingleWindow(&w)) {
		return 0;
	}

	w.show();

	return a.exec();
}

#include "app/application.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);

	Dialog d(&a);

	if(!a.setSingleWindow(&d)) {
		return 0;
	}

	d.show();

	return a.exec();
}

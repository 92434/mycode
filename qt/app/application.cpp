#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

Application::~Application()
{
}

bool Application::setSingleWindow(QWidget *w)
{
	bool enable = true;

	QString serverName = QCoreApplication::applicationName();
	serverName = QDir::cleanPath(QDir::tempPath()) + QLatin1Char('/') + serverName;

	this->w = w;

	//printf("serverName:%s\n", qPrintable(serverName));

	socket.connectToServer(serverName);

	if (socket.waitForConnected(50)) { //如果能够连接得上的话，将参数发送到服务器，然后退出
		printf("instance exist!!!\n");

		QTextStream stream(&socket);
		QStringList args = QCoreApplication::arguments();

		if (args.count() > 1) {
			stream << args.last();
		} else {
			stream << QString();
		}

		stream.flush();
		socket.waitForBytesWritten();
		enable = false;

	} else {
		if (server.listen(serverName) == false) {
			enable = false;

			if (server.serverError() == QAbstractSocket::AddressInUseError && QFile::exists(serverName)) { //确保能够监听成功
				printf("serverName exist:%s!!!\n", qPrintable(serverName));
				QFile::remove(serverName);
			}
		}
	}

	if(this->w != 0) {
		connect(&server, SIGNAL(newConnection()), this, SLOT(newLocalSocketConnection())); //监听新到来的连接
	}


	return enable;
}

void Application::newLocalSocketConnection()
{
	QLocalSocket *socket = server.nextPendingConnection();

	if (!socket) {
		return;
	}

	socket->waitForReadyRead(1000);
	QTextStream stream(socket);
	delete socket;

	this->w->raise();
	this->w->activateWindow();
}

bool Application::addFont(const QString &fileName)
{
	bool status = false;
	int nIndex = QFontDatabase::addApplicationFont(fileName);

	if(nIndex != -1) {
		status = true;

		QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

		if(strList.count() > 0) {
			QFont font(strList.at(0));
			font.setPointSize(9);
			setFont(font);
		} else {
			printf("failed!!!");
		}
	}

	return status;
}

// 这个是qt 自动的默认 例子. 关于QBarSeries的. 
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
 
#include <QApplication>
#include <QtCore>
#include "server.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QGuiApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
	Server server;
	server.show();
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	return app.exec();
}
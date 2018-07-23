// 这个是qt 自动的默认 例子. 关于QBarSeries的. 
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
 
#include <QApplication>
#include <QtCore>

#include <stdlib.h>
 
#include <QDialog>

class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;

class Server : public QDialog
{
	Q_OBJECT

public:
	explicit Server(QWidget *parent = Q_NULLPTR);

	private slots:
	void sessionOpened();
	void sendFortune();

private:
	QLabel *statusLabel;
	QTcpServer *tcpServer;
	QStringList fortunes;
	QNetworkSession *networkSession;
};

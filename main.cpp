#include <QCoreApplication>
#include <QtNetwork>
#include "UpstreamProxy.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
	app.setApplicationName("Astar Proxy Server");
	server general;
	if (!general.listen(QHostAddress::Any, 8080)) {
        return 1;
	}
	server socks2http;
	socks2http.enableConvertSocksToHttp();
	if (!socks2http.listen(QHostAddress::Any, 8081)) {

		return 1;
	}
    return app.exec();
}


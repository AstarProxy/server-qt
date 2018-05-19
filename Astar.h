#ifndef ASTAR_H
#define ASTAR_H

#include <QThread>
#include <QTcpSocket>

#include "HttpHeader.h"
#include "UpstreamProxy.h"

class Astar : public QThread
{
    Q_OBJECT
public:
	enum IncomingProtocol {
		HTTP,
		SOCKS,
		Auto,
		SOCKS2HTTP
	};

    int connect_timeout = 3000; // 3 Secends
    int read_timeout = 120000; // 120 Secends
    int client_maxsize = 1024 * 1024 * 1024; // 1GB
    int lastTransaction = 0;
    Astar(int socketDescriptor, QObject *parent);
    void run() override;

	IncomingProtocol getIncomingProtocol() const;
	void setIncomingProtocol(const IncomingProtocol &incomingProtocol);
	void setUpstreamProxy(const UpstreamProxy &upstreamProxy);
	static IncomingProtocol detectIncomingProtocol(const QByteArray &data);
private:
    int socketDescriptor;
	QTcpSocket *client = nullptr;
	IncomingProtocol incomingProtocol = IncomingProtocol::Auto;
	UpstreamProxy upstreamProxy;
	void handleRequest();
	static void disconnect(QTcpSocket * socket);
};

#endif // ASTAR_H

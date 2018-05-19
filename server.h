#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "UpstreamProxy.h"

class server : public QTcpServer
{
    Q_OBJECT
public:
    server(QObject *parent = 0);
	void enableConvertSocksToHttp();
	void setUpstreamProxy(const UpstreamProxy &upstreamProxy);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
	bool convertSocksToHttp = false;
	UpstreamProxy upstreamProxy;
};

#endif // SERVER_H

#ifndef ASTARPROXYCLIENT_H
#define ASTARPROXYCLIENT_H

#include <QHostAddress>
#include "../ProxyClient.h"

class AstarProxyClient : public ProxyClient
{
	public:
		AstarProxyClient(const QString &hostname, int port, const QString &username, const QString password);
		void connectToHost(const QString &address, quint16 port);
		void connectToHost(const QHostAddress &address, quint16 port);
	protected:
		QString hostname;
		QHostAddress hostnameAddress;
		int port;
		QString username;
		QString password;
};

#endif // ASTARPROXYCLIENT_H

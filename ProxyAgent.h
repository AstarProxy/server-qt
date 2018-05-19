#ifndef PROXYAGENT_H
#define PROXYAGENT_H

#include <QTcpSocket>
#include "UpstreamProxy.h"
#include "TcpClient.h"
class ProxyAgent
{
	public:
		ProxyAgent(QTcpSocket *client);
		~ProxyAgent();
		void setUpstreamProxy(const UpstreamProxy &upstreamProxy);
		virtual void handle() = 0;
	protected:
		TcpClient *client;
		TcpClient *remote = nullptr;
		UpstreamProxy upstreamProxy;
		void connectTCPConnection(const QString &host, int port);
		void connectTCPConnection(const QHostAddress &host, int port);
		void transport(TcpClient *from, TcpClient *to);
		void transportToClient();
		void transportToRemote();
		bool checkAuthentication(const QString &username, const QString &password) const;
};

#endif // PROXYAGENT_H

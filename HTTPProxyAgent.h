#ifndef HTTPPROXYAGENT_H
#define HTTPPROXYAGENT_H

#include "ProxyAgent.h"
#include "HttpHeader.h"

class HTTPProxyAgent : public ProxyAgent
{
	public:
		HTTPProxyAgent(const HttpHeader &header, QTcpSocket *client);
		void handle();
	protected:
		HttpHeader header;
		void checkHttpProxyAuth() const;
		QByteArray getHttpAuthError() const;
		QByteArray getHttpError(const QString &status) const;
		QByteArray getHttpEstablishedMessage() const;
		QByteArray getRewritedRequestForRemote() const;
		QString getDomain() const;
		int getPort() const;
};

#endif // HTTPPROXYAGENT_H

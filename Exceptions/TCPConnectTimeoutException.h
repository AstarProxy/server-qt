#ifndef TCPCONNECTTIMEOUTEXCEPTION_H
#define TCPCONNECTTIMEOUTEXCEPTION_H

#include "Exception.h"
#include <QHostAddress>

class TCPConnectTimeoutException : public Exception
{
	public:
		TCPConnectTimeoutException(const QHostAddress &host, int port, int timeout);
		TCPConnectTimeoutException(const QString &host, int port, int timeout);
		QHostAddress getHost() const;
		int getPort() const;
		int getTimeout() const;

	protected:
		QHostAddress host;
		int port;
		int timeout;
};

#endif // TCPCONNECTTIMEOUTEXCEPTION_H

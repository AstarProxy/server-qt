#include "TCPConnectTimeoutException.h"

TCPConnectTimeoutException::TCPConnectTimeoutException(const QHostAddress &host, int port, int timeout):
	Exception("Cannot connect to " + host.toString() + ":" + QString::number(port) + " in " + QString::number(timeout) + " ms")
{
	this->host = host;
	this->port = port;
	this->timeout = timeout;
}

TCPConnectTimeoutException::TCPConnectTimeoutException(const QString &host, int port, int timeout):
	Exception("Cannot connect to " + host + ":" + QString::number(port) + " in " + QString::number(timeout) + " ms")
{
	this->host = host;
	this->port = port;
	this->timeout = timeout;
}

QHostAddress TCPConnectTimeoutException::getHost() const
{
	return host;
}

int TCPConnectTimeoutException::getPort() const
{
	return port;
}

int TCPConnectTimeoutException::getTimeout() const
{
	return timeout;
}

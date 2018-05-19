#include "ProxyClient.h"

ProxyClient::ProxyClient():
	TcpClient()
{

}

bool ProxyClient::waitForConnected(int msecs)
{
	return true;
}

void ProxyClient::setTimeoutForConnectToProxy(int timeoutForConnectToProxy)
{
	this->timeoutForConnectToProxy = timeoutForConnectToProxy;
}

int ProxyClient::getTimeoutForConnectToProxy() const
{
	return timeoutForConnectToProxy;
}

int ProxyClient::getTimeoutForConnectToHost() const
{
	return timeoutForConnectToHost;
}

void ProxyClient::setTimeoutForConnectToHost(int timeoutForConnectToHost)
{
	this->timeoutForConnectToHost = timeoutForConnectToHost;
}


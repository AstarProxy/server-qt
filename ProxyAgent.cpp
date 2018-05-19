#include "ProxyAgent.h"
#include <QHostInfo>
#include "ProxyClients/HttpProxyClient.h"
#include "Exceptions/TCPConnectTimeoutException.h"
#include "Exceptions/DomainResolveException.h"

ProxyAgent::ProxyAgent(QTcpSocket *client)
{
	this->client = new TcpClient(client);
}

ProxyAgent::~ProxyAgent()
{
	if (remote){
		if(remote->state() != QAbstractSocket::SocketState::UnconnectedState){
			qDebug() << "Closing remote";
			remote->disconnectFromHost();
			if(remote->state() != QAbstractSocket::SocketState::UnconnectedState){
				remote->waitForDisconnected();
			}
		}
		delete remote;
	}
	if (client) {
		delete client;
	}
}

void ProxyAgent::setUpstreamProxy(const UpstreamProxy &upstreamProxy)
{
	this->upstreamProxy = upstreamProxy;
}

void ProxyAgent::connectTCPConnection(const QString &host, int port) {
	if (!upstreamProxy.hostname.isEmpty()) {
		if (upstreamProxy.type == UpstreamProxy::UpstreamProxyType::HTTP) {
			remote = new HttpProxyClient(upstreamProxy.hostname, upstreamProxy.port, upstreamProxy.username, upstreamProxy.password);
		}
	} else {
		remote = new TcpClient();
	}
	remote->connectToHost(host, port);
	if(!remote->waitForConnected(3000)){
		throw TCPConnectTimeoutException(host, port, 3000);
	}
}

void ProxyAgent::connectTCPConnection(const QHostAddress &host, int port)
{
	if (!upstreamProxy.hostname.isEmpty()) {
		if (upstreamProxy.type == UpstreamProxy::UpstreamProxyType::HTTP) {
			remote = new HttpProxyClient(upstreamProxy.hostname, upstreamProxy.port, upstreamProxy.username, upstreamProxy.password);
		}
	} else {
		remote = new TcpClient();
	}
	remote->connectToHost(host, port);
	if(!remote->waitForConnected(3000)){
		throw TCPConnectTimeoutException(host, port, 3000);
	}
}


void ProxyAgent::transport(TcpClient *from, TcpClient *to){
	const int bytes = from->bytesAvailable();
	for(int i = 0;i < bytes;i+=1024){
		QByteArray buffer = from->read(1024);
		to->write(buffer);
		to->flush();
	}
}

void ProxyAgent::transportToClient()
{
	transport(remote, client);
}

void ProxyAgent::transportToRemote()
{
	transport(client, remote);
}

bool ProxyAgent::checkAuthentication(const QString &username, const QString &password) const
{
	qDebug() << "username: " <<username;
	qDebug() << "password: " <<password;
	return (
			(username == "amir" && password == "123") ||
			(username == "abedi" && password == "mahdi123") ||
			(username == "elahe" && password == "haniye123") ||
			(username == "jafari" && password == "qwer") ||
			(username == "guest-user" && password == "139702")
		);

}

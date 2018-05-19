#include "Astar.h"

#include <QtNetwork>
#include <QHostInfo>
#include <QDebug>
#include "HTTPProxyAgent.h"
#include "SocksProxyAgent.h"
#include "Socks2HTTPProxyAgent.h"
#include "Exceptions/InvalidProtocolException.h"
#include "Exceptions/BadHttpRequestException.h"
#include "Exceptions/BadFormattedAddressException.h"
#include "Exceptions/DomainResolveException.h"
#include "Exceptions/TCPConnectTimeoutException.h"
#include "Exceptions/TCPWriteTimeoutException.h"
#include "Exceptions/ProxyAuthException.h"

Astar::Astar(int socketDescriptor, QObject *parent):
	QThread(parent),
	socketDescriptor(socketDescriptor)
{

}
void Astar::run(){
	qDebug() << "new thread"<<this;
	client = new QTcpSocket();
	try {
		if(!client->setSocketDescriptor(socketDescriptor)){
			throw Exception("Bad Socket Descriptor");
		}
		handleRequest();
	} catch(BadHttpRequestException e) {
		qDebug() << e.getMessage();
	} catch(Exception e) {
		qDebug() << e.getMessage();
	}
	if (client) {
		disconnect(client);
		delete client;
		client = nullptr;
	}
}

Astar::IncomingProtocol Astar::getIncomingProtocol() const
{
	return incomingProtocol;
}

void Astar::setIncomingProtocol(const Astar::IncomingProtocol &incomingProtocol)
{
	this->incomingProtocol = incomingProtocol;
}

void Astar::setUpstreamProxy(const UpstreamProxy &upstreamProxy)
{
	this->upstreamProxy = upstreamProxy;
}

Astar::IncomingProtocol Astar::detectIncomingProtocol(const QByteArray &data)
{
	if (data.at(0) == 0x05) {
		return IncomingProtocol::SOCKS;
	}
	{
		QByteArray first3byte = data.left(3).toUpper();
		if (first3byte == "GET" || first3byte == "PUT") {
			return IncomingProtocol::HTTP;
		}
		QByteArray first7byte = data.left(7).toUpper();
		if (first7byte == "OPTIONS" || first7byte == "CONNECT") {
			return IncomingProtocol::HTTP;
		}
		QByteArray first4byte = data.left(4).toUpper();
		if (first3byte == "POST" || first4byte == "HEAD") {
			return IncomingProtocol::HTTP;
		}
		if (data.left(6).toUpper() == "DELETE") {
			return IncomingProtocol::HTTP;
		}
	}
	return IncomingProtocol::Auto;
}

void Astar::handleRequest(){
	qDebug()<<"receiveing from client";
	if(!client->waitForReadyRead(10000)){
		return;
	}
	QByteArray data;
	if (incomingProtocol == IncomingProtocol::Auto) {
		data = client->read(7);
		incomingProtocol = detectIncomingProtocol(data);
		if (incomingProtocol == IncomingProtocol::HTTP) {
			while(data.indexOf("\r\n\r\n") == -1 && data.length() < 1024 * 1024 && client->bytesAvailable() > 0) {
				data.append(client->read(1));
			}
			HttpHeader header = HttpHeader::fromRequest(data);
			try {
				HTTPProxyAgent agent(header, client);
				agent.setUpstreamProxy(upstreamProxy);
				agent.handle();
			}catch(BadFormattedAddressException e) {
				qDebug() << e.getMessage();
			}
		} else if (incomingProtocol == IncomingProtocol::SOCKS) {
			data.append(client->readAll());
			SocksProxyAgent agent(data, client);
			agent.setUpstreamProxy(upstreamProxy);
			agent.handle();
		}else {
			throw InvalidProtocolException();
		}
	} else if (incomingProtocol == IncomingProtocol::HTTP) {
		while(data.indexOf("\r\n\r\n") == -1 && data.length() < 1024 * 1024 && client->bytesAvailable() > 0) {
			data.append(client->read(1));
		}
		HttpHeader header = HttpHeader::fromRequest(data);
		try {
			HTTPProxyAgent agent(header, client);
			agent.handle();
		}catch(BadFormattedAddressException e) {
			qDebug() << e.getMessage();
		}
	} else if (incomingProtocol == IncomingProtocol::SOCKS) {
		data.append(client->readAll());
		SocksProxyAgent agent(data, client);
		agent.handle();
	} else if (incomingProtocol == IncomingProtocol::SOCKS2HTTP) {
		data.append(client->readAll());
		qDebug() << data;
		Socks2HTTPProxyAgent agent(data, client);
		agent.handle();
	} else {
		throw InvalidProtocolException();
	}

}


void Astar::disconnect(QTcpSocket *socket){
	if(socket->state() != QAbstractSocket::SocketState::UnconnectedState){
		qDebug() << "Closing";
		socket->disconnectFromHost();
		if(socket->state() != QAbstractSocket::SocketState::UnconnectedState){
			socket->waitForDisconnected();
		}
	}
}


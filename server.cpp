#include "server.h"
#include "Astar.h"

server::server(QObject *parent)
    : QTcpServer(parent)
{

}

void server::enableConvertSocksToHttp()
{
	this->convertSocksToHttp = true;
}

void server::setUpstreamProxy(const UpstreamProxy &upstreamProxy)
{
	this->upstreamProxy = upstreamProxy;
}

void server::incomingConnection(qintptr socketDescriptor)
{
	Astar *thread = new Astar(socketDescriptor, this);
	if (convertSocksToHttp) {
		thread->setIncomingProtocol(Astar::IncomingProtocol::SOCKS2HTTP);
	}
	thread->setUpstreamProxy(upstreamProxy);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

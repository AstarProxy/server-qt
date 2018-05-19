#include "SocksProxyAgent.h"
#include <QList>
#include <QHostAddress>
#include "Exceptions/InvalidProtocolException.h"
#include "Exceptions/UnsupportedSocksAuthMethodException.h"
#include "Exceptions/InvalidSocksCommandException.h"
#include "Exceptions/ProxyAuthException.h"
#include "Exceptions/DomainResolveException.h"
#include "Exceptions/TCPConnectTimeoutException.h"
#include "Exceptions/TCPWriteTimeoutException.h"

SocksProxyAgent::SocksProxyAgent(const QByteArray &firstPacket, QTcpSocket *client):
	ProxyAgent(client)
{
	this->firstPacket = firstPacket;
}

void SocksProxyAgent::handle()
{
	try {
		handshake();
		if (authMethod != AuthMethod::NoAuthentication) {
			authentication();
		}
		handleRequest();
	}catch(InvalidProtocolException) {
		client->write("InvalidProtocolException");
	}catch(UnsupportedSocksAuthMethodException) {
		QByteArray message;
		message[0] = 0x05;
		message[1] = 0xFF;
		client->write(message);
	}catch(InvalidSocksCommandException) {
		client->write("InvalidSocksCommandException");
	}catch(ProxyAuthException) {
		QByteArray message;
		message[0] = 0x01;
		message[1] = 0x01;
		client->write(message);
	}
}

void SocksProxyAgent::handshake()
{
	if (firstPacket.at(0) != 0x05) {
		throw InvalidProtocolException();
	}
	clientProtocol = ClientProtocol::Socks5;
	QList<AuthMethod> methods;
	const int numberOfAuthMethods = firstPacket.at(1);
	for (int x = 0;x < numberOfAuthMethods; x++) {
		const int methodCode = firstPacket.at(2 + x);
		if (methodCode == 0x00) {
			methods.append(AuthMethod::NoAuthentication);
		} else if (methodCode == 0x01) {
			methods.append(AuthMethod::GSSAPI);
		} else if (methodCode == 0x02) {
			methods.append(AuthMethod::Password);
		} else {
			throw UnsupportedSocksAuthMethodException();
		}
	}
	authMethod = AuthMethod::Password;
	QByteArray message;
	message[0] = 0x05;
	message[1] = 0x02;
	client->write(message);
	client->flush();

}

void SocksProxyAgent::authentication()
{
	if (!client->waitForReadyRead(3000)) {
		throw ProxyAuthException();
	}
	QByteArray line = client->readAll();
	if (line.at(0) != 0x01) {
		throw ProxyAuthException();
	}
	QString username = line.mid(2, line.at(1));
	QString password = line.mid(2 + username.length() + 1, line.at(2 + username.length()));
	if (!checkAuthentication(username, password)) {
		throw ProxyAuthException();
	}
	QByteArray message;
	message[0] = 0x01;
	message[1] = 0x00;
	client->write(message);
	client->flush();
}

char SocksProxyAgent::readByte()
{
	return client->read(1).at(0);
}

void SocksProxyAgent::runConnectCommand(AddressType addressType, const QString &address, int port)
{
	if (addressType == AddressType::Domain) {
		connectTCPConnection(address, port);
	} else if (addressType == AddressType::IPv4 || addressType == AddressType::IPv6) {
		QHostAddress ip(address);
		connectTCPConnection(ip, port);
	}
	client->write(sendRequestResponse(0x00, addressType, address, port));
	while(remote->state() == QAbstractSocket::SocketState::ConnectedState && client->state() == QAbstractSocket::SocketState::ConnectedState){
		while(remote->bytesAvailable() > 0) {
			qDebug() << "transport from remote to client";
			transportToClient();
		}
		while(client->bytesAvailable() > 0) {
			qDebug() << "transport from client to remote";
			transportToRemote();
		}
		if (!client->waitForReadyRead(50)) {
			remote->waitForReadyRead(50);
		}
	}
}

QByteArray SocksProxyAgent::sendRequestResponse(char status, SocksProxyAgent::AddressType addressType, QString address, int port)
{
	QByteArray bytes;
	if (clientProtocol == ClientProtocol::Socks5) {
		bytes.append(0x05);
	}
	bytes.append((char)status);
	bytes.append((char)0x00);
	if (addressType == AddressType::IPv4) {
		bytes.append((char)0x01);
		QStringList parts = address.split('.');
		for (int x = 0, l = parts.length(); x < l; x++) {
			bytes.append((char)parts[x].toInt());
		}
	} else if(addressType == AddressType::Domain) {
		bytes.append((char)0x03);
		bytes.append((char)address.length());
		bytes.append(address);
	} else if(addressType == AddressType::IPv6) {
		bytes.append((char)0x04);
		bytes.append("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	}
	bytes.append((char)(port / 256));
	bytes.append((char)(port % 256));
	return bytes;
}

void SocksProxyAgent::handleRequest()
{
	if (!client->waitForReadyRead(3000)) {
		return;
	}
	QByteArray firstpart = client->read(4);
	if (firstpart.at(0) != 0x05) {
		throw InvalidProtocolException();
	}
	Socks5Command command;
	switch(firstpart.at(1)) {
		case(0x01):
			command = Socks5Command::Connect;
			break;
		case(0x02):
			command = Socks5Command::Bind;
			break;
		case(0x03):
			command = Socks5Command::UDP_ASSOCIATE;
			break;
		default:
			throw InvalidSocksCommandException();
	}

	AddressType addressType;
	switch(firstpart.at(3)) {
		case(0x01):
			addressType = AddressType::IPv4;
			break;
		case(0x03):
			addressType = AddressType::Domain;
			break;
		case(0x04):
			addressType = AddressType::IPv6;
			break;
		default:
			throw Exception("Invalid address type");
	}
	QString address;
	if (addressType == AddressType::IPv4) {
		QByteArray addressBytes = client->read(4);
		address = QString::number((unsigned char)addressBytes.at(0)) + "." +  QString::number((unsigned char)addressBytes.at(1)) + "." +  QString::number((unsigned char)addressBytes.at(2)) + "." + QString::number((unsigned char)addressBytes.at(3));
	} else if (addressType == AddressType::Domain) {
		const int domainSize = readByte();
		address = client->read(domainSize);
	} else if(addressType == AddressType::IPv6) {
		throw Exception("IPv6 is not supported");
	}
	QByteArray portBytes = client->read(2);
	int port = ((unsigned char)portBytes.at(0)) * 256 + ((unsigned char)portBytes.at(1));
	qDebug() << "address = " << address;
	qDebug() << "port = " << port;
	try {
		if (command == Socks5Command::Connect) {
			runConnectCommand(addressType, address, port);
		}
	}catch(DomainResolveException) {
		client->write(sendRequestResponse(0x01, addressType, address, port));
	}catch(TCPConnectTimeoutException e) {
		client->write(sendRequestResponse(0x06, addressType, address, port));
	}catch(TCPWriteTimeoutException e) {
		client->write(sendRequestResponse(0x06, addressType, address, port));
	}
}


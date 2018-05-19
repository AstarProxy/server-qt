#include "TcpClient.h"
#include <QHostInfo>
#include "HttpHeader.h"
#include "Exceptions/DomainResolveException.h"
#include "Exceptions/TCPConnectTimeoutException.h"
#include "Exceptions/BadHttpResponseException.h"
#include "Exceptions/ProxyAuthException.h"

TcpClient::TcpClient()
{
	remote = new QTcpSocket();
	free_remote = true;
}

TcpClient::TcpClient(QTcpSocket *remote, bool free_remote)
{
	this->remote = remote;
	this->free_remote = free_remote;
}

TcpClient::~TcpClient()
{
	if (free_remote && remote) {
		delete remote;
	}
}

void TcpClient::connectToHost(const QHostAddress &address, quint16 port)
{
	remote->connectToHost(address, port);
}

void TcpClient::connectToHost(const QString &host, quint16 port)
{
	QHostInfo remoteInfo = QHostInfo::fromName(host);
	if (remoteInfo.addresses().isEmpty()) {
		throw DomainResolveException(host);
	}
	QHostAddress address = remoteInfo.addresses().first();
	connectToHost(address, port);
}

qint64 TcpClient::bytesAvailable() const
{
	if (remote) {
		return remote->bytesAvailable();
	}
	return 0;
}

qint64 TcpClient::bytesToWrite() const
{
	if (remote) {
		return remote->bytesToWrite();
	}
	return 0;
}

QByteArray TcpClient::read(qint64 maxlen)
{
	if (remote) {
		return remote->read(maxlen);
	}
	return QByteArray();
}

QByteArray TcpClient::readAll()
{
	if (remote) {
		return remote->readAll();
	}
	return QByteArray();
}

qint64 TcpClient::write(const char *data, qint64 len)
{
	if (remote) {
		return remote->write(data, len);
	}
	return 0;
}

qint64 TcpClient::write(const char *data)
{
	if (remote) {
		return remote->write(data);
	}
	return 0;
}

qint64 TcpClient::write(const QByteArray &data)
{
	if (remote) {
		return remote->write(data);
	}
	return 0;
}

void TcpClient::close()
{
	if (remote) {
		remote->close();
	}
}

bool TcpClient::flush()
{
	if (remote) {
		return remote->flush();
	}
	return false;
}

bool TcpClient::waitForConnected(int msecs)
{
	if (remote) {
		return remote->waitForConnected(msecs);
	}
	return false;
}

bool TcpClient::waitForReadyRead(int msecs)
{
	if (remote) {
		return remote->waitForReadyRead(msecs);
	}
	return false;
}

bool TcpClient::waitForBytesWritten(int msecs)
{
	if (remote) {
		return remote->waitForBytesWritten(msecs);
	}
	return false;
}

bool TcpClient::waitForDisconnected(int msecs)
{
	if (remote) {
		return remote->waitForDisconnected(msecs);
	}
	return false;
}

void TcpClient::disconnectFromHost()
{
	if (remote) {
		remote->disconnectFromHost();
	}
}

QAbstractSocket::SocketType TcpClient::socketType() const
{
	if (remote) {
		return remote->socketType();
	}
	return QAbstractSocket::SocketType::UnknownSocketType;
}

QAbstractSocket::SocketState TcpClient::state() const
{
	if (remote) {
		return remote->state();
	}
	return QAbstractSocket::SocketState::UnconnectedState;
}

QAbstractSocket::SocketError TcpClient::error() const
{
	if (remote) {
		return remote->error();
	}
	return QAbstractSocket::SocketError::UnknownSocketError;
}

QTcpSocket *TcpClient::getRemote() const
{
	return remote;
}

void TcpClient::setRemote(QTcpSocket *remote, bool free_remote)
{
	this->remote = remote;
	this->free_remote = free_remote;
}

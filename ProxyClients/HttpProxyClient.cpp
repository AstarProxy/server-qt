#include "HttpProxyClient.h"
#include <QHostAddress>
#include <QHostInfo>
#include "../HttpHeader.h"
#include "../Exceptions/DomainResolveException.h"
#include "../Exceptions/TCPConnectTimeoutException.h"
#include "../Exceptions/BadHttpResponseException.h"
#include "../Exceptions/ProxyAuthException.h"

HttpProxyClient::HttpProxyClient(const QString &hostname, int port, const QString &username, const QString password):
	ProxyClient()
{
	this->hostname = hostname;
	this->port = port;
	this->username = username;
	this->password  = password;
}

void HttpProxyClient::connectToHost(const QString &address, quint16 port)
{

	{
		QHostInfo hostnameInfo = QHostInfo::fromName(hostname);
		if (hostnameInfo.addresses().isEmpty()) {
			throw DomainResolveException(hostname);
		}
		hostnameAddress = hostnameInfo.addresses().first();
	}

	{
		QString header  = "CONNECT " + address + ":" + QString::number(port) + " HTTP/1.1\r\n";
		header += "Host: " + address + ":" + QString::number(port) + "\r\n";
		if (!username.isEmpty()) {
			QString decoded = username + ":" + password;
			header +=  "Proxy-Authorization: basic " + decoded.toUtf8().toBase64() + "\r\n";
		}
		header += "\r\n";
		remote->connectToHost(hostnameAddress, this->port);
		if (!remote->waitForConnected(timeoutForConnectToProxy)) {
			throw TCPConnectTimeoutException(hostnameAddress,this->port, timeoutForConnectToProxy);
		}
		remote->write(header.toUtf8());
		remote->waitForBytesWritten(3000);
	}
	{
		if (!remote->waitForReadyRead(3000)) {
			throw TCPConnectTimeoutException(hostnameAddress,this->port, 3000);
		}
		QByteArray header;
		char ch;
		while(!header.endsWith("\r\n\r\n") && remote->getChar(&ch)) {
			header.append(ch);
		}
		if (!header.endsWith("\r\n\r\n")) {
			throw BadHttpResponseException("http header cannot be read");
		}
		HttpHeader httpHeader = HttpHeader::fromResponse(header);
		int status = httpHeader.getStatus();
		if (status == 407) {
			throw ProxyAuthException();
		} else if(status != 200) {
			throw BadHttpResponseException(QString("http status is ") + QString::number(status));
		}
	}
}

void HttpProxyClient::connectToHost(const QHostAddress &address, quint16 port)
{
	connectToHost(address.toString(), port);
}

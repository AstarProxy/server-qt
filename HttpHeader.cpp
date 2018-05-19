#include "HttpHeader.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "Exceptions/BadFormattedAddressException.h"
#include "Exceptions/BadHttpRequestException.h"

#include <QDebug>

HttpHeader::HttpHeader()
{

}

QString HttpHeader::getMethod() const
{
	return method;
}

void HttpHeader::setMethod(const QString &value)
{
	method = value;
}

QString HttpHeader::getPath() const
{
	return path;
}

void HttpHeader::setPath(const QString &value)
{
	path = value;
}

void HttpHeader::setHostname(const QString &hostname)
{
	int colonAt = hostname.indexOf(":");
	if (colonAt > 0) {
		domain = hostname.left(colonAt);
		port = hostname.mid(colonAt + 1).toInt();
	} else {
		domain = hostname;
		port = 80;
	}
}

void HttpHeader::setAddress(const QString &address)
{
	qDebug() << address;
	QRegularExpression hostname("^((?<scheme>[a-z]+)://(?<domain>[a-z0-9\\-\\.]+)(?:\\:(?<port>\\d+))?)?(?<path>.*)?", QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match = hostname.match(address);

	if (!match.hasMatch()) {
		throw BadFormattedAddressException(address);
	}
	domain = match.captured("domain");
	scheme = match.captured("scheme").toLower();
	QString port = match.captured("port");
	if(!port.isNull()){
		this->port = port.toInt();
	}else if(scheme == "http"){
		this->port = 80;
	}else{
		this->port = 443;
	}
	path = match.captured("path");
}

void HttpHeader::setParam(const QString &name, const QString &value)
{
	params.insert(name.toLower(), value);
}

QString HttpHeader::getParam(const QString &name) const
{
	return params.value(name.toLower());
}

QMap<QString, QString> HttpHeader::getParams() const
{
	return params;
}

QString HttpHeader::getDomain() const
{
	return domain;
}

void HttpHeader::setDomain(const QString &value)
{
	domain = value;
}

QString HttpHeader::getScheme() const
{
	return scheme;
}

void HttpHeader::setScheme(const QString &value)
{
	scheme = value;
}

int HttpHeader::getPort() const
{
	return port;
}

void HttpHeader::setPort(int value)
{
	port = value;
}

bool HttpHeader::isConnect() const
{
	return method.compare("connect", Qt::CaseInsensitive) == 0;
}

QString HttpHeader::getProtocol() const
{
    return protocol;
}

void HttpHeader::setProtocol(const QString &value)
{
	protocol = value;
}

int HttpHeader::getStatus() const
{
	return status;
}

void HttpHeader::setStatus(int status)
{
	this->status = status;
}

QString HttpHeader::getStatusText() const
{
	return statusText;
}

void HttpHeader::setStatusText(const QString &statusText)
{
	this->statusText = statusText;
}

HttpHeader HttpHeader::fromRequest(const QByteArray &data)
{
	int endOfHeader = data.indexOf("\r\n\r\n");
	if (endOfHeader == -1) {
		throw BadHttpRequestException("there is no \\r\\n\\r\\n in data");
	}
	QString headerString = data.left(endOfHeader);
	QStringList lines = headerString.split("\r\n");

	QStringList parts = lines[0].split(' ');
	if (parts.length() != 3) {
		throw BadHttpRequestException("First line is not there space-sperated parts");
	}
	QString method = parts.at(0).toUpper();
	if (method != "CONNECT" && method != "GET" && method != "POST" && method != "PUT" && method != "DELETE" && method != "HEAD" && method != "OPTIONS") {
		throw BadHttpRequestException("Http method is undefined: " + method);
	}
	HttpHeader header;
	header.setMethod(method);
	header.setProtocol(parts.at(2));
	if (method == "CONNECT") {
		header.setHostname(parts.at(1));
	} else {
		header.setAddress(parts.at(1));
	}
	for(int x = 1, l = lines.length(); x < l; x++) {
		const int colon = lines[x].indexOf(":");
		QString key = lines[x].left(colon).trimmed();
		QString value = lines[x].mid(colon + 1).trimmed();
		header.setParam(key, value);
	}
	return header;
}

HttpHeader HttpHeader::fromResponse(const QByteArray &data)
{
	int endOfHeader = data.indexOf("\r\n\r\n");
	if (endOfHeader == -1) {
		throw BadHttpRequestException("there is no \\r\\n\\r\\n in data");
	}
	QString headerString = data.left(endOfHeader);
	QStringList lines = headerString.split("\r\n");
	QString protocol;
	int status = 0;
	QString statusText;
	{
		int firstSpace = lines[0].indexOf(" ");
		int secondSpace = lines[0].indexOf(" ", firstSpace + 1);
		if (firstSpace == -1 || secondSpace == -1) {
			throw BadHttpRequestException("First line is not there space-sperated parts");
		}
		protocol = lines[0].left(firstSpace);
		status = lines[0].mid(firstSpace + 1, secondSpace - firstSpace - 1).toInt();
		statusText = lines[0].mid(secondSpace + 1);
	}
	HttpHeader header;
	header.setStatus(status);
	header.setStatusText(statusText);
	header.setProtocol(protocol);
	for(int x = 1, l = lines.length(); x < l; x++) {
		const int colon = lines[x].indexOf(":");
		QString key = lines[x].left(colon).trimmed();
		QString value = lines[x].mid(colon + 1).trimmed();
		header.setParam(key, value);
	}
	return header;
}

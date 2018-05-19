#include "HTTPProxyAgent.h"
#include "Exceptions/TCPConnectTimeoutException.h"
#include "Exceptions/TCPWriteTimeoutException.h"
#include "Exceptions/DomainResolveException.h"
#include "Exceptions/ProxyAuthException.h"

HTTPProxyAgent::HTTPProxyAgent(const HttpHeader &header, QTcpSocket *client):
	ProxyAgent(client)
{
	this->header = header;
	const QByteArray encoded = header.getParam("X-Encoded").toUtf8();
	if (!encoded.isEmpty()) {
		const QByteArray decoded = QByteArray::fromBase64(encoded);
		this->header = HttpHeader::fromRequest(decoded);
	}
}

void HTTPProxyAgent::handle()
{
	try {
		checkHttpProxyAuth();
		connectTCPConnection(header.getDomain(), header.getPort());
		if (header.isConnect()) {
			client->write(getHttpEstablishedMessage());
			client->waitForBytesWritten(3000);
		} else {
			remote->write(getRewritedRequestForRemote());
			remote->waitForBytesWritten(3000);
		}
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
	}catch(ProxyAuthException) {
		client->write(getHttpAuthError());
	}catch(DomainResolveException e) {
		qDebug() << e.getMessage();
	} catch(TCPConnectTimeoutException e) {
		client->write(getHttpError("408 Request Timeout"));
		qDebug() << e.getMessage();
	} catch(TCPWriteTimeoutException e) {
		client->write(getHttpError("408 Request Timeout"));
		qDebug() << e.getMessage();
	}
}


void HTTPProxyAgent::checkHttpProxyAuth() const
{
	const QString proxyAuth = header.getParam("Proxy-Authorization");
	if (proxyAuth.isEmpty()) {
		throw ProxyAuthException();
	}
	if (proxyAuth.left(5).compare("basic", Qt::CaseInsensitive) != 0) {
		throw ProxyAuthException();
	}
	QString decoded = QByteArray::fromBase64(proxyAuth.mid(6).trimmed().toUtf8());

	int colon = decoded.indexOf(":");
	if (colon == -1) {
		throw ProxyAuthException();
	}
	QString username = decoded.left(colon);
	QString password = decoded.mid(colon + 1);
	if (!checkAuthentication(username, password)) {
		throw ProxyAuthException();
	}
}

QByteArray HTTPProxyAgent::getHttpAuthError() const
{
	QString message =   header.getProtocol() + " 407 Unauthorized\r\n" +
						"Server: Proxy\r\n" +
						"Proxy-Authenticate: Basic realm=\"Astar Authorization\"\r\n" +
						"Cache-control: no-cache\r\n" +
						"Connection: Close\r\n" +
						"Proxy-Connection: Close\r\n" +
						"Content-Length: 0\r\n\n";
	return message.toUtf8();
}
QByteArray HTTPProxyAgent::getHttpError(const QString &status) const
{
	QString body =  QString() + "<html>" +
					"<head><title>" + status + "</title></head>" +
					"<body>" +
					"<center><h1>"+status+"</h1></center>" +
					"<hr><center>Astar/1.0</center>"+
					"</body>" +
					"</html>";
	QByteArray bodyBytes = body.toUtf8();
	QString message =   header.getProtocol() + " "+ status+"\r\n" +
						"Server: Proxy\r\n" +
						"Cache-control: no-cache\r\n" +
						"Connection: Close\r\n" +
						"Proxy-Connection: Close\r\n" +
						"Content-Length: " + bodyBytes.size() + "\r\n\n";
	return message.toUtf8().append(bodyBytes);
}

QByteArray HTTPProxyAgent::getHttpEstablishedMessage() const
{
	QString message = header.getProtocol() + " 200 Connection established\r\n" + "Proxy-Agent: Astar/1.0\r\n\r\n";
	return message.toUtf8();
}

QByteArray HTTPProxyAgent::getRewritedRequestForRemote() const
{
	QString message =   header.getMethod() + " " + header.getPath() + " " +header.getProtocol()+ "\r\n";
	const QMap<QString, QString> params = header.getParams();
	QMap<QString, QString>::const_iterator i = params.constBegin();
	while (i != params.constEnd()) {
		const QString key = i.key();
		if (key.left(5).compare("proxy", Qt::CaseInsensitive) != 0)  {
			message += key + ": " + i.value() + "\r\n";
		}
		++i;
	}
	message += "\r\n";
	return message.toUtf8();
}

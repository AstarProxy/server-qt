#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QByteArray>

class TcpClient
{
	public:
		TcpClient();
		TcpClient(QTcpSocket *remote, bool free_remote = false);
		virtual ~TcpClient();
		virtual void connectToHost(const QHostAddress &address, quint16 port);
		virtual void connectToHost(const QString &host, quint16 port);
		virtual qint64 bytesAvailable() const;
		virtual qint64 bytesToWrite() const;
		virtual QByteArray read(qint64 maxlen);
		virtual QByteArray readAll();
		virtual qint64 write(const char *data, qint64 len);
		virtual qint64 write(const char *data);
		virtual qint64 write(const QByteArray &data);
		virtual void close();
		virtual bool flush();
		virtual bool waitForConnected(int msecs = 30000);
		virtual bool waitForReadyRead(int msecs = 30000);
		virtual bool waitForBytesWritten(int msecs = 30000);
		virtual bool waitForDisconnected(int msecs = 30000);
		virtual void disconnectFromHost();

		QAbstractSocket::SocketType socketType() const;
		QAbstractSocket::SocketState state() const;
		QAbstractSocket::SocketError error() const;

		QTcpSocket *getRemote() const;
		void setRemote(QTcpSocket *remote, bool free_remote = false);
	protected:
		QTcpSocket *remote = nullptr;
		bool free_remote = false;
		int timeoutForConnectToHost = 10000;
};

#endif // TCPCLIENT_H

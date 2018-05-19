#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include <QString>
#include <QMap>

class HttpHeader
{
	public:
		HttpHeader();
		QString getMethod() const;
		void setMethod(const QString &value);

		QString getPath() const;
		void setPath(const QString &value);

		void setHostname(const QString &hostname);
		void setAddress(const QString &address);

		void setParam(const QString &name, const QString &value);
		QString getParam(const QString &name) const;
		QMap<QString, QString> getParams() const;
		QString getDomain() const;
		void setDomain(const QString &value);

		QString getScheme() const;
		void setScheme(const QString &value);

		int getPort() const;
		void setPort(int value);

		bool isConnect() const;
		QString getProtocol() const;
		void setProtocol(const QString &value);

		int getStatus() const;
		void setStatus(int status);

		QString getStatusText() const;
		void setStatusText(const QString &statusText);

		static HttpHeader fromRequest(const QByteArray &data);
		static HttpHeader fromResponse(const QByteArray &data);


	protected:
		QString protocol;
		QString method;
		QString path;
		QString domain;
		QString scheme;
		int port;
		int status = 0;
		QString statusText;
		QMap<QString, QString> params;
};

#endif // HTTPHEADER_H

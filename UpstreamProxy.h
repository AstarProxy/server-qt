#ifndef UPSTREAMPROXY_H
#define UPSTREAMPROXY_H

#include <QString>


struct UpstreamProxy {
	enum UpstreamProxyType {
		HTTP,
		SOCKS,
		Astar
	} type;
	QString hostname;
	int port = 0;
	QString username;
	QString password;
};

#endif // UPSTREAMPROXY_H


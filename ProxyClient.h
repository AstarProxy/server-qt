#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include "TcpClient.h"

class ProxyClient : public TcpClient
{
	public:
		ProxyClient();
		bool waitForConnected(int msecs = 30000);
		void setTimeoutForConnectToProxy(int timeoutForConnectToProxy);
		int getTimeoutForConnectToProxy() const;
		int getTimeoutForConnectToHost() const;
		void setTimeoutForConnectToHost(int timeoutForConnectToHost);

	protected:

		int timeoutForConnectToProxy = 10000;
		int timeoutForConnectToHost = 10000;
};

#endif // PROXYCLIENT_H

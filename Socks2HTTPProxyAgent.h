#ifndef SOCKS2HTTPPROXYAGENT_H
#define SOCKS2HTTPPROXYAGENT_H


#include "ProxyAgent.h"

class Socks2HTTPProxyAgent : public ProxyAgent
{
	public:
		Socks2HTTPProxyAgent(const QByteArray &firstPacket, QTcpSocket *client);
		void handle() override;
	protected:
		enum ClientProtocol {
			Socks4,
			Socksa4,
			Socks5,
		};
		enum AuthMethod {
			NoAuthentication,
			GSSAPI,
			Password,
		};
		enum Socks5Command {
			Connect,
			Bind,
			UDP_ASSOCIATE,
		};
		enum AddressType {
			IPv4,
			Domain,
			IPv6
		};
		QByteArray firstPacket;
		ClientProtocol clientProtocol;
		AuthMethod authMethod;
		void handshake();
		void authentication();
		void handleRequest();
		char readByte();
		void runConnectCommand(AddressType addressType, const QString &address, int port);
		QByteArray sendRequestResponse(char status, AddressType addressType, QString address, int port);
};
#endif // SOCKS2HTTPPROXYAGENT_H

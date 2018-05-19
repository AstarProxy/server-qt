QT += core network
QT -= gui

TARGET = server
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    Astar.cpp \
    Exceptions/Exception.cpp \
    Exceptions/InvalidProtocolException.cpp \
    Exceptions/BadHttpRequestException.cpp \
    Exceptions/DomainResolveException.cpp \
    Exceptions/TCPConnectTimeoutException.cpp \
    HttpHeader.cpp \
    Exceptions/BadFormattedAddressException.cpp \
    Exceptions/ProxyAuthException.cpp \
    Exceptions/TCPWriteTimeoutException.cpp \
    HTTPProxyAgent.cpp \
    ProxyAgent.cpp \
    SocksProxyAgent.cpp \
    Exceptions/UnsupportedSocksAuthMethodException.cpp \
    Exceptions/InvalidSocksCommandException.cpp \
    Socks2HTTPProxyAgent.cpp \
    ProxyClients/HttpProxyClient.cpp \
    Exceptions/UpstreamConnectionException.cpp \
    Exceptions/BadHttpResponseException.cpp \
    TcpClient.cpp \
    ProxyClient.cpp \
    ProxyClients/AstarProxyClient.cpp

HEADERS += \
    server.h \
    Astar.h \
    Exceptions/Exception.h \
    Exceptions/InvalidProtocolException.h \
    Exceptions/BadHttpRequestException.h \
    Exceptions/DomainResolveException.h \
    Exceptions/TCPConnectTimeoutException.h \
    HttpHeader.h \
    Exceptions/BadFormattedAddressException.h \
    Exceptions/ProxyAuthException.h \
    Exceptions/TCPWriteTimeoutException.h \
    HTTPProxyAgent.h \
    ProxyAgent.h \
    SocksProxyAgent.h \
    Exceptions/UnsupportedSocksAuthMethodException.h \
    Exceptions/InvalidSocksCommandException.h \
    Socks2HTTPProxyAgent.h \
    ProxyClients/HttpProxyClient.h \
    UpstreamProxy.h \
    Exceptions/UpstreamConnectionException.h \
    Exceptions/BadHttpResponseException.h \
    TcpClient.h \
    ProxyClient.h \
    ProxyClients/AstarProxyClient.h


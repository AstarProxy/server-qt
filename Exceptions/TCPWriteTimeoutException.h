#ifndef TCPWRITETIMEOUTEXCEPTION_H
#define TCPWRITETIMEOUTEXCEPTION_H

#include "Exception.h"

class TCPWriteTimeoutException : public Exception
{
	public:
		TCPWriteTimeoutException();
};

#endif // TCPWRITETIMEOUTEXCEPTION_H

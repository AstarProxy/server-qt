#ifndef BADHTTPREQUESTEXCEPTION_H
#define BADHTTPREQUESTEXCEPTION_H

#include "Exception.h"

class BadHttpRequestException : public Exception
{
	public:
		BadHttpRequestException(const QString &message);
};

#endif // BADHTTPREQUESTEXCEPTION_H

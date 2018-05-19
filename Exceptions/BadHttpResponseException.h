#ifndef BADHTTPRESPONSEEXCEPTION_H
#define BADHTTPRESPONSEEXCEPTION_H

#include "./Exception.h"

class BadHttpResponseException : public Exception
{
	public:
		BadHttpResponseException(const QString &message);
};

#endif // BADHTTPRESPONSEEXCEPTION_H

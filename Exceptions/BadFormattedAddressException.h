#ifndef BADFORMATTEDADDRESSEXCEPTION_H
#define BADFORMATTEDADDRESSEXCEPTION_H

#include "Exception.h"

class BadFormattedAddressException : public Exception
{
	public:
		BadFormattedAddressException(const QString &address);
		QString getAddress() const;

	protected:
		QString address;
};

#endif // BADFORMATTEDADDRESSEXCEPTION_H

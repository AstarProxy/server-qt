#include "BadFormattedAddressException.h"

BadFormattedAddressException::BadFormattedAddressException(const QString &address)
	:Exception("Cannot parse address")
{
	this->address = address;
}

QString BadFormattedAddressException::getAddress() const
{
	return address;
}


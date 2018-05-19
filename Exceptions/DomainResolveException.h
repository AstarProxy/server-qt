#ifndef DOMAINRESOLVEEXCEPTION_H
#define DOMAINRESOLVEEXCEPTION_H

#include "Exception.h"

class DomainResolveException : public Exception
{
	public:
		DomainResolveException(const QString &domain);
		QString getDomain() const;

	protected:
		QString domain;
};

#endif // DOMAINRESOLVEEXCEPTION_H

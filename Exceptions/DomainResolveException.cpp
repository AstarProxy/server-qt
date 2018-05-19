#include "DomainResolveException.h"

DomainResolveException::DomainResolveException(const QString &domain):
	Exception("Cannot resolve " + domain)
{
	this->domain = domain;
}

QString DomainResolveException::getDomain() const
{
	return domain;
}


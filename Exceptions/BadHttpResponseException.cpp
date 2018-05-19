#include "BadHttpResponseException.h"

BadHttpResponseException::BadHttpResponseException(const QString &message):
	Exception("BadHttpResponseException: " + message)
{

}


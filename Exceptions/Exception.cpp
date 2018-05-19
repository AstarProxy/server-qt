#include "Exception.h"

Exception::Exception()
{

}

Exception::Exception(const QString &message)
{
	this->message = message;
}

QString Exception::getMessage() const
{
	return message;
}

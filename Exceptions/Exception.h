#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

class Exception
{
	public:
		Exception();
		Exception(const QString &message);
		QString getMessage() const;

	protected:
		QString message;
};

#endif // EXCEPTION_H

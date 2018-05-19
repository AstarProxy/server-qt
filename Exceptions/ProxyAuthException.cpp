#include "ProxyAuthException.h"

ProxyAuthException::ProxyAuthException():
	Exception("Client have invalid username and password or have not them at all")
{

}


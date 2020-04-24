#include <netdb.h>
#include <stdlib.h>

struct hostent *gethostbyname2(const char *name, int af)
	{
	// Winsock's gethostbyname() only returns AF_INET addresses.
	if ( af != AF_INET )
		return NULL;
	return gethostbyname(name);
	}

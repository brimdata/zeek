#include <stdio.h>
#include <stdlib.h>

int vasprintf(char **ptr, const char *format, va_list arg)
	{
	int n = _vscprintf(format, arg);
	if (n < 0)
		return -1;
	char *p = (char *)malloc(n+1);
	if (p == NULL)
		return -1;
	int rv =vsprintf_s(p, n+1, format, arg);
	if (rv < 0)
		{
		free(p);
		return -1;
		}
	*ptr = p;
	return rv;
	}

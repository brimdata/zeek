#include <stdlib.h>

long random(void)
	{
	return rand();
	}

void srandom(unsigned seed)
	{
	srand(seed);
	}

char *realpath(const char *file_name, char *resolved_name)
	{
	return _fullpath(resolved_name, file_name, PATH_MAX);
	}

int setenv(const char *envname, const char *envval, int overwrite)
	{
	if (!overwrite && getenv(envname) != NULL)
		return 0;
	return _putenv_s(envname, envval);
	}

int unsetenv(const char *name)
	{
	return _putenv_s(name, "");
	}

#include <regex.h>

int regcomp(regex_t *preg, const char *pattern, int cflags)
	{
	return -1;
	}

size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
	{
	return 0;
	}

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
	{
	return -1;
	}

void regfree(regex_t *preg)
	{
	}

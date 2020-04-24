#include <glob.h>

int glob(const char *pattern, int flags, int(*errfunc)(const char *epath, int eerrno), glob_t *pglob)
	{
	return -1;
	}

void globfree(glob_t *pglob)
	{
	}

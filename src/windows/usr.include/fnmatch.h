#pragma once

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((dllimport)) int PathMatchSpecA(const char *pszFile, const char*pszSpec);

static int fnmatch(const char *pattern, const char *string, int flags)
	{
	return PathMatchSpecA(string, pattern);
	}

#ifdef __cplusplus
}
#endif

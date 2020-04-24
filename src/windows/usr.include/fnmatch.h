#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define FNM_NOMATCH 1

int fnmatch(const char *pattern, const char *string, int flags);

#ifdef __cplusplus
}
#endif

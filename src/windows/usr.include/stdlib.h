#pragma once

#include <../include/stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

long random(void);
void srandom(unsigned seed);
int setenv(const char *envname, const char *envval, int overwrite);
int unsetenv(const char *name);

#ifdef __cplusplus
}
#endif

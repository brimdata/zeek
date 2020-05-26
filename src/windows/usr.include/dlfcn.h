#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define RTLD_LAZY 0
#define RTLD_GLOBAL 0

void *dlopen(const char *file, int mode);
char *dlerror(void);

#ifdef __cplusplus
}
#endif

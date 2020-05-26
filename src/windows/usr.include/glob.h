#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t gl_pathc;
	char ** gl_pathv;
	size_t gl_offs;
} glob_t;

int glob(const char *pattern, int flags, int(*errfunc)(const char *epath, int eerrno), glob_t *pglob);
void globfree(glob_t *pglob);

#ifdef __cplusplus
}
#endif

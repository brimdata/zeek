#pragma once

#include <../include/stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define setlinebuf(stream) setvbuf((stream), (char *)NULL, _IOLBF, 0)

int vasprintf(char **ptr, const char *format, va_list arg);

#ifdef __cplusplus
}
#endif

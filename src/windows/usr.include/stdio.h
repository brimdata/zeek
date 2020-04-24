#pragma once

#include <../include/stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef L_tmpnam_s
#define L_tmpnam_s L_tmpnam
#endif

int vasprintf(char **ptr, const char *format, va_list arg);

#ifdef __cplusplus
}
#endif

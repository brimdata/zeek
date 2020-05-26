#pragma once

#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hostent *gethostbyname2(const char *name, int af);

#ifdef __cplusplus
}
#endif

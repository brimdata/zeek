#pragma once

#include <../include/string.h>

#define strerror_r(errnum, strerrbuf, buflen) strerror_s((strerrbuf), (buflen), (errnum))

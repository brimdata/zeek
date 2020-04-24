#pragma once

#include <../include/fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FD_CLOEXEC 0
#define F_GETFD 0
#define F_SETFD 0
#define F_GETFL 0
#define F_SETFL 0

#define O_CREAT  _O_CREAT
#define O_TRUNC  _O_TRUNC
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_CLOEXEC 0
#define O_NONBLOCK 0

int fcntl(int fildes, int cmd, ...);
int open(const char *path, int oflag, ... );

#ifdef __cplusplus
}
#endif


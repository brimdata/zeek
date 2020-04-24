#pragma once

#include <../include/unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned alarm(unsigned seconds);
int fork();
int fsync(int fildes);
pid_t getppid(void);
int link(const char *path1, const char * path2);
int pipe(int fildes[2]);
ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off64_t offset);
int setpgid(pid_t, pid_t);

#ifdef __cplusplus
}
#endif

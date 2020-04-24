#pragma once

#include <../include/signal.h>

#ifndef  _KQUEUE_WINDOWS_PLATFORM_H
typedef int sigset_t;
#endif

#define SIGHUP 1
#define SIGKILL 9
#define SIGBUS 10
#define SIGALRM 14
#define SIGPIPE 13
#define SIGCHLD 20

#define kill(pid, sig) (-1)
#define sigfillset(x) 0
#define sigdelset(x, y) 0
#define sigprocmask(x, y, z) 0

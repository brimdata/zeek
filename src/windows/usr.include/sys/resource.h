# pragma once

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RLIMIT_NOFILE 0

#define RLIM_INFINITY (~0ULL)

typedef unsigned long long rlim_t;

struct rlimit {
	rlim_t rlim_cur;
	rlim_t rlim_max;
};

int getrlimit(int resource, struct rlimit *rlp);
int setrlimit(int resource, const struct rlimit *rlp);

#define RUSAGE_SELF 0

struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	long ru_maxrss;
	long ru_ixrss;
	long ru_idrss;
	long ru_isrss;
	long ru_minflt;
	long ru_majflt;
	long ru_nswap;
	long ru_inblock;
	long ru_oublock;
	long ru_msgsnd;
	long ru_msgrcv;
	long ru_nsignals;
	long ru_nvcsw;
	long ru_nivcsw;
};

int getrusage(int who, struct rusage *r_usage);

#ifdef __cplusplus
}
#endif

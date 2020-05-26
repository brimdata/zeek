#include <string.h>
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit *rlp)
	{
	memset(rlp, 0, sizeof(*rlp));
	return 0;
	}

int setrlimit(int resource, const struct rlimit *rlp)
	{
	return 0;
	}

int getrusage(int who, struct rusage *r_usage)
	{
	memset(r_usage, 0, sizeof(*r_usage));
	return 0;
	}

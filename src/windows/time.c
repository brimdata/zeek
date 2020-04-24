#include <stdlib.h>
#include <time.h>

struct tm *gmtime_r(const time_t *timer, struct tm *result)
	{
	int err = gmtime_s(result, timer);
	if (err) {
		_set_errno(err);
		return NULL;
		}
	return result;
	}

struct tm *localtime_r(const time_t *timer, struct tm *result)
	{
	int err = localtime_s(result, timer);
	if (err) {
		_set_errno(err);
		return NULL;
		}
	return result;
	}

char* strptime(const char* s, const char* f, struct tm* tm)
	{
	return NULL;
	}

time_t timegm(struct tm *tm)
	{
	return _mkgmtime(tm);
	}

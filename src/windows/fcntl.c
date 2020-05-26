#include <fcntl.h>
#include <sys/stat.h>

int fcntl(int fildes, int cmd, ...)
	{
	return -1;
	}

int open(const char *path, int oflag, ... )
	{
	return _open(path, oflag, _S_IREAD | _S_IWRITE);
	}

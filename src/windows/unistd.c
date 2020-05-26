#include <unistd.h>
#include <windows.h>

unsigned alarm(unsigned seconds)
	{
	return 0;
	}

int fork()
	{
	return -1;
	}

int fsync(int fildes)
	{
	return FlushFileBuffers((HANDLE)_get_osfhandle(fildes)) ? 0 : -1;
	}

pid_t getppid(void)
	{
	return -1;
	}

int link(const char *path1, const char * path2)
	{
	return CreateHardLink(path2, path1, NULL) ? 0 : -1;
	}

int pipe(int fildes[2])
	{
	return -1;
	}

ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off64_t offset)
	{
	if (nbyte == 0)
		return 0;
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.Offset = offset;
	overlapped.OffsetHigh = offset >> 32;
	DWORD written;
	if (!WriteFile((HANDLE)_get_osfhandle(fildes), buf, nbyte, &written, &overlapped))
		return -1;
	return written;
	}

int setpgid(pid_t pid, pid_t pgid)
	{
	return -1;
	}

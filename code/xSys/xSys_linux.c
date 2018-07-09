#include "Macro_Proj.h"

#if WHICH_PLATFORM == _LINUX_PLATFORM_V
#include "xSys.h"
#include <windows.h>


TLPVOID xCreateThread(ThreadStartAddr startAddr, TLPVOID params) {
	return CreateThread(NULL, 0, startAddr, params, 0, NULL);
}

void xUsleep(TUINT32 milliseconds) {
	Sleep(milliseconds);
}


TUINT8 xIsAppRunning(const char *pAppName) {

	char pszLockFileName[256];
	strcpy(pszLockFileName, "/var/.lock.");
	if (pszAppName) strcat(pszLockFileName, pszAppName);
	int fd = open(pszLockFileName, O_CREAT | O_WRONLY);
	if (fd == -1)
	{
		printf("Unable to open the file: %s\n", pszLockFileName);
		close(fd);
		return false;
	}
	static struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int iRetVal = fcntl(fd, F_SETLK, &lock);
	return (iRetVal == -1);
}


#endif

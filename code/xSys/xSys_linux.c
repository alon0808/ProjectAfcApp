#include "Macro_Proj.h"

#if WHICH_PLATFORM == _LINUX_PLATFORM_V
#include "xSys.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/msg.h>

typedef struct
{
    long rcvThreadId;
    TMsgInfo msgInfo;
}Msg;


typedef struct thread_start_routine_param
{
    ThreadStartAddr ThreadEntry;
    TLPVOID pThreadParam;
    pthread_mutex_t *pMutex;
}tsrp;


static int gs_msgQueueId = -1;
static pthread_mutex_t gs_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_myMutex = PTHREAD_MUTEX_INITIALIZER;



static long getActualThreadId(TUINT32 threadId)
{
    const unsigned long mask = (unsigned long)(1 << (sizeof(TUINT32) * 8 - 1)) - 1;
    return (mask & threadId);
}

static void thread_signal_handler(int signum)
{
    signum = signum;
    pthread_exit(0);
}

static void * thread_start_routine(void *pParam)
{
	ThreadStartAddr ThreadEntry = NULL;
    TLPVOID pThreadParam = NULL;
	tsrp *myTSRP = (tsrp *)pParam;


	pthread_mutex_t *pMutex = NULL;

	ThreadEntry = myTSRP->ThreadEntry;
	pThreadParam = myTSRP->pThreadParam;
	pMutex = myTSRP->pMutex;

	signal(SIGUSR1, thread_signal_handler);
	pthread_mutex_unlock(pMutex);
	pMutex = NULL;
	return ThreadEntry(pThreadParam);
}


TLPVOID xCreateThread(TUINT32 *pThreadId, ThreadStartAddr ThreadEntry, TLPVOID pThreadParam)
{
	pthread_t threadId = 0;
	tsrp myTSRP;
	pthread_attr_t threadAttr;
	//pthread_mutex_t myMutex;

	if (ThreadEntry == NULL)
	{
		return Ret_Error;
	}

	myTSRP.ThreadEntry = ThreadEntry;
	myTSRP.pThreadParam = pThreadParam;
	myTSRP.pMutex = &s_myMutex;

	pthread_attr_init(&threadAttr);
	//pthread_attr_setschedpolicy(&attr, SCHED_RR);
	//param.sched_priority = 15;
	//pthread_attr_setschedparam(&attr, param);
	//pthread_create(&threadid, &attr, &threadfunc, NULL);
	//pthread_attr_destroy(&attr); 

	//pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
	//pthread_mutex_init(&myMutex, NULL);
	pthread_mutex_lock(&s_myMutex);
	if (pthread_create(&threadId, &threadAttr, thread_start_routine, (void *)(&myTSRP)) != 0)
	{
		return Ret_Error;
	}

	if (pThreadId != NULL)
	{
		*pThreadId = threadId;
	}
	pthread_mutex_lock(&s_myMutex);
	pthread_mutex_unlock(&s_myMutex);
	pthread_attr_destroy(&threadAttr);

	return Ret_OK;
}

TINT32 xSysStopThread(TUINT32 ThreadId)
{
	if (ThreadId == 0)
	{
		return Ret_Error;
	}
	if (pthread_kill(ThreadId, SIGUSR1) != 0)
	{
		return Ret_Error;
	}
	return Ret_OK;
}

TINT32 xSysGetCurrentThreadId(TUINT32 *pThreadId)
{
	if (pThreadId == NULL)
	{
		return Ret_Error;
	}
	*pThreadId = pthread_self();
	return Ret_OK;
}

void xUsleep(TUINT32 milliseconds) {
    usleep(milliseconds);
}


TUINT8 xIsAppRunning(const char *pAppName) {

	char pszLockFileName[256];
	strcpy(pszLockFileName, "/var/.lock.");
    if (pAppName) strcat(pszLockFileName, pAppName);
	int fd = open(pszLockFileName, O_CREAT | O_WRONLY);
	if (fd == -1)
	{
        PRINT_ERROR("Unable to open the file: %s\n", pszLockFileName);
		close(fd);
        return FALSE;
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



void xSysdestroyQueueIfNotExisted()
{
	pthread_mutex_lock(&gs_queue_mutex);
	if (gs_msgQueueId >= 0)
	{
		msgctl(gs_msgQueueId, IPC_RMID, NULL);

		gs_msgQueueId = -1;
	}
	pthread_mutex_unlock(&gs_queue_mutex);
}



TINT32 xSysPostThreadMsg(int threadId, TUINT32 msgId, stDataBuffer *pStData)
{
	Msg tempMsg;
	//createQueueIfNotExisted();
    tempMsg.rcvThreadId = getActualThreadId(threadId);
    tempMsg.msgInfo.pData = pStData->pBuffer;
    tempMsg.msgInfo.nData = pStData->blen;
	if (msgsnd(gs_msgQueueId, &tempMsg, sizeof(tempMsg) - sizeof(tempMsg.rcvThreadId), 0) != 0)
	{
		return Ret_Error;
	}
	return Ret_OK;
}

TINT32 xSysRecvThreadMsg(int threadId, TUINT32 msgID, /*@out@*/stDataBuffer *pRecvInfo)
{
	Msg tempMsg;
	long rcvThreadId;
	if (pRecvInfo == NULL)
	{
		return Ret_Error;
	}
	//createQueueIfNotExisted();
	memset(&tempMsg, 0, sizeof(tempMsg));
    rcvThreadId = getActualThreadId(threadId);
	if (msgrcv(gs_msgQueueId, &tempMsg, sizeof(tempMsg) - sizeof(tempMsg.rcvThreadId), rcvThreadId, 0) <= 0)
	{
		return Ret_Error;
	}
    pRecvInfo->pBuffer = tempMsg.msgInfo.pData;
    pRecvInfo->blen = tempMsg.msgInfo.nData;
	return Ret_OK;
}


#endif

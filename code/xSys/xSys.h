#ifndef _INCLUDED_X_SYS_H
#define _INCLUDED_X_SYS_H


//typedef TUINT32 (*ThreadStartAddr)(TLPVOID lpThreadParameter);

typedef unsigned int(*ThreadStartAddr)(TLPVOID lpThreadParameter);

/////////////////////////////////////////////////////////////////////////////////////////
// Macro for function invoke

// suspend the task
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysvTaskSuspend(task) SuspendThread(task)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysvTaskSuspend(task) pthread_suspend(task)
#endif

// resume the task
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysvTaskResume(task) ResumeThread((THANDLE)task)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysvTaskResume(task) pthread_resume(task)
#endif

// wait for task until end
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define	xSysTaskWaitToEnd
#endif

// Create mutex for multiple tasks
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysCreateMutext(mutext) InitializeCriticalSection(&mutext)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysCreateMutext(mutext) pthread_mutex_init(&mutext)
#endif

// judge the mutex is valid or not
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysIsMutextValid(mutext) BOOL_TRUE
#else
#define xSysIsMutextValid(mutext) (mutext != NULL)										
#endif

// Destroy mutex for multiple tasks
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysDeleteMutex(mutext) DeleteCriticalSection(&mutext)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysDeleteMutex(mutext) pthread_mutex_destroy(&mutext)
#endif

// take mutex
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysTakeMute(mutext)	EnterCriticalSection(&mutext)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysTakeMute(mutext)	pthread_mutex_lock(&mutext)
#endif

// give mutex
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysGiveMute(mutext)	LeaveCriticalSection(&mutext)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#define xSysGiveMute(mutext)	pthread_mutex_unlock(&mutext)
#endif

// Create queue
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysCreateQueue(queue, count, unitSize)	queue
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#endif

// Destroy queue
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysDeleteQueue(queue, count, unitSize)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
//#define xSysDeleteQueue(queue, count, unitSize)
#endif

// Destroy queue
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#define xSysDeleteQueueByModuleID(moduleId)
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
//#define xSysDeleteQueueByModuleID(moduleId)
#endif

// thread functions


#ifdef __cplusplus
extern "C"
{
#endif

	extern TLPVOID xCreateThread(TUINT32 *pThreadId, ThreadStartAddr startAddr, TLPVOID params);

	extern inline void xUsleep(TUINT32 milliseconds);

	extern TUINT8 xIsAppRunning(const char *pAppName);

	extern TLPVOID xSysLoadLib(const TINT8* pLibName, TINT32 OptionParam);

	extern TINT32 xSysGetLibName(const TLPVOID handle, TINT8* pLibPath, TINT32 pathMaxLen);

	extern void *xSysGetProcEntryAddr(TLPVOID libHandle, const TINT8 *pEntryName);


	// message queue
	//extern void *xSys_createQueueIfNotExisted(void);
	//extern TINT32 xSysdestroyQueueIfNotExisted(void *pQueueHandle);

	/**
	*@Description - Post module message
	*@Param ThreadId   :[IN]  Thread ID.
	*
	*@Return      :0, SUCCESSFUL; other, FAIL;
	*/
	/****************************************************/
	extern TINT32 xSysPostThreadMsg(int threadId, TUINT32 msgId, stDataBuffer *pStData);



	/****************************************************/
	/* Post module message
	@ RecvThreadId:[IN]   Receive thread ID.
	@ pRecvInfo   :[OUT]  Receive data info.

	@ return      :0, SUCCESSFUL; other, FAIL;
	*/
	/****************************************************/
	//extern TINT32 SysRecvThreadMsg(TThreadId RecvThreadId, UINT8 *pRecvInfo, TINT32 *pnRecvInfo);
	extern TINT32 xSysRecvThreadMsg(int threadId, TUINT32 msgID, /*@out@*/stDataBuffer *pRecvInfo);


#ifdef __cplusplus
};
#endif


#endif




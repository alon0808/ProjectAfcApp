#include "Macro_Proj.h"
#include "xStorage.h"
#include "UtilityProc.h"
#include "xSys.h"

#include <string.h>
#include <stdio.h>

#define SWITCH_s_SemaHandleCoreRam_USED		1
#define SWITCH_s_SemaHandleAppRam_USED		1

#define SIZE_RAM_CONTENT	10

typedef struct
{
	TUINT8 *m_pBuffer;
	TUINT32 m_buferLen;
}TBufferUnit;


static TStorageInit *s_pStorage = NULL;

static volatile TINT32 s_posAppRamUsed_TEMP = 0;
static volatile TINT32 s_posCoreRamUsed_TEMP = 0;
static volatile TINT32 s_posAppRamUsed = 0;
static volatile TINT32 s_posCoreRamUsed = 0;
static volatile TINT32 s_posTempRamUsed = 0;


//////////////////////////////////////////////////////////////////////////
// use for synchronization multi threads
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include <windows.h>
static CRITICAL_SECTION s_SemaHandleTempRam;
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#include <pthread.h>
static pthread_mutex_t s_SemaHandleTempRam = PTHREAD_MUTEX_INITIALIZER;
#endif

#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include <windows.h>
static CRITICAL_SECTION s_SemaHandleCoreRam;
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#include <pthread.h>
static pthread_mutex_t s_SemaHandleCoreRam = PTHREAD_MUTEX_INITIALIZER;
#endif

#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include <windows.h>
static CRITICAL_SECTION s_SemaHandleApplicationRam;
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#include <pthread.h>
static pthread_mutex_t s_SemaHandleApplicationRam = PTHREAD_MUTEX_INITIALIZER;
#endif

static TBufferUnit s_ramContent[SIZE_RAM_CONTENT];
//static volatile TINT32 s_ramContentSize = 0;

// for Doubly linked list storage manager
typedef struct
{
	TUINT8 *m_pBuffer;
	TUINT32 m_buferLen;
	void *m_pPrevious;
	void *m_pNext;
}TDoubleLinkNode;


#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include <windows.h>
static CRITICAL_SECTION s_SemaHandleDoubleLinkList;
#elif (WHICH_PLATFORM == _LINUX_PLATFORM_V)
#include <pthread.h>
static pthread_mutex_t s_SemaHandleDoubleLinkList = PTHREAD_MUTEX_INITIALIZER;
#endif

#define MaxSize_DoubleLinkList		20
static TDoubleLinkNode s_doubleLinkList[MaxSize_DoubleLinkList];
static TDoubleLinkNode *s_pHead = s_doubleLinkList;

// the buffer set from outside must be align to 4
TINT32 xStor_InstallStorage(TStorageInit *pStorage)
{
	if (pStorage == NULL) {
		return Ret_Err_Param;
	}
	s_pStorage = pStorage;

	xSysCreateMutext(s_SemaHandleTempRam);
	xSysCreateMutext(s_SemaHandleCoreRam);
	xSysCreateMutext(s_SemaHandleApplicationRam);


	memset(s_ramContent, 0x00, sizeof(TBufferUnit)*SIZE_RAM_CONTENT);
	s_posCoreRamUsed = 0;
	s_posTempRamUsed = 0;
	s_posAppRamUsed = 0;

	// for Doubly linked list storage manager
	xSysCreateMutext(s_SemaHandleDoubleLinkList);
	memset(s_doubleLinkList, 0x00, sizeof(TDoubleLinkNode)*MaxSize_DoubleLinkList);
	s_pHead->m_pBuffer = pStorage->m_pDoubleLinkListRamBegin;
	s_pHead->m_buferLen = 0;
	s_pHead->m_pNext = s_pHead;
	s_pHead->m_pPrevious = s_pHead;

	return Ret_OK;
}

TINT32 xStor_UnInstallStorage(void)
{
#if SWITCH_s_SemaHandleAppRam_USED
	xSysDeleteMutex(s_SemaHandleApplicationRam);
#endif
#if SWITCH_s_SemaHandleCoreRam_USED
	xSysDeleteMutex(s_SemaHandleCoreRam);
#endif
	xSysDeleteMutex(s_SemaHandleTempRam);

	xSysDeleteMutex(s_SemaHandleTempRam);
	xSysDeleteMutex(s_SemaHandleCoreRam);
	xSysDeleteMutex(s_SemaHandleApplicationRam);

	s_pStorage = NULL;
	// for Doubly linked list storage manager
	xSysDeleteMutex(s_SemaHandleDoubleLinkList);

	return Ret_OK;
}



#if SPLIT_BAT	// for kernel use
#endif

TINT32 xStor_ClearCoreStorage(void)
{
#if SWITCH_s_SemaHandleCoreRam_USED
	xSysTakeMute(s_SemaHandleCoreRam);
#endif
	s_posCoreRamUsed = 0;
#if SWITCH_s_SemaHandleCoreRam_USED
	xSysGiveMute(s_SemaHandleCoreRam);
#endif
	return Ret_OK;
}

TUINT8 *xStor_MallocCoreRam(TINT32 bufferSize)
{
	TUINT8 *pRetValue = NULL;
	//TINT32 tmpI = 0;

	if ((s_pStorage == NULL) || (s_pStorage->m_pCoreRamBegin == NULL) || (bufferSize <= 0))
	{
		return NULL;
	}
	// make sure align to 4
	if ((bufferSize & 0x03) != 0)
	{
		bufferSize = ((bufferSize + 3) & 0xFFFFFFFC);
	}
#if SWITCH_s_SemaHandleCoreRam_USED
	xSysTakeMute(s_SemaHandleCoreRam);
#endif
	if ((bufferSize) > (s_pStorage->m_coreRamLen - s_posCoreRamUsed))
	{	// fail to malloc ram
		xSysGiveMute(s_SemaHandleCoreRam);
		return NULL;
	}
	pRetValue = s_pStorage->m_pCoreRamBegin + s_posCoreRamUsed;
	// must be align 4 bytes,  otherwise TLV_PTR* will be incorrect when get its value 

	s_posCoreRamUsed += bufferSize;

	memset(pRetValue, 0x00, bufferSize);
#if SWITCH_s_SemaHandleCoreRam_USED
	xSysGiveMute(s_SemaHandleCoreRam);
#endif
	return pRetValue;
}

#if SWITCH_CODE_USED
TUINT8 *xStor_MallocKernelRamExclusive(TINT32 bufferSize, TINT32 isStart)
{
	TUINT8 *pRetValue = NULL;

	if (isStart != BOOL_FALSE)
	{
#if SWITCH_s_SemaHandleCoreRam_USED
		xSysTakeMute(s_SemaHandleCoreRam);
#endif
		s_posCoreRamUsed_TEMP = s_posCoreRamUsed;
	}

	if ((s_pStorage == NULL) || (s_pStorage->m_pCoreRamBegin == NULL))
	{
		return NULL;
	}
	if ((bufferSize) > (s_pStorage->m_coreRamLen - s_posCoreRamUsed_TEMP))
	{	// fail to malloc ram, max is 800 20160124
		return NULL;
	}
	pRetValue = s_pStorage->m_pCoreRamBegin + s_posCoreRamUsed_TEMP;
	memset(pRetValue, 0x00, bufferSize);
	s_posCoreRamUsed_TEMP += bufferSize;
#if SWITCH_s_SemaHandleCoreRam_USED
	//xSemaphoreGive(s_SemaHandleRam);
#endif
	return pRetValue;
}

TINT32 xStor_FreeKernelRamExclusive(void *pBuffer)
{
	if (pBuffer == NULL || (pBuffer == s_pStorage->m_pCoreRamBegin + s_posCoreRamUsed))
	{
#if SWITCH_s_SemaHandleCoreRam_USED
		xSysGiveMute(s_SemaHandleCoreRam);
#endif
		return Ret_OK;
	}
	return Ret_Error;
}
#endif

#if SPLIT_BAT	// for demo use
#endif

TINT32 xStor_ClearAppStorage(void)
{
#if SWITCH_s_SemaHandleAppRam_USED
	xSysTakeMute(s_SemaHandleApplicationRam);
#endif
	s_posAppRamUsed = 0;
#if SWITCH_s_SemaHandleAppRam_USED
	xSysGiveMute(s_SemaHandleApplicationRam);
#endif
	return Ret_OK;
}

TUINT8 *xStor_MallocAppRam(TINT32 bufferSize)
{
	TUINT8 *pRetValue = NULL;
	//TINT32 tmpI = 0;

	if ((s_pStorage == NULL) || (s_pStorage->m_pAppRamBegin == NULL) || (bufferSize <= 0))
	{
		return NULL;
	}
	// make sure align to 4
	if ((bufferSize & 0x03) != 0)
	{
		bufferSize = ((bufferSize + 3) & 0xFFFFFFFC);
	}
#if SWITCH_s_SemaHandleAppRam_USED
	xSysTakeMute(s_SemaHandleApplicationRam);
#endif
	if ((bufferSize) > (s_pStorage->m_appRamLen - s_posAppRamUsed))
	{	// fail to malloc ram
		xSysGiveMute(s_SemaHandleApplicationRam);
		return NULL;
	}
	pRetValue = s_pStorage->m_pAppRamBegin + s_posAppRamUsed;

	s_posAppRamUsed += bufferSize;

	memset(pRetValue, 0x00, bufferSize);
#if SWITCH_s_SemaHandleAppRam_USED
	xSysGiveMute(s_SemaHandleApplicationRam);
#endif
	return pRetValue;
}

#if SWITCH_CODE_USED
TUINT8 *xStor_MallocDemoRamExclusive(TINT32 bufferSize, TINT32 isStart)
{
	TUINT8 *pRetValue = NULL;

	if (isStart != BOOL_FALSE)
	{
#if SWITCH_s_SemaHandleAppRam_USED
		xSysTakeMute(s_SemaHandleApplicationRam);
#endif
		s_posAppRamUsed_TEMP = s_posAppRamUsed;
	}

	if ((s_pStorage == NULL) || (s_pStorage->m_pAppRamBegin == NULL))
	{
		return NULL;
	}

	if ((bufferSize) > (s_pStorage->m_appRamLen - s_posAppRamUsed_TEMP))
	{	// fail to malloc ram, max is 800 20160124
		return NULL;
	}
	pRetValue = s_pStorage->m_pAppRamBegin + s_posAppRamUsed_TEMP;
	memset(pRetValue, 0x00, bufferSize);
	s_posAppRamUsed_TEMP += bufferSize;
#if SWITCH_s_SemaHandleAppRam_USED
	//xSemaphoreGive(s_SemaHandleRam);
#endif
	return pRetValue;
}

TINT32 xStor_FreeDemoRamExclusive(void *pBuffer)
{
	if (pBuffer == NULL || (pBuffer == s_pStorage->m_pAppRamBegin + s_posAppRamUsed))
	{
#if SWITCH_s_SemaHandleAppRam_USED
		xSysGiveMute(s_SemaHandleApplicationRam);
#endif
		return Ret_OK;
	}
	LOOP_FOREVER;
	//return Ret_Error;
}
#endif


#if SPLIT_BAT	// for temporary use(maybe kernel or demo)
#endif

TINT32 xStor_ClearTempStorage()
{
	xSysTakeMute(s_SemaHandleTempRam);
	memset(s_ramContent, 0x00, sizeof(TBufferUnit)*SIZE_RAM_CONTENT);
	s_posTempRamUsed = 0;	// 0x6A78
	xSysGiveMute(s_SemaHandleTempRam);

	//s_posRamUsed = 0;
	return Ret_OK;
}



TINT32 xStor_MallocTempRam(TINT32 bufferSize)
{
	TINT32 contentIndex = SIZE_RAM_CONTENT;
	TINT32 i = 0;
	//	UINT8 *pRam = NULL;
	TBufferUnit *pBufferUnit = NULL;
	TINT32 retValue = 0;

	if ((s_pStorage == NULL) || (s_pStorage->m_pTempRamBegin == NULL) || (bufferSize <= 0))
	{
		return retValue;
	}
#if 0
	if (bufferSize == 56)
	{
		bufferSize = bufferSize;
	}
#endif
	// make sure align to 4
	if ((bufferSize & 0x03) != 0)
	{
		bufferSize = ((bufferSize + 3) & 0xFFFFFFFC);
	}

	xSysTakeMute(s_SemaHandleTempRam);
	if ((bufferSize) > (s_pStorage->m_tempRamLen - s_posTempRamUsed))
	{	// fail to malloc ram
		xSysGiveMute(s_SemaHandleTempRam);
		return retValue;
	}
	for (i = 0; i<SIZE_RAM_CONTENT; ++i)
	{
		if (s_ramContent[i].m_pBuffer == NULL)
		{
			contentIndex = i;
			break;
		}
	}
	if (contentIndex == SIZE_RAM_CONTENT)
	{
		xSysGiveMute(s_SemaHandleTempRam);
		return retValue;
	}
	pBufferUnit = s_ramContent + contentIndex;

	pBufferUnit->m_pBuffer = s_pStorage->m_pTempRamBegin + s_posTempRamUsed;
	pBufferUnit->m_buferLen = bufferSize;

	s_posTempRamUsed += bufferSize;

	memset(pBufferUnit->m_pBuffer, 0x00, bufferSize);

	//xSysGiveMute(s_SemaHandleTempRam);
	retValue = contentIndex + 1;

	return retValue;
}


TINT32 xStor_FreeTempRam(TINT32 pointRef)
{
	TINT32 bufferSize;
	TBufferUnit *pBufferUnit = NULL;
	TINT32 i = 0;

	if ((s_pStorage == NULL) || (s_pStorage->m_pTempRamBegin == NULL)
		|| (pointRef > SIZE_RAM_CONTENT) || (pointRef < 0))
	{	// fail to malloc ram
		return Ret_Error;
	}
	if (pointRef == 0)
	{	// no need to release the buffer
		return Ret_OK;
	}

	--pointRef;
	pBufferUnit = s_ramContent + pointRef;
	//xSysTakeMute(s_SemaHandleTempRam);
	bufferSize = pBufferUnit->m_buferLen;
	if (pBufferUnit->m_pBuffer == NULL || bufferSize == 0)
	{
		xSysGiveMute(s_SemaHandleTempRam);
		return Ret_OK;
	}
	// decrease the position of ram
	if ((pBufferUnit->m_pBuffer + bufferSize) == (s_pStorage->m_pTempRamBegin + s_posTempRamUsed))
	{	// the last one
		s_posTempRamUsed -= bufferSize;
	}
	else
	{
		// this situation is forbidden
		PRINT_ERROR("LOOP_FOREVER");
		LOOP_FOREVER;

		s_posTempRamUsed -= bufferSize;
		memcpyE(pBufferUnit->m_pBuffer, pBufferUnit->m_pBuffer + bufferSize, (s_posTempRamUsed - (pBufferUnit->m_pBuffer - s_pStorage->m_pTempRamBegin)));
		for (i = 0; i<SIZE_RAM_CONTENT; ++i)
		{
			if (s_ramContent[i].m_pBuffer > pBufferUnit->m_pBuffer)
			{
				s_ramContent[i].m_pBuffer -= bufferSize;
			}
		}
	}
	/*if (s_posTempRamUsed == 0)
	{
	s_posTempRamUsed = s_posTempRamUsed;
	}*/
	memset(pBufferUnit, 0x00, sizeof(TBufferUnit));

	xSysGiveMute(s_SemaHandleTempRam);

	return Ret_OK;
}

TUINT8 *xStor_GetTempRamPoint(TINT32 pointRef)
{
	TBufferUnit *pBufferUnit = NULL;
	TUINT8 *pRetValue = NULL;

	if ((s_pStorage == NULL) || (s_pStorage->m_pTempRamBegin == NULL)
		|| (pointRef > SIZE_RAM_CONTENT) || (pointRef < 0))
	{	// fail to malloc ram
		return pRetValue;
	}
	if (pointRef == 0)
	{	// no need to release the buffer
		return pRetValue;
	}
	--pointRef;
	pBufferUnit = s_ramContent + pointRef;
	//xSysTakeMute(s_SemaHandleTempRam);
	pRetValue = pBufferUnit->m_pBuffer;
	//xSysGiveMute(s_SemaHandleTempRam);

	return pRetValue;
}

TUINT8 *xStor_MallocForThreads(TINT32 bufferSize)
{
	TDoubleLinkNode *pDoubleLink = NULL;
	//TUINT8 *pBuffer = NULL;
	TINT32 i = 0;

	if (s_pStorage == NULL || s_pStorage->m_pDoubleLinkListRamBegin == NULL)
	{
		return NULL;
	}
	if (bufferSize <= 0)
	{	// 
		return NULL;
	}
	// make sure align to 4
	if ((bufferSize & 0x03) != 0)
	{
		bufferSize = ((bufferSize + 3) & 0xFFFFFFFC);
	}
	pDoubleLink = s_pHead;
	xSysTakeMute(s_SemaHandleDoubleLinkList);
	// check whether the space is enough or not
	do
	{
		if (pDoubleLink->m_pNext == s_pHead)
		{	// the last node
			if ((s_pStorage->m_pDoubleLinkListRamBegin + s_pStorage->m_doubleLinkListRamLen) < (pDoubleLink->m_pBuffer + pDoubleLink->m_buferLen + bufferSize))
			{	// space not enough
				xSysGiveMute(s_SemaHandleDoubleLinkList);
				return NULL;
			}
			//pBuffer = pDoubleLink->m_pBuffer+pDoubleLink->m_buferLen;
			break;
		}
		else
		{	// not the last node
			if ((((TDoubleLinkNode *)pDoubleLink->m_pNext)->m_pBuffer - (pDoubleLink->m_pBuffer + pDoubleLink->m_buferLen)) > bufferSize)
			{	// idle space has enough space
				//pBuffer = pDoubleLink->m_pBuffer+pDoubleLink->m_buferLen;
				break;
			}
		}
		pDoubleLink = pDoubleLink->m_pNext;
	} while (pDoubleLink != s_pHead);

	//pBuffer = pDoubleLink->m_pBuffer+pDoubleLink->m_buferLen;
	// search the null node in array
	i = 0;
	//if (s_pHead->m_buferLen > 0 && pDoubleLink == s_pHead)
	{
		for (/*i=0*/; i<MaxSize_DoubleLinkList; ++i)
		{
			if (s_doubleLinkList[i].m_pBuffer == NULL)
			{
				break;
			}
		}
		if (i >= MaxSize_DoubleLinkList)
		{	// array list is full
			xSysGiveMute(s_SemaHandleDoubleLinkList);
			return NULL;
		}
	}

	s_doubleLinkList[i].m_pBuffer = pDoubleLink->m_pBuffer + pDoubleLink->m_buferLen;
	s_doubleLinkList[i].m_buferLen = bufferSize;
	memset(s_doubleLinkList[i].m_pBuffer, 0x00, s_doubleLinkList[i].m_buferLen);
	s_doubleLinkList[i].m_pNext = pDoubleLink->m_pNext;
	s_doubleLinkList[i].m_pPrevious = pDoubleLink;
	//
	((TDoubleLinkNode *)pDoubleLink->m_pNext)->m_pPrevious = s_doubleLinkList + i;
	pDoubleLink->m_pNext = s_doubleLinkList + i;
	xSysGiveMute(s_SemaHandleDoubleLinkList);

	return s_doubleLinkList[i].m_pBuffer;
}


TINT32 xStor_FreeForThreads(void *pSrc)
{
	TDoubleLinkNode *pDoubleLink = NULL;
	//TUINT8 *pBuffer = NULL;
	//	TINT32 i = 0;

	if (s_pStorage == NULL || s_pStorage->m_pDoubleLinkListRamBegin == NULL)
	{
		return Ret_Error;
	}

	if (pSrc == NULL)
	{
		return Ret_OK;
	}

	xSysTakeMute(s_SemaHandleDoubleLinkList);
	pDoubleLink = s_pHead->m_pNext;
	// check whether the space is enough or not
	while (pDoubleLink != s_pHead)
	{
		if (pDoubleLink->m_pBuffer == pSrc)
		{
			break;
		}
		pDoubleLink = pDoubleLink->m_pNext;
	}
	if (pDoubleLink == s_pHead)
	{	// not found
		xSysGiveMute(s_SemaHandleDoubleLinkList);
		return Ret_Error;
	}

	//
	((TDoubleLinkNode *)pDoubleLink->m_pNext)->m_pPrevious = pDoubleLink->m_pPrevious;
	((TDoubleLinkNode *)pDoubleLink->m_pPrevious)->m_pNext = pDoubleLink->m_pNext;

	memset(pDoubleLink, 0x00, sizeof(TDoubleLinkNode));

	xSysGiveMute(s_SemaHandleDoubleLinkList);

	return Ret_OK;
}


#if SPLIT_BAR	// other buffer
#endif


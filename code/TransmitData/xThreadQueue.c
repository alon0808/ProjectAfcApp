#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xThreadQueue.h"
#include "UtilityProc.h"
#include "xSys.h"
#include "xStorage.h"

#include <stdio.h>

#define POS_FLAG	0
#define POS_LEN		1
#define POS_DATA	3

#define xSYS_MSG_ID		5
#define TQ_TIME_SLEEP	100

typedef enum {
	DF_INVALID = 0,
	DF_WRITE,
	DF_READ
}emDataFlag;
/**
* @Description - create hand of shared memory
*
* @Param szName(char []) - name of shared memory
* @Param projId(char *) - id of shared memory, not invalid in windows
* @Param size(int) - size of shared memory
*
* @Return int(ERet_Code) - 0 is successful
*/
TLPVOID xTQOpenQueue(TUINT32 threadId) {
	return xSysCreateQueue(threadId);
}

int xTQCloseQueue(TLPVOID pQueueHandle) {
	xSysDeleteQueue(pQueueHandle);
	return Ret_OK;
}
/**
* @Description - shared memory write data, not check the len limit
*
* @Param hFile(TLPVOID) - handle of shared memory
* @Param pData(char *) - data buffer for write
* @Param len(int) - data length
*
* @Return int(ERet_Code) - 0 is successful
*/
int xTQWriteData(IN TLPVOID threadId, IN stDataBuffer *pStData) {
	TUINT16 remainTimes = 0;
	TUINT8 count = 3;
	TUINT8 sigId = 0;
	//UINT8 *pSigBuf = (UINT8*)xStor_Malloc(3000);
	TUINT8 *pSigBuf = NULL;


	if (pStData == NULL)
	{
		return Ret_Error;
	}
	pSigBuf = (TUINT8*)xStor_MallocForThreads(pStData->blen);
	memcpyE(pSigBuf, pStData->pBuffer, pStData->blen);

	pStData->pBuffer = pSigBuf;
	//xDbgHex("send signal: ", signalBuffer, length);
	//STD_PRINTF("SysPostThreadMsg1: %d.\n", sModule_ThreadID[recvModule]);
	while (xSysPostThreadMsg((int)threadId, xSYS_MSG_ID, pStData) != Ret_OK)
	{
		if (count == 0)
		{
			PRINT_ERRORBYS("Send signal fail2", &threadId, 1);
			//xStor_Free(pSigBuf);
			return Ret_Error;
		}
		--count;

		//retCode = GetLastError();
		xUsleep(TQ_TIME_SLEEP);
		PRINT_ERRORBYS("Send signal fail2 continue: ", pSigBuf, pStData->blen);
		//return FAIL;
	}

	return Ret_OK;
}
/**
* @Description - shared memory read data, not check the len limit
*
* @Param hFile(TLPVOID) - handle of shared memory
* @Param pData(char *) - data buffer for read
* @Param len(int *) - data length, length limit for IN and read length for OUT
*
* @Return int(ERet_Code) - 0 is successful
*/
int xTQReadData(IN TLPVOID threadId, OUT stDataBuffer *pStData) {
	int retcode = Ret_OK;
	stDataBuffer tmpstData;

	retcode = xSysRecvThreadMsg((int)threadId, xSYS_MSG_ID, &tmpstData);
	if (retcode != Ret_OK)  //return only detect a message
	{
		return retcode;
	}
	memcpyE(pStData->pBuffer, tmpstData.pBuffer, tmpstData.blen);
	pStData->blen = tmpstData.blen;

	xStor_FreeForThreads(tmpstData.pBuffer);

	return Ret_OK;
}


#endif

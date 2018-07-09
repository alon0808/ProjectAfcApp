#include "Macro_Proj.h"
#include "TransmitData.h"
#include "xCom.h"
#include "xSharedMemory.h"
#include "xSocketClient.h"
#include "xStorage.h"

#include <string.h>
#include <stdio.h>

//#define FIFO_WATCHDOG	"fifowatchdog"	
#define HANDLES_COUNT	5


typedef enum {
	TT_INVALID = 0,
	TT_COM = 1,
	TT_SOCKET,
	TT_SHARED_MEMORY,
	TT_TREAD_QUEUE
}emTransmitType;

typedef struct {
	TLPVOID handle;
	int transmitType;
	TUINT8 *pDataBuffer;
}stHandleInfo;

//TLPVOID s_handles[5] = { NULL, NULL, NULL, NULL, NULL };
//TLPVOID s_handles[5] = { NULL, NULL, NULL, NULL, NULL };
static stHandleInfo *s_phandleInfo = NULL;
static TUINT8 s_isInit = FALSE;


static int registHandle(int transmitType, TLPVOID pHandle) {
	int i;

	if (s_phandleInfo == NULL) {
		return REF_INVALID;
	}
	//s_phandleInfo = xStor_MallocAppRam(sizeof(stHandleInfo)*HANDLES_COUNT);
	if (pHandle <= 0) {
		return -1;
	}
	for (i = 0; i < HANDLES_COUNT; ++i)
	{
		if (s_phandleInfo[i].handle == NULL) {
			s_phandleInfo[i].handle = pHandle;
			s_phandleInfo[i].transmitType = transmitType;
			break;
		}
	}
	return i;
}

static void initTransmitData() {
	if (s_phandleInfo == NULL) {
		s_phandleInfo = (stHandleInfo *)xStor_MallocAppRam(sizeof(stHandleInfo)*HANDLES_COUNT);
		//memset(s_phandleInfo, 0x00, sizeof(stHandleInfo)*HANDLES_COUNT);
		//s_isInit = TRUE;
	}
}
/**
 * @Description - init com serial
 *
 * @Param comN(char[]) - N of COM
 * @Param baund(int) - baund of COM
 *
 * @Return (int) - fail when less than 0, else is successful
 */
int InitCom(char comN[], int baund) {
	TLPVOID hand = 0;

	initTransmitData();
	hand = xOpenCom(comN, baund);

	return registHandle(TT_COM, hand);
}

int initSharedMemory(char curPath[], int projId, int size) {
	TLPVOID hand = 0;

	initTransmitData();
	hand = xSMOpenSharedMemory(curPath, projId, size);

	PRINT_DEBUG("initSharedMemory:%d,%s", hand, curPath);

	return registHandle(TT_SHARED_MEMORY, hand);
}

int initSocket(char ip[], int port) {
	TUINT32 hand = 0;

	initTransmitData();
	SocketClientInit(ip, port, &hand);

	return registHandle(TT_SOCKET, (TLPVOID)hand);
}

int initThreadQueue(TUINT32 threadId) {
	TUINT32 hand = 0;

	initTransmitData();
	hand = xTQOpenQueue(threadId);

	return registHandle(TT_TREAD_QUEUE, (TLPVOID)hand);
}

int uninstallHandle(int hand) {
	stHandleInfo *pHandleInfor = NULL;
	int retcode = Ret_OK;
	pHandleInfor = s_phandleInfo + hand;
	if (pHandleInfor->handle == NULL) {
		return Ret_Error;
	}
	switch (pHandleInfor->transmitType)
	{
	case TT_COM:
		return Ret_Error;
		break;
	case TT_SHARED_MEMORY:
		retcode = xSMCloseSharedMemory(pHandleInfor->handle);
		break;
	case TT_SOCKET:
		return Ret_Error;
		break;
	case TT_TREAD_QUEUE:
		retcode = xTQCloseQueue(pHandleInfor->handle);
		break;
	default:
		return Ret_Err_N_Support;
	}
	memset(pHandleInfor, 0x00, sizeof(stHandleInfo));

	return retcode;
}

int writeData(int hand, stDataBuffer *pStData) {
	stHandleInfo *pHandleInfor = NULL;
	int retcode = Ret_OK;

	if (hand < 0 || hand > HANDLES_COUNT) {
		return Ret_Err_Param;
	}
	pHandleInfor = s_phandleInfo + hand;
	if (pHandleInfor->handle == NULL) {
		return Ret_Error;
	}
	switch (pHandleInfor->transmitType)
	{
	case TT_COM:
		return Ret_Error;
		break;
	case TT_SHARED_MEMORY:
		retcode = xSMWriteData(pHandleInfor->handle, pStData);
		break;

	case TT_SOCKET:
		return Ret_Error;
		break;
	case TT_TREAD_QUEUE:
		retcode = xTQWriteData(pHandleInfor->handle, pStData);
		break;
	default:
		return Ret_Err_N_Support;
	}

	return retcode;
}

int readData(int hand, stDataBuffer *pStData) {
	stHandleInfo *pHandleInfor = NULL;
	int retcode = Ret_OK;

	if (hand < 0 || hand > HANDLES_COUNT) {
		return Ret_Err_Param;
	}
	pHandleInfor = s_phandleInfo + hand;
	if (pHandleInfor->handle == NULL) {
		return Ret_Error;
	}
	switch (pHandleInfor->transmitType)
	{
	case TT_COM:
		return Ret_Error;
		break;
	case TT_SHARED_MEMORY:
		retcode = xSMReadData(pHandleInfor->handle, pStData);
		break;

	case TT_SOCKET:
		return Ret_Error;
		break;
	case TT_TREAD_QUEUE:
		retcode = xTQReadData(pHandleInfor->handle, pStData);
		break;
	default:
		return Ret_Err_N_Support;
	}

	return retcode;
}

#if SPLIT_BAR	// other buffer
#endif


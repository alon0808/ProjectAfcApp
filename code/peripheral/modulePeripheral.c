/************************************************************************/
/* process display, audio, keypad                                       */
/************************************************************************/
#include "Macro_Proj.h"
#include "xDev.h"
#include "modulePeripheral.h"
#include "UtilityProc.h"
#include "xSys.h"
#include "TransmitData.h"
#include "xStorage.h"
#include "TransmitData.h"

#include <string.h>
#include <stdio.h>

#define SharedMemoryName			"sharedMemoryPeripheral"
#define SharedMemoryName_Watchdog	"sharedMemoryWatchDog"

#define POS_BUF_TYPE	0
#define POS_BUF_PARAM	1

#define PeripheralRAMSize	500

// declare static function
static void * ProcModulePeripheral(TLPVOID params);

static TLPVOID s_threadDisplay = NULL;
static TUINT32 s_threadDisplayId = 0;
static int s_hSharedMoery = REF_INVALID;
static int s_hSharedMoeryWatchDog = REF_INVALID;
static unsigned char *s_pDataBuffer = NULL;
static volatile int s_threadDisplayStatus = TS_Invalid;



static void * ProcModulePeripheral(TLPVOID params) {

    char *curPath = (char *)params;
    unsigned char *pDataBuf = NULL;;
    //int len = 0;
    int tmpI, tmpI1, tmpI2;
    char *pTmpCh = NULL;
    int pos = 0;
    int retcode = 0;
    stDataBuffer stData;
    //	char aaaa[1000];

    PRINT_DEBUG("ProcModulePeripheral(curPath):%s", curPath);

    s_threadDisplayStatus = TS_Running;
    stData.pBuffer = s_pDataBuffer;

    while (curPath != NULL)
    {
        stData.blen = PeripheralRAMSize;
        //pDataBuf = s_pDataBuffer;
        retcode = readData(s_hSharedMoery, &stData);
        if (retcode == Ret_OK) {

            pDataBuf = stData.pBuffer;
            switch (pDataBuf[POS_BUF_TYPE])
            {
            case PT_Display:
                pos = POS_BUF_PARAM;
                tmpI = GET_INT32(pDataBuf + pos);
                pos += 4;
                tmpI1 = GET_INT32(pDataBuf + pos);
                pos += 4;
                tmpI2 = GET_INT32(pDataBuf + pos);
                pos += 4;
                pTmpCh = (char *)pDataBuf + pos;
#if SWITCH_DEBUG_UI
                sprintf(pDataBuf + len, "%d,%d,%d,%s", tmpI, tmpI1, tmpI2, pTmpCh);
                PRINT_DEBUG(pDataBuf + len);
#endif
                ShowInUI(tmpI, tmpI1, pTmpCh, tmpI2);
                break;
            default:
                break;
            }
        }
        else if (retcode == Ret_Err_Fatal) {	// need to realloc the share memory
            unregisterHandle(s_hSharedMoery);
            s_hSharedMoery = initSharedMemory(SharedMemoryName, 0x00, 50);
            if (s_hSharedMoery < 0) {
                return (void *)Ret_Error;
            }
        }
        PRINT_INFOR("xUsleep");
        //xUsleep(50);
    }
    s_threadDisplayStatus = TS_Terminate;

    PRINT_INFOR("ProcModulePeripheral exit");
    return (void *)Ret_OK;
}



/**
* @Description - start application, start thread
*
* @Param curPath(char[]) - current path of the executable binary
* @Param funcShowInUI(FuncShowInUI) - function of show user interface
*
* @Return (int,ERet_Code) - fail when less than 0, else is successful
*/
int initPeripheral(char curPath[], TFuncShowInUI funcShowInUi) {
	// init device

	xDev_Init(funcShowInUi);

	s_hSharedMoeryWatchDog = initSharedMemory(SharedMemoryName_Watchdog, 0x00, 8);
	if (s_hSharedMoeryWatchDog < 0) {
		return Ret_Error;
	}
	if (s_pDataBuffer == NULL) {
		s_pDataBuffer = xStor_MallocAppRam(PeripheralRAMSize);
		if (s_pDataBuffer == NULL) {
			PRINT_ERROR("initPeripheral.fail to xStor_MallocAppRam:%d\n", PeripheralRAMSize);
			return Ret_Error;
		}
	}
	// start service
	if (s_threadDisplay == NULL) {
		s_threadDisplay = xCreateThread(&s_threadDisplayId, ProcModulePeripheral, curPath);
		if (s_threadDisplay == NULL) {
			PRINT_ERROR("initPeripheral.fail to create AppTransaction by application, application exit\n");
			return Ret_Error;
		}
	}
	s_hSharedMoery = initThreadQueue(s_threadDisplayId);
	if (s_hSharedMoery < 0) {
		PRINT_ERROR("initPeripheral.fail to initThreadQueue:%d\n", s_threadDisplayId);
		return Ret_Error;
	}
	return Ret_OK;
}

int sendDisplayMsg(int row, int column, char *pMsg, int textPosition) {
	int len = 0;
	int refTmp = 0;
    TUINT8 *pTmpBuf = NULL;
	int pos = 0;
	int retCode = Ret_OK;
	stDataBuffer stData;

	if (pMsg == NULL) {
		return Ret_Err_Param;
	}
	len = strlen(pMsg) + 1;

	refTmp = xStor_MallocTempRam(len + 12);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		return Ret_Error;
	}
	pos = 0;
	pTmpBuf[pos] = PT_Display;
	++pos;
	SET_INT32(pTmpBuf + pos, row);
	pos += 4;
	SET_INT32(pTmpBuf + pos, column);
	pos += 4;
	SET_INT32(pTmpBuf + pos, textPosition);
	pos += 4;
	memcpyE(pTmpBuf + pos, pMsg, len);
	pos += len;
	stData.pBuffer = pTmpBuf;
	stData.blen = pos;
	retCode = writeData(s_hSharedMoery, &stData);
	xStor_FreeTempRam(refTmp);

	return retCode;
}

int feedWatchDog(void) {
	int retCode = Ret_OK;
	stDataBuffer stData;


    stData.pBuffer = (TUINT8 *)"\x01\x01\x01\x01";
	stData.blen = 5;
	retCode = writeData(s_hSharedMoeryWatchDog, &stData);

	return retCode;
}


#if SPLIT_BAR	// other buffer
#endif


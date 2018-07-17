#include "Macro_Proj.h"
#include "Application.h"
#include "xSys.h"
#include "xStorage.h"
#include "xAppConfig.h"
#include "modulePeripheral.h"
#include "xCardOpera.h"
#include "UtilityProc.h"
#include "Transaction_M1.h"
#include "SecuritySam.h"
#include "Transaction.h"

#include <stdlib.h>
//#include <windows.h>

#define AppName "afc_app.x"

static TLPVOID s_threadApp = NULL;
TStorageInit s_storInit;
static TUINT8 s_coreRam[1000];
static TUINT8 s_appRam[1000];
static TUINT8 s_tempRam[1000];
static TUINT8 s_doublelinkRam[1000];


static void * ProcAppMain(TLPVOID params) {
	TUINT32 retValue = THREAD_AppTransaction;
	int i = 3, j = 1;
	int retcode = Ret_OK;

	retcode = InitTransaction();
	if (retcode != Ret_OK) {
        UNUSED_VAR(params);
		PRINT_ERROR("fail to InitTransaction:%d", retcode);
		//return retcode;
	}

	while (TRUE) {

		retcode = ProcessCardTransaction(); 
		if (retcode != Ret_OK) {
			//PRINT_ERROR("fail to InitTransaction:%d", retcode);
			//return retcode;
		}

		sendDisplayMsg(i, j, "ProcAppMain³ÌÐò", TP_INVALID);
		++i;
		++j;
		if (i > 8) {

			feedWatchDog();
			i = 1;
		}
		if (j > 30) {
			j = 1;
		}

		xUsleep(1000);

	}

	PRINT_INFOR("ProcAppMain exit");

    return (void *)retValue;
}

void InitApp(char curPath[], TFuncShowInUI funcShowInUI) {
	// init storage
	int retCode = Ret_Error;
	TStorageInit *pStorInit = &s_storInit;

	pStorInit->m_pCoreRamBegin = s_coreRam;
	pStorInit->m_coreRamLen = sizeof(s_coreRam);
	pStorInit->m_pAppRamBegin = s_appRam;
	pStorInit->m_appRamLen = sizeof(s_appRam);
	pStorInit->m_pTempRamBegin = s_tempRam;
	pStorInit->m_tempRamLen = sizeof(s_tempRam);
	pStorInit->m_pDoubleLinkListRamBegin = s_doublelinkRam;
	pStorInit->m_doubleLinkListRamLen = sizeof(s_doublelinkRam);
	retCode = xStor_InstallStorage(pStorInit);
	if (retCode != Ret_OK)
	{
		PRINT_ERROR("fail to install storage, application exit\n");
		return;
	}
	if (xIsAppRunning(AppName)) {
		exit(1);
		return;
	}
	// init param file
	installAfcParam(curPath);
	// init peripheral	
	retCode = initPeripheral(curPath, funcShowInUI);
	if (retCode != Ret_OK) {
		PRINT_ERROR("initPeripheral err:%d", retCode);
	}

	xUsleep(200);
}


void StartApp(void) {
	// init storage
//	int retCode = Ret_Error;

	feedWatchDog();
	//funcShowInUI(3, 5, "ÇëË¢¿¨°ÉM1", TP_MIDDLE);
	// config core
	if (s_threadApp == NULL) {
		s_threadApp = xCreateThread(NULL, ProcAppMain, NULL);
		if (s_threadApp == NULL) {
			PRINT_ERROR("fail to create Main Thread, application exit\n");
			return;
		}
	}
	//xUsleep(500);

}

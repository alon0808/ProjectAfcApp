#include "Macro_Proj.h"
#include "Transaction.h"
#include "UtilityProc.h"
#include "xCardOpera.h"
//#define _MODULE_PERIPHERAL_N_FUNCTION_C
#include "modulePeripheral.h"
#include "Transaction_M1.h"
#include "SecuritySam.h"
//#define _XCARD_OPERA_N_FUNCTION_C


#include <string.h>
//#include <stdio.h>

int GetCardKind(int cardType)
{
	if (cardType < 20)
		return CT_USER_CARD;
	if ((cardType >= 61) && (cardType < 74))
		return CT_CONTROL_CARD;
	return CT_OTHER;
}

int InitTransaction() {
	int pos = 0;
	int retcode = Ret_OK;

	// init smart card
	xSCardInit();
	// init sam card
	pos = 1;
	sendDisplayMsg(pos, 1, "初始化SAM卡1", TP_LEFT);
    //retcode = SAM_Verify(SAM_SLOT_GS);
	if (retcode != Ret_OK) {
		sendDisplayMsg(pos, 1, "Err", TP_RIGHT);
		PRINT_ERROR("fail to SAM_Verify:%d,%d", SAM_SLOT_GS, retcode);
	}
	else {
		sendDisplayMsg(pos, 1, "OK", TP_RIGHT);
	}
	++pos;
	sendDisplayMsg(pos, 1, "初始化SAM卡2", TP_LEFT);
    //retcode = SAM_Verify(SAM_SLOT_ZJB);
	if (retcode != Ret_OK) {
		sendDisplayMsg(pos, 1, "Err", TP_RIGHT);
		PRINT_ERROR("fail to SAM_Verify:%d,%d", SAM_SLOT_ZJB, retcode);
	}
	else {
		sendDisplayMsg(pos, 1, "OK", TP_RIGHT);
	}
	++pos;

	// init parameters


	return retcode;
}


int ProcessCardTransaction() {
	int retcode = Ret_OK;
	stCardInfor *pCardInfo = NULL;
	int slot = SSLOT_CONTACTLESS;

	retcode = xSCardWaitForPresentCard(slot, 0, 0);
	if (retcode == XSCARD_S_SUCCESS) {	// card is already on reader
		pCardInfo = xScardGetCardInfor(slot);
		switch (pCardInfo->cardtype)
		{
		case eXSCardType_M1:
			retcode = dealCardM1(pCardInfo);
			break;
		default:
			retcode = Ret_Error;
			break;
		}

		if (retcode != Ret_OK) {	// error occur in transaction
			PRINT_ERROR("error in transaction:%d", retcode);
		}
	}




	return retcode;
}

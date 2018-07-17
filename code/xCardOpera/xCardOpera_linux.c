#include "Macro_Proj.h"

#if WHICH_PLATFORM == _LINUX_PLATFORM_V
#include "xCardOpera.h"
#include "xSys.h"
#include "xStorage.h"
#include "UtilityProc.h"

#include <stdio.h>
#include <string.h>

static stCardInfor s_cardInfor[SSLOT_MAX_LEN];

static inline int getSlotReal(int slot) {
	int slot_real = SSLOT_INVALID;

	switch (slot)
	{
	case SSLOT_SAM1:
		slot_real = SSLOT_SAM1_REL;
		break;
	case SSLOT_SAM2:
		slot_real = SSLOT_SAM2_REL;
		break;
	case SSLOT_CONTACTLESS:
		slot_real = SSLOT_CONTACTLESS_REL;
		break;
	default:
		//tmpI = SSLOT_INVALID;
		break;
	}

	return slot_real;
}

int xSCardInit() {
#define PATH_LEN	500
/*	void *hModDll = NULL;
	int retCode = Ret_OK;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
    int tmpI = 0;*/

	return Ret_OK;

#undef PATH_LEN
}
// pnReaders is reader buffer, pReaders is amount
int xSCardGetReaderList(TINT8 *pReaders, TINT32 *pnReaders) {
	if (pReaders == NULL || pnReaders == NULL) {
		return Ret_Err_Param;
	}
	return Ret_OK;
}

//Extend function
int xSCardSetReaderName(const char *name)
{
	int retcode = 0;
	int reader = GET_INT16(name);
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	short port = 100;
	int baud = 115200;
	int hcard = 0;
	int i;

	refTmp = xStor_MallocTempRam(32);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
        UNUSED_VAR(reader);
		goto xSCardSetReaderName_OVER;
	}

	for (i = 0; i < SSLOT_MAX_LEN; ++i) {
		s_cardInfor[i].hCard = hcard;
	}

	PRINT_INFOR("Name of reader:%s", pTmpBuf);
    sprintf((char *)pTmpBuf, "information of reader(port, baud):%d, %d", port, baud);
	PRINT_INFOR(pTmpBuf);

xSCardSetReaderName_OVER:
	xStor_FreeTempRam(refTmp);

	return retcode;
}

int xSCardWaitForPresentCard(TUINT8 slot, int baund, TUINT32 dwTimeout) {
	//unsigned char cardid[6] = { 0x00 };
	int retcode = Ret_OK;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int pos = 0;
	int tmpI = 0;
	stCardInfor *pCardInfor = NULL;
    int slot_real = 0;
//	unsigned char rLen;

	if (slot <= SSLOT_INVALID || slot > SSLOT_MAX_LEN) {
		retcode = XSCARD_ERROR;
        UNUSED_VAR(baund);
        UNUSED_VAR(slot_real);
        UNUSED_VAR(dwTimeout);
		goto xSCardWaitForPresentCard_OVER;
	}
	pCardInfor = s_cardInfor + slot - 1;
	pCardInfor->slot = slot;
	slot_real = getSlotReal(slot);
	memset(pCardInfor->ats_atr, 0x00, sizeof(pCardInfor->ats_atr));
	memset(pCardInfor->cardId, 0x00, sizeof(pCardInfor->cardId));
	pCardInfor->cardtype = eXSCardType_Invalid;
	pCardInfor->sak = 0;

	refTmp = xStor_MallocTempRam(32);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
		goto xSCardWaitForPresentCard_OVER;
	}

	if (retcode < Ret_OK) {
		if (retcode == -1) {	// invalid handle
			retcode = XSCARD_ERROR;
		}
		else if (retcode == -2) {	// no card on reader, or multiple cards
			retcode = XSCARD_ERROR;
		}
		else {
			PRINT_ERROR("xSCardWaitForPresentCard unknown error:%d", retcode);
			retcode = XSCARD_F_UNKNOWN_ERROR;
		}

		goto xSCardWaitForPresentCard_OVER;
	}
	if (retcode == 0x05) {	// M1 card,sak is 0x08  f0 38 e0 3c 08
		pCardInfor->cardtype = eXSCardType_M1;
		pos = 0;
		memcpyE(pCardInfor->cardId + SCARD_POS_VALUE, pTmpBuf + pos, CARDID_SIZE);
		pCardInfor->cardId[SCARD_POS_LEN] = CARDID_SIZE;
		pos += CARDID_SIZE;
		pCardInfor->sak = pTmpBuf[pos];
	}
	else if (retcode == 0x15 || retcode == 0x17 || retcode == 0x0A || retcode == 0x16) {	// CPU card as M1, sak is 0x28, 0x20
		if (retcode == 0x15) {// sak is 0x28, 23 56 49 e7 28 10 78 80 b0 02 20 90 00 c0 fb 75 fa 23 56 49 e7 00 株洲CPU卡
			pCardInfor->cardtype = eXSCardType_CPU_M1;
		}
		else if (retcode == 0x17) {	// sak is 0x28, 71 29 8c be 28 12 78 00 80 02 80 65 05 47 04 86 53 00 00 00 00 00 00 00 长沙银行卡
			pCardInfor->cardtype = eXSCardType_CPU;
		}
		else if (retcode == 0x16) { // financial card. sak is 0x20, 83 77 c5 a1 20 11 78 80 b0 02 4d 54 00 90 00 20 00 90 83 77 c5 a1 00,桂民测试卡,交通联合
			pCardInfor->cardtype = eXSCardType_BANK;
		}
		else if (retcode == 0x0A) { // financial card. sak is 0x20, cd 88 71 62 20 05 78 80 a0 02 00 工行卡
			pCardInfor->cardtype = eXSCardType_BANK;
		}
		//pCardInfor->cardtype = retcode == 0x15 ? eXSCardType_CPU_M1 : eXSCardType_CPU;
		pos = 0;
		memcpy(pCardInfor->cardId + SCARD_POS_VALUE, pTmpBuf + pos, CARDID_SIZE);
		pCardInfor->cardId[SCARD_POS_LEN] = CARDID_SIZE;
		pos += CARDID_SIZE;
		pCardInfor->sak = pTmpBuf[pos];
		++pos;
		tmpI = pTmpBuf[pos];
		++pos;
        if (tmpI > (int)sizeof(pCardInfor->ats_atr)) {
			tmpI = sizeof(pCardInfor->ats_atr);
		}
		memcpy(pCardInfor->ats_atr + SCARD_POS_VALUE, pTmpBuf + pos, tmpI);
		pCardInfor->ats_atr[SCARD_POS_LEN] = tmpI;

	}
	else {	// other card
		PRINT_WARNING("xSCardWaitForPresentCard unknown card:%d", retcode);
		pCardInfor->cardtype = eXSCardStatus_Unknown;
		pos = 0;
		memcpy(pCardInfor->cardId + SCARD_POS_VALUE, pTmpBuf + pos, CARDID_SIZE);
		pCardInfor->cardId[SCARD_POS_LEN] = CARDID_SIZE;
		pos += CARDID_SIZE;
		pCardInfor->sak = pTmpBuf[pos];
	}
	PRINT_DEBUGBYS("already has card:", pCardInfor, sizeof(stCardInfor));
	retcode = XSCARD_S_SUCCESS;

xSCardWaitForPresentCard_OVER:
	xStor_FreeTempRam(refTmp);
	return retcode;
}


int xSCardTransmit(TUINT8 slot, const stDataBuffer *pSend, stDataBuffer *pRecv, TUINT16 *pSw1sw2)
{
	stCardInfor *pCardInfor = NULL;
	int retcode = Ret_OK;
	TUINT8 rLen = 0;
	int slot_real = SSLOT_INVALID;
	//	TUINT8 kmode;


	pRecv->blen = 0;
	*pSw1sw2 = 0;

	if (slot <= SSLOT_INVALID || slot > SSLOT_MAX_LEN) {
		retcode = XSCARD_ERROR;
        UNUSED_VAR(pCardInfor);
		goto xSCardTransmit_OVER;
	}
	pCardInfor = s_cardInfor + slot - 1;
	slot_real = getSlotReal(slot);

	if (slot_real == SSLOT_INVALID) {
		PRINT_ERROR("xSCardTransmit: invalid slot:%d", slot_real);
		retcode = XSCARD_ERROR;
		goto xSCardTransmit_OVER;
	}
	PRINT_DEBUGBYS("xSCardTransmit send:", pSend->pBuffer, pSend->blen);

	if (retcode != Ret_OK) {	// fail to read
		PRINT_ERRORBYS("xSCardTransmit: fail to getKeyCardKeyA:", pSend->pBuffer, rLen);
		goto xSCardTransmit_OVER;
	}
	// pRecv->blen = rLen;
	PRINT_DEBUGBYS("xSCardTransmit recv:", pRecv->pBuffer, rLen);
	if (rLen >= 2) {
		*pSw1sw2 = GET_INT16(pRecv->pBuffer + rLen - 2);
		rLen -= 2;
	}
	pRecv->blen = rLen;

xSCardTransmit_OVER:
	return retcode;
}



int xSCardTransmit_M1(TUINT8 slot, int func, int block, TUINT8 isTypeA, const stDataBuffer *pKey, const stDataBuffer *pSend, stDataBuffer *pRecv)
{
	stCardInfor *pCardInfor = NULL;
    int retcode = XSCARD_S_SUCCESS;
    TUINT8 kmode;

	if (slot != SSLOT_CONTACTLESS) {
		retcode = XSCARD_ERROR;
        UNUSED_VAR(pCardInfor);
        UNUSED_VAR(kmode);
        UNUSED_VAR(pSend);
		goto xSCardTransmit_OVER;
	}
	--slot;
	pCardInfor = s_cardInfor + slot;

	if (func == SMM_AuthBlock || func == SMM_ReadBlock || func == SMM_WriteBlock) {
		// authentication the block
		if (pKey != NULL) {
            kmode = (isTypeA != BOOL_FALSE) ? 0 : 1;
            if (retcode == 0x14) {	// error key
				PRINT_ERRORBYS("xSCardTransmit_M1: fail to auth(error key):", pKey->pBuffer, pKey->blen);
				goto xSCardTransmit_OVER;
			}
			else if (retcode != Ret_OK) {	// fail to authentication
				PRINT_ERROR("xSCardTransmit_M1: fail to auth(block):%d", block);
				goto xSCardTransmit_OVER;
			}
		}
		else if (func == SMM_AuthBlock) {
			retcode = XSCARD_ERROR;
			goto xSCardTransmit_OVER;
		}
	}
	if (func == SMM_ReadBlock) {
		// read block
		if (pRecv == NULL) {
			retcode = XSCARD_ERROR;
			goto xSCardTransmit_OVER;
		}
		if (retcode != Ret_OK) {	// fail to read
			PRINT_ERROR("xSCardTransmit_M1: fail to read(block):%d", block);
			goto xSCardTransmit_OVER;
		}
		pRecv->blen = BLOCK_SIZE;
		PRINT_DEBUGBYS("APDU resp:", pRecv->pBuffer, pRecv->blen);
	}

xSCardTransmit_OVER:
	return retcode;
}


TINT32 xSCardConnect(int slot)
{
	UNUSED_VAR(slot);

	return XSCARD_S_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////

stCardInfor *xScardGetCardInfor(TUINT8 slot) {
	if (slot <= SSLOT_INVALID || slot > SSLOT_MAX_LEN) {
		return NULL;
	}
	--slot;
	return s_cardInfor + slot;
}



#endif

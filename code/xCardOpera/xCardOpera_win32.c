#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xCardOpera.h"
#include "xSys.h"
#include "xStorage.h"
#include "UtilityProc.h"

#include <stdio.h>
#include <string.h>

#define LIB_SMARTCARD	"R8FRDLL.dll"

#pragma region 读卡器动态库函数类型定义
typedef int(CALL_CONVERT_STD *TFuncdc_getLibVer)(void);
typedef int(CALL_CONVERT_STD *TFuncdc_init)(TINT16 port, int baud);
typedef int (CALL_CONVERT_STD *TFuncgetComName)(TUINT8 *Val);
typedef unsigned short (CALL_CONVERT_STD *TFuncdc_getUsbList)(TUINT16 *uListBuf);
typedef unsigned short(CALL_CONVERT_STD *TFuncdc_setUsbRev)(TUINT16 usbREvNo);
typedef int (CALL_CONVERT_STD *TFuncdc_exit)(int icdev);

typedef int (CALL_CONVERT_STD *TFuncdc_cpureset)(int icdev, unsigned char _SAMID, int baund, unsigned char *rlen, unsigned char *rbuff);
// SAM卡
typedef int (CALL_CONVERT_STD *TFuncdc_cpuapdu)(int icdev, unsigned char _SAMID, unsigned char slen, unsigned char *sbuff, unsigned char *rlen, unsigned char *rbuff);
// CONTACTLESS卡
typedef int (CALL_CONVERT_STD *TFuncdc_pro_command)(int icdev, unsigned char slen, unsigned char *sbuff, unsigned char *rlen, unsigned char *rbuff);

typedef int (CALL_CONVERT_STD *TFuncdc_card)(int icdev, unsigned char _Mode, unsigned char *_Snr);
typedef int (CALL_CONVERT_STD *TFuncdc_authentication_passaddr)(int icdev, unsigned char _Mode, unsigned char blockAddr, unsigned char *_Snr, unsigned char *passbuff);
typedef int (CALL_CONVERT_STD *TFuncdc_read)(int icdev, unsigned char _Adr, unsigned char *_Data);

static TFuncdc_getLibVer s_funcdc_getLibVer = NULL;
static TFuncdc_init s_funcdc_init = NULL;
static TFuncgetComName s_funcgetComName = NULL;
static TFuncdc_getUsbList s_funcdc_getUsbList = NULL;
static TFuncdc_setUsbRev s_funcdc_setUsbRev = NULL;

static TFuncdc_cpureset s_funcdc_cpureset = NULL;
// typedef int (CALL_CONVERT_STD *TFuncdc_cpuapdu)(int icdev, unsigned char _SAMID, unsigned char slen, unsigned char *sbuff, unsigned char *rlen, unsigned char *rbuff);
static TFuncdc_cpuapdu s_funcdc_cpuapdu = NULL;
// typedef int (CALL_CONVERT_STD *TFuncdc_pro_command)(int icdev, unsigned char slen, unsigned char *sbuff, unsigned char *rlen, unsigned char *rbuff);
static TFuncdc_pro_command s_funcdc_pro_command = NULL;
/**
功 能：寻卡，能返回在工作区域内某张卡的序列号(该函数包含了dc_request,dc_anticoll,dc_select的整体功能)

参 数：icdev：通讯设备标识符

_Mode：寻卡模式mode_card

=0x80卡作为M1卡模式使用。【如果CPU卡当作M1卡使用时】

=0x40卡作为CPU卡模式使用，会进行卡复位操作。

其它正常使用，_Mode 传入0，刷卡机跟据卡返回状态自动判断是CPU卡还是M1卡。

_Snr：返回的卡序列号或加上CPU卡的复位信息。

返 回：成功则返回 0
*/
static TFuncdc_card s_funcdc_card = NULL;
static TFuncdc_authentication_passaddr s_funcdc_authentication_passaddr = NULL;
static TFuncdc_read s_funcdc_read = NULL;


static stCardInfor s_cardInfor[SSLOT_MAX_LEN];

#pragma endregion 读卡器动态库函数类型定义

static inline getSlotReal(int slot) {
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
	void *hModDll = NULL;
	int retCode = Ret_OK;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int tmpI = 0;

	if (s_funcdc_init == NULL) {
		memset(&s_cardInfor, 0x00, sizeof(stCardInfor)*SSLOT_MAX_LEN);

		refTmp = xStor_MallocTempRam(PATH_LEN);
		pTmpBuf = xStor_GetTempRamPoint(refTmp);

		hModDll = xSysLoadLib(LIB_SMARTCARD, 0);
		if (!hModDll)
		{
			PRINT_ERROR("Fail to load "LIB_SMARTCARD);
			return Ret_Error;
		}
		if (pTmpBuf != NULL) {
			retCode = xSysGetLibName(hModDll, pTmpBuf, PATH_LEN);

			PRINT_INFOR("InitDefaultValue: %s", pTmpBuf);
			xStor_FreeTempRam(refTmp);
		}

		s_funcdc_getLibVer = (TFuncdc_getLibVer)xSysGetProcEntryAddr(hModDll, "dc_getLibVer");
		s_funcdc_init = (TFuncdc_init)xSysGetProcEntryAddr(hModDll, "dc_init");
		s_funcgetComName = (TFuncgetComName)xSysGetProcEntryAddr(hModDll, "getComName");
		s_funcdc_getUsbList = (TFuncdc_getUsbList)xSysGetProcEntryAddr(hModDll, "dc_getUsbList");
		s_funcdc_setUsbRev = (TFuncdc_setUsbRev)xSysGetProcEntryAddr(hModDll, "dc_setUsbRev");

		s_funcdc_cpureset = (TFuncdc_cpureset)xSysGetProcEntryAddr(hModDll, "dc_cpureset");
		s_funcdc_cpuapdu = (TFuncdc_cpuapdu)xSysGetProcEntryAddr(hModDll, "dc_cpuapdu");
		s_funcdc_pro_command = (TFuncdc_pro_command)xSysGetProcEntryAddr(hModDll, "dc_pro_command");

		s_funcdc_card = (TFuncdc_card)xSysGetProcEntryAddr(hModDll, "dc_card");
		s_funcdc_authentication_passaddr = (TFuncdc_authentication_passaddr)xSysGetProcEntryAddr(hModDll, "dc_authentication_passaddr");
		s_funcdc_read = (TFuncdc_read)xSysGetProcEntryAddr(hModDll, "dc_read");

		tmpI = s_funcdc_getLibVer();
		PRINT_INFOR(LIB_SMARTCARD "Version: %d", tmpI);
	}
	if (s_funcdc_getLibVer == NULL) {
		return Ret_Error;
	}
	return Ret_OK;

#undef PATH_LEN
}
// pnReaders is reader buffer, pReaders is amount
int xSCardGetReaderList(TINT8 *pReaders, TINT32 *pnReaders) {
	if (pReaders == NULL || pnReaders == NULL) {
		return Ret_Err_Param;
	}
	*pReaders = (TINT8)s_funcdc_getUsbList((TUINT16 *)pnReaders);
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
		goto xSCardSetReaderName_OVER;
	}

	retcode = s_funcdc_setUsbRev(reader);
	if (retcode != reader) {
		retcode = Ret_Error;
		goto xSCardSetReaderName_OVER;
	}
	hcard = s_funcdc_init(port, baud);
	if (hcard <= REF_INVALID) {
		retcode = Ret_Error;
		goto xSCardSetReaderName_OVER;
	}
	for (i = 0; i < SSLOT_MAX_LEN; ++i) {
		s_cardInfor[i].hCard = hcard;
	}
	retcode = s_funcgetComName(pTmpBuf);

	PRINT_INFOR("Name of reader:%s", pTmpBuf);
	sprintf(pTmpBuf, "information of reader(port, baud):%d, %d", port, baud);
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
	unsigned char rLen;

	if (slot <= SSLOT_INVALID || slot > SSLOT_MAX_LEN) {
		retcode = XSCARD_ERROR;
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
	if (slot_real == SSLOT_CONTACTLESS_REL) {
		retcode = s_funcdc_card(pCardInfor->hCard, 0, pTmpBuf);
	}
	else {
		retcode = s_funcdc_cpureset(pCardInfor->hCard, slot_real, baund, &rLen, pTmpBuf);
		if (retcode < Ret_OK) {
			retcode = XSCARD_ERROR;
		}
		else {
			memcpyE(pCardInfor->ats_atr + SCARD_POS_VALUE, pTmpBuf, rLen);
			pCardInfor->ats_atr[SCARD_POS_LEN] = rLen;
			pCardInfor->cardtype = eXSCardType_SAM;
			retcode = XSCARD_S_SUCCESS;
			goto xSCardWaitForPresentCard_OVER;
		}
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
		if (tmpI > sizeof(pCardInfor->ats_atr)) {
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
	if (slot_real == SSLOT_CONTACTLESS_REL) {	// contactless reader
		retcode = s_funcdc_pro_command(pCardInfor->hCard, pSend->blen, pSend->pBuffer, &rLen, pRecv->pBuffer);
	}
	else {
		retcode = s_funcdc_cpuapdu(pCardInfor->hCard, slot_real, pSend->blen, pSend->pBuffer, &rLen, pRecv->pBuffer);
	}
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
	int retcode;
	TUINT8 kmode;

	if (slot != SSLOT_CONTACTLESS) {
		retcode = XSCARD_ERROR;
		goto xSCardTransmit_OVER;
	}
	--slot;
	pCardInfor = s_cardInfor + slot;

	if (func == SMM_AuthBlock || func == SMM_ReadBlock || func == SMM_WriteBlock) {
		// authentication the block
		if (pKey != NULL) {
			kmode = (isTypeA != BOOL_FALSE) ? 0 : 1;
			retcode = s_funcdc_authentication_passaddr(pCardInfor->hCard, kmode, block, pCardInfor->cardId + SCARD_POS_VALUE, pKey->pBuffer);
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
		retcode = s_funcdc_read(pCardInfor->hCard, block, pRecv->pBuffer);	// 0x10 bytes
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

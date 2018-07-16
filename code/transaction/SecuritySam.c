#include "Macro_Proj.h"
#define _TRANSACTION_M1_N_FUNCTION_C
#include "Transaction_M1.h"
#include "SecuritySam.h"
#include "xCardOpera.h"
#include "xSys.h"
#include "xStorage.h"
#include "UtilityProc.h"

#include <stdio.h>


#define SAM_CMD_SELECT_FILE	"\x00\xA4\x00\x00\x02"	// 选择文件
#define SAM_CMD_VERIFY_PIN	"\x00\x20\x00\x00\x03"	// 选择文件
#define SAM_CMD_GET_M1KEY_GS	"\x80\xFC\x10\x40\x0F"	// 公司
#define SAM_CMD_GET_M1KEY_ZJB	"\x80\xFC\x01\x01\x0F"	// 住建部

typedef struct {
	TUINT8 m1Key62[6];                // key of sector 2,6
	TUINT8 m1Key543[6];                // key of sector 2,6
	TUINT8 m1Key87[6];                // key of sector 2,6
}stM1KeyStore;

typedef union
{
	stM1KeyStore m1KeyStore;
}unKeyStore;


static unKeyStore *s_pKeyStore = NULL;
static s_isSamOK[SSLOT_MAX_LEN];

static int GS_PSAM_Verify(int Idx);

static inline int initSam(int slot)
{
	int baud = 9600;
	int retcode = Ret_OK;

	if (slot == SAM_SLOT_ZJB) {
		baud = 38400;
	}
	if (s_pKeyStore == NULL) {	// init first time
		memset(s_isSamOK, 0x00, SSLOT_MAX_LEN);
		s_pKeyStore = (unKeyStore *)xStor_MallocCoreRam(sizeof(unKeyStore));
		if (s_pKeyStore == NULL) {
			PRINT_ERROR("initSam.fail to malloc unKeyStore:%d", s_pKeyStore);
			return Ret_Error;
		}
	}

	retcode = xSCardWaitForPresentCard(slot, baud, 0);
	return retcode;
}

int SAM_Verify(int slot)
{
	int ret = Ret_OK;

	if (slot <= SSLOT_INVALID || slot > SSLOT_MAX_LEN) {
		return Ret_Err_Param;
	}
	s_isSamOK[slot - 1] = BOOL_FALSE;
	ret = initSam(slot);
	if (ret != Ret_OK)
	{
		return ret;
	}
	//xUsleep(500);
	switch (slot)
	{
	case SAM_SLOT_GS://公司密钥
		ret = GS_PSAM_Verify(slot);
		break;
	case SAM_SLOT_ZJB://住建部密钥
		//ret = ZJB_PSAM_Verify();
		break;
	case 21://岭南通密钥
		//ret = LNT_PSAM_Verify();
		break;
	case 31://交通部密钥
		//ret = JTB_PSAM_Verify();
		break;
	default:
		ret = Ret_Error;
		break;
	}
	if (ret == Ret_OK) {
		s_isSamOK[slot - 1] = BOOL_TRUE;
	}
	return ret;
}

//公司密钥初始化,某些项目会用到,勿删
static int GS_PSAM_Verify(int Idx)
{
	stDataBuffer dataSend;
	stDataBuffer dataRecv;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int retcode = Ret_OK;
	int tmpI = 0;
	int pos;
	int slot = SAM_SLOT_GS;
	TUINT16 sw1sw2 = 0x0000;


	refTmp = xStor_MallocTempRam(100);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
		goto GS_PSAM_Verify_OVER;
	}

	//ret = IFD_ForceParam(Idx, IFD_PROTO_T0, IFD_FD_4x);//9600

	//retcode = xSCardTransmit(SAM_SLOT_GS, &dataSend, &dataRecv);
	pos = 0;
	tmpI = sizeof(SAM_CMD_SELECT_FILE) - 1;
	memcpyE(pTmpBuf + pos, SAM_CMD_SELECT_FILE, tmpI);
	pos += tmpI;
	pTmpBuf[pos] = 0x3F;
	++pos;
	pTmpBuf[pos] = 0x00;
	++pos;
	dataSend.pBuffer = pTmpBuf;
	dataSend.blen = pos;
	dataRecv.pBuffer = pTmpBuf + pos;
	retcode = xSCardTransmit(slot, &dataSend, &dataRecv, &sw1sw2);
	if (retcode != Ret_OK) {
		PRINT_DEBUG("GS_PSAM_Verify, fail to xSCardTransmit:%d", retcode);
		goto GS_PSAM_Verify_OVER;
	}
	if (sw1sw2 != SW1SW2_SUCCESS) {
		retcode = Ret_Error;
		PRINT_DEBUG("GS_PSAM_Verify, fail to Process:%d", sw1sw2);
		goto GS_PSAM_Verify_OVER;
	}
	pTmpBuf[tmpI] = 0x10;
	pTmpBuf[tmpI + 1] = 0x03;
	retcode = xSCardTransmit(slot, &dataSend, &dataRecv, &sw1sw2);
	if (retcode != Ret_OK) {
		PRINT_DEBUG("GS_PSAM_Verify, fail to xSCardTransmit:%d", retcode);
		goto GS_PSAM_Verify_OVER;
	}
	if (sw1sw2 != SW1SW2_SUCCESS) {
		retcode = Ret_Error;
		PRINT_DEBUG("GS_PSAM_Verify, fail to Process:%d", sw1sw2);
		goto GS_PSAM_Verify_OVER;
	}
	// verify pin
	pos = 0;
	tmpI = sizeof(SAM_CMD_VERIFY_PIN) - 1;
	memcpyE(pTmpBuf + pos, SAM_CMD_VERIFY_PIN, tmpI);
	pos += tmpI;
	memcpyE(pTmpBuf + pos, "\x12\x34\x56", 3);
	pos += 3;
	dataSend.blen = pos;
	dataRecv.pBuffer = pTmpBuf + pos;
	retcode = xSCardTransmit(slot, &dataSend, &dataRecv, &sw1sw2);
	if (retcode != Ret_OK) {
		PRINT_DEBUG("GS_PSAM_Verify, fail to xSCardTransmit:%d", retcode);
		goto GS_PSAM_Verify_OVER;
	}
	if (sw1sw2 != SW1SW2_SUCCESS) {
		retcode = Ret_Error;
		PRINT_DEBUG("GS_PSAM_Verify, fail to Process:%d", sw1sw2);
		goto GS_PSAM_Verify_OVER;
	}

	//printf("GS_PSAM_NO: %s\n", buf_print(Param.SAMNO, 6));
GS_PSAM_Verify_OVER:
	xStor_FreeTempRam(refTmp);

	return retcode;
}


int SAM_M1_GetMiKey(stDataBuffer *pCardId, stM1_PublicSecBlock4 *pblock4) {
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int retcode = Ret_OK;
	int pos = 0;
	stDataBuffer dataSend;
	stDataBuffer dataRecv;
	int tmpI;
	TUINT16 sw1sw2 = 0;
	stM1KeyStore *pM1keys = NULL;


	if (pCardId == NULL || pblock4 == NULL || s_pKeyStore == NULL) {
		retcode = Ret_Err_Param;
		goto SAM_M1_GetMiKey_OVER;
	}
	if (s_isSamOK[SAM_SLOT_GS - 1] == BOOL_FALSE) {
		retcode = Ret_Error;
		goto SAM_M1_GetMiKey_OVER;
	}
	refTmp = xStor_MallocTempRam(100);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
		goto SAM_M1_GetMiKey_OVER;
	}
	pM1keys = &s_pKeyStore->m1KeyStore;

	pos = 0;
	tmpI = sizeof(SAM_CMD_GET_M1KEY_GS) - 1;
	memcpyE(pTmpBuf + pos, SAM_CMD_GET_M1KEY_GS, tmpI);
	pos += tmpI;
	memcpyE(pTmpBuf + pos, pblock4->CityCode, 2);
	pos += 2;
	memcpyE(pTmpBuf + pos, pCardId->pBuffer, pCardId->blen);
	pos += pCardId->blen;
	memcpyE(pTmpBuf + pos, pblock4->PublishBicker, 2);
	pos += 2;
	memcpyE(pTmpBuf + pos, pblock4->CardCheckCode, 4);
	pos += 4;
	pTmpBuf[pos] = 0x02;
	++pos;
	pTmpBuf[pos] = 0x03;
	++pos;
	pTmpBuf[pos] = 0x07;
	++pos;
	dataSend.pBuffer = pTmpBuf;
	dataSend.blen = pos;
	dataRecv.pBuffer = pTmpBuf + pos;
	retcode = xSCardTransmit(SAM_SLOT_GS, &dataSend, &dataRecv, &sw1sw2);
	if (retcode != Ret_OK) {
		PRINT_DEBUG("SAM_M1_GetMiKey, fail to xSCardTransmit:%d", retcode);
		goto SAM_M1_GetMiKey_OVER;
	}
	if (sw1sw2 != SW1SW2_SUCCESS || dataRecv.blen != sizeof(stM1KeyStore)) {
		retcode = Ret_Error;
		PRINT_DEBUG("SAM_M1_GetMiKey, fail to Process:%d,%d", sw1sw2, dataRecv.blen);
		goto SAM_M1_GetMiKey_OVER;
	}

	pos = 0;
	tmpI = sizeof(pM1keys->m1Key62);
	memcpyE(pM1keys->m1Key62, dataRecv.pBuffer + pos, tmpI);
	pos += tmpI;
	PRINT_DEBUGBYS("dataKey62:", pM1keys->m1Key62, tmpI);
	tmpI = sizeof(pM1keys->m1Key543);
	memcpyE(pM1keys->m1Key543, dataRecv.pBuffer + pos, tmpI);
	pos += tmpI;
	PRINT_DEBUGBYS("dataKey543:", pM1keys->m1Key543, tmpI);
	tmpI = sizeof(pM1keys->m1Key87);
	memcpyE(pM1keys->m1Key87, dataRecv.pBuffer + pos, tmpI);
	//pos += tmpI;
	PRINT_DEBUGBYS("dataKey87:", pM1keys->m1Key87, tmpI);

SAM_M1_GetMiKey_OVER:
	xStor_FreeTempRam(refTmp);

	return retcode;
}


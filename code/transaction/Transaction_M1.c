#include "Macro_Proj.h"
#include "xCardOpera.h"
#include "Transaction_M1.h"
#include "UtilityProc.h"
#include "xStorage.h"
#include "Transaction.h"
#include "SecuritySam.h"

#include <string.h>
//#include <stdio.h>

// flag of startup
enum emStartupFlag {
	STARTUP_OK = 0,		// normal
	STARTUP_NO = 1,		// no startup
	STARTUP_YES = 2,	// normal
	STARTUP_STOP = 6,	// has stop
};

static stM1_CardMapping *s_pM1CardMapping = NULL;
static stCardInfor *s_pCardInfo = NULL;
static stM1CARD_DEAL *s_pM1Deal = NULL;

/**
* @Description - get mifare card list information
*
* @Return int(ERet_Code) - 0 is successful
*/
static int M1_Card_GetListinfo() {
	stM1_ListInfoBlock1 *pM1ListInfo = NULL;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int retcode = Ret_OK;
	stDataBuffer dataBuffer;
	stDataBuffer dataKey;
	int i, j = 0;
	stCardInfor *pCardInfo = s_pCardInfo;


	// read list information

	PRINT_DEBUG(STR_SPLIT_LINE2);
	PRINT_DEBUG("list card content:1");
	if (pCardInfo == NULL || s_pM1CardMapping == NULL) {
		retcode = Ret_Err_Param;
		goto M1_Card_GetListinfo_OVER;
	}

	refTmp = xStor_MallocTempRam(50);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
		goto M1_Card_GetListinfo_OVER;
	}

	pM1ListInfo = &s_pM1CardMapping->listInfoblock1;
	if (pM1ListInfo == NULL) {
		retcode = Ret_Error;
		goto M1_Card_GetListinfo_OVER;
	}

	dataKey.pBuffer = pTmpBuf;
	dataKey.blen = 6;
	memcpyE(dataKey.pBuffer, "\xA0\xA1\xA2\xA3\xA4\xA5", dataKey.blen);
	dataBuffer.pBuffer = pTmpBuf + 6;
	retcode = xSCardTransmit_M1(pCardInfo->slot, SMM_ReadBlock, 1, BOOL_TRUE, &dataKey, NULL, &dataBuffer);
	if (retcode != Ret_OK) {
		PRINT_ERROR("M1_Card_GetListinfo fail to SMM_ReadBlock:%d", retcode);
		goto M1_Card_GetListinfo_OVER;
	}


	pM1ListInfo->MonthTopupSector = 0xFF;
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		switch (dataBuffer.pBuffer[i])
		{
		case AREA_CONTENT:
			pM1ListInfo->ListSector = i;
			break;
		case AREA_ISSUE:
			pM1ListInfo->PublishSector = i;
			break;
		case AREA_TXNRECORD:
            if (j >= (int)sizeof(pM1ListInfo->HistoryRecordSector)) {
				PRINT_ERROR("illegal format of list information block:%d", j);
				retcode = Ret_Error;
				goto M1_Card_GetListinfo_OVER;
			}
			pM1ListInfo->HistoryRecordSector[j] = i;
			j++;
			break;
		case AREA_PUBLICINFO:
			pM1ListInfo->PublicMessageSector = i;
			break;
		case AREA_MONTHTOPUP:
			pM1ListInfo->MonthTopupSector = i;
			break;
		case AREA_MONTH:
			pM1ListInfo->MonthMoneySector = i;
			break;
		case AREA_EPURSE:
			pM1ListInfo->PublicMoneySector = i;
			break;
		default:
			break;
		}
	}
	PRINT_DEBUGBYS("stM1ListInfo:", pM1ListInfo, sizeof(stM1_ListInfoBlock1));
M1_Card_GetListinfo_OVER:
	xStor_FreeTempRam(refTmp);

	return retcode;

}

static int M1_Card_ReadIssueSector(void) {
	stM1_ListInfoBlock1 *pM1ListInfo = NULL;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;
	int retcode = Ret_OK;
	stDataBuffer dataBuffer;
	stDataBuffer dataKey;
	//	int i, j = 0;
	stCardInfor *pCardInfo = s_pCardInfo;
	int slot = 0;
	int block = 0;
	stM1_PublicSecBlock4 *pM1PublicSecb4 = NULL;
	stM1_PublicSecBlock5 *pM1PublicSecb5 = NULL;
	stM1_PublicSecBlock6 *pM1PublicSecb6 = NULL;
	stM1CARD_DEAL *pM1Deal = s_pM1Deal;
	int tmpI;


	// read list information

	PRINT_DEBUG(STR_SPLIT_LINE2);
	PRINT_DEBUG("M1_Card_ReadIssueSector");
	if (pCardInfo == NULL || s_pM1CardMapping == NULL || pM1Deal == NULL) {
		retcode = Ret_Err_Param;
		goto M1_Card_GetListinfo_OVER;
	}

	refTmp = xStor_MallocTempRam(10);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		retcode = Ret_Error;
		goto M1_Card_GetListinfo_OVER;
	}

	pM1ListInfo = &s_pM1CardMapping->listInfoblock1;
	pM1PublicSecb4 = &s_pM1CardMapping->publicsecblock4;
	pM1PublicSecb5 = &s_pM1CardMapping->publicsecblock5;
	pM1PublicSecb6 = &s_pM1CardMapping->publicsecblock6;
	if (pM1ListInfo == NULL || pM1PublicSecb4 == NULL || pM1PublicSecb5 == NULL || pM1PublicSecb6 == NULL) {
		retcode = Ret_Error;
		goto M1_Card_GetListinfo_OVER;
	}

	slot = pCardInfo->slot;
	block = pM1ListInfo->PublishSector << 2;
	PRINT_DEBUG("pM1ListInfo->PublishSector-block:%d", block);

	dataKey.pBuffer = pTmpBuf;
	dataKey.blen = 6;
	memcpyE(dataKey.pBuffer, pCardInfo->cardId + SCARD_POS_VALUE, 4);
	memcpyE(dataKey.pBuffer + 4, pCardInfo->cardId + SCARD_POS_VALUE, 2);
	retcode = xSCardTransmit_M1(slot, SMM_AuthBlock, block, BOOL_TRUE, &dataKey, NULL, NULL);
	if (retcode != Ret_OK) {
		PRINT_ERROR("M1_Card_ReadIssueSector fail to SMM_AuthBlock[%d]:%d", block, retcode);
		goto M1_Card_GetListinfo_OVER;
	}
	dataBuffer.pBuffer = (TUINT8 *)pM1PublicSecb4;
	retcode = xSCardTransmit_M1(slot, SMM_ReadBlock, block, BOOL_TRUE, NULL, NULL, &dataBuffer);
	if (retcode != Ret_OK) {
		PRINT_ERROR("M1_Card_ReadIssueSector fail to SMM_ReadBlock[%d]:%d", block, retcode);
		goto M1_Card_GetListinfo_OVER;
	}
	//PRINT_DEBUGBYS("pM1PublicSecb4:", pM1PublicSecb4, sizeof(stM1_PublicSecBlock4));
	// check the start flag
	PRINT_DEBUG("m1 card is start flag:%d", pM1PublicSecb4->StartFlag);
	if (pM1PublicSecb4->StartFlag != STARTUP_YES) {
		goto M1_Card_GetListinfo_OVER;
	}
	// store data
	PRINT_DEBUGBYS("city code of m1 card in code:", WHICH_CITY_CODE, 2);
	PRINT_DEBUGBYS("city code of m1 card:", pM1PublicSecb4->CityCode, 2);
	PRINT_DEBUGBYS("trade code of m1 card:", pM1PublicSecb4->TradeCode, 2);
	PRINT_DEBUGBYS("PublishBicker of m1 card:", pM1PublicSecb4->PublishBicker, 4);
	tmpI = 0;
	memcpyE(pM1Deal->AppSerialNo + tmpI, WHICH_CITY_CODE, 2);
	tmpI += 2;
	memcpyE(pM1Deal->AppSerialNo + tmpI, pM1PublicSecb4->CityCode, 2);
	tmpI += 2;
	memcpyE(pM1Deal->AppSerialNo + tmpI, pM1PublicSecb4->TradeCode, 2);
	tmpI += 2;
	memcpyE(pM1Deal->AppSerialNo + tmpI, pM1PublicSecb4->PublishBicker, 4);
	//tmpI += 4;
	PRINT_DEBUGBYS("CardAppType of m1 card:", &pM1PublicSecb4->CardType, 1);
	pM1Deal->CardType = pM1PublicSecb4->CardType;
	if (pM1Deal->CardType == CARD_WHITE_BUS) {
		return Ret_Error;
	}

	++block;

	dataBuffer.pBuffer = (TUINT8 *)pM1PublicSecb5;
	retcode = xSCardTransmit_M1(slot, SMM_ReadBlock, block, BOOL_TRUE, NULL, NULL, &dataBuffer);
	if (retcode != Ret_OK) {
		PRINT_ERROR("M1_Card_ReadIssueSector fail to SMM_ReadBlock[%d]:%d", block, retcode);
		goto M1_Card_GetListinfo_OVER;
	}
	//PRINT_DEBUGBYS("pM1PublicSecb5:", &pM1PublicSecb5, sizeof(stM1_PublicSecBlock5));	
	++block;

	dataBuffer.pBuffer = (TUINT8 *)pM1PublicSecb6;
	retcode = xSCardTransmit_M1(slot, SMM_ReadBlock, block, BOOL_TRUE, NULL, NULL, &dataBuffer);
	if (retcode != Ret_OK) {
		PRINT_ERROR("M1_Card_ReadIssueSector fail to SMM_ReadBlock[%d]:%d", block, retcode);
		goto M1_Card_GetListinfo_OVER;
	}
	//PRINT_DEBUGBYS("pM1PublicSecb6:", pM1PublicSecb6, sizeof(stM1_PublicSecBlock6));

M1_Card_GetListinfo_OVER:
	xStor_FreeTempRam(refTmp);

	return retcode;
}

static int M1_Card_Type() {
	int cardtype = CARD_INVALID;
	stM1_PublicSecBlock4 *pM1PublicSecb4 = NULL;
	stCardInfor *pCardInfo = s_pCardInfo;
	stM1CARD_DEAL *pM1Deal = s_pM1Deal;
	stDataBuffer dataCardId;
	//	stDataBuffer dataBlock4;
	int refTmp = 0;
	TUINT8 *pTmpBuf = NULL;

	if (pCardInfo == NULL || s_pM1CardMapping == NULL || pM1Deal == NULL) {
		cardtype = ST_ERROR;
		goto M1_Card_Type_OVER;
	}
	refTmp = xStor_MallocTempRam(20);
	pTmpBuf = xStor_GetTempRamPoint(refTmp);
	if (pTmpBuf == NULL) {
		cardtype = ST_ERROR;
		goto M1_Card_Type_OVER;
	}

	pM1PublicSecb4 = &s_pM1CardMapping->publicsecblock4;
	if (pM1PublicSecb4 == NULL) {

		cardtype = ST_ERROR;
		goto M1_Card_Type_OVER;
	}
	switch (pM1PublicSecb4->StartFlag)
	{
	case STARTUP_YES:
		break;
	case STARTUP_STOP:
		return Err_StopCard;
	case STARTUP_NO:
		return Err_CardNo;
	default:
		return Err_NoInSystem;
	}
	int cardKind = GetCardKind(pM1Deal->CardType);

	if (cardKind == CT_OTHER)
	{
		cardtype = CARD_NO_SYSTEM;
		goto M1_Card_Type_OVER;
	}

	dataCardId.pBuffer = pCardInfo->cardId + SCARD_POS_VALUE;
	dataCardId.blen = pCardInfo->cardId[SCARD_POS_LEN];
	//dataBlock4.pBuffer = (TUINT8*)pM1PublicSecb4;
	//dataBlock4.blen = sizeof(stM1_PublicSecBlock4);
	if (SAM_M1_GetMiKey(&dataCardId, pM1PublicSecb4) != Ret_OK) {
		return ST_ERROR;
	}


M1_Card_Type_OVER:
	xStor_FreeTempRam(refTmp);

	return cardtype;
}

static int M1_Card_DealStart(void) {
	int retcode = Ret_OK;
	int cardtype = CARD_INVALID;

	PRINT_DEBUG(STR_SPLIT_LINE2);
	PRINT_DEBUG("M1_Card_DealStart");
	cardtype = M1_Card_Type();
	switch (cardtype)
	{
	case ST_ERROR:
		break;
	case CARD_BLACK:
	case CARD_BLACK_PRO:
		//ErrorMessage(Err_CardBlack);
		break;
	case CARD_MONEY:
		//M1_Card_PurchaseMoney();
		break;
	case CARD_MONTH:
		//M1_Card_PurchaseMonth();
		break;
	case CARD_CONTROL:
		//M1_Card_ControlManage();
		break;
	case CARD_FREE:
		//M1_Card_FreeCard();
		break;
	default:
		//ErrorMessage(ret);
		break;
	}

	return retcode;
}
/**
* @Description - deal the mifare card, transaction
*
* @Param pCardInfo(stCardInfor *) - card information
*
* @Return int(ERet_Code) - 0 is successful
*/
int dealCardM1(stCardInfor *pCardInfo) {
	int retcode = Ret_OK;

	if (pCardInfo == NULL) {
		retcode = Ret_Err_Param;
		goto dealCardM1_OVER;
	}
	s_pCardInfo = pCardInfo;

	PRINT_DEBUG(STR_SPLIT_LINE1);

	PRINT_DEBUG("dealCardM1:%02X", pCardInfo->sak);

	// malloc the core ram
	if (s_pM1CardMapping == NULL) {
		s_pM1CardMapping = (stM1_CardMapping *)xStor_MallocCoreRam(sizeof(stM1_CardMapping));
		if (s_pM1CardMapping == NULL) {
			PRINT_ERROR("dealCardM1 fail to malloc M1 card mapping:%d", s_pM1CardMapping);
			retcode = Ret_Err_Overflow;
			goto dealCardM1_OVER;
		}
	}
	if (s_pM1Deal == NULL) {
		s_pM1Deal = (stM1CARD_DEAL *)xStor_MallocCoreRam(sizeof(stM1CARD_DEAL));
		if (s_pM1Deal == NULL) {
			PRINT_ERROR("dealCardM1 fail to malloc M1 card deal struct:%d", s_pM1Deal);
			retcode = Ret_Err_Overflow;
			goto dealCardM1_OVER;
		}
	}

	retcode = M1_Card_GetListinfo();
	if (retcode != Ret_OK) {
		PRINT_ERROR("fail to M1_Card_GetListinfo:%d", retcode);
		goto dealCardM1_OVER;
	}

	retcode = M1_Card_ReadIssueSector();
	if (retcode != Ret_OK) {
		PRINT_ERROR("fail to M1_Card_ReadIssueSector:%d", retcode);
		goto dealCardM1_OVER;
	}

	retcode = M1_Card_DealStart();
	if (retcode != Ret_OK) {
		PRINT_ERROR("fail to M1_Card_DealStart:%d", retcode);
		goto dealCardM1_OVER;
	}

dealCardM1_OVER:

	return retcode;
}

#include "Macro_Proj.h"
#include "ProcCharacter.h"

#include <string.h>


unsigned char getSignXor(unsigned char *pSrc, int len) {
	unsigned char xorCh = 0;
	int i;

	for (i = 0; i < len; i++)		//kid 到xr 值之前的校验
	{
		xorCh ^= pSrc[i];
	}

	return xorCh;
}

int PareseT1L2V(void *pSrc, int len, stTlv *pTlvs, int *pOLen) {
	unsigned char *pTmpSrc = (unsigned char *)pSrc;
	int i = 0, index;
	int tmpI = 0;
	int lenLimit = 0;


	if (pTmpSrc == NULL || pTlvs == NULL || pOLen == NULL) {
		return Ret_Err_Param;
	}
	lenLimit = *pOLen;
	*pOLen = 0;

	index = 0;
	for (i = 0; i < len;)
	{
		pTlvs[index].pBegin = pTmpSrc + i;
		pTlvs[index].tag = pTmpSrc[i];
		++i;
		tmpI = GET_INT16(pTmpSrc + i);
		pTlvs[index].len = tmpI;
		i += 2;
		pTlvs[index].pValue = pTmpSrc + i;
		i += tmpI;
		++index;
		if (index > lenLimit) {
			//*pOLen = index;
			return Ret_Err_Overflow;
		}
	}
	if (i != len) {
		return Ret_Err_Format;
	}
	*pOLen = index;
	return Ret_OK;
}

int PareseT1L2V_8583(void *pSrc, int len, stTlv pTlvs[65]) {
	unsigned char *pTmpSrc = (unsigned char *)pSrc;
	int i = 0, index;
	int tmpI = 0;
	int lenLimit = 0;
	int count = 0;

	if (pTmpSrc == NULL || pTlvs == NULL) {
		return Ret_Err_Param;
	}

	memset(pTlvs, 0x00, sizeof(stTlv)*lenLimit);

	index = 0;
	for (i = 0; i < len;)
	{
		index = pTmpSrc[i] - 1;
		if (index >= 65 || index <= 0) {
			return Ret_Err_Format;
		}
		if (pTlvs[index].tag != 0) {
			return Ret_Err_Repeat;
		}
		pTlvs[index].pBegin = pTmpSrc + i;
		pTlvs[index].tag = pTmpSrc[i];
		++i;
		tmpI = GET_INT16(pTmpSrc + i);
		pTlvs[index].len = tmpI;
		i += 2;
		pTlvs[index].pValue = pTmpSrc + i;
		i += tmpI;
		++count;
	}
	if (i != len) {
		return Ret_Err_Format;
	}
	return Ret_OK;
}

int BuildT1L2V(stTlv *pTlvs, int len, unsigned char *pSrc, int *pOLen) {
	unsigned char *pTmpSrc = (unsigned char *)pSrc;
	int i = 0, index = 0;
	int tmpI = 0;
	int lenLimit = 0;


	if (pTmpSrc == NULL || pTlvs == NULL || pOLen == NULL) {
		return Ret_Err_Param;
	}
	lenLimit = *pOLen;
	*pOLen = 0;

	index = 0;
	for (i = 0; i < len; ++i)
	{
		if (pTlvs->tag > 0xFF) {
			return Ret_Err_Format;
		}
		pTmpSrc[index] = (unsigned char)(pTlvs[i].tag & 0x0FF);
		++index;
		tmpI = pTlvs[i].len;
		SET_INT16(pTmpSrc + index, tmpI);
		index += 2;
		if (index + tmpI > lenLimit) {
			return Ret_Err_Overflow;
		}
		memcpy(pTmpSrc + index, pTlvs[i].pValue, tmpI);
		index += tmpI;
	}
	*pOLen = index;
	return Ret_OK;
}

int findT1L2VFirst(void *pSrc, int len, int tag, stTlv *pTlvs) {
	unsigned char *pTmpSrc = (unsigned char *)pSrc;
	int i = 0, index;
	int tmpI = 0;


	if (pTmpSrc == NULL || pTlvs == NULL) {
		return Ret_Err_Param;
	}

	index = 0;
	for (i = 0; i < len;)
	{
		pTlvs[index].pBegin = pTmpSrc + i;
		pTlvs[index].tag = pTmpSrc[i];
		++i;
		tmpI = GET_INT16(pTmpSrc + i);
		if (pTlvs[index].tag == tag) {
			pTlvs[index].len = tmpI;
			i += 2;
			pTlvs[index].pValue = pTmpSrc + i;
			i += tmpI;
			++index;
			break;
		}
		else {
			i += 2;
			i += tmpI;
		}
	}
	if (i >= len) {
		return Ret_Err_N_Exist;
	}

	return Ret_OK;
}

int SetTLV(stTlv *pTlv, int tag, int len, void *pSrc) {

	pTlv->tag = tag;
	pTlv->len = len;
	pTlv->pValue = (unsigned char *)pSrc;


//	MSG_LOG("SetTLV(%d,%d):", tag, len);
	//BCD_LOG(pSrc, len, 1);

	return Ret_OK;
}

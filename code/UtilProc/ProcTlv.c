#include "Macro_Proj.h"
#include "UtilityProc.h"


#define MAX_TLV_TAG		4
#define MAX_TLV_LEN		4

// the input and output rules for parameters
// 4bytes TAG + 4bytes LEN + LEN bytes value

int T4L4VToBytes(StrTlv *pTlvs, int tlvCount, TUINT8 *pDes, int *pDesLen)
{
	int retCode = Ret_OK;
	int tmpI = 0;
	int lenLimit = 0;


	if (pTlvs == NULL || pDes == NULL || pDesLen == NULL)
	{
		retCode = Ret_Err_Param;
		return retCode;
	}
	lenLimit = *pDesLen;
	*pDesLen = 0;

	tmpI = 0;
	while(tlvCount > 0)
	{
		if (lenLimit < (tmpI+pTlvs->len+MAX_TLV_TAG+MAX_TLV_LEN))
		{
			retCode = Ret_Err_Overflow;
			return retCode;
		}
		SET_INT32(pDes+tmpI, pTlvs->tag);
		tmpI += MAX_TLV_TAG;
		SET_INT32(pDes+tmpI, pTlvs->len);
		tmpI += MAX_TLV_TAG;
		if (pTlvs->len > 0)
		{
			if (pTlvs->pValue == NULL)
			{
				retCode = Ret_Err_Param;
				return retCode;
			}
			memcpyE(pDes+tmpI, pTlvs->pValue, pTlvs->len);
			tmpI += pTlvs->len;
		}
		--tlvCount;
		++pTlvs;
	}
	*pDesLen = tmpI;
	
	return retCode;
}

int PareseAllT4L4V(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount)
{
	int i = 0;
	int maxTlvCount = 0;
	TUINT8 isContructed = BOOL_FALSE;
	//TUINT32 tmpU32 = 0;
	int tmpI32 = 0;
	int tmpI321 = 0;
	int retCode = Ret_OK;
	int tlvCount = 0;


	if (pSrc == NULL || pTlvs == NULL || pTlvCount == NULL)
	{
		srcLen = 0;
		retCode = Ret_Err_Param;
	}
	maxTlvCount = *pTlvCount;
	*pTlvCount = 0;

	for(i=0; i<srcLen; )
	{
		if (maxTlvCount <= 0)
		{
			retCode = Ret_Err_Overflow;
			break;
		}
		/*if (pSrc[i] == 0x00)
		{
			++i;
			continue;
		}*/
		pTlvs->tag = 0x00000000;
		pTlvs->len= 0x00000000;
		pTlvs->pValue = NULL;
		pTlvs->pTlvStart = NULL;

		isContructed = pSrc[i]&0x80;
		// tag
		pTlvs->pTlvStart = pSrc+i;
		pTlvs->tag = GET_INT32(pSrc+i);
		i += MAX_TLV_TAG;
		// len
		tmpI321 = GET_INT32(pSrc+i);
		i += MAX_TLV_LEN;
		pTlvs->len = tmpI321;
		// value
		pTlvs->pValue = pSrc+i;
		++pTlvs;
		if (isContructed != BOOL_FALSE)
		{
			tmpI32 = maxTlvCount;
			retCode = PareseAllT4L4V(pSrc+i, tmpI321, pTlvs, &tmpI32);
			if (retCode != Ret_OK)
			{
				break;
			}
			pTlvs += tmpI32;
			tlvCount += tmpI32;
			maxTlvCount -= tmpI32;
		}
		i += tmpI321;

		--maxTlvCount;
		++tlvCount;
	}
	*pTlvCount = tlvCount;

	return retCode;
}



StrTlv * SearchTlv(StrTlv *pTlvs, int tlvCount, TUINT32 tag)
{
	int i = 0;
	StrTlv *retValue = NULL;


	if (pTlvs == NULL)
	{
		tlvCount = 0;
	}

	for (i=0; i<tlvCount; ++i)
	{
		if (pTlvs->tag == tag)
		{
			retValue = pTlvs;
			break;
		}
		++pTlvs;
	}

	return retValue;
}

int PareseAllTlvs(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount)
{
	int i = 0;
	int maxTlvCount = 0;
	TUINT8 isContructed = BOOL_FALSE;
	TUINT32 tmpU32 = 0;
	int tmpI32 = 0;
	int retCode = Ret_OK;
	int tlvCount = 0;


	if (pSrc == NULL || pTlvs == NULL || pTlvCount == NULL)
	{
		srcLen = 0;
		retCode = Ret_Err_Param;
	}
	maxTlvCount = *pTlvCount;
	*pTlvCount = 0;

	for(i=0; i<srcLen; )
	{
		if (maxTlvCount <= 0)
		{
			retCode = Ret_Err_Overflow;
			break;
		}
		if (pSrc[i] == 0x00)
		{
			++i;
			continue;
		}
		pTlvs->tag = 0x00000000;
		pTlvs->len= 0x00000000;
		pTlvs->pValue = NULL;
		pTlvs->pTlvStart = NULL;

		// tag
		pTlvs->pTlvStart = pSrc+i;
		tmpU32 = pSrc[i];
		++i;
		//tmpI32 = 1;
		isContructed = tmpU32&0x20;

		if(0x1F == (tmpU32 & 0x1F))
		{
			tmpU32 = (tmpU32<<8) | pSrc[i];
			++i;
			//++tmpI32;
			if (0x80 == (tmpU32 & 0x80))
			{
				tmpU32 = (tmpU32<<8) | pSrc[i];
				++i;
				//++tmpI32;
				if (0x80 == (tmpU32 & 0x80))
				{
					tmpU32 = (tmpU32<<8) | pSrc[i];
					++i;
					//++tmpI32;
					if (0x80 == (tmpU32 & 0x80))
					{
						retCode = Ret_Err_Overflow;
						break;
					}
				}
			}
		}
		pTlvs->tag = tmpU32;
		// len
		tmpU32 = pSrc[i];
		++i;
		if (0x80 == (tmpU32 & 0x80))
		{
			// count bytes of length
			tmpI32 = tmpU32&0x7F;
			if (tmpI32 > 4 || tmpI32 < 0)
			{
				retCode = Ret_Err_Overflow;
				break;
			}
			tmpU32 = 0;
			while(tmpI32 > 0)
			{
				tmpU32 = (tmpU32<<8) | pSrc[i];
				++i;
				--tmpI32;
			}
		}
		pTlvs->len = tmpU32;
		// value
		pTlvs->pValue = pSrc+i;
		++pTlvs;
		if (isContructed != BOOL_FALSE)
		{
			tmpI32 = maxTlvCount;
			retCode = PareseAllTlvs(pSrc+i, tmpU32, pTlvs, &tmpI32);
			if (retCode != Ret_OK)
			{
				break;
			}
			pTlvs += tmpI32;
			tlvCount += tmpI32;
			maxTlvCount -= tmpI32;
		}
		i += tmpU32;

		--maxTlvCount;
		++tlvCount;
	}
	*pTlvCount = tlvCount;

	return retCode;
}

int PareseAllTls(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount)
{
	int i = 0;
	int maxTlvCount = 0;
	TUINT8 isContructed = BOOL_FALSE;
	TUINT32 tmpU32 = 0;
	int tmpI32 = 0;
	int retCode = Ret_OK;
	int tlvCount = 0;


	if (pSrc == NULL || pTlvs == NULL || pTlvCount == NULL)
	{
		srcLen = 0;
		retCode = Ret_Err_Param;
	}
	maxTlvCount = *pTlvCount;
	*pTlvCount = 0;

	for(i=0; i<srcLen; )
	{
		if (maxTlvCount <= 0)
		{
			retCode = Ret_Err_Overflow;
			break;
		}
		if (pSrc[i] == 0x00)
		{
			++i;
			continue;
		}
		pTlvs->tag = 0x00000000;
		pTlvs->len= 0x00000000;
		pTlvs->pValue = NULL;
		pTlvs->pTlvStart = NULL;

		// tag
		pTlvs->pTlvStart = pSrc+i;
		tmpU32 = pSrc[i];
		++i;
		//tmpI32 = 1;
		isContructed = tmpU32&0x20;

		if(0x1F == (tmpU32 & 0x1F))
		{
			tmpU32 = (tmpU32<<8) | pSrc[i];
			++i;
			//++tmpI32;
			if (0x80 == (tmpU32 & 0x80))
			{
				tmpU32 = (tmpU32<<8) | pSrc[i];
				++i;
				//++tmpI32;
				if (0x80 == (tmpU32 & 0x80))
				{
					tmpU32 = (tmpU32<<8) | pSrc[i];
					++i;
					//++tmpI32;
					if (0x80 == (tmpU32 & 0x80))
					{
						retCode = Ret_Err_Overflow;
						break;
					}
				}
			}
		}
		pTlvs->tag = tmpU32;
		// len
		tmpU32 = pSrc[i];
		++i;
		if (0x80 == (tmpU32 & 0x80))
		{
			// count bytes of length
			tmpI32 = tmpU32&0x7F;
			if (tmpI32 > 4 || tmpI32 < 0)
			{
				retCode = Ret_Err_Overflow;
				break;
			}
			tmpU32 = 0;
			while(tmpI32 > 0)
			{
				tmpU32 = (tmpU32<<8) | pSrc[i];
				++i;
				--tmpI32;
			}
		}
		pTlvs->len = tmpU32;
		// value
		/*pTlvs->pValue = pSrc+i;
		i += tmpU32;*/
		if (isContructed != BOOL_FALSE)
		{
			retCode = Ret_Error;
			break;
		}

		++pTlvs;
		++tlvCount;
		--maxTlvCount;
	}
	*pTlvCount = tlvCount;

	return Ret_OK;
}





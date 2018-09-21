#include "Macro_Proj.h"
#include "cJSON.h"
#include "string.h"
//#include "szct.h"
#include <stdio.h>

typedef enum {
	JSinvalid = 0,
	JSidle,
	JSname,
	JSaftername,
	JSprevalue,
	JSvalue,
	JSaftervalue,
	JSagain,
}emJsonStatus;


typedef enum {
	JTinvalid = 0,
	JTobject,
	JTarray,
	JTstring,
	JTnumber,
}emJsonType;



//const unsigned char c_statusNext[] = { JSidle , JSname,JSaftername,JSprevalue , JSvalue };

// parse JSON on current level
int parseJsonCurLevel(char *pInput, int iLen, stJSON *pstJson, int *pCount) {
	char *pCh = NULL;
	int i = 0;
	char valCh = 0;
	char status = JSinvalid;
	char type = JTinvalid;
	int index = 0;
	double number = 0;
	int limit = 0;
	char isEscape = 0;
	char loopDepth = 0;
	char isNegative = 0;
	int decimalN = -1;
	int valLen = 0;

	//MSG_LOG("parseJsonCurLevel:%s\n", pInput);
	if (pInput == NULL || pstJson == NULL || pCount == NULL || iLen < 0) {
		//MSG_LOG("parseJsonCurLevel:参数有非法\n");
		return -1;
	}
	limit = *pCount;
	*pCount = 0;
	if (iLen == 0) {
		return 0;
	}
	if (limit <= 0) {
		//MSG_LOG("parseJsonCurLevel:输出缓冲空间非法:%d\n", limit);
		return -2;
	}

	pCh = pInput;
	status = JSidle;
	for (i = 0; i < iLen; ++i)
	{
		valCh = *pCh;
		if (valCh == '\0') {
			//MSG_LOG("parseJsonCurLevel:数据非法,带了截止符\n");
			return -6;
		}

		if (status == JSidle) {
			++pCh;
			if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {
				continue;
			}
			else if (valCh == '\"') {
				status = JSname;
				pstJson[index].pmName = pCh;
			}
			else if (valCh == '{') {	// 对象
				type = JTobject;
			}/*
			else if (valCh == "[") {	// 对象
				type = JTarray;
			}*/
			else {
				//MSG_LOG("查找名字错误:%c\n", valCh);
				return -1;
			}
		}
		else if (status == JSname) {
			if (valCh == '\"') {
				status = JSaftername;
				*pCh = '\0';
			}
			++pCh;
		}
		else if (status == JSaftername) {
			++pCh;
			if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {
				continue;
			}
			else if (valCh == ':') {
				status = JSprevalue;
			}
			else {
				//MSG_LOG("名字后的分隔符错误:%c\n", valCh);
				return -2;
			}
		}
		else if (status == JSprevalue) {
			++pCh;
			if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {
				continue;
			}

			valLen = 0;
			if (valCh == '\"') {
				status = JSvalue;
				type = JTstring;
			}
			else if (valCh == '{') {
				status = JSvalue;
				type = JTobject;
			}
			else if (valCh == '[') {
				status = JSvalue;
				type = JTarray;
			}
			else if (valCh == '-' || (valCh >= '0' && valCh <= '9')) {
				valLen = 1;
				if (valCh == '-') {
					isNegative = 1;
					number = 0;
				}
				else {
					isNegative = 0;
					number = valCh - '0';
				}
				status = JSvalue;
				type = JTnumber;
			}
			else {
				//MSG_LOG("数据前的分隔符错误:%c\n", valCh);
				return -2;
			}

			pstJson[index].type = type;
			if (type == JTnumber) {
				pstJson[index].pmValue = pCh - 1;
			}
			else {
				pstJson[index].pmValue = pCh;
			}
			loopDepth = 0;
			decimalN = -1;
		}
		else if (status == JSvalue) {
			++valLen;
			switch (type)
			{
			case JTstring:
				if (isEscape != 0) {
					isEscape = 0;
				}
				else if (valCh == '\"') {
					--valLen;
					status = JSaftervalue;
					*pCh = '\0';
				}
				else if (valCh == '\\') {
					isEscape = 1;
				}
				break;
			case JTobject:
				if (valCh == '}') {
					if (loopDepth > 0) {
						--loopDepth;
					}
					else {
						--valLen;
						status = JSaftervalue;
						*pCh = '\0';
					}
				}
				else if (valCh == '{') {
					++loopDepth;
				}
				break;
			case JTarray:
				if (valCh == ']') {
					if (loopDepth > 0) {
						--loopDepth;
					}
					else {
						--valLen;
						status = JSaftervalue;
						*pCh = '\0';
					}
				}
				else if (valCh == '[') {
					++loopDepth;
				}
				break;
			case JTnumber:
				if (valCh >= '0' && valCh <= '9') {
					number *= 10;
					number += valCh - '0';
					if (decimalN >= 1) {
						decimalN *= 10;
					}
				}
				else if (valCh == '.') {
					if (decimalN >= 1) {
						//MSG_LOG("多个小数点:%d,%s\n", decimalN, pstJson[index].pmName);
						return -2;
					}
					decimalN = 1;
				}
				else {
					////MSG_LOG("number222222222222(%c):%f,%d,%d\n", valCh, number, isNegative, decimalN);
					if (isNegative) {
						number *= -1;
					}
					if (decimalN >= 1) {
						number /= decimalN;
					}
					--valLen;
					pstJson[index].number = number;
					*pCh = '\0';
					if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {

						status = JSaftervalue;
					}
					else if (valCh == '}') {
						break;
					}
					else if (valCh == ',') {
						++index;
						status = JSagain;
					}
					else {
						//MSG_LOG("非数值类型:%c\n", valCh);
						return -2;
					}
				}
				break;
			default:
				//MSG_LOG("无此类型的值:%d\n", type);
				return -2;
			//	break;
			}
			++pCh;
		}
		else if (status == JSaftervalue) {
			++pCh;
			if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {
				continue;
			}
			else if (valCh == '}') {
				break;
			}
			else if (valCh == ',') {
				pstJson[index].len = valLen;
				valLen = 0;
				++index;
				if (index >= limit) {
					//MSG_LOG("parseJsonCurLevel:输出缓冲空间非法(%d):%d\n", index, limit);
					return -4;
				}
				status = JSagain;
			}
			else {
				//MSG_LOG("数据后的分隔符错误:%c\n", valCh);
				return -2;
			}
		}
		else if (status == JSagain) {
			++pCh;
			if (valCh == ' ' || valCh == '\t' || valCh == '\r' || valCh == '\n') {
				continue;
			}
			else if (valCh == '\"') {
				status = JSname;
				pstJson[index].pmName = pCh;
				////MSG_LOG("tJson[index].pmName = pCh;:%s\n", pCh);
			}
			else {
				//MSG_LOG("继续查找名字错误\n");
				return -1;
			}
		}
		else {
			//MSG_LOG("状态非法:%d\n", status);
			return -10;
		}
	}
	if (status == JSaftervalue) {
		pstJson[index].len = valLen;
		valLen = 0;
		++index;
	}
	*pCount = index;

	return 0;
}

// get object of JSON by name
stJSON *getJsonObjByName(stJSON *pstJson, int count, char *pName) {
	int i = 0;
	int len = 0;

	//MSG_LOG("1111getJsonObjByName:%s,%d\n", pName, count);
	//BCD_LOG((unsigned char *)pstJson, sizeof(stJSON)*count, 1);
	if (pstJson == NULL || pName == NULL || count <= 0) {
		return NULL;
	}
	len = strlen(pName);

	for (i = 0; i < count; ++i)
	{
		////MSG_LOG("pstJson->pmName:%s\n", pstJson->pmName);
		////MSG_LOG("pstJson->pmValue:%s\n", pstJson->pmValue);
		////MSG_LOG("pstJson->type:%d\n", pstJson->type);
		if (memcmp(pstJson->pmName, pName, len) == 0) {
			//BCD_LOG((unsigned char *)pstJson, sizeof(stJSON), 1);
			return  pstJson;
		}
		++pstJson;
	}

	return NULL;
}

int getJsonString(stJSON *pstJson, int count, char *pOut, int *pLen) {
	int i = 0;
	int len = 0;
	int limit = 0;
	stJSON *tmpJson = NULL;

	if (pstJson == NULL || pOut == NULL || pLen == NULL || count < 0) {
		//MSG_LOG("getJsonString:参数有非法\n");
		return -1;
	}
	limit = *pLen;
	*pLen = 0;
	if (count == 0) {
		return 0;
	}
	if (limit <= 0) {
		//MSG_LOG("getJsonString:缓冲缓冲不够:%d\n", limit);
		return -1;
	}

	for (i = 0; i < count; ++i)
	{
		tmpJson = pstJson + i;
		if (i == 0) {
			len += sprintf(pOut + len, "\"%s\":", tmpJson->pmName);
		}
		else {
			len += sprintf(pOut + len, ",\n\"%s\":", tmpJson->pmName);
		}

		switch (tmpJson->type)
		{
		case JTstring:
			len += sprintf(pOut + len, "\"%s\"", tmpJson->pmValue);
			break;
		case JTobject:
			len += sprintf(pOut + len, "{%s}", tmpJson->pmValue);
			break;
		case JTarray:
			len += sprintf(pOut + len, "[%s]", tmpJson->pmValue);
			break;
		case JTnumber:
			len += sprintf(pOut + len, "%s", tmpJson->pmValue);
			break;
		default:
			//MSG_LOG("无此类型的值:%d\n", tmpJson->type);
			return -2;
//			break;
		}
	}
	*pLen = strlen(pOut);

	return 0;
}


#include "Macro_Proj.h"

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#include "xSharedMemory.h"
#include "UtilityProc.h"

#include <windows.h>  
#include <stdio.h>

#define POS_FLAG	0
#define POS_LEN		1
#define POS_DATA	3

typedef enum {
	DF_INVALID = 0,
	DF_WRITE,
	DF_READ
}emDataFlag;
/**
* @Description - create hand of shared memory
*
* @Param szName(char []) - name of shared memory
* @Param projId(char *) - id of shared memory, not invalid in windows
* @Param size(int) - size of shared memory
*
* @Return int(ERet_Code) - 0 is successful
*/
TLPVOID xSMOpenSharedMemory(IN char szName[], IN int projId, IN int size) {
	TLPVOID hMapFile = NULL;
	char *pBuffer = NULL;

	if (size <= 0) {
		projId = projId;
		return hMapFile;
	}
	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, szName);
	if (hMapFile == NULL) {
		// 创建共享文件句柄  
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // 物理文件句柄  
			NULL,                    // 默认安全级别  
			PAGE_READWRITE,          // 可读可写  
			0,                       // 高位文件大小  
			size + POS_DATA,		// 地位文件大小  
			szName                   // 共享内存名称  
		);
		if (hMapFile == NULL) {
			return hMapFile;
		}
	}
	pBuffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (pBuffer == NULL) {
		xSMCloseSharedMemory(hMapFile);
		return hMapFile;
	}
	pBuffer[POS_FLAG] = DF_INVALID;
	SET_INT16(pBuffer + POS_LEN, 0);
	//pBuffer = UnmapViewOfFile(hMapFile);

	return pBuffer;
}

int xSMCloseSharedMemory(TLPVOID pHmapFile) {
	//CloseHandle(pHmapFile);
	return Ret_OK;
}
/**
* @Description - shared memory write data, not check the len limit
*
* @Param hFile(TLPVOID) - handle of shared memory
* @Param pData(char *) - data buffer for write
* @Param len(int) - data length
*
* @Return int(ERet_Code) - 0 is successful
*/
int xSMWriteData(IN TLPVOID hFile, IN stDataBuffer *pStData) {
	char *pBuffer = NULL;
	pBuffer = (char *)hFile;// MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int len = 0;
	if (pBuffer == NULL) {
		PRINT_ERROR("MapViewOfFile return NULL");

		//LOOP_FOREVER;
		return Ret_Err_Fatal;
	}	
	if (pStData == NULL) {
		PRINT_ERROR("Data is ]NULL");
		//LOOP_FOREVER;
		return Ret_Err_Param;
	}
	len = pStData->blen;
	if (pBuffer[POS_FLAG] != DF_INVALID) {	// can't write
		return Ret_Error;
	}

	//pBuffer[POS_FLAG] = DF_WRITE;

	memcpyE(pBuffer + POS_DATA, pStData->pBuffer, len);
	SET_INT16(pBuffer + POS_LEN, len);

	pBuffer[POS_FLAG] = DF_WRITE;

	//pBuffer = UnmapViewOfFile(pBuffer);

	return Ret_OK;
}
/**
* @Description - shared memory read data, not check the len limit
*
* @Param hFile(TLPVOID) - handle of shared memory
* @Param pData(char *) - data buffer for read
* @Param len(int *) - data length, length limit for IN and read length for OUT
*
* @Return int(ERet_Code) - 0 is successful
*/
int xSMReadData(IN TLPVOID hFile, stDataBuffer *pStData) {
	char *pBuffer = NULL;
	int tmpI = 0;
	int lenLimit = 0;

	if (pStData == NULL) {
		return Ret_Err_Param;
	}
	lenLimit = pStData->blen;
	pStData->blen = 0;

	pBuffer = (char *)hFile;// MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (pBuffer == NULL) {
		PRINT_FATAL_ERROR("xSMReadData error:MapViewOfFile return NULL:%d\n", (int)hFile);
		//LOOP_FOREVER;
		return Ret_Err_Fatal;
	}

	if (pBuffer[POS_FLAG] != DF_WRITE) {	// no data to read
		return Ret_Error;
	}

	pBuffer[POS_FLAG] = DF_READ;

	tmpI = GET_INT16(pBuffer + POS_LEN);
	if (tmpI > lenLimit) {
		return Ret_Err_Overflow;
	}
	memcpyE(pStData->pBuffer, pBuffer + POS_DATA, tmpI);
	pStData->blen = tmpI;
	SET_INT16(pBuffer + POS_LEN, 0);

	pBuffer[POS_FLAG] = DF_INVALID;

	//pBuffer = UnmapViewOfFile(pBuffer);

	return Ret_OK;
}


#endif

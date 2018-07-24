#include "Macro_Proj.h"

#if WHICH_PLATFORM == _LINUX_PLATFORM_V

#include "xSharedMemory.h"
#include "UtilityProc.h"

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


    //pBuffer = UnmapViewOfFile(pBuffer);

    return Ret_OK;
}


#endif

#ifndef _INCLUDED_X_SHARED_MEMORY_H
#define _INCLUDED_X_SHARED_MEMORY_H



#ifdef __cplusplus
extern "C"
{
#endif

	extern TLPVOID xSMOpenSharedMemory(IN char szName[], IN int projId, IN int size);
	extern int xSMCloseSharedMemory(TLPVOID pHmapFile);
	extern int xSMWriteData(IN TLPVOID hFile, IN stDataBuffer *pStData);
	extern int xSMReadData(IN TLPVOID hFile, OUT stDataBuffer *pStData);
#ifdef __cplusplus
}
#endif

#endif

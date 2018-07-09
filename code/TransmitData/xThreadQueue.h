#ifndef _INCLUDED_X_THREAD_QUEUE_H
#define _INCLUDED_X_THREAD_QUEUE_H



#ifdef __cplusplus
extern "C"
{
#endif

	extern TLPVOID xTQOpenQueue(TUINT32 threadId);
	extern int xTQCloseQueue(TLPVOID pQueueHandle);
	extern int xTQWriteData(IN TLPVOID hFile, IN stDataBuffer *pStData);
	extern int xTQReadData(IN TLPVOID hFile, OUT stDataBuffer *pStData);
#ifdef __cplusplus
}
#endif

#endif

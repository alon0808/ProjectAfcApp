#ifndef _INCLUDED_TRANSMIT_DATA_H
#define _INCLUDED_TRANSMIT_DATA_H



#ifdef __cplusplus
extern "C"
{
#endif

	extern int InitCom(char comN[], int baund);
	extern int initSharedMemory(char curPath[], int projId, int size);
	extern int initSocket(char ip[], int port);
	extern int initThreadQueue(TUINT32 threadId);

	extern int writeData(int hand, stDataBuffer *pStData);
	extern int readData(int hand, stDataBuffer *pStData);

	extern int unregisterHandle(int hand);

#ifdef __cplusplus
};
#endif


#endif

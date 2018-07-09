#ifndef _INCLUDED_xStorage_H
#define _INCLUDED_xStorage_H


typedef enum EBufferType
{
	EInvalidBuffer = 0,
	EConfigFile,
	ECaPublicKey,
	ECRList,
	EEFList,
	EDekDetFile,
	ETornLogFile,
	ELanguageFile,
}TEBufferType;

typedef struct
{
	TUINT8 *m_pCoreRamBegin;
	TINT32 m_coreRamLen;

	TUINT8 *m_pAppRamBegin;
	TINT32 m_appRamLen;

	TUINT8 *m_pTempRamBegin;
	TINT32 m_tempRamLen;

	TUINT8 *m_pDoubleLinkListRamBegin;
	TINT32 m_doubleLinkListRamLen;

}TStorageInit;


#define xStor_Malloc	xStor_MallocForThreads
#define xStor_Free		xStor_FreeForThreads

#ifdef __cplusplus
extern "C"
{
#endif
	// install storage function at first
	extern TINT32 xStor_InstallStorage(TStorageInit *pStorage);
	// uninstall the storage function
	extern TINT32 xStor_UnInstallStorage(void);
	// for kernel ram
	// malloc kernel ram
	extern TUINT8 *xStor_MallocCoreRam(TINT32 bufferSize);
#if SWITCH_CODE_USED
	// malloc kernel ram temporary with exclusive property
	extern TUINT8 *xStor_MallocKernelRamExclusive(TINT32 bufferSize, TINT32 isStart);
	// free kernel ram temporary with exclusive property
	extern TINT32 xStor_FreeKernelRamExclusive(void *pBuffer);
#endif
	// clear kernel ram
	extern TINT32 xStor_ClearAppStorage(void);

	// for demo ram
	// malloc demo ram
	extern TUINT8 *xStor_MallocAppRam(TINT32 bufferSize);
#if SWITCH_CODE_USED
	// malloc demo ram temporary with exclusive property
	extern TUINT8 *xStor_MallocDemoRamExclusive(TINT32 bufferSize, TINT32 isStart);
	// free demo ram temporary with exclusive property
	extern TINT32 xStor_FreeDemoRamExclusive(void *pBuffer);
#endif
	// clear demo ram
	extern TINT32 xStor_ClearCoreStorage(void);

	// for temporary ram
	// clear temporary storage
	extern TINT32 xStor_ClearTempStorage(void);
	// malloc temporary ram
	extern TINT32 xStor_MallocTempRam(TINT32 bufferSize);
	// free temporary ram
	extern TINT32 xStor_FreeTempRam(TINT32 pointRef);
	// get temporary ram pointer by reference
	extern TUINT8 *xStor_GetTempRamPoint(TINT32 pointRef);
	// get the mass storage of some important function
	//extern TUINT8 * xStor_GetBuffer(TINT32 bufferType, TINT32 *pBufferLen);

	extern TUINT8 *xStor_MallocForThreads(TINT32 bufferSize);

	extern TINT32 xStor_FreeForThreads(void *pSrc);

#ifdef __cplusplus
}
#endif

#endif

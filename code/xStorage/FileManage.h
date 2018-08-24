#ifndef _INCLUDED_FILE_MANAGE_H
#define _INCLUDED_FILE_MANAGE_H



#ifdef __cplusplus
extern "C"
{
#endif

	extern int FmOpen(int *pHandle, char *pFilePath, int mode);

	extern int FmWriteOnce(char *pFilePath, void *pData, int datLen);

	extern int FmClose(int *pHandle);


#ifdef __cplusplus
}
#endif

#endif

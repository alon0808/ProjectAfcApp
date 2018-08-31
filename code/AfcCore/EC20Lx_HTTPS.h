#ifndef _INCLUDE_EC20LX_HTTPS_H
#define _INCLUDE_EC20LX_HTTPS_H

//#include "SlzrTypeDef.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern void https_closeHandle(void);
	extern int https_performSsl(void *pData, int len, unsigned char *pOutput, int *pOLen);
	extern int socket_performNSsl(void *pData, int len, unsigned char *pOutput, int *pOLen);
	extern int https_OpenSsl(int *fd);
	extern int https_setServerInfor(stServerInfo *pStServerInfo);

#ifdef __cplusplus
}
#endif

#endif


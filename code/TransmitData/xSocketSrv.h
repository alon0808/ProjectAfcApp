#ifndef _INCLUDED_XSOCKET_SRV_H
#define _INCLUDED_XSOCKET_SRV_H

#include "TypeDef.h"

#ifdef __cplusplus
extern "C"
{
#endif


	enum eSocketErrInfo
	{
		eSeccessful = 0,
		eCreateSocketErr = 1,
		eBindSocketErr = 2,
		eListenErr = 3,
		eAcceptErr = 4,
		eRecvErr = 5,
		eSendErr = 6,
		eSocketVersionErr = 7,
		eTimeOut = 8,

	};

	/*
函数功能：初始化服务器
参数：
    输入：pSourceIPAdds - IP地址， nSourcePort - 端口号
    输出：pSock_fd 套接字
返回：
    失败：
        eCreateSocketErr = 1,
        eBindSocketErr = 2,
        eListenErr = 3,
    成功：0
*/
TINT32 SocketSvrInit(TINT8 *pSourceIPAdds, TUINT32 nSourcePort, TUINT32 *pSock_fd);

/*
函数功能：等待客户端链接（死等）
参数：
    输入：sock_fd 套接字
    输出：pConn_fd 客户端的
返回：
    失败：
        eAcceptErr
    成功：0
*/
TUINT32 SocketSvrAccept(TUINT32 sock_fd, TUINT32 *pConn_fd);

/*
函数功能： 接收数据
参数：
    输入：conn_fd 套接字
    输入: 期望接收的长度，一直等待接，如有超时，超时返回错误。
返回：
    失败：
        eRecvErr
    成功：0
*/
//TUINT32 SocketSvrRecv(TUINT32 conn_fd, BYTE *pData, TINT32 *nDataLen);
TINT32 SocketSvrRecv(TUINT32 conn_fd, TUINT8 *pData, TINT32 nDataLen);

//set recv timeout. ms
TINT32 SocketSvrSetRecvTimeout(TUINT32 overtime);

/*
函数功能： 发送数据
参数：
    输入：conn_fd 套接字
    输出：pData - 数据，  nDataLen发送数据的长度
返回：
    失败：
        eSendErr
    成功：0
*/
TINT32 SocketSvrSend(TUINT32 conn_fd,const TUINT8 *pData, TINT32 nDataLen);

/*
函数功能： 关闭客户端
参数：
    输入：conn_fd 套接字

*/
void SocketSvrClose(TUINT32 connfd);

void SocketSvrExist(void);


#ifdef __cplusplus
}
#endif




#endif


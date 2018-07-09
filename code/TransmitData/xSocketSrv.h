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
�������ܣ���ʼ��������
������
    ���룺pSourceIPAdds - IP��ַ�� nSourcePort - �˿ں�
    �����pSock_fd �׽���
���أ�
    ʧ�ܣ�
        eCreateSocketErr = 1,
        eBindSocketErr = 2,
        eListenErr = 3,
    �ɹ���0
*/
TINT32 SocketSvrInit(TINT8 *pSourceIPAdds, TUINT32 nSourcePort, TUINT32 *pSock_fd);

/*
�������ܣ��ȴ��ͻ������ӣ����ȣ�
������
    ���룺sock_fd �׽���
    �����pConn_fd �ͻ��˵�
���أ�
    ʧ�ܣ�
        eAcceptErr
    �ɹ���0
*/
TUINT32 SocketSvrAccept(TUINT32 sock_fd, TUINT32 *pConn_fd);

/*
�������ܣ� ��������
������
    ���룺conn_fd �׽���
    ����: �������յĳ��ȣ�һֱ�ȴ��ӣ����г�ʱ����ʱ���ش���
���أ�
    ʧ�ܣ�
        eRecvErr
    �ɹ���0
*/
//TUINT32 SocketSvrRecv(TUINT32 conn_fd, BYTE *pData, TINT32 *nDataLen);
TINT32 SocketSvrRecv(TUINT32 conn_fd, TUINT8 *pData, TINT32 nDataLen);

//set recv timeout. ms
TINT32 SocketSvrSetRecvTimeout(TUINT32 overtime);

/*
�������ܣ� ��������
������
    ���룺conn_fd �׽���
    �����pData - ���ݣ�  nDataLen�������ݵĳ���
���أ�
    ʧ�ܣ�
        eSendErr
    �ɹ���0
*/
TINT32 SocketSvrSend(TUINT32 conn_fd,const TUINT8 *pData, TINT32 nDataLen);

/*
�������ܣ� �رտͻ���
������
    ���룺conn_fd �׽���

*/
void SocketSvrClose(TUINT32 connfd);

void SocketSvrExist(void);


#ifdef __cplusplus
}
#endif




#endif


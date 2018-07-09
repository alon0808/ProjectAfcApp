#ifndef _INCLUDED_XSOCKET_CLIENT_H
#define _INCLUDED_XSOCKET_CLIENT_H


#ifdef __cplusplus
extern "C"
{
#endif



#ifdef WIN32   
	typedef int socklen_t;  
	typedef int ssize_t;  
#endif 

#ifdef __LINUX__   
	typedef int SOCKET;  
    //typedef unsigned char BYTE;
    //typedef unsigned long DWORD;
#define FALSE 0   
#define SOCKET_ERROR (-1)   
#endif


	//Initialize socket and
	/*
	pIPAdds     [IN] : IP Address
	nSourcePort [IN] : Port
	pSock_fd    [OUT]: connected file descriptor
	Return           : 0,Successful; other,fail;
	*/
	TUINT32 SocketClientInit(char *pIPAdds, TUINT32 nSourcePort, TUINT32 *pSock_fd);

	//client Receive data
	/*
	conn_fd   [IN] : connected socket file descriptor
	pData     [OUT]: Received data
	nDataLen  [OUT]: Received length
	Return         : 0,Successful; other,fail;
	*/
	TUINT32 SocketClientRecv(TUINT32 conn_fd, TUINT8 *pData, TINT32 *nDataLen);

	//Send data
	/*
	conn_fd   [IN] : connected socket file descriptor
	pData     [OUT]: Sent data
	nDataLen  [OUT]: Sent length
	Return         : 0,Successful; other,fail;
	*/
	TUINT32 SocketClientSend(TUINT32 conn_fd, TUINT8 *pData, TUINT32 nDataLen);


	//close connecting socket file descriptor
	/*
	conn_fd   [IN] : connected socket file descriptor
	Return         : void.
	*/
	void SocketClientClose(TUINT32 connfd);
/*
 Socket Client exist.
 不同平台, 退出socket 的处理(windows 需要WSACleanup())
*/
void SocketClientExist(void);


#ifdef __cplusplus
}
#endif


#endif


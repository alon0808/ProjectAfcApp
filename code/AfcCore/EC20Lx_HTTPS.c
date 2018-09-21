#include "Macro_Proj.h"
#include "EC20Lx_HTTPS.h"
#include <stdio.h>//printf
#include <string.h>//字符串处理
#include <sys/socket.h>//套接字
#include <arpa/inet.h>//ip地址处理
#include <fcntl.h>//open系统调用
#include <unistd.h>//write系统调用
#include <netdb.h>//查询DNS
#include <stdlib.h>//exit函数
#include <dirent.h>
#include <sys/stat.h>//stat系统调用获取文件大小
#include <sys/time.h>//获取下载时间
#include <errno.h>
#include <pthread.h>

#include "GprsSocket.h"
#include "szct.h"


#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif
//#include "sslapi.h"
#include "openssl/ssl.h"
#include "curl/curl.h"
static SSL_CTX* s_hSslCtx = NULL;
static SSL *s_hSsl;

#define CACERT_DIR		BackDir
#define CACERT_FILEPATH	CACERT_DIR"cacert.pem"

typedef struct {
	char *memory;
	size_t size;
}MemoryStruct;

#define TIME_OVER	6

//extern void debugdata(unsigned char *buff, int len, unsigned int mode);

static stServerInfo s_stServerInfo = { 0 };

//static CURL *s_hnd = NULL;
static unsigned char s_isConnectOk[4] = { 0 ,0,0,0 };
static CURL *s_hnd[4] = { NULL ,NULL,NULL,NULL };
static int s_sockfd = 0;

/**
 *@Description delete the http head node from
 *
 */
static char curl_header_delete(struct curl_slist **csl_head, const char *data)
{
	struct curl_slist   *node;
	struct curl_slist   *head = *csl_head;
	struct curl_slist   *node_prev = NULL;
	struct curl_slist   *node_del = NULL;
	const char          *colon;
	size_t               head_name_len;
	int count;

	colon = strchr(data, ':');
	if (colon == NULL) {
		return FALSE;
	}
	head_name_len = colon - data + 1;

	node = head;
	count = 0;
	while (node != NULL) {
		if (memcmp(node->data, data, head_name_len) == 0) {
			++count;
			if (count > 1) {
				node_del = node;
				node = node->next;
				if (node_prev != NULL) {
					node_prev->next = node_del->next;
				}
				else {
					*csl_head = node_del->next;
				}

				node_del->next = NULL;
				curl_slist_free_all(node_del);
				continue;
			}
		}

		node_prev = node;
		node = node->next;
	}

	return TRUE;
}


static int closesocket(void *clientp, curl_socket_t item)
{
	MSG_LOG("libcurl wants to close %d nown\n", (int)item);
	close(item);
	return 0;
}

int https_setServerInfor(stServerInfo *pStServerInfo) {
	int linkNo;
	CURL *curl = NULL;

	if (pStServerInfo == NULL) {
		return Ret_Error;
	}
	linkNo = pStServerInfo->linkNo;
	curl = s_hnd[linkNo];
	if ((s_stServerInfo.linkNo != linkNo || s_stServerInfo.port != pStServerInfo->port)) {
		//curl = s_hnd[linkNo];
		if (curl != NULL) {
			curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, closesocket);
			curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, &s_sockfd);
			curl_easy_cleanup(curl);
			curl = NULL;
		}
	}
	if (curl == NULL) {
		s_hnd[linkNo] = curl_easy_init();
	}

	//s_stServerInfo = pStServerInfo;
	memcpy(&s_stServerInfo, pStServerInfo, sizeof(stServerInfo));

	return Ret_OK;
}

/* Auxiliary function that waits on the socket. */
int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
	struct timeval tv;
	fd_set infd, outfd, errfd;
	int res;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;

	FD_ZERO(&infd);
	FD_ZERO(&outfd);
	FD_ZERO(&errfd);

	FD_SET(sockfd, &errfd); /* always check for error */

	if (for_recv) {
		FD_SET(sockfd, &infd);
	}
	else {
		FD_SET(sockfd, &outfd);
	}

	/* select() returns the number of signalled sockets or -1 */
	res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

//extern void buildComSendStyle_09_01(unsigned char errLink);
void https_closeHandle(void) {

	int linkNo = 0;

	CURL *curl = s_hnd[linkNo];

	if (s_stServerInfo.port != 0) {
		linkNo = s_stServerInfo.linkNo;
		MSG_LOG("https_closeHandle:\n");
		if (s_hnd[linkNo] != NULL) {
			//buildComSendStyle_09_01(linkNo);

			s_isConnectOk[linkNo] = 0;
			/* call this function to close sockets */
			curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, closesocket);
			curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, &s_sockfd);
			curl_easy_cleanup(curl);
			s_hnd[linkNo] = NULL;
		}
	}
}

static size_t save_response_callback(char *buffer, size_t size, size_t count, void *response)
{
	MemoryStruct *pChunk = (MemoryStruct *)response;
	size_t length = size*count;

	//MSG_LOG("save_response_callback:%d,%2X\n", pChunk->size, buffer[0]);
	memcpy(pChunk->memory + pChunk->size, buffer, length);
	pChunk->size += length;


	return length;
}
/* make libcurl use the already established socket 'sockfd' */

static curl_socket_t opensocket(void *clientp,
	curlsocktype purpose,
	struct curl_sockaddr *address)
{
	curl_socket_t sockfd;
	sockfd = *(curl_socket_t *)clientp;
	/* the actual externally set socket is passed in via the OPENSOCKETDATA
	option */
	return sockfd;
}

static int sockopt_callback(void *clientp, curl_socket_t curlfd,
	curlsocktype purpose)
{
	/* This return code was added in libcurl 7.21.5 */
	return CURL_SOCKOPT_ALREADY_CONNECTED;
}

/**
 * @Description perform TLS one time
 *
 * @Param
 *
 * @Return RetCode
 */
int https_performSsl(void *pData, int len, unsigned char *pOutput, int *pOLen) {

	unsigned char response[16240];
	unsigned short rcv_len;
	CURLcode res;
	int linkNo = 0;
	CURL *curl = NULL;
	MemoryStruct chunk;
	struct curl_slist *headers = NULL;
	unsigned char tmpUC = 0;
	unsigned char *pContent = NULL;
	unsigned char *pHead = NULL;
	unsigned char *pInterface = NULL;
	int i;
	int retCode = Ret_OK;
	int nsent_total;
	int errTime = 0;
	int tmpI = 0;
	int limitLen = 0;
	char *pInput = (char *)pData;

	unsigned char isHasNHttpH = 0;

	MSG_LOG("https_performSsl:%d\n", len);
	if (pInput == NULL || pOutput == NULL || pOLen == NULL)
	{
		*pOLen = 0;
		PRINT_ERROR("param error:%d, %d, %d\n", pInput, pOutput, pOLen);
		return Ret_Err_Param;
	}
	limitLen = *pOLen;
	*pOLen = 0;

	if (s_stServerInfo.port == 0) {
		PRINT_ERROR("param error1:%s:%d\n", s_stServerInfo.IPaddr, s_stServerInfo.port);
		return Ret_Err_Param;
	}
	linkNo = s_stServerInfo.linkNo;
	curl = s_hnd[linkNo];

	chunk.memory = response;
	chunk.size = 0;

	if (pInput != NULL) {
		pInput[len] = '\0';	// 确保字符串结束
	}
	isHasNHttpH = s_stServerInfo.linkAttr.bits.bit2;
	MSG_LOG("isHasNHttpH:%d\n", isHasNHttpH);
	if (curl == NULL)
	{
		// curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		s_hnd[linkNo] = curl;
	}
	if (isHasNHttpH == 0) {
		s_isConnectOk[linkNo] = 0;
		//MSG_LOG("pInput:%s\n", pInput);
		pHead = strstr(pInput, "\r\n");
		if (pHead == NULL) {
			pHead = strstr(pInput, "\n");
			if (pHead == NULL) {
				return Ret_Err_Format;
			}
			pHead += 1;
		}
		else {
			pHead += 2;
		}
		tmpUC = 0;
		for (i = 0; pInput[i] != '\n'; ++i) {
			if (tmpUC == 0) {	// 去掉开头的空格
				if (pInput[i] != ' ') {
					tmpUC = 1;
				}
				continue;
			}
			else if (tmpUC == 1) {	// 去掉http method
				if (pInput[i] == ' ') {
					tmpUC = 2;
				}
				continue;
			}
			else if (tmpUC == 2) {	// 去掉http method与interface间空格
				if (pInput[i] != ' ') {
					pInterface = pInput + i;
					tmpUC = 3;
				}
				continue;
			}
			else if (tmpUC == 3) {	// 截取http interface
				if (pInput[i] == ' ') {
					pInput[i] = '\0';
					tmpUC = 4;
					break;
				}
				continue;
			}
		}//for (i = 0; pInput[i] != '\n'; ++i)

		pContent = strstr((char *)pHead, "\r\n\r\n");
		if (pContent == NULL) {
			pContent = strstr((char *)pHead, "\n\n");
			if (pContent == NULL) {
				return Ret_Err_Format;
			}

			pContent[0] = '\0';
			pContent += 2;
		}
		else {
			pContent[0] = '\0';
			pContent += 4;
		}
		len = pInput + len - (char*)pContent;


#if _debug_Socket
		MSG_LOG("http head:%s", pHead);
#endif

#if 1

		headers = curl_slist_append(headers, pHead);
#if !_debug_tms
		headers = curl_slist_append(headers, "Content-Length:");
		headers = curl_slist_append(headers, "Accept:");
		headers = curl_slist_append(headers, "Content-Type:");
#endif
		//sprintf(response, "HOST: %s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		//headers = curl_slist_append(headers, response);


		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_POST, 1L);		//strcpy(tmpbuff1, "003c600601000061010001020108000020000000c00012000253343030313333303838323133343031363035313430363100110000000000300003303031");
#if _debug_tms
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);       //设置返回的数据量  
		//printf(" send package2>%d [%s] \n", strlen(tmpbuff1), tmpbuff1);
		//res = str2Hex(pInput, strlen(pInput), tmpbuff, 1024);
		//printf(" send package>%d [%2X,%2X,%2X] \n", res, tmpbuff[0], tmpbuff[1], tmpbuff[61]);
#endif
#if _debug_Socket
		{
			char tmpbuff[1024];
			res = hex2Str(pContent, len, tmpbuff, 1024);
			MSG_LOG(" send package1>%d [%s] \n", len, tmpbuff);
		}
#endif
		//curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PO");
		sprintf(response, "https://%s:%d%s", s_stServerInfo.IPaddr, s_stServerInfo.port, pInterface);
		//sprintf(response, "https://%s%s", "qr-test1.chinaums.com", pInterface);

		MSG_LOG("response:%s\n", response);
		curl_easy_setopt(curl, CURLOPT_URL, response);// "https://202.101.25.188:20141/mjc/webtrans/VPB_lb");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)pContent);//POSTDATA//
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);//POSTDATA//

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_response_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

		// tls certification
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_CAPATH, CACERT_DIR);
		//curl_easy_setopt(hnd,CURLOPT_CAINFO,"./res/cacert-test.pem"); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//不验证主机证书// 
		curl_easy_setopt(curl, CURLOPT_CAINFO, CACERT_FILEPATH);
		curl_easy_setopt(curl, CURLOPT_HEADER, TRUE);    //表示需要response header
		curl_easy_setopt(curl, CURLOPT_NOBODY, FALSE); //表示需要response body

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);  

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OVER);
#if _debug_tms
		curl_easy_setopt(curl, CURL_REDIR_POST_ALL, 1);
#endif
#else
		{
			//HTTP报文头
			struct curl_slist *headers = NULL;
			char tmp_str[256] = { 0 };

			int ret;
			char ascbuff[1024] = { 0 };
			const char random[17] = "2222334455667788";

			//ret = TMS_InitMsg(0, &msg, "qr-test1.chinaums.com", "CL", "CL1306", "SN201806050000000001", "1.0", random, "020");
			//ret=TMS_InitMsg(0,&msg,"chinaumsapp.chinaums.com","CL","CL1306","SN201806050000000001","1.0",random);

			//CURL*curl = NULL; //处理句柄
			CURLcode res = CURLE_OK;

			//构建HTTP报文头
			headers = curl_slist_append(headers, "Host:qr-test1.chinaums.com:8001");
			headers = curl_slist_append(headers, "Accept:*/*");
			headers = curl_slist_append(headers, "FID:CL");
			headers = curl_slist_append(headers, "MID:CL1306");
			headers = curl_slist_append(headers, "SN:SN201806050000000001");
			headers = curl_slist_append(headers, "VER:1.0");
			headers = curl_slist_append(headers, "Encoding:utf-8");
			headers = curl_slist_append(headers, "TransCode:020");
			headers = curl_slist_append(headers, "Random:2222334455667788");
			headers = curl_slist_append(headers, "AuthInfo:cLIaYWHLBlQ=");
			headers = curl_slist_append(headers, "Content-Type:application/json"); //最后项必须加\r\n
			headers = curl_slist_append(headers, "Content-Length:0\r\n\r\n");

			if (curl) {
				//设置HTTP头
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				//设置post请求的url地址

				sprintf(response, "https://%s:%d%s", "qr-test1.chinaums.com", s_stServerInfo.port, pInterface);
				//sprintf(response, "https://%s%s", "qr-test1.chinaums.com", pInterface);

				MSG_LOG("response1212:%s\n", response);
				curl_easy_setopt(curl, CURLOPT_URL, response);//

				//curl_easy_setopt(curl, CURLOPT_URL, "https://qr-test1.chinaums.com:443/TMSWebService/nostandard/10003");
				//curl_easy_setopt(curl,CURLOPT_URL,"http://qr-test1.chinaums.com:8001/TMSWebService/standard/10003");
				//设置为pos请求

				//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_response_callback);
				//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

				//传递数据
				/*
				传递二进制数据，例如8583报文
				curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDS,data);
				curl_easy_setopt(easy_handle,CURLOPT_POSTFIELDSIZE,sizeof(data)); //必须明确数据长度，如是字符串，则不需要

				*/


				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

				curl_easy_setopt(curl, CURLOPT_POST, 1L);
				//
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


				//设置发送超时时间
				curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OVER);
			}
		}
#endif
	}	// if (isHasHttpH == 0)
	else if (s_isConnectOk[linkNo] == 0) {	// 仅连接ssl后台
			// tls certification
			// 两种连接方式
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_CAPATH, CACERT_DIR);
		//curl_easy_setopt(hnd,CURLOPT_CAINFO,"./res/cacert-test.pem"); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//不验证主机证书// 
		curl_easy_setopt(curl, CURLOPT_CAINFO, CACERT_FILEPATH);

		sprintf(response, "https://%s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		//sprintf(response, "%s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		curl_easy_setopt(curl, CURLOPT_URL, response);// "https://202.101.25.188:20141/mjc/webtrans/VPB_lb");
		MSG_LOG(response);
#if 1
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OVER);

#else
		/* call this function to get a socket */
		curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket);
		curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &s_sockfd);

		/* call this function to set options for the socket */
		curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_callback);
#endif
	}	// if (isHasHttpH == 0)

	MSG_LOG("\ns_isConnectOk[s_stServerInfo.linkNo]:%d\n", s_isConnectOk[linkNo]);
	if (s_isConnectOk[linkNo] == 0) {

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			https_closeHandle();
			PRINT_ERROR("curl_wasy_perform error = %s\n", curl_easy_strerror(res));
			retCode = Ret_Err_Function;
			goto https_performSsl_OVER;
		}
		/* Extract the socket from the curl handle - we'll need it for waiting. */
		res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &s_sockfd);
		s_isConnectOk[linkNo] = 1;
	}	// if (s_isConnectOk[s_stServerInfo.linkNo] == 0)

	if (pInput == NULL) {
		goto https_performSsl_OVER;
	}

	if (isHasNHttpH != 0) {	// 没有http头
		size_t nsent;

		MSG_LOG("Sending request.\n");
		nsent_total = 0;
		errTime = 0;
		do {
			/* Warning: This example program may loop indefinitely.
			* A production-quality program must define a timeout and exit this loop
			* as soon as the timeout has expired. */
			do {
				nsent = 0;
				res = curl_easy_send(curl, pInput + nsent_total, len - nsent_total, &nsent);
				MSG_LOG("curl_easy_send:res, nsent_total, len, nsent:%d,%d,%d,%d\n", res, nsent_total, len, nsent);
				nsent_total += nsent;

				if (res == CURLE_AGAIN && !wait_on_socket(s_sockfd, 0, 6000L)) {
					PRINT_ERROR("Error: timeout curl_easy_send.\n");
					//return Ret_Error;
				}
				++errTime;
				if (errTime > 5) {
					res = CURLE_OPERATION_TIMEDOUT;
					break;
				}
			} while (res == CURLE_AGAIN);

			if (res != CURLE_OK) {
				PRINT_ERROR("Error: %s\n", curl_easy_strerror(res));
				https_closeHandle();
				return Ret_Error;
			}

			MSG_LOG("Sent %" CURL_FORMAT_CURL_OFF_T " bytes.\n", (curl_off_t)nsent);

		} while (nsent_total < len);

		MSG_LOG("Reading response.\n");

		//for (;;) {
			/* Warning: This example program may loop indefinitely (see above). */
		nsent = 0;
		res = CURLE_AGAIN;
		do {
			if (res == CURLE_AGAIN && !wait_on_socket(s_sockfd, 1, 6000L)) {
				PRINT_ERROR("Error: timeout curl_easy_recv.\n");
				//return Ret_Error;
			}

			res = curl_easy_recv(curl, response, sizeof(response), &nsent);
			MSG_LOG("curl_easy_recv:res, nsent,s_sockfd:%d,%d,%d\n", res, nsent, s_sockfd);

			++errTime;
			if (errTime > 5 || (res == 0 && nsent <= 0)) {
				res = CURLE_OPERATION_TIMEDOUT;
				break;
			}
		} while (res == CURLE_AGAIN || nsent <= 0);

		if (res != CURLE_OK) {
			PRINT_ERROR("Error2222(%d): %s\n", errTime, curl_easy_strerror(res));
			https_closeHandle();
			return Ret_Error;
		}

		MSG_LOG("Received %" CURL_FORMAT_CURL_OFF_T " bytes.\n", (curl_off_t)nsent);
		//debugdata(response, nsent, 1);
	//}
		rcv_len = nsent;
	}// if (isHasNHttpH != 0)
	else {
		rcv_len = chunk.size;
	}

	MSG_LOG("len:%d\n", rcv_len);
	if (rcv_len > limitLen) {
		PRINT_ERROR("no sufficient buffer error:%d, %d", rcv_len, limitLen);
		MSG_LOG("response:%s\n", response);
		retCode = Ret_Err_Overflow;
		goto https_performSsl_OVER;
	}
	else {
		memcpy(pOutput, response, rcv_len);
		*pOLen = rcv_len;
	}

#if 1&&_debug_Socket 
	if (isHasNHttpH == 0) {
		char tmpbuff[1024];

		hex2Str((unsigned char*)response, rcv_len, (unsigned char*)tmpbuff, rcv_len * 2);

		//printf("response<%s>\n", tmpbuff);
		printf("response head:%s\n", tmpbuff);

		pContent = strstr(response, "\r\n\r\n");
		if (pContent == NULL) {
			pContent = strstr(response, "\n\n");
			if (pContent == NULL) {
				retCode = Ret_Err_Format;
				goto https_performSsl_OVER;
			}

			pContent[0] = '\0';
			pContent += 2;
		}
		else {
			pContent[0] = '\0';
			pContent += 4;
		}
		rcv_len = response + rcv_len - pContent;

		printf("response head:%s\n", response);

		printf("response len<%d>\n", rcv_len);
		hex2Str((unsigned char*)pContent, rcv_len, (unsigned char*)tmpbuff, rcv_len * 2);

		printf("response<%s>\n", tmpbuff);
	}
#endif
	//MSG_LOG("response111<%d>\n", *pOLen);

https_performSsl_OVER:
	if (headers != NULL) {
		curl_slist_free_all(headers);
	}
	//https_closeHandle();

	return retCode;
}



/**
* @Description perform TLS one time
*
* @Param
*
* @Return RetCode
*/
int socket_performNSsl(void *pData, int len, unsigned char *pOutput, int *pOLen) {

	unsigned char response[16240];
	unsigned short rcv_len;
	CURLcode res;
	int linkNo = 0;
	CURL *curl = NULL;
	MemoryStruct chunk;
	struct curl_slist *headers = NULL;
	unsigned char tmpUC = 0;
	unsigned char *pContent = NULL;
	unsigned char *pHead = NULL;
	unsigned char *pInterface = NULL;
	int i;
	int retCode = Ret_OK;
	int nsent_total;
	int errTime = 0;
	int tmpI = 0;
	int limitLen = 0;
	int totoleRecv = 0;
	char *pInput = (char *)pData;
	unsigned char isHasNHttpH = 0;

	MSG_LOG("socket_performNSsl:%d\n", len);
	if (pInput == NULL || pOutput == NULL || pOLen == NULL)
	{
		*pOLen = 0;
		PRINT_ERROR("param error:%d, %d, %d\n", pInput, pOutput, pOLen);
		return Ret_Err_Param;
	}
	limitLen = *pOLen;
	*pOLen = 0;
	if (s_stServerInfo.port == 0) {
		PRINT_ERROR("param error1:%s:%d\n", s_stServerInfo.IPaddr, s_stServerInfo.port);
		return Ret_Err_Param;
	}
	linkNo = s_stServerInfo.linkNo;
	curl = s_hnd[linkNo];

	chunk.memory = response;
	chunk.size = 0;

	pInput[len] = '\0';	// 确保字符串结束

	isHasNHttpH = s_stServerInfo.linkAttr.bits.bit2;
	MSG_LOG("isHasNHttpH:%d\n", isHasNHttpH);
	if (curl == NULL) {
		curl = curl_easy_init();
		s_hnd[linkNo] = curl;
	}
	if (isHasNHttpH == 0) {
		s_isConnectOk[linkNo] = 0;
		pHead = strstr(pInput, "\r\n");
		if (pHead == NULL) {
			pHead = strstr(pInput, "\n");
			if (pHead == NULL) {
				return Ret_Err_Format;
			}
			pHead += 1;
		}
		else {
			pHead += 2;
		}
		tmpUC = 0;
		for (i = 0; pInput[i] != '\n'; ++i) {
			if (tmpUC == 0) {	// 去掉开头的空格
				if (pInput[i] != ' ') {
					tmpUC = 1;
				}
				continue;
			}
			else if (tmpUC == 1) {	// 去掉http method
				if (pInput[i] == ' ') {
					tmpUC = 2;
				}
				continue;
			}
			else if (tmpUC == 2) {	// 去掉http method与interface间空格
				if (pInput[i] != ' ') {
					pInterface = pInput + i;
					tmpUC = 3;
				}
				continue;
			}
			else if (tmpUC == 3) {	// 截取http interface
				if (pInput[i] == ' ') {
					pInput[i] = '\0';
					tmpUC = 4;
					break;
				}
				continue;
			}
		}//for (i = 0; pInput[i] != '\n'; ++i)

		pContent = strstr((char *)pHead, "\r\n\r\n");
		if (pContent == NULL) {
			pContent = strstr((char *)pHead, "\n\n");
			if (pContent == NULL) {
				return Ret_Err_Format;
			}

			pContent[0] = '\0';
			pContent += 2;
		}
		else {
			pContent[0] = '\0';
			pContent += 4;
		}
		len = pInput + len - (char*)pContent;


#if 0 && _debug_Socket
		MSG_LOG("http head:%s", pHead);
#endif

		headers = curl_slist_append(headers, pHead);
		headers = curl_slist_append(headers, "Content-Length:");
		headers = curl_slist_append(headers, "Accept:");
		headers = curl_slist_append(headers, "Content-Type:");
		//sprintf(response, "HOST: %s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		//headers = curl_slist_append(headers, response);


		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		//strcpy(tmpbuff1, "003c600601000061010001020108000020000000c00012000253343030313333303838323133343031363035313430363100110000000000300003303031");
														//printf(" send package2>%d [%s] \n", strlen(tmpbuff1), tmpbuff1);
														//res = str2Hex(pInput, strlen(pInput), tmpbuff, 1024);
														//printf(" send package>%d [%2X,%2X,%2X] \n", res, tmpbuff[0], tmpbuff[1], tmpbuff[61]);
#if 0 && _debug_Socket
		{
			char tmpbuff[1024];
			res = hex2Str(pContent, len, tmpbuff, 1024);
			MSG_LOG(" send package1>%d [%s] \n", len, tmpbuff);
		}
#endif
		//curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PO");
		sprintf(response, "http://%s:%d%s", s_stServerInfo.IPaddr, s_stServerInfo.port, pInterface);
		MSG_LOG("response:%s\n", response);
		curl_easy_setopt(curl, CURLOPT_URL, response);// "https://202.101.25.188:20141/mjc/webtrans/VPB_lb");

		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)pContent);//POSTDATA//
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);//POSTDATA//

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_response_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);  

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OVER);
	}	// if (isHasHttpH == 0)
	else if (s_isConnectOk[linkNo] == 0) {	// 仅连接ssl后台

		s_sockfd = unblock_connect(s_stServerInfo.IPaddr, s_stServerInfo.port, 10);
		if (s_sockfd < 0)
		{
			printf("connect error %s,%s:%d, errno: %d\n", strerror(errno), s_stServerInfo.IPaddr, s_stServerInfo.port, errno);
			// sendbuffer[0]=(unsigned char*)&i; 
			memcpy(response, (unsigned char*)&i, 4);
			len += 4;
			memcpy(response, (unsigned char*)&s_sockfd, 4);
			len += 4;
			//com_send_data_OTHER_CMD(0xF2, 0x21, len, response);
			//sleep(1);
		}
		else {
#ifdef _debug_
			printf("connect OK:%d\n", s_sockfd);
#endif
			s_isConnectOk[linkNo] = 1;
		}
	}	// if (isHasHttpH == 0)

	MSG_LOG("\nsocket  s_isConnectOk[s_stServerInfo.linkNo]:%d\n", s_isConnectOk[linkNo]);
	if (s_isConnectOk[linkNo] == 0) {

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			https_closeHandle();
			PRINT_ERROR("curl_wasy_perform error = %s\n", curl_easy_strerror(res));
			retCode = Ret_Err_Function;
			goto socket_performSsl_OVER;
		}
		/* Extract the socket from the curl handle - we'll need it for waiting. */
		//res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &s_sockfd);
		s_isConnectOk[linkNo] = 1;
	}	// if (s_isConnectOk[s_stServerInfo.linkNo] == 0)

	if (isHasNHttpH != 0) {	// 没有http头
		size_t nsent;

		MSG_LOG("socket Sending request.:%d\n", s_sockfd);
		nsent_total = 0;
		errTime = 0;

		nsent = 0;
		do {
			if (!wait_on_socket(s_sockfd, 0, 6000L)) {
				PRINT_ERROR("send Error: timeout _on_socket(s_sockfd, 0, 600.\n");
				//return Ret_Error;
			}

			++errTime;
			if (errTime > 5) {
				res = CURLE_OPERATION_TIMEDOUT;
				break;
			}
		} while (len > 0);
		res = send(s_sockfd, pInput + nsent_total, len - nsent_total, MSG_WAITALL); // SOCKET_ERROR

		MSG_LOG("send:res, nsent_total, len, nsent:%d,%d,%d,%d\n", res, nsent_total, len, nsent);
		nsent_total += len;

		if (res == -1) {
			PRINT_ERROR("Fail to send data:%d\n", errTime);
			//https_closeHandle();
			goto socket_performSsl_OVER;
		}

		//for (;;) {
		/* Warning: This example program may loop indefinitely (see above). */
		nsent = 0;
		res = CURLE_AGAIN;
		totoleRecv = 0;
		errTime = 0;
		do {
			if (!wait_on_socket(s_sockfd, 1, 6000L)) {
				PRINT_ERROR("Error: timeout n_socket(s_sockfd, 1, 600.\n");
				//return Ret_Error;
			}

			res = recv(s_sockfd, response + nsent, sizeof(response) - nsent, MSG_WAITALL);
			MSG_LOG("recv:res, nsent,s_sockfd:%d,%d,%d\n", res, nsent, s_sockfd);
			nsent += res;

			if (totoleRecv == 0 && nsent > 2) {
				totoleRecv = GET_INT16(response) + 2;
			}

			MSG_LOG("totoleRecv:%d\n", totoleRecv);
			if (totoleRecv > 0 && totoleRecv <= nsent) {
				res = 0;
				break;
			}

			++errTime;
			if (errTime > 5) {
				res = CURLE_OPERATION_TIMEDOUT;
				break;
			}
		} while ((int)res >= 0);

#ifdef _debug_	
		printf("receive socket:");
		debugdata(response, totoleRecv, 1);
#endif

		if (res != CURLE_OK) {
			PRINT_ERROR("Error2222(%d): %s\n", errTime, curl_easy_strerror(res));
			//https_closeHandle();
			goto socket_performSsl_OVER;
		}

		MSG_LOG("socket Received %" CURL_FORMAT_CURL_OFF_T " bytes.\n", (curl_off_t)nsent);
		//debugdata(response, nsent, 1);
		//}
		rcv_len = nsent;
	}// if (isHasNHttpH != 0)
	else {
		rcv_len = chunk.size;
	}

	MSG_LOG("len:%d\n", rcv_len);
	if (rcv_len > limitLen) {
		PRINT_ERROR("no sufficient buffer error:%d, %d", rcv_len, limitLen);
		MSG_LOG("response:%s\n", response);
		retCode = Ret_Err_Overflow;
		goto socket_performSsl_OVER;
	}
	else {
		memcpy(pOutput, response, rcv_len);
		*pOLen = rcv_len;
	}

#if 0 && _debug_Socket
	{
		char tmpbuff[1024];

		hex2Str((unsigned char*)response, rcv_len, (unsigned char*)tmpbuff, rcv_len * 2);

		//printf("response<%s>\n", tmpbuff);
		printf("response head:%s\n", tmpbuff);

		pContent = strstr(response, "\r\n\r\n");
		if (pContent == NULL) {
			pContent = strstr(response, "\n\n");
			if (pContent == NULL) {
				retCode = Ret_Err_Format;
				goto socket_performSsl_OVER;
			}

			pContent[0] = '\0';
			pContent += 2;
		}
		else {
			pContent[0] = '\0';
			pContent += 4;
		}
		rcv_len = response + rcv_len - pContent;

		printf("response head:%s\n", response);

		printf("response len<%d>\n", rcv_len);
		hex2Str((unsigned char*)pContent, rcv_len, (unsigned char*)tmpbuff, rcv_len * 2);

		printf("response<%s>\n", tmpbuff);
	}
#endif
	//MSG_LOG("response111<%d>\n", *pOLen);

socket_performSsl_OVER:
	if (headers != NULL) {
		curl_slist_free_all(headers);
	}
	if (s_sockfd > 0) {
		MSG_LOG("close:%d\n", s_sockfd);
		close(s_sockfd);
		s_isConnectOk[linkNo] = 0;
		s_sockfd = 0;
	}

	return retCode;
}

#if 1

int https_OpenSsl(int *fd) {

	unsigned char response[16240];
	unsigned short rcv_len;
	CURLcode res;
	int linkNo = 0;
	CURL *curl = NULL;
	struct curl_slist *headers = NULL;
	unsigned char tmpUC = 0;
	unsigned char *pContent = NULL;
	unsigned char *pHead = NULL;
	unsigned char *pInterface = NULL;
	int i;
	int retCode = Ret_OK;
	int nsent_total;
	int errTime = 0;
	int tmpI = 0;
	int limitLen = 0;
	unsigned char isHasNHttpH = 0;


	MSG_LOG("https_OpenSsl:%d\n", 123479);
	if (fd == NULL) {
		PRINT_ERROR("param error:%d, %d, %d\n", fd);
		retCode = Ret_Err_Param;
		goto https_OpenSsl_OVER;
	}
	*fd = 0;

	if (s_stServerInfo.port == 0) {
		PRINT_ERROR("param error1:%s:%d\n", s_stServerInfo.IPaddr, s_stServerInfo.port);
		retCode = Ret_Err_Param;
		goto https_OpenSsl_OVER;
	}
	linkNo = s_stServerInfo.linkNo;
	curl = s_hnd[linkNo];

	isHasNHttpH = s_stServerInfo.linkAttr.bits.bit2;
	MSG_LOG("isHasNHttpH:%d\n", isHasNHttpH);
	if (curl == NULL)
	{
		// curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		s_hnd[linkNo] = curl;
	}
	if (isHasNHttpH == 0) {
		goto https_OpenSsl_OVER;
	}	// if (isHasHttpH == 0)
	else if (s_isConnectOk[linkNo] == 0) {	// 仅连接ssl后台
											// tls certification
											// 两种连接方式
		if (s_stServerInfo.linkAttr.bits.bit3 != 0) {	// socket连接
			s_sockfd = unblock_connect(s_stServerInfo.IPaddr, s_stServerInfo.port, 10);
			if (s_sockfd < 0)
			{
				retCode = Ret_Err_Function;
				printf("connect error %s,%s:%d, errno: %d\n", strerror(errno), s_stServerInfo.IPaddr, s_stServerInfo.port, errno);
			}
			else {
#ifdef _debug_
				printf("connect OK:%d\n", s_sockfd);
#endif
				s_isConnectOk[linkNo] = 1;
			}
			goto https_OpenSsl_OVER;
		}
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_CAPATH, CACERT_DIR);
		//curl_easy_setopt(hnd,CURLOPT_CAINFO,"./res/cacert-test.pem"); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//不验证主机证书// 
		curl_easy_setopt(curl, CURLOPT_CAINFO, CACERT_FILEPATH);

		sprintf(response, "https://%s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		//sprintf(response, "%s:%d", s_stServerInfo.IPaddr, s_stServerInfo.port);
		curl_easy_setopt(curl, CURLOPT_URL, response);// "https://202.101.25.188:20141/mjc/webtrans/VPB_lb");
		MSG_LOG(response);
#if 1
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIME_OVER);

#else
		/* call this function to get a socket */
		curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket);
		curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &s_sockfd);

		/* call this function to set options for the socket */
		curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_callback);
#endif
	}	// if (isHasHttpH == 0)

	MSG_LOG("\ns_isConnectOk[s_stServerInfo.linkNo]:%d\n", s_isConnectOk[linkNo]);
	if (s_isConnectOk[linkNo] == 0) {

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			https_closeHandle();
			PRINT_ERROR("curl_wasy_perform error = %s\n", curl_easy_strerror(res));
			retCode = Ret_Err_Function;
			goto https_OpenSsl_OVER;
		}
		/* Extract the socket from the curl handle - we'll need it for waiting. */
		res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &s_sockfd);
		//*fd = s_sockfd;
		s_isConnectOk[linkNo] = 1;
	}	// if (s_isConnectOk[s_stServerInfo.linkNo] == 0)

https_OpenSsl_OVER:
	*fd = s_sockfd;

	return retCode;
}
#endif

int https_performSsl_Test(const char* ip, int port, int time) {


	char response[16240];
	char response1[52400];
	unsigned short rcv_len;
	CURLcode res;
	char str1[1024] = "";
	CURL *hnd = NULL;
	int pos = 0;
	char tmpbuff[1024], tmpbuff1[1024];
	MemoryStruct chunk;

	chunk.memory = response;
	chunk.size = 0;
#if 0
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "HOST: 145.4.206.244:5000");
	headers = curl_slist_append(headers, "User-Agent: Donjin Http 0.1");
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Content-Type:x-ISO-TPDU/x-auth");
	headers = curl_slist_append(headers, "Accept: */*");
	headers = curl_slist_append(headers, "Content-Length: 62");
	//headers = curl_slist_append(headers, "");

	strcpy(tmpbuff1, "003C600601000060320032170208000020000000C00012000001323838303037383839333635323233343131313838303100110000000100400003303031");
	str2Hex(tmpbuff1, strlen(tmpbuff1), tmpbuff, 1024);

	hnd = curl_easy_init();


	curl_easy_setopt(hnd, CURLOPT_POST, 1L);
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://120.204.69.139:30000/mjc/webtrans/VPB_lb");

	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, tmpbuff);//POSTDATA//
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE, strlen(tmpbuff));//POSTDATA//

	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, save_response_callback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, response);

	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 1);
	curl_easy_setopt(hnd, CURLOPT_CAPATH, "./res/");
	//curl_easy_setopt(hnd,CURLOPT_CAINFO,"./res/cacert-test.pem"); 
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);//不验证主机证书// 


	curl_easy_setopt(hnd, CURLOPT_CAINFO, "./res/cacert.pem");


	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	//curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);  

	curl_easy_setopt(hnd, CURLOPT_TIMEOUT, 60);

	res = curl_easy_perform(hnd);

	if (res != CURLE_OK) {

		printf("curl_wasy_perform error = %s\n", curl_easy_strerror(res));
	}
	memset(response1, 0, sizeof(response1));
	rcv_len = response[0] << 8 | response[1];
	printf("response len<%d>\n", rcv_len);
	//hex2Str((unsigned char*)response + 2, rcv_len, (unsigned char*)response1, rcv_len * 2);

	printf("response<%s>\n", response);

	curl_easy_cleanup(hnd);

	return 0;
#else
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Host: 145.4.206.244:5000");
	headers = curl_slist_append(headers, "User-Agent: Donjin Http 0.1");
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Content-Type:x-ISO-TPDU/x-auth");
	headers = curl_slist_append(headers, "Accept: */*");
	headers = curl_slist_append(headers, "Content-Length: 62");
	headers = curl_slist_append(headers, "");
	//headers = curl_slist_append(headers, "\x00\x3c\x60\x06\x01\x00\x00\x61\x01\x00\x01\x02\x01\x08\x00\x00\x20\x00\x00\x00\xc0\x00\x12\x00\x02\x53\x34\x30\x30\x31\x33\x33\x30\x38\x38\x32\x31\x33\x34\x30\x31\x36\x30\x35\x31\x34\x30\x36\x31\x00\x11\x00\x00\x00\x00\x00\x30\x00\x03\x30\x30\x31");

	strcpy(tmpbuff1, "003c600601000061010001020108000020000000c00012000253343030313333303838323133343031363035313430363100110000000000300003303031");
	printf(" send package2>%d [%s] \n", strlen(tmpbuff1), tmpbuff1);
	res = CharsToBytes(tmpbuff1, strlen(tmpbuff1), tmpbuff, 1024);
	printf(" send package>%d [%2X,%2X,%2X] \n", res, tmpbuff[0], tmpbuff[1], tmpbuff[61]);
	res = BytesToChars(tmpbuff, 62, tmpbuff1, 1024);
	printf(" send package1>%d [%s] \n", res, tmpbuff1);

	hnd = curl_easy_init();


	curl_easy_setopt(hnd, CURLOPT_POST, 1L);
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://202.101.25.188:20141/mjc/webtrans/VPB_lb");

	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, tmpbuff);//POSTDATA//
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE, 62);//POSTDATA//

	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, save_response_callback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &chunk);

	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(hnd, CURLOPT_CAPATH, CACERT_DIR);
	//curl_easy_setopt(hnd,CURLOPT_CAINFO,"./res/cacert-test.pem"); 
	curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0);//不验证主机证书// 

	curl_easy_setopt(hnd, CURLOPT_CAINFO, CACERT_FILEPATH);


	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
	//curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);  

	curl_easy_setopt(hnd, CURLOPT_TIMEOUT, TIME_OVER);

	res = curl_easy_perform(hnd);

	if (res != CURLE_OK) {

		printf("curl_wasy_perform error = %s\n", curl_easy_strerror(res));
	}
	memset(response1, 0, sizeof(response1));
	rcv_len = response[0] << 8 | response[1];
	printf("response len<%d>\n", rcv_len);
	BytesToChars((unsigned char*)response + 2, rcv_len, (unsigned char*)response1, rcv_len * 2);

	printf("response<%s>\n", response1);

	curl_easy_cleanup(hnd);
#if 0
#define HTTP_POST "POST /mjc/webtrans/VPB_lb HTTP/1.1\r\nHOST: 145.4.206.244:5000\r\nUser-Agent: Donjin Http 0.1\r\nCache-Control: no-cache\r\nContent-Type:x-ISO-TPDU/x-auth\r\nAccept: */*\r\nContent-Length: %d\r\n\r\n"
	rcv_len = sprintf(response1, HTTP_POST, 63);

	printf("%d,%s\n", rcv_len, response1);
	memset(response1 + rcv_len, 0x39, 3);
	response1[rcv_len + 3] = '\0';
	printf("%d,%s\n", rcv_len, response1);
#endif

	return 0;
#endif
}



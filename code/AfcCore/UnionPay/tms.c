/**************************************************
* 文件名 :tms.c
* 描述   :
* 建立   :
**************************************************/
#include "Macro_Proj.h"
#include "string.h"
#include "stdio.h"
#include "include.h"
#include "szct.h"

#include "Gszct.h"

#include "citydifferent.h"
#include "SL8583.h"
#include "MD5.h"
#include "libsm.h"
#include "MYDES.h"
#include "RSA.h"
#include "qPBOC.h"
#ifdef qPBOC_BUS
#include "qpboc_head.h"
#endif
#include "qpboc_8583.h"
#include "add.h"
#include "StructDef.h"
#include "tms.h"
#include "cJSON.h"
#include "City-handan.h"
#include "GprsSocket.h"

extern void savesysbupara(void);
extern void saveCpuInfo(unsigned char mode, unsigned char *dat);
extern void tcpipClose(unsigned char mode);

unsigned char updata_need(void);

#define D_type "XGD"
#define D_NUM  "XGD N5"
#define D_XLH  "K3201234567"
#define test_tms


#define size_http 80

extern int buildSeverInfo_06tls(char cmd, char link,unsigned char mode);//mode//银联选用那个ip
//////////
typedef struct
{
	unsigned char POST_POST[size_http];
	unsigned char POST_HOST[size_http];
	unsigned char POST_Accept[size_http];
	unsigned char POST_FID[size_http];
	unsigned char POST_MID[size_http];
	unsigned char POST_SN[size_http];
	unsigned char POST_VER[size_http];
	unsigned char POST_ENCODING[size_http];
	unsigned char POST_TransCode[size_http];
	unsigned char POST_Random[size_http];
	unsigned char POST_Authinfo[size_http];
	unsigned char CONTENT_TYPE[size_http];
	unsigned char CONTENT_LENGTH[size_http];
}msg_http_Head;

typedef struct {
	unsigned int tfd_fileLen;
	unsigned int tfd_downLen;
	char *pFilePathPos;
	unsigned int tmsport;
	char ipPortPath[200];
}stTmsFileDown;

extern unsigned char GPRSLinkProcess;
extern unsigned char gmissflag;
extern unsigned char isNetOK[MAX_RCV_PACKET_NUM];
extern unsigned char gprsErrorTimes;
extern volatile stGPrsDataStyle gGPRS_data_style;
unsigned char DEBUG_COM;
//extern char  const  MachineCode[64];

//key:密钥,initdat:初始值默认全00,Sdat加解密数据,len:加解密数据长度,outdat:输出的数据
extern void DES3_decrypt_CBC(unsigned char *key, unsigned char *initdat, unsigned char *Sdat, unsigned int len, unsigned char *outdat);
extern void DES3_encrypt_CBC(unsigned char *key, unsigned char *initdat, unsigned char *Sdat, unsigned int len, unsigned char *outdat);
//BASE64加密，输出在base64
extern char *base64_encodesl(const unsigned char * bindata, char * base64, int binlength);
//BASE64解密，输入base64为字符串\0结束,返回输出的长度
extern int base64_decodesl(const char * base64, unsigned char * bindata);
extern void gprs_send_data(unsigned char linkNum, unsigned int len, void *dat);
extern pFistVary_1 pFistVary;
extern stcomtime SysTime;
extern unsigned char restore_flag;

static unsigned char s_tmsStatus = Key_activation;
static unsigned char s_serverVer[10] = { 0 };
//static unsigned char s_serverVer_app2[10] = { 0 };
//static char s_downFilePath[50] = { 0 };
static stTmsFileDown s_tmsFileDown;

#define APPIndex		"2"
#define APPIndex_app2	"2"
#define LENUNIT_DOWN	1100

#if 1

int main1111(int argc, const char * argv[]) {

	stJSON stjsons[100];
	stJSON *pStJson = NULL;
	int length = 0;
	int retcode;
	char *pTmpCh = NULL;
	char output[1000];
	int outLen = 0;


	/* a bunch of json: */
	char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\": false,\n\"frame rate\": 24\n}\n}";
	char text2[] = "\"sfksjfksj\":[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[] = "{\"APP2\":{\"APPState\":\"1\",\"APPIndex\":\"1\",\"APPVer\":\"3371080001\",\"APPID\":\"银商POS通B2C\"},\"APP1\":{\"APPState\":\"1\",\"APPIndex\":\"0\",\"APPVer\":\"0000000001\",\"APPID\":\"银商多应用\"},\"APP3\":{\"APPState\":\"1\",\"APPIndex\":\"2\",\"APPVer\":\"33831802\",\"APPID\":\"银商MISPOS\",\"Parameter\":{\"deptcode\":\"100600\",\"04000029\":\"upos.chinaums.com\",\"04000031\":\"upos.chinaumswh.com\",\"04000001\":\"1\",\"04000030\":\"10003\"}},\"ReturnCode\":\"00\",\"APPCount\":3,\"ReturnMsg\":\"成功\"}";
	char text4[] = "{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[] = "{\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 }";


	/* Process each json textblock by parsing, then rebuilding: */
	MSG_LOG("**********************************************************\n");
	length = 100;
	pTmpCh = text1;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);
	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "format");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	//MSG_LOG("pStJson:%s=%d\n", pStJson->pmName, pStJson->number);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("getJsonString:%s\n", output);
	MSG_LOG("**********************************************************\n");
	length = 100;
	pTmpCh = text2;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);
	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "sfksjfksj");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("pStJson:%s=%s\n", pStJson->pmName, pStJson->pmValue);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("getJsonString:%s\n", output);

	MSG_LOG("**********************************************************\n");
	length = 100;
	pTmpCh = text3;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);

	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "APP1");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("pStJson:%s=%s\n", pStJson->pmName, pStJson->pmValue);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("getJsonString:%s\n", output);
	MSG_LOG("**********************************************************\n");
	length = 100;
	pTmpCh = text4;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);

	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "Image");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("pStJson:%s=%s\n", pStJson->pmName, pStJson->pmValue);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("*****************************************************************************************\n");
	length = 100;
	pTmpCh = pStJson->pmValue;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);

	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "Height");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("pStJson:%s=%s\n", pStJson->pmName, pStJson->pmValue);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}

	MSG_LOG("getJsonString:%s\n", output);
	MSG_LOG("**********************************************************\n");
	length = 100;
	pTmpCh = text5;
	retcode = parseJsonCurLevel(pTmpCh, strlen(pTmpCh), stjsons, &length);

	if (retcode != 0) {
		MSG_LOG("fail to parseJsonCurLevel:%s\n", pTmpCh);
		return -1;
	}
	pStJson = getJsonObjByName(stjsons, length, "Longitude");
	if (pStJson == NULL) {
		MSG_LOG("fail to getJsonObj:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("pStJson:%s=%f\n", pStJson->pmName, pStJson->number);

	outLen = 1000;
	retcode = getJsonString(stjsons, length, output, &outLen);
	if (retcode != 0) {
		MSG_LOG("fail to getJsonString:%s\n", pTmpCh);
		return -1;
	}
	MSG_LOG("getJsonString:%s\n", output);


	return 0;
}
#endif



//#define XGD
#define SLZR_TMS
//#define KL

#ifdef SLZR_TMS
#define test_ 0
#define HOST_ "qr-test1.chinaums.com"
#define Accept_ "*/*"
#define FID_ "SLZR"
#define MID_ "ESF3000L"
#define VER_ "1.0"
#define ENCODING_ "UTF-8"
#define CONTENTTYPE "application/json"
#define SN_ "ESF3000L00005541"
#define rand_ "2222334455667788"
#define Infor_aut "2bnGCVImilo="
//#define KEYM_1 "\xB1\xC6\xD9\xCC\xAC\x9A\xEE\xCD\x48\x2D\x86\x15\x58\x6A\xEF\xD7"
//#define KEYM_2  "\xBF\x5E\x99\xD5\xB1\x0B\x2C\x41\xEC\x54\xA1\x50\x09\x50\x43\xF6"
//#define KEYM_1 "\x96\x7F\xC2\x4D\x46\x35\x5A\xA3\xAE\x89\xA0\xA6\x10\xFD\xC3\xCD"
#define KEYM_1 "\x35\xF5\xB3\x19\x31\x04\x18\x63\xA6\x75\xDB\xB4\xED\x9E\x66\xE9"
//35F5B31931041863A675DBB4ED9E66E9
#define KEYM_2  "\x12\xAB\x48\x1A\x5C\xCC\xCB\xA1\x7E\x86\xAA\x1D\x11\x90\xD7\x58"
//	#define HTTP_SSL_POST_POST "POST /TMSWebService/nostandard/%05d HTTP/1.1\r\n"
#elif defined XGD
#define test_ 1

#define HOST_ "qr-test1.chinaums.com:8001"
#define Accept_ "*/*"
#define FID_ "XGD"
#define MID_ "XGD N5"
#define VER_ "1.0"
#define ENCODING_ "UTF-8"
#define CONTENTTYPE "application/json"
#define SN_ "K3201234567"
#define rand_ "5C9EA04B291DB829"
#define Infor_aut "2bnGCVImilo="
#define KEYM_1 "\x16\x0f\xc5\x7a\x34\x1f\xce\xb9\x80\x50\xd1\x35\xc0\xb0\x44\x99"
#define KEYM_2  "\x1d\xa1\x24\x9e\x15\xe8\x4e\xbd\x16\x46\xca\x2d\x7a\xf7\x5f\x1a"
//	#define HTTP_SSL_POST_POST "POST /TMSWebService/nostandard/%05d HTTP/1.1\r\n"
#elif defined KL
#define test_ 1
//#define POST_ "POST /TMSWebServicestandard/%05d HTTP/1.1"
//	#define TransCode_ "TransCode:%03x\r\n"
#define HOST_ "qr-test1.chinaums.com:8001"
#define Accept_ "*/*"
#define FID_ "CL"
#define MID_ "CL1306"
#define VER_ "1.0"
#define ENCODING_ "utf-8"
#define CONTENTTYPE "application/json"
#define SN_ "SN201806050000000001"
#define rand_ "2222334455667788"
#define Infor_aut "cLIaYWHLBlQ="
#define KEYM_1 "\xB1\xC6\xD9\xCC\xAC\x9A\xEE\xCD\x48\x2D\x86\x15\x58\x6A\xEF\xD7"
#define KEYM_2  "\xB1\xC6\xD9\xCC\xAC\x9A\xEE\xCD\x48\x2D\x86\x15\x58\x6A\xEF\xD7"
//	#define HTTP_SSL_POST_POST "POST /TMSWebServicestandard/%05d HTTP/1.1\r\n"
#endif
/////////////////

#define HTTP_SSL_POST_POST "POST /TMSWebService/nostandard/%05d HTTP/1.1\r\n"
#define HTTP_SSL_POST_POST_1000 "POST /TMSWebService/standard/%05d HTTP/1.1\r\n"
#define HTTP_SSL_GET_DOWN "GET /%s HTTP/1.1\r\n"


#define HTTP_SSL_POST_HOST_PORT  "HOST:%s:%d\r\n"
#define HTTP_SSL_POST_HOST  "HOST:%s\r\n"
#define HTTP_SSL_POST_Accept   "Accept:%s\r\n"
#define HTTP_SSL_POST_FID   "FID:%s\r\n"
#define HTTP_SSL_POST_MID   "MID:%s\r\n"
#define HTTP_SSL_POST_SN     "SN:%s\r\n"
#define HTTP_SSL_POST_VER    "VER:%s\r\n"
#define HTTP_SSL_POST_ENCODING  "Encoding:%s\r\n"
#define HTTP_SSL_POST_TransCode  "TransCode:%03x\r\n"
#define HTTP_SSL_POST_Random     "Random:%s\r\n"
#define HTTP_SSL_POST_Authinfo    "Authinfo:%s\r\n"
#define HTTP_SSL_POST_CONTENT_TYPE  "Content-type:%s\r\n"
#define HTTP_SSL_POST_CONTENT_LENGTH "Content-Length:%d\r\n"
#define HTTP_SSL_POST_Range     "Range:bytes=%d-%d\r\n"



unsigned char get_SN(unsigned char * SN, unsigned char mode)
{
	unsigned  char buff[20];
	unsigned len = 0;
	memset(buff, 0, sizeof(buff));
	strcat((char*)buff + len, DEV_TYPE);
	len += (strlen(DEV_TYPE));
	memcpy(buff + len, pFistVary.DeviceNo, 8);
	len += 8;
	MSG_LOG("厂商序列号=%d\r\n", (strlen(DEV_TYPE) + 8));
	//MSG_LOG("长度=%d\r\n", len);
	BCD_LOG(buff, len, 1);
	if (mode == 0)
	{
		memcpy(SN, buff, len);

	}
	else
	{
		strcat((char *)SN, SN_);
		len = strlen((const char *)SN);
	}
	MSG_LOG("总长度=%d\r\n", len);
	return len;

}
unsigned char public_msg_head(msg_http_Head *msg_http_Head, unsigned char mode)
{
	char buff[30];
	memset(buff, 0, sizeof(buff));
	//len = 0;
	//host
	sprintf((char *)msg_http_Head->POST_HOST, HTTP_SSL_POST_HOST, HOST_);
	//
	sprintf((char *)msg_http_Head->POST_Accept, HTTP_SSL_POST_Accept, Accept_);
	//POST_FID
	sprintf((char *)msg_http_Head->POST_FID, HTTP_SSL_POST_FID, FID_);
	//POST_MID
	sprintf((char *)msg_http_Head->POST_MID, HTTP_SSL_POST_MID, MID_);
	//SN	//get 终端硬件序列号
	get_SN((unsigned char *)buff, mode);
	sprintf((char *)msg_http_Head->POST_SN, HTTP_SSL_POST_SN, buff);
	//VER
	sprintf((char *)msg_http_Head->POST_VER, HTTP_SSL_POST_VER, VER_);
	//Encoding
	sprintf((char *)msg_http_Head->POST_ENCODING, HTTP_SSL_POST_ENCODING, ENCODING_);
	//
	sprintf((char *)msg_http_Head->CONTENT_TYPE, HTTP_SSL_POST_CONTENT_TYPE, CONTENTTYPE);

	return ST_OK;

}

//void get_infor_aut(msg_http_Head *msg_http_Head,unsigned char *rands,unsigned char *keyMain,unsigned char mode )
unsigned char  get_infor_aut(unsigned char *rands, unsigned char *keyMain, unsigned char mode, char * out)
{
	unsigned int len = 0;
	unsigned char buff[64];
	unsigned char autkey[16];
	unsigned char infor_aut[65];
	unsigned char inbuff[64];
	unsigned int i, j;
	//MSG_LOG("随机数\r\n");
	//BCD_LOG(rands, 16, 1);
	MSG_LOG("主控\r\n");
	BCD_LOG(keyMain, 16, 1);

	//3es加密
#if 0
	if (mode == 1)
	{
		len = strlen(Infor_aut);
		memcpy(out, Infor_aut, len);
		MSG_LOG("认证信息:%s\r\n", infor_aut);
		MSG_LOG("认证信息长度:%d\r\n", len);
		return len;
	}
#endif
	DES3_encrypt(rands, keyMain, autkey);
	DES3_encrypt(rands + 8, keyMain, autkey + 8);
	MSG_LOG("认证\r\n");
	BCD_LOG(autkey, 16, 1);

	//拼
	len = 0;
	memset(buff, 0, sizeof(buff));


	strcat((char*)buff + len, FID_);
	len += (strlen(FID_));
	MSG_LOG("厂商标识长度=%d\r\n", (strlen(FID_)));

	strcat((char*)buff + len, MID_);
	len += (strlen(MID_));
	MSG_LOG("型号=%d\r\n", (strlen(MID_)));

	len += get_SN(buff + len, mode);

	//MSG_LOG("总长度=%d\r\n", len);
	BCD_LOG(buff, len, 1);

	if (len % 8)
	{
		//MSG_LOG("需要补0\r\n");
		len += (8 - (len % 8));
	}
	//MSG_LOG("总长度+补0=%d\r\n", len);
	BCD_LOG(buff, len, 1);
	// 
	// #ifdef _debug_
	// 	Ascii2BCD("584744584744204E354B3332303132333435363700000000",buff,48);
	// 		MSG_LOG("总长度\r\n");
	// 	BCD_LOG(buff, 24, 1);
	// 	
	// 	memcpy(autkey,"\xF3\xC4\xA7\x2B\x3F\x02\x4D\x74\xE0\xCB\xEE\xED\x4A\x3D\x2A\x05",16);
	// 	len=24;
	// #endif
	//key:密钥,initdat:初始值默认全00,Sdat加解密数据,len:加解密数据长度,outdat:输出的数据
	memset(infor_aut, 0, sizeof(infor_aut));
	memset(inbuff, 0, sizeof(inbuff));

	for (i = 0; i < len; i += 8)
	{
		for (j = 0; j < 8; ++j)
		{
			inbuff[j] ^= buff[i + j];
		}
		DES3_encrypt(inbuff, autkey, inbuff + 8);
		//MSG_LOG("计算后\r\n");
		//BCD_LOG(inbuff + 8, 8, 1);
		memcpy(inbuff, inbuff + 8, 8);
	}
	memset(infor_aut, 0, sizeof(infor_aut));
	base64_encodesl(inbuff, (char *)infor_aut, 8);
	//	DES3_encrypt_CBC(autkey, inbuff, buff, len, infor_aut);
	len = strlen((const char *)infor_aut);
	MSG_LOG("认证信息:%s\r\n", infor_aut);
	MSG_LOG("认证信息长度:%d\r\n", len);
	memcpy(out, infor_aut, len);
	return len;
	// 	MSG_LOG("数据\r\n");
	// 	BCD_LOG(buff, 60, 1);


}
int  build_HTTP_qpboc_HEAD_2(unsigned char mode, char *oDat, unsigned int len1)
{

	char buff[1024]; //整个报文的buff
	unsigned int len_all = 0, len_data = 0;
	msg_http_Head msg_http_;
	unsigned char Rand[17];
	unsigned char KEYM[16];
	unsigned char AUT_inf[32];
	//清o
	memset((unsigned char *)&msg_http_, 0, sizeof(msg_http_Head));
	//得到公共的头
	public_msg_head(&msg_http_, test_);

	//get 随机数-暂时写死
	memset(Rand, 0, sizeof(Rand));
	memcpy(Rand, rand_, strlen(rand_));
	sprintf((char *)msg_http_.POST_Random, HTTP_SSL_POST_Random, Rand);
	//get 认证信息
	memset(AUT_inf, 0, 32);
	if (mode == Key_activation)
	{
#if test_
		memcpy(KEYM, KEYM_1, 16);
#else
		getMobileParameter(9, KEYM);
#endif
	}
	else
	{
#if test_
		memcpy(KEYM, KEYM_2, 16);
#else
			getMobileParameter(7, KEYM);
#endif

	}


	get_infor_aut(Rand, KEYM, test_, (char *)AUT_inf);
	sprintf((char *)msg_http_.POST_Authinfo, HTTP_SSL_POST_Authinfo, AUT_inf);

	len_data = len1;
	//赋值
	//MSG_LOG("步骤-1-\r\n");
	switch (mode)
	{
	case Update_check:
		sprintf((char *)msg_http_.POST_TransCode, HTTP_SSL_POST_TransCode, mode);
		sprintf((char *)msg_http_.POST_POST, HTTP_SSL_POST_POST_1000, 10001);
		sprintf((char *)msg_http_.CONTENT_LENGTH, HTTP_SSL_POST_CONTENT_LENGTH, len_data);
		break;
	case Download_query:
		sprintf((char *)msg_http_.POST_TransCode, HTTP_SSL_POST_TransCode, mode);
		sprintf((char *)msg_http_.POST_POST, HTTP_SSL_POST_POST_1000, 10002);
		sprintf((char *)msg_http_.CONTENT_LENGTH, HTTP_SSL_POST_CONTENT_LENGTH, len_data);
		break;
	case Key_activation:
		//get正文内容，与长度 
		sprintf((char *)msg_http_.POST_TransCode, HTTP_SSL_POST_TransCode, mode);
		sprintf((char *)msg_http_.POST_POST, HTTP_SSL_POST_POST, 10003);
		sprintf((char *)msg_http_.CONTENT_LENGTH, HTTP_SSL_POST_CONTENT_LENGTH, len_data);
		break;
	case Key_download:
		sprintf((char *)msg_http_.POST_TransCode, HTTP_SSL_POST_TransCode, mode);
		sprintf((char *)msg_http_.POST_POST, HTTP_SSL_POST_POST_1000, 10004);
		sprintf((char *)msg_http_.CONTENT_LENGTH, HTTP_SSL_POST_CONTENT_LENGTH, len_data);
		break;
	case Device_key_Download:
		sprintf((char *)msg_http_.POST_TransCode, HTTP_SSL_POST_TransCode, mode);
		sprintf((char *)msg_http_.POST_POST, HTTP_SSL_POST_POST_1000, 10005);
		sprintf((char *)msg_http_.CONTENT_LENGTH, HTTP_SSL_POST_CONTENT_LENGTH, len_data);
		break;
	default:
		break;
	}


	memset((char *)buff, 0, sizeof(buff));
	strcat((char *)buff, (char *)msg_http_.POST_POST);
	strcat((char *)buff, (char *)msg_http_.POST_HOST);
	strcat((char *)buff, (char *)msg_http_.POST_Accept);
	strcat((char *)buff, (char *)msg_http_.POST_FID);
	strcat((char *)buff, (char *)msg_http_.POST_MID);
	strcat((char *)buff, (char *)msg_http_.POST_SN);
	strcat((char *)buff, (char *)msg_http_.POST_VER);
	strcat((char *)buff, (char *)msg_http_.POST_ENCODING);
	strcat((char *)buff, (char *)msg_http_.POST_TransCode);
	strcat((char *)buff, (char *)msg_http_.POST_Random);
	strcat((char *)buff, (char *)msg_http_.POST_Authinfo);
	strcat((char *)buff, (char *)msg_http_.CONTENT_TYPE);

	strcat((char *)buff, (char *)msg_http_.CONTENT_LENGTH);
	strcat((char *)buff, "\r\n");

	if (len_data != 0)
	{
		strcat(buff, oDat);
		MSG_LOG("加json正文\r\n");
	}
	else
	{
		MSG_LOG("没有正文\r\n");
	}
	len_all = strlen((const char *)buff);
	MSG_LOG("长度= %d\r\n", len_all);
	MSG_LOG("组装的报文= %s\r\n", buff);
	BCD_LOG((unsigned char *)buff, len_all, 1);
	memcpy(oDat, buff, len_all);
	//	strcat((char *)oDat,(char *)buff);
	return len_all;
}

int  build_HTTP_qpboc_HEAD_DOWN(unsigned char mode, char *oDat, unsigned int len1)
{

	char buff[1024]; //整个报文的buff
	unsigned int len_all = 0, len_data = 0;
	msg_http_Head msg_http_;
	//unsigned char Rand[17];
	//	unsigned char KEYM[16];
//	unsigned char AUT_inf[32];
	//清o
	memset((unsigned char *)&msg_http_, 0, sizeof(msg_http_Head));

	len_all = sprintf(buff, HTTP_SSL_GET_DOWN, s_tmsFileDown.pFilePathPos);
	MSG_LOG("build_HTTP_qpboc_HEAD_DOWN:%s,%s\n", s_tmsFileDown.pFilePathPos, buff);
	//len_all += sprintf(buff + len_all, HTTP_SSL_POST_HOST_PORT, s_tmsFileDown.ipPortPath, s_tmsFileDown.tmsport);
	len_all += sprintf(buff + len_all, HTTP_SSL_POST_Accept, "*/*");
	len_all += sprintf(buff + len_all, HTTP_SSL_POST_Range, s_tmsFileDown.tfd_downLen, LENUNIT_DOWN + s_tmsFileDown.tfd_downLen - 1);
	len_all += sprintf(buff + len_all, "\r\n");

	if (len_data != 0)
	{
		strcat(buff, oDat);
		MSG_LOG("加json正文111\r\n");
	}
	else
	{
		MSG_LOG("没有正文111\r\n");
	}
	MSG_LOG("长度= %d\r\n", len_all);
	MSG_LOG("组装的报文= %s\r\n", buff);
	BCD_LOG((unsigned char *)buff, len_all, 1);
	memcpy(oDat, buff, len_all);
	//	strcat((char *)oDat,(char *)buff);
	return len_all;
}


extern void main_card(void);
//extern int buildSeverInfo_06tlstims(char cmd, char link, char *ip, int port);
extern int buildSeverInfo_06(char cmd);
//extern int buildSeverInfo_06tls_TMS(char cmd, char link) ;
static unsigned char s_isDownloadSend = 0;
static unsigned char s_errTime = 0;
unsigned char show_para_tms(void);
void save_tms_para(void);
//extern int open_sock(unsigned int addr, unsigned short port, unsigned int outtime, unsigned char link);
int ProcessTmsData(void) {
	int outdly = 0;
	int ret;
	int flag = 0;
	char buff[1000];
	unsigned char IP_shift__stop=0;
	//unsigned char jsonBuf[400];

	//	unsigned int ip = 0, port = 0;
	//	unsigned char isOpen = 0;
	int link = LINK_PBOC_TMS;
	int delay = 0;
	int exeStatus = 0;
	int jasonLen = 0;

	//gmissflag = MISS_G_TMS;
	MSG_LOG("ProcessTmsData:\n");

	
// 
// 	getMobileParameter(16,(unsigned char *)buff);

// 	if(	set_TMS_START_FLAGE_FLAG==)
// 	{
// 		cls();
// 		display(6, 0, "没有下载分量1 ", DIS_CENTER);
// 		//	MessageBox(0, "更新检查成功!");
// 		beep(1, 200, 200);
// 		delayxs(2);
// 		cls();
// 		return -1;
// 	}
	//main1111(0, NULL);
	s_errTime = 0;
	s_isDownloadSend = 0;
 //	memset((unsigned char *)&s_tmsFileDown,0,sizeof(stTmsFileDown));
   // 	sysfewrite(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char *)&s_tmsFileDown);
	set_QPBOC_IP_MODE(TMS_sign_shift,0xAB);
	if(Get_QPBOC_IP_MODE(IP_ok_shift)==0xCD) 
	{
		IP_shift__stop=0xCD;
		set_QPBOC_IP_MODE(IP_ok_shift,0xab);  //不去轮询
	}

	 //
	sysferead(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char *)&s_tmsFileDown);

	if (s_tmsFileDown.pFilePathPos != NULL && s_tmsFileDown.ipPortPath[0] != '\0' && s_tmsFileDown.tfd_downLen > 0 && s_tmsFileDown.tfd_fileLen >= 0) {
		exeStatus = 4;
		s_tmsStatus = Download_File;
	}
	else {
		memset(&s_tmsFileDown, 0x00, sizeof(stTmsFileDown));
		s_tmsStatus = 0;
	}

	while (1) {//开始申请

		if (s_errTime > 20) {

			MSG_LOG("TMS下载错误次数过多:%d\n", s_errTime);
			sprintf((char *)buff, "TMS下载错误次数过多:%d", s_errTime);
			MessageBox(1, buff);
			delayxms(1000);
			break;
		}

		if (outdly % 200 == 0) {
			clr_wdt();
			cls();
			sprintf((char *)buff, "TMS一键更新(%d):%d", s_errTime, outdly / 100);
			display(0, 0, (const char *)buff, 0);
			//com_snd(COM4,len,buff);
		}
		gGprsinfo.gmissflag = MISS_G_TMS;
		//main_GPRS(NULL);
		
		if (gGprsinfo.GPRSLinkProcess == 20) {

			while (1)
			{
				delayxms(1);
				if (getkey(1) != 0)
				{
					return -1;
				}
#ifdef _debug_gprs00
				memset(disbuff, 0, sizeof(disbuff));
				sprintf((char *)disbuff, "T2:%04d", outtime - delay);
				miniDispstr(7, 30, (const char *)disbuff, 0);
#endif

				if (isNetOK[link] == 3)
				{
#ifdef _debug_gprs
					MSG_LOG("open time:%u\r\n", delay);
#endif
	clr_wdt();
					continue;
				}
				delay = get_timer0(1);
				if (delay++ <= 0)
				{

					MSG_LOG("ProcessTmsData.while.if.while.if :%d", delay);
					//	gprsErrorTimes+=25;		//直接这样试试
					MSG_LOG("19==\r\n");
					gGPRS_data_style.openLink = link;
					//GPRSLinkProcess = 19;	//
					tcpipClose(link);
					gGprsinfo.GPRSLinkProcess = 19;
					break;
					//return -1;
				}

			}

		}
		if (gGprsinfo.GPRSLinkProcess < 21) {//已经连接
			if (flag != 1) {
				flag = 1;
				display(6, 0, "TMS正在拔号", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	
			clr_wdt();
			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == 0x2F) {
			if (flag != 1) {
				flag = 1;
				display(6, 0, "TMS正在重新连接", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);
				clr_wdt();
			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == TCPSTARTSTAT) {

		}
		if (gprsErrorTimes > 20) {

			gGprsinfo.GPRSLinkProcess = GPRS_HWPOWER_RESET;
		}

		if (isNetOK[LINK_PBOC_TMS] == 0)
		{
			goto ProcessTmsData_LOOP;
		}

		if (exeStatus == 0) {

			Q_QPBOC_para.TMS_KEY_FLAGE= get_switch_control(17);
			if(Q_QPBOC_para.TMS_KEY_FLAGE==set_Key_activation_FLAG) //已经做过密钥激活
			{
					exeStatus = 1;
					s_tmsStatus = Key_download;	
					MSG_LOG("做过密钥激活\r\n");
			}
			else
			{
				//	ret = build_HTTP_qpboc_HEAD(Key_activation, buff, 0);
			ret = build_HTTP_qpboc_HEAD_2(Key_activation, buff, 0);
			//		ret = build_HTTP_qpboc_HEAD_2(Device_key_Download, buff, 0);

			//		ret = build_HTTP_qpboc_HEAD_2(Key_download, buff, 0);

				// 		memcpy((unsigned char*)&us1, gBusVerInfo.newbusBLKVer, 2);
				// 		if((us1 ==  0)||(us1 ==  0xffff)){//服务器的版本号.还没有登陆
				// 			if(flag != 2){
				// 				flag = 2;
				// 				display(4, 0, "正在登陆", DIS_ClsLine|DIS_CENTER);
				// 			}
				// 			continue;
				// 		}

			//MSG_LOG("do=1= %s\r\n", buff);
			//MSG_LOG("do=1= %d\r\n", ret);
			s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			}
			outdly = 0;
		}
		else if (s_tmsStatus == Tms_Over) {
			MSG_LOG("_tmsStatus == Tms_Ov\n");
			MessageBox(0, "TMS检查完成");
			beep(1, 200, 200);
			delayxs(1000);
			break;
		}
		else if (exeStatus == 1 && s_tmsStatus == Key_download) {
		if(Q_QPBOC_para.TMS_KEY_FLAGE==set_Key_activation_FLAG) //已经做过密钥激活
		{
			cls();
			display(6, 0, "密钥已经激活!", DIS_CENTER);
		}
		else
		{
			cls();
			display(6, 0, "密钥激活成功!", DIS_CENTER);
			//MessageBox(0, "密钥激活成功!");
		}
		beep(1, 200, 200);
		delayxms(200);

			ret = build_HTTP_qpboc_HEAD_2(s_tmsStatus, buff, 0);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			outdly = 0;
		}
		else if (exeStatus == 2 && s_tmsStatus == Update_check) {
			cls();
			display(6, 0, "密钥下载成功!", DIS_CENTER);
		//	MessageBox(0, "密钥下载成功!");
			beep(1, 200, 200);
			delayxms(200);
#if 1
			//"Guilin v1.0.1"
			jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, SYS_HEAD_STREn);//SYS_HEAD_STREn
			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_TIME_LOG, APPIndex);
			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"1\"}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"01000005\",\"01000002\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"04000055\",\"04000054\",\"04000008\",\"deptcode\",\"04000056\",\"04000051\",\"04000039\",\"04000038\",\"04000031\",\"04000030\",\"03000005\",\"04000030\",\"04000029\",\"04000028\",\"04000026\",\"04000023\",\"04000022\",\"04000017\",\"04000016\",\"04000015\",\"04000014\",\"04000013\",\"04000012\",\"04000007\",\"04000006\",\"04000003\",\"04000002\",\"04000001\",\"03000012\",\"02000002\",\"01000005\",\"01000002\",\"01000001\"]}}");

// 			jasonLen += sprintf(buff+ jasonLen, "\"APP2\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, "Guilin");//"Guilin"
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_TIME_LOG, APPIndex);
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\"}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"deptcode\",\"04000056\",\"04000051\",\"04000039\",\"04000038\",\"04000031\",\"04000030\",\"03000005\",\"04000030\",\"04000029\",\"04000028\",\"04000026\",\"04000023\",\"04000022\",\"04000017\",\"04000016\",\"04000015\",\"04000014\",\"04000013\",\"04000012\",\"04000007\",\"04000006\",\"04000003\",\"04000002\",\"04000001\",\"03000012\",\"02000002\",\"01000005\",\"01000002\",\"01000001\"]}}");
// //
// 			//"Guilin v1.0.1"
// 			jasonLen = sprintf(buff, "{\"APPCount\":\"2\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, SYS_HEAD_STREn);
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_bASE_LOG, APPIndex);
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"1\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000003\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"01000005\",\"01000002\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]},");
// 		
// 			jasonLen += sprintf(buff+ jasonLen, "\"APP2\":{\"APPID\":\"SLZR_%s_%s\"", "ESF300L", "Guilin");
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_bASE_LOG, "2");
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");

#else
			jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_ESF300L_Guilin\",\"APPVer\":\"v1.0.0\",\"APPIndex\":\"0\",\"APPState\":\"1\",\"ParameterUp\":{\"04000001\":\"1\"},\"Parameter\":[\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000030\",\"04000031\",\"deptcode\"]}");
#endif

			ret = build_HTTP_qpboc_HEAD_2(s_tmsStatus, (char *)buff, jasonLen);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			outdly = 0;
		}
		else if (exeStatus == 3 && s_tmsStatus == Download_query) {
			cls();
			display(6, 0, "更新检查成功!", DIS_CENTER);
			//	MessageBox(0, "更新检查成功!");
			beep(1, 200, 200);
			delayxms(200);
			if(show_para_tms() !=ST_OK)  //显示参数
			{
				restore_flag=3;
				break;
			}
				save_tms_para();	    //更新参数	
//					Q_QPBOC_para_INIT();
			if(updata_need()==ST_ERROR)  //判断是否需要检查更新
			{
				restore_flag=3;
					break;
			}
		
		jasonLen = sprintf((char *)buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);
	//	jasonLen = sprintf((char *)buff+jasonLen, ",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}  ", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);
			
			ret = build_HTTP_qpboc_HEAD_2(s_tmsStatus, buff, jasonLen);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			outdly = 0;
		}
		else if (exeStatus == 4 && s_tmsStatus == Download_File) {
			cls();
			display(6, 0, "下载查询成功!", DIS_CENTER);
		//	MessageBox(0, "下载查询成功!");
			beep(1, 200, 200);
			delayxms(400);
			cls();
		
			//jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);

			//buildSeverInfo_06tlstims(0x26, 5, s_tmsFileDown.ipPortPath, s_tmsFileDown.tmsport);

			delayxms(1000);
			++exeStatus;
			outdly = 0;
		}
		else if (s_tmsStatus == Download_File && s_isDownloadSend == 0) {
			s_isDownloadSend = 1;
// #ifdef _debug_
// 			return ST_OK;
// #endif
			ret = build_HTTP_qpboc_HEAD_DOWN(s_tmsStatus, buff, 0);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, (unsigned char *)buff);
			++exeStatus;
			outdly = 0;
		}
		else {
			//MSG_LOG("s_tmsStatus,s_isDownloadSend:%02X,%d\n", s_tmsStatus, s_isDownloadSend);
		}

	ProcessTmsData_LOOP:
		delayxms(10);

		ret = getkey(1);
		if (ret == SLZRKEY_ESC) {
			//exeStatus = 0;
			MSG_LOG("TMSKEY_ESC\n");
			break;
		}
		if (outdly++ > 3000) {

			display(2, 0, "接收超时", DIS_CONVERT | DIS_CENTER);
			beep(3, 100, 30);
			delayxms(2000);
			break;
		}
	}
//QPBOC_IP_MODE_INIT();
	set_QPBOC_IP_MODE(TMS_sign_shift,0xCD);
	MSG_LOG("TMS过程线束\n");
	if(IP_shift__stop==0xCD)  //切回轮询
	{
		set_QPBOC_IP_MODE(IP_ok_shift,0xCD);
	
	}
//	buildSeverInfo_06(0x26);
		clr_wdt();
	restore_flag = 3;
	return ST_OK;
}
//开机调用
int ProcessTmsData_2(void) {
	int outdly = 0;
	int ret;
	int flag = 0;
	char buff[1000];
	unsigned char IP_shift__stop=0;
	//unsigned char jsonBuf[400];

	//	unsigned int ip = 0, port = 0;
	//	unsigned char isOpen = 0;
	int link = LINK_PBOC_TMS;
	int delay = 0;
	int exeStatus = 0;
	int jasonLen = 0;

	//gmissflag = MISS_G_TMS;
	MSG_LOG("ProcessTmsData_AUTO:\n");

	
// 
// 	getMobileParameter(16,(unsigned char *)buff);
// 	if(memcmp(buff,"\x33\x88",2)!=0)
// 	{
// 		cls();
// 		display(6, 0, "没有下载分量1 ", DIS_CENTER);
// 		//	MessageBox(0, "更新检查成功!");
// 		beep(1, 200, 200);
// 		delayxs(2);
// 		cls();
// 		return -1;
// 	}
	set_QPBOC_IP_MODE(TMS_sign_shift,0xab);
	if(Get_QPBOC_IP_MODE(IP_ok_shift)==0xCD) 
	{
		IP_shift__stop=0xCD;
		set_QPBOC_IP_MODE(IP_ok_shift,0xab);  //不去轮询
	}

	s_errTime = 0;
	s_isDownloadSend = 0;
// 	memset((unsigned char *)&s_tmsFileDown,0,sizeof(stTmsFileDown));
//    	sysfewrite(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char *)&s_tmsFileDown);
	sysferead(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char *)&s_tmsFileDown);

	if (s_tmsFileDown.pFilePathPos != NULL && s_tmsFileDown.ipPortPath[0] != '\0' && s_tmsFileDown.tfd_downLen > 0 && s_tmsFileDown.tfd_fileLen >= 0) {
		exeStatus = 4;
		s_tmsStatus = Download_File;
	}
	else {
		memset(&s_tmsFileDown, 0x00, sizeof(stTmsFileDown));
		s_tmsStatus = 0;
	}

	while (1) {//开始申请

		if (s_errTime > 20) {

			MSG_LOG("TMS下载错误次数过多:%d\n", s_errTime);
			sprintf((char *)buff, "TMS下载错误次数过多:%d", s_errTime);
			MessageBox(1, buff);
			delayxms(1000);
			break;
		}

		if (outdly % 200 == 0) {
			clr_wdt();
			cls();
			sprintf((char *)buff, "TMS应用更新(%d):%d", s_errTime, outdly / 100);
			display(0, 0, (const char *)buff, 0);
			//com_snd(COM4,len,buff);
		}

		gGprsinfo.gmissflag = MISS_G_TMS;
		//main_GPRS(NULL);
	//	main_card();
		if (gGprsinfo.GPRSLinkProcess == 20) {

			while (1)
			{
			//	main_card();
				delayxms(1);
				if (getkey(1) != 0)
				{
					return -1;
				}
#ifdef _debug_gprs00
				memset(disbuff, 0, sizeof(disbuff));
				sprintf((char *)disbuff, "T2:%04d", outtime - delay);
				miniDispstr(7, 30, (const char *)disbuff, 0);
#endif

				if (isNetOK[link] == 3)
				{
#ifdef _debug_gprs
					MSG_LOG("open time:%u\r\n", delay);
#endif

					continue;
				}
				delay = get_timer0(1);
				if (delay++ <= 0)
				{

					MSG_LOG("ProcessTmsData.while.if.while.if :%d", delay);
					//	gprsErrorTimes+=25;		//直接这样试试
					MSG_LOG("19==\r\n");
					gGPRS_data_style.openLink = link;
					//GPRSLinkProcess = 19;	//
					tcpipClose(link);
					gGprsinfo.GPRSLinkProcess = 19;
					break;
					//return -1;
				}

			}

		}
		if (gGprsinfo.GPRSLinkProcess < 21) {//已经连接
			if (flag != 1) {
				flag = 1;
				display(6, 0, "TMS正在拔号", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	
			clr_wdt();
			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == 0x2F) {
			if (flag != 1) {
				flag = 1;
				display(6, 0, "TMS正在重新连接", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);
				clr_wdt();
			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == TCPSTARTSTAT) {

		}
		if (gprsErrorTimes > 20) {

			gGprsinfo.GPRSLinkProcess = GPRS_HWPOWER_RESET;
		}

		if (isNetOK[LINK_PBOC_TMS] == 0)
		{
			goto ProcessTmsData_LOOP;
		}

		if (exeStatus == 0) {

			exeStatus =2;
			s_tmsStatus = Update_check;
			outdly = 0;
		}
		else if (s_tmsStatus == Tms_Over) {
			MSG_LOG("_tmsStatus == Tms_Ov\n");
			MessageBox(0, "TMS检查完成");
			beep(1, 200, 200);
			delayxs(1000);
			break;
		}

		else if (exeStatus == 2 && s_tmsStatus == Update_check) {

			

#if 1
			//"Guilin v1.0.1"
			jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, SYS_HEAD_STREn);//SYS_HEAD_STREn
			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_TIME_LOG, APPIndex);
			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"1\"}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"01000005\",\"01000002\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"04000055\",\"04000054\",\"04000008\",\"deptcode\",\"04000056\",\"04000051\",\"04000039\",\"04000038\",\"04000031\",\"04000030\",\"03000005\",\"04000030\",\"04000029\",\"04000028\",\"04000026\",\"04000023\",\"04000022\",\"04000017\",\"04000016\",\"04000015\",\"04000014\",\"04000013\",\"04000012\",\"04000007\",\"04000006\",\"04000003\",\"04000002\",\"04000001\",\"03000012\",\"02000002\",\"01000005\",\"01000002\",\"01000001\"]}}");

// 			jasonLen += sprintf(buff+ jasonLen, "\"APP2\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, "Guilin");//"Guilin"
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_bASE_LOG, APPIndex);
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\"}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"deptcode\",\"04000056\",\"04000051\",\"04000039\",\"04000038\",\"04000031\",\"04000030\",\"03000005\",\"04000030\",\"04000029\",\"04000028\",\"04000026\",\"04000023\",\"04000022\",\"04000017\",\"04000016\",\"04000015\",\"04000014\",\"04000013\",\"04000012\",\"04000007\",\"04000006\",\"04000003\",\"04000002\",\"04000001\",\"03000012\",\"02000002\",\"01000005\",\"01000002\",\"01000001\"]}}");
// //
// 			//"Guilin v1.0.1"
// 			jasonLen = sprintf(buff, "{\"APPCount\":\"2\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\"", DEV_TYPE, SYS_HEAD_STREn);
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_bASE_LOG, APPIndex);
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"1\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000003\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"01000005\",\"01000002\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]},");
// 		
// 			jasonLen += sprintf(buff+ jasonLen, "\"APP2\":{\"APPID\":\"SLZR_%s_%s\"", "ESF300L", "Guilin");
// 			jasonLen += sprintf(buff + jasonLen, ",\"APPVer\":\"1.0.%03X\",\"APPIndex\":\"%s\",\"APPState\":\"1\"", SOFT_VER_bASE_LOG, "2");
// 			jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\"}");
// 		//	jasonLen += sprintf(buff + jasonLen, ",\"ParameterUp\":{\"04000001\":\"2\",\"04000022\":\"L123\",\"04000023\":\"11223344\"}");
// 			jasonLen += sprintf(buff + jasonLen, ",\"Parameter\":[\"01000001\",\"04000030\",\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000031\",\"deptcode\"]}}");

#else
			jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_ESF300L_Guilin\",\"APPVer\":\"v1.0.0\",\"APPIndex\":\"0\",\"APPState\":\"1\",\"ParameterUp\":{\"04000001\":\"1\"},\"Parameter\":[\"03000005\",\"04000001\",\"04000022\",\"04000023\",\"04000029\",\"04000030\",\"04000031\",\"deptcode\"]}");
#endif

			ret = build_HTTP_qpboc_HEAD_2(s_tmsStatus, (char *)buff, jasonLen);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			outdly = 0;
		}
		else if (exeStatus == 3 && s_tmsStatus == Download_query) {
// 			cls();
// 			display(6, 0, "更新检查成功!", DIS_CENTER);
// 			//	MessageBox(0, "更新检查成功!");
// 			beep(1, 200, 200);
// 			delayxs(2);
// 			if(show_para_tms() !=ST_OK)  //显示参数
// 			{
// 				restore_flag=3;
// 				return ST_ERROR;
// 			}
				save_tms_para();	    //更新参数		
				Q_QPBOC_para_INIT();
				if(updata_need()==ST_ERROR)  //判断是否需要检查更新
			{
				restore_flag=3;
				break;
			}

		jasonLen = sprintf((char *)buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);
	//	jasonLen = sprintf((char *)buff+jasonLen, ",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}  ", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);
			
			ret = build_HTTP_qpboc_HEAD_2(s_tmsStatus, buff, jasonLen);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, buff);
			++exeStatus;
			outdly = 0;
		}
		else if (exeStatus == 4 && s_tmsStatus == Download_File) {
			cls();
			display(6, 0, "下载查询成功!", DIS_CENTER);
		//	MessageBox(0, "下载查询成功!");
			beep(1, 200, 200);
			delayxms(300);
			cls();
		
			//jasonLen = sprintf(buff, "{\"APPCount\":\"1\",\"APP1\":{\"APPID\":\"SLZR_%s_%s\",\"APPIndex\":\"%s\",\"APPVer\":\"%s\"}}", DEV_TYPE, SYS_HEAD_STREn, APPIndex, s_serverVer);

			//buildSeverInfo_06tlstims(0x26, 5, s_tmsFileDown.ipPortPath, s_tmsFileDown.tmsport);

			delayxms(1000);
			++exeStatus;
			outdly = 0;
		}
		else if (s_tmsStatus == Download_File && s_isDownloadSend == 0) {
			s_isDownloadSend = 1;
// #ifdef _debug_
// 			return ST_OK;
// #endif
			ret = build_HTTP_qpboc_HEAD_DOWN(s_tmsStatus, buff, 0);
			//s_tmsStatus = Key_activation;
			gprs_send_data(LINK_PBOC_TMS, ret, (unsigned char *)buff);
			++exeStatus;
			outdly = 0;
		}
		else {
			//MSG_LOG("s_tmsStatus,s_isDownloadSend:%02X,%d\n", s_tmsStatus, s_isDownloadSend);
		}

	ProcessTmsData_LOOP:
		delayxms(10);

		ret = getkey(1);
		if (ret == SLZRKEY_ESC) {
			//exeStatus = 0;
			MSG_LOG("TMSKEY_ESC\n");
			break;
		}
		if (outdly++ > 3000) {

			display(2, 0, "接收超时", DIS_CONVERT | DIS_CENTER);
			beep(3, 100, 30);
			delayxms(2000);
			
			break;
		}
	}

	set_QPBOC_IP_MODE(TMS_sign_shift,0xCD);
	MSG_LOG("TMS过程线束\n");
//	buildSeverInfo_06(0x26);
		if(IP_shift__stop==0xCD)  //切回轮询
	{
		set_QPBOC_IP_MODE(IP_ok_shift,0xCD);
	
	}
	restore_flag = 3;
		clr_wdt();
	return ST_OK;
}



///
typedef struct
{
	
	unsigned char tag[9];			//
	unsigned char val[65];

}PARA_TMS_tlv;


typedef struct
{
	PARA_TMS_tlv para_01000001;
	PARA_TMS_tlv para_01000002;
	PARA_TMS_tlv para_01000005;
//	PARA_TMS_tlv para_02000001;
	PARA_TMS_tlv para_02000002;
	PARA_TMS_tlv para_03000012;
	PARA_TMS_tlv para_04000001;
	PARA_TMS_tlv para_04000002;
	PARA_TMS_tlv para_04000003;
	PARA_TMS_tlv para_04000006;
	PARA_TMS_tlv para_04000007;
	PARA_TMS_tlv para_04000008;
	PARA_TMS_tlv para_04000012;
	PARA_TMS_tlv para_04000013;
	PARA_TMS_tlv para_04000014;
	PARA_TMS_tlv para_04000015;
	PARA_TMS_tlv para_04000016;
	PARA_TMS_tlv para_04000017;
	PARA_TMS_tlv para_04000022;
	PARA_TMS_tlv para_04000023;
	PARA_TMS_tlv para_04000026;
	PARA_TMS_tlv para_04000028;
	PARA_TMS_tlv para_04000029;
	PARA_TMS_tlv para_04000030;
	PARA_TMS_tlv para_04000031;
	PARA_TMS_tlv para_04000038;
	PARA_TMS_tlv para_04000039;
	PARA_TMS_tlv para_04000051;
	PARA_TMS_tlv para_04000054;
	PARA_TMS_tlv para_04000055;
	PARA_TMS_tlv para_04000056;
	PARA_TMS_tlv para_deptcode;
}PARA_TMS;

PARA_TMS TMS_para;

#define D_para_01000001  "01000001"
#define D_para_01000002  "01000002"
#define D_para_01000005  "01000005"
#define D_para_02000001  "02000001"
#define D_para_02000002  "02000002" //add
#define D_para_03000012  "03000012"
#define D_para_04000001  "04000001"
#define D_para_04000002  "04000002"
#define D_para_04000003  "04000003"
#define D_para_04000006  "04000006"
#define D_para_04000007  "04000007"
#define D_para_04000008  "04000008" //add2
#define D_para_04000012  "04000012"
#define D_para_04000013  "04000013"
#define D_para_04000014	 "04000014"
#define D_para_04000015  "04000015"
#define D_para_04000016  "04000016"
#define D_para_04000017  "04000017"
#define D_para_04000022  "04000022"
#define D_para_04000023  "04000023"
#define D_para_04000026  "04000026"
#define D_para_04000028  "04000028" //add
#define D_para_04000029  "04000029"
#define D_para_04000030  "04000030"
#define D_para_04000031  "04000031"
#define D_para_04000038  "04000038"
#define D_para_04000039  "04000039"
#define D_para_04000051  "04000051"
#define D_para_04000054  "04000054" //add2
#define D_para_04000055  "04000055"//add2
#define D_para_04000056  "04000056"
#define D_para_deptcode  "deptcode"

void TMS_PARA_INIT(void)
{
	memset((unsigned char *)&TMS_para,0,sizeof(PARA_TMS));
	strcat((char *)TMS_para.para_01000001.tag,D_para_01000001);
	strcat((char *)TMS_para.para_01000002.tag,D_para_01000002);
	strcat((char *)TMS_para.para_01000005.tag,D_para_01000005);
//	strcat((char *)TMS_para.para_02000001.tag,D_para_02000001);
	strcat((char *)TMS_para.para_02000002.tag,D_para_02000002);
	strcat((char *)TMS_para.para_03000012.tag,D_para_03000012);
	strcat((char *)TMS_para.para_04000001.tag,D_para_04000001);
	strcat((char *)TMS_para.para_04000002.tag,D_para_04000002);
	strcat((char *)TMS_para.para_04000003.tag,D_para_04000003);
	strcat((char *)TMS_para.para_04000006.tag,D_para_04000006);
	strcat((char *)TMS_para.para_04000007.tag,D_para_04000007);
	strcat((char *)TMS_para.para_04000008.tag,D_para_04000008);
	strcat((char *)TMS_para.para_04000012.tag,D_para_04000012);
	strcat((char *)TMS_para.para_04000013.tag,D_para_04000013);
	strcat((char *)TMS_para.para_04000014.tag,D_para_04000014);
	strcat((char *)TMS_para.para_04000015.tag,D_para_04000015);
	strcat((char *)TMS_para.para_04000016.tag,D_para_04000016);
	strcat((char *)TMS_para.para_04000017.tag,D_para_04000017);
	strcat((char *)TMS_para.para_04000022.tag,D_para_04000022);
	strcat((char *)TMS_para.para_04000023.tag,D_para_04000023);
	strcat((char *)TMS_para.para_04000026.tag,D_para_04000026);
	strcat((char *)TMS_para.para_04000028.tag,D_para_04000028);
	strcat((char *)TMS_para.para_04000029.tag,D_para_04000029);
	strcat((char *)TMS_para.para_04000030.tag,D_para_04000030);
	strcat((char *)TMS_para.para_04000031.tag,D_para_04000031);
	strcat((char *)TMS_para.para_04000038.tag,D_para_04000038);
	strcat((char *)TMS_para.para_04000039.tag,D_para_04000039);
	strcat((char *)TMS_para.para_04000051.tag,D_para_04000051);
	strcat((char *)TMS_para.para_04000054.tag,D_para_04000054);
	strcat((char *)TMS_para.para_04000055.tag,D_para_04000055);
	strcat((char *)TMS_para.para_04000056.tag,D_para_04000056);
	strcat((char *)TMS_para.para_deptcode.tag,D_para_deptcode);

}

unsigned char deal_para( char * data,unsigned int len)
{
	unsigned int i,j, para_len=0;
	unsigned int time_tlv=0;
	char *pTmpCh = NULL;
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char buff_test[10];
	memset(buff_test,0,sizeof(buff_test));
	time_tlv=sizeof(PARA_TMS_tlv);
	para_len=sizeof(PARA_TMS)/time_tlv;

	TMS_PARA_INIT();
	for(i=0;i<para_len;i++)
	{
		pTmpCh =strstr(data,(const char *)TMS_para.para_01000001.tag+(i*time_tlv));
		MSG_LOG("结构图==数据%s\n",TMS_para.para_01000001.tag+(i*time_tlv));
		if (pTmpCh == NULL) {
			MSG_LOG("JSON数据不存在\n");
			//return ST_ERROR;
			continue;
		}
		else
		{
			memcpy(buff_test,pTmpCh,8);
			MSG_LOG("数据%s\n",buff_test);
			p3=pTmpCh;
			for(j=0;j<3;j++)
			{
				p1=strstr(p3,"\"");
				p3=p1+1;
				if(j==1)
				{
					p2=p1;
				}
			
			}
// 			p1=strstr(pTmpCh,"\"");
// 			pTmpCh=strstr(p1+1,"\"");
// 			p2=strstr(p1+1,"\"");
			MSG_LOG("长度=%d\n",(p1-p2-1));
		
	
			memcpy(TMS_para.para_01000001.val+(i*time_tlv),p2+1,p1-p2-1);
			MSG_LOG("数据域=%s\n",TMS_para.para_01000001.val+(i*time_tlv));
		}

	}
return ST_OK;
	
}

unsigned char switch_IP2INT(unsigned char mode, unsigned char *dataip)
{
    char buffer[16];
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;


	
	memset(buffer,0,sizeof(buffer));

	p1 =strstr((char *)dataip,".");
	p2=strstr(p1+1,".");
	p3=strstr(p2+1,".");

	buffer[0]=Ascii2INT(dataip,(unsigned char)(p1-(char *)dataip));
	buffer[1] = Ascii2INT((unsigned char *)p1+1,(p2-p1-1));
	buffer[2] = Ascii2INT((unsigned char *)p2+1,(p3-p2-1));
	buffer[3] = Ascii2INT((unsigned char *)p3+1,strlen((const char *)(p3+1)));
//	MSG_LOG("p0==%p\r\n",dataip);
//	MSG_LOG("p1==%p\r\n",p1);
//	MSG_LOG("p2==%p\r\n",p2);
///	MSG_LOG("p3==%p\r\n",p3);

	MSG_LOG("ip==\r\n");
	BCD_LOG((unsigned char *)buffer,4,1);
	saveMobileParameter(mode, (unsigned char *)buffer);
	return ST_OK;
}



void GET_string_IP(unsigned char *dataip)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	unsigned char buffer[20];

		p1 =strstr((char *)dataip,"|");
	p2=strstr(p1+1,"|");
	p3=strstr(p2+1,"|");

	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,p1+1,(p2-p1-1));
	switch_IP2INT(3,buffer);
	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,p2+1,(p3-p2-1));
	switch_IP2INT(23,buffer);
//	return ST_OK;

}
void GET_string_int(unsigned char *dataip)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	unsigned char buffer[20];
	unsigned int U_temp=0;
	p1 =strstr((char *)dataip,"|");
	p2=strstr(p1+1,"|");
	p3=strstr(p2+1,"|");

	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,p1+1,(p2-p1-1));
	U_temp=Ascii2INT(buffer,strlen((const char * )buffer));
	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,p2+1,(p3-p2-1));
	U_temp=Ascii2INT(buffer,strlen((const char * )buffer));
	saveMobileParameter(21,(unsigned char *)&U_temp);
//	return ST_OK;

}
extern unsigned int BCD2HEX(unsigned char *bcd, unsigned char len);
extern unsigned char DriveCardNo[4];
void save_tms_para(void)
{
	unsigned char U_time=0;
	unsigned int U_temp=0;
	unsigned char buffer[10];
	unsigned char buff[24];
	//线路
	if(memcmp(TMS_para.para_04000013.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		Ascii2BCD(TMS_para.para_04000013.val, pFistVary.LineNo, 4);
		sysfewrite(BIT_LINENO, 3, pFistVary.LineNo);
		sysfewrite(BIT_GPSChildLine, 2, pFistVary.LineNo);//刚开始时，子线路和主线路一致
		savesysbupara();
		//	memcpy(NewPriceTab.lineVer, "\x00\x00", 2);
		saveCpuInfo(1, buffer);
	}
	if(memcmp(TMS_para.para_02000002.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		// 		第一位：交易模式，0联机、1脱机ODA、2脱联机，默认1
		// 		第二位：扫码开关，0关闭、1开启，默认1
		// 		第三位：提示音开关，0关闭，1开启，默认1
		// 		第四位：预留
		switch(TMS_para.para_02000002.val[0])
		{
		case 0x30:
			online_1();
			break;
		case 0x31:
			online_2();
			break;
		case 0x32:
			online_3();
			break;
		default :
			break;
		}
		switch(TMS_para.para_02000002.val[1])
		{
		case 0x30:
			DisableAliPay(); 
			DisableWeiXin();
			
			Disableqpoc(); 
			break;
		case 0x31:
			EnableWeiXin() ;
			EnableAliPay() ;
			Enableqpoc();
			break;
			
		default :
			break;
		}
		
	}
	if(memcmp(TMS_para.para_04000014.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		memset(buffer,0,sizeof(buffer));// 		switch(TMS_para.para_02000002.val[2])
		// 		{
		// 		case 0x30:
		// 			break;
		// 		case 0x31:
		// 			break;
		// 			
		// 		default :
		// 			break;
		// 		}
#if 0
		memcpy(pFistVary.DeviceNo,TMS_para.para_04000014.val, 9);
		memcpy(buffer, pFistVary.DeviceNo, 9);
		sysfewrite(BIT_DEVICE_NO, 9, pFistVary.DeviceNo);
		sysferead(BIT_DEVICE_NO, 9, buffer);
		savesysbupara();
		saveCpuInfo(2, buffer);
#endif
	}
	if(memcmp(TMS_para.para_01000001.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		saveMobileParameter(1, TMS_para.para_01000001.val);
	}
	if(memcmp(TMS_para.para_01000005.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		saveMobileParameter(6, TMS_para.para_01000005.val);
	}
	if(memcmp(TMS_para.para_04000002.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		Ascii2BCD(TMS_para.para_04000002.val,buffer,10);
		saveMobileParameter(5, buffer);
	}
	if(memcmp(TMS_para.para_04000003.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		
		U_time=Ascii2INT(TMS_para.para_04000003.val,2);
		MSG_LOG("保存=%d\r\n",U_time);
		saveMobileParameter(14,(unsigned char *)&U_time);
		Q_QPBOC_para.shuangmian_OVER_TIME=get_over_time_shuang();
	}
	if(memcmp(TMS_para.para_04000006.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0) //ip
	{
		switch_IP2INT(24,(unsigned char *)TMS_para.para_04000006.val);
	}
	if(memcmp(TMS_para.para_04000007.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		U_temp=Ascii2INT(TMS_para.para_04000007.val,strlen((const char * )TMS_para.para_04000007.val));
		
		MSG_LOG("保存=%d\r\n",U_temp);
		saveMobileParameter(25, (unsigned char *)&U_temp);
	}
	if(memcmp(TMS_para.para_04000008.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0) //ip
	{
		switch_IP2INT(26,(unsigned char *)TMS_para.para_04000008.val);
	}
	if(memcmp(TMS_para.para_04000051.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		
		U_time=Ascii2INT(TMS_para.para_04000051.val,1);
		//U_time=TMS_para.para_04000051.val-0x30;
		MSG_LOG("保存=%d\r\n",U_time);
		saveMobileParameter(15,(unsigned char *)&U_time);
		Q_QPBOC_para.chongzheng_OVER_TIME=get_over_time_re();
	}
	if(memcmp(TMS_para.para_04000038.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		U_temp=Ascii2INT(TMS_para.para_04000038.val,strlen((const char * )TMS_para.para_04000038.val));
		//Ascii2INT()
		//U_temp=	BCD2HEX("\x12\x34\x56",3);
		sysfewrite(BIT_DRICE_CARD, 4, (unsigned char *)&U_temp);
		memcpy(DriveCardNo, (unsigned char *)&U_temp, 4);
		MSG_LOG("保存xjjj=%d\r\n",U_temp);
		
		//	sprintf(disbuf[index], "司机号:%08d", i);
	}
	if(memcmp(TMS_para.para_04000026.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		if(TMS_para.para_04000026.val[0]==0x31)
		{
			set_TMS_switch_control(27,0x31);
			MSG_LOG("开启\r\n");
			memset(buff,0,sizeof(buff));
			strcat((char *)buff,(const char *)TMS_para.para_04000015.val);
			saveMobileParameter(19,buff);
		}
		else
		{
			set_TMS_switch_control(27,0x30);
			MSG_LOG("关闭\r\n");
		}
	}
	if(memcmp(TMS_para.para_04000028.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		if(TMS_para.para_04000028.val[0]==0x31)
		{
			set_TMS_switch_control(22,0x31);
			MSG_LOG("dns开启\r\n");
			
		}
		else
		{
			set_TMS_switch_control(22,0x30);
			MSG_LOG("dns关闭\r\n");
		}
	}
	if(memcmp(TMS_para.para_04000029.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		memset(buff,0,sizeof(buff));
		strcat((char *)buff,(const char *)TMS_para.para_04000029.val);
		saveMobileParameter(12, buff);
	}
	if(memcmp(TMS_para.para_04000031.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		memset(buff,0,sizeof(buff));
		strcat((char *)buff,(const char *)TMS_para.para_04000031.val);
		saveMobileParameter(13, buff);
	}
	if(memcmp(TMS_para.para_04000054.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		memset(buff,0,sizeof(buff));
		strcat((char *)buff,(const char *)TMS_para.para_04000054.val);
		saveMobileParameter(20, buff);
	}
	if(memcmp(TMS_para.para_04000030.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		U_temp=Ascii2INT(TMS_para.para_04000030.val,strlen((const char * )TMS_para.para_04000030.val));
		
		
		MSG_LOG("保存=%d\r\n",U_temp);
		saveMobileParameter(4, (unsigned char *)&U_temp);
	}
	if(memcmp(TMS_para.para_04000055.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		
		GET_string_int(TMS_para.para_04000055.val);
	}
		if(memcmp(TMS_para.para_04000056.val,"\x00\x00\x00\x00\x00\x00\x00",6)!=0)
	{
		
		GET_string_IP(TMS_para.para_04000056.val);
		MSG_LOG("555 保存\r\n");
	
	}

	cls();
	display(3,3,"设置成功",DIS_CENTER);
	delayxms(1500);

}



volatile unsigned char ucMENU_NUM_tms = 0;
unsigned int gCUM_PER_SCREEN_TMS = 14;
int MenuFrame_tms(char menu[][20], void (*function[])(void), char *titlestr, unsigned char ucMENU_NUM, unsigned int dlywaite)
{
#define LINE_START				1							//首行菜单位置


	unsigned char i,j,ucRnt,		//
		ucLastScrLineNum,			//最后一屏的数目
		ucScrNum,					//菜单的总屏数
		ucCurScr,					//当前屏
		ucCurLine;					//当前行
	unsigned char ucShowMargin;
	char disbuf[20];
	unsigned int ulDelay, MENU_NUM_PER_SCREEN;
	
	ucCurLine=0;
	ucCurScr=0;
	ulDelay=(dlywaite&0x00FFFFFF);;

	MENU_NUM_PER_SCREEN = (gCUM_PER_SCREEN_TMS/2)-1;

	ucShowMargin = 1;
	ucScrNum=ucMENU_NUM / MENU_NUM_PER_SCREEN;
	ucLastScrLineNum = ucMENU_NUM % MENU_NUM_PER_SCREEN;
	if(ucLastScrLineNum)
		ucScrNum++;
	else
		ucLastScrLineNum=MENU_NUM_PER_SCREEN;				//最后一屏的行数
	
	cls();
	while(1)
	{
		if(ucShowMargin)
		{
			display(0, 0, titlestr, DIS_CENTER|DIS_Cls);
			ucShowMargin=0;
		}
		//显示整屏信息
		for(i=0; i<MENU_NUM_PER_SCREEN; i++)
		{
			j=ucCurScr*MENU_NUM_PER_SCREEN+i;
			if( j>(ucMENU_NUM-1) )
			{
				display((LINE_START+i)*2, 0, "                ", 0);
				continue;
			}
			memset(disbuf,0,sizeof(disbuf));
			sprintf(disbuf, "%d.", j+1);
			strcat(disbuf, menu[j]);
			disbuf[16] = 0;
			if(i==ucCurLine){
				display((LINE_START+i)*2, 0, disbuf, DIS_CONVERT);
			}
			else{
				display((LINE_START+i)*2, 0, disbuf, 0);
			}
		}
		
		//按键 超时判断
		while(1)
		{
			delayxms(50);
			ucRnt=getkey(1);
			ulDelay--;
			if(ulDelay==0)
				return -1;
			if(ucRnt!=0){
				if((ucRnt >= '1')&&(ucRnt<=(ucMENU_NUM+'0'))){
					i = ucRnt-'1';
					ucCurScr = i/MENU_NUM_PER_SCREEN;
					ucCurLine = i%MENU_NUM_PER_SCREEN;
					ucRnt = SLZRKEY_ENTER;
				}
				break;
			}
		}
		ulDelay = (dlywaite&0x00FFFFFF);;
		//按键处理
		switch(ucRnt)
		{
// 		case KEY_UP:
// 		if(ucCurLine==0)
// 			{
// 				if(ucCurScr==0)
// 				{
// 					ucCurScr=ucScrNum-1;
// 					//					if(ucCurScr!=0)
// 					ucCurLine=ucLastScrLineNum-1;
// 				}
// 				else{
// 					ucCurScr--;
// 					ucCurLine = MENU_NUM_PER_SCREEN-1;
// 				}
// 			}
// 			else
// 				ucCurLine--;
// 			break;
	case SLZRKEY_UP:
		case SLZRKEY_DOWN:
			if(ucCurScr == (ucScrNum-1))
			{
				if(ucCurLine == (ucLastScrLineNum-1))
				{
						cls();
					display(0, 0, titlestr, DIS_CENTER|DIS_Cls);
					ucCurLine = 0;
					ucCurScr = 0;
					break;
				}
			}
			ucCurLine++;
			if(ucCurLine < MENU_NUM_PER_SCREEN)
				break;
			cls();
			display(0, 0, titlestr, DIS_CENTER|DIS_Cls);
			ucCurLine=0;
			ucCurScr++;
			if(ucCurScr < ucScrNum)
				break;
			ucCurScr=0;
			break;
		case SLZRKEY_ESC:
	
			return -2;
		case SLZRKEY_ENTER:

			ucMENU_NUM_tms=ucCurScr*MENU_NUM_PER_SCREEN+ucCurLine;
			MSG_LOG("TMS参数菜单索引=%d\r\n",ucMENU_NUM_tms);
			function[ucCurScr*MENU_NUM_PER_SCREEN+ucCurLine]();
			ucShowMargin=1;
			if((dlywaite&0xFF000000) != 0)
				return 0;
			break;
		default:
			break;
		}
	}	
//	return;
}

 void SHOW_tms_(void)
 {
//	unsigned int para_len=0;
	unsigned int time_tlv=0;
	time_tlv=sizeof(PARA_TMS_tlv);
//	para_len=sizeof(PARA_TMS)/time_tlv;
	cls();

	//display(3,3,"没有",DIS_CENTER);
	MSG_LOG("显示 TMS参数菜单索引=%d\r\n",ucMENU_NUM_tms);
	switch(ucMENU_NUM_tms)
	{
	case 0:
		display(3,3,"商户号",DIS_CENTER);
		break;
	case 1:
		display(3,3,"中文商户名",DIS_CENTER);
		break;
	case 2:
		display(3,3,"终端号",DIS_CENTER);
		break;
	case 3:
		display(3,3,"功能开关",DIS_CENTER);
		break;
	case 4:
		display(3,3,"最大交易比数",DIS_CENTER);
		break;
	case 5:
		display(3,3,"通讯方式",DIS_CENTER);
		break;
	case 6:
		display(3,3,"TPDU",DIS_CENTER);
		break;
	case 7:
		display(3,3,"通讯超时时间",DIS_CENTER);
		break;
	case 8:
		display(3,3,"主机IP",DIS_CENTER);
		break;
	case 9:
		display(3,3,"主机端口",DIS_CENTER);
		break;
	case 10:
		display(3,3,"电话一/单位编号",DIS_CENTER);
		break;
	case 11:
		display(3,3,"电话二/线路号",DIS_CENTER);
		break;
	case 12:
		display(3,3,"电话三/车辆号",DIS_CENTER);
		break;
	case 13:
		display(3,3,"GPRS业务参数",DIS_CENTER);
		break;
	case 14:
		display(3,3,"GPRS、CDMA用户名",DIS_CENTER);
		break;
	case 15:
		display(3,3,"GPRS、CDMA密码",DIS_CENTER);
		break;
	case 16:
		display(3,3,"WIFI用户名",DIS_CENTER);
		break;
	case 17:
		display(3,3,"WIFI密码",DIS_CENTER);
		break;
	case 18:
		display(3,3,"是否公网接入",DIS_CENTER);
		break;
	case 19:
		display(3,3,"是否使用DNS",DIS_CENTER);
		break;
	case 20:
		display(3,3,"主机域名",DIS_CENTER);
		break;
	case 21:
		display(3,3,"主机端口",DIS_CENTER);
		break;
	case 22:
		display(3,3,"备份主机域名1",DIS_CENTER);
		break;
	case 23:
		display(3,3,"司机编号",DIS_CENTER);
			break;
	case 24:
		display(3,3,"公交公司参数（预留）",DIS_CENTER);
		break;
	case 25:
		display(3,3,"重刷超时时间",DIS_CENTER);
		break;
	case 26:
		display(3,3,"公网IP通讯参数",DIS_CENTER);
		break;
	case 27:
		display(3,3,"机构号",DIS_CENTER);
		break;
	default:
		display(3,3,"其他",DIS_CENTER);
		break;
	}
	display(6,3,(const char *)TMS_para.para_01000001.val+(ucMENU_NUM_tms*time_tlv),DIS_CENTER);
	delayxs(2);
	

 }

unsigned char  show_para_tms(void)
{
	unsigned char ucMENU_NUM=0;
	char menu[35][20];
	void(*function[35])(void);

	unsigned int i = 0,DELAY_TIME=0;;

	for (i = 0; i < 35; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}

	unsigned char buff[64];
	unsigned int para_len=0;
	unsigned int time_tlv=0;
	time_tlv=sizeof(PARA_TMS_tlv);
	para_len=sizeof(PARA_TMS)/time_tlv;

	memset(buff,0,sizeof(buff));
	memcpy(buff, "APP2:",5);
	sprintf((char *)buff+5, "V%s",s_serverVer);
	ucMENU_NUM = 0;
// 	strcpy(menu[ucMENU_NUM],TMS_para.para_01000001.tag+(i*time_tlv));
// 	function[ucMENU_NUM++] = SHOW_tms_;
	for(i=0;i<para_len;i++)
	{
		if(memcmp(TMS_para.para_01000001.val+(i*time_tlv),"\x00\x00\x00\x00\x00\x00",6)==0 )
		{
			MSG_LOG("不存在\n");
			//return ST_ERROR;
			continue;
		}
		strcpy(menu[ucMENU_NUM],(const char *)TMS_para.para_01000001.tag+(i*time_tlv));
		function[ucMENU_NUM++] = SHOW_tms_;
	}
	
	MenuFrame_tms(menu, function,(char *) buff, ucMENU_NUM, (100));
	cls();
//	display(0, 0, "恢复到出厂状态", DIS_CENTER | DIS_CONVERT);
	display(2, 1, "更新参数!",DIS_CENTER);
	display(6, 0, "确定吗?", DIS_CENTER);
	while (1) {
		i = getkey(1);
		if (i == SLZRKEY_ENTER)
		{
			return ST_OK;
		}
		else if (i == SLZRKEY_ESC)
		{
			return ST_ERROR;
		}
		delayxms(30);
		DELAY_TIME++;
		if(DELAY_TIME>20)
		{
			return ST_OK;
		}
	}
	
}






void set_TMS_switch_control(unsigned char mode, unsigned char data) //
{	
	
	saveMobileParameter(mode,(unsigned char *)&data);
}

unsigned char get_switch_control(unsigned char mode)
{
	unsigned char data;
	getMobileParameter(mode,(unsigned char *)&data);
	return data;
}




unsigned char updata_need(void)
{
	unsigned char buff_up[15];
	unsigned char len=0;
	unsigned char disbuf[30];
	memset(buff_up,0,sizeof(buff_up));
	memset(disbuf,0,sizeof(disbuf));
	cls();
	
	sprintf((char *)buff_up , "1.0.%03x", SOFT_VER_TIME_LOG);
	len=strlen((const char *)&buff_up);
	if(len>10)len=10;
	
	cls();
	sprintf((char *)disbuf , "后台版本: %s", s_serverVer);
	display(3, 0, (char *)disbuf, DIS_CENTER);
	sprintf((char *)disbuf , "终端版本: 1.0.%03x", SOFT_VER_TIME_LOG);
	display(6, 0, (char *)disbuf, DIS_CENTER);
	delayxs(2);
	if(memcmp(buff_up,s_serverVer,len)==0)
	{
		cls();
		display(6, 0, "不需要下载!", DIS_CENTER);	display(6, 0, "不需要下载!", DIS_CENTER);
		//	MessageBox(0, "更新检查成功!");
		beep(1, 200, 200);
		delayxms(200);
		return ST_ERROR;
	}
	else
	{
		cls();
	
		display(6, 0, "需要下载!", DIS_CENTER);
		//	MessageBox(0, "更新检查成功!");
		beep(1, 200, 200);
	delayxms(200);
		return ST_OK;
	}
}

//
typedef struct
{
	unsigned char TMS_sign;//tms标志 ,开始 0x00  结束0xab
	unsigned char IP_MODE;	//

	unsigned char IP_wait_time;
	unsigned char IP_ok; //ip切换  开始切换：0xcd  切换结束 0xab
	unsigned char Ip_frist_sign;
	unsigned char RES[10];

}QPBOC_IP_MODE;
QPBOC_IP_MODE ipmode;


void QPBOC_IP_MODE_INIT(void)
{
	memset((unsigned char *)&ipmode,0,sizeof(QPBOC_IP_MODE));
	ipmode.IP_MODE=QPBOC_IP_MODE_FRIST;  //开机默认值
}
void set_QPBOC_IP_MODE(unsigned char member ,unsigned char mode)
{
	unsigned char data=0;
	
	data=mode;

	memcpy((unsigned char *)&ipmode+member,(unsigned char *)&data,1);
// 	MSG_LOG("set_QPBOC_IP_MODE-ipmodde=%d\r\n",ipmode.IP_ok);
// 	MSG_LOG("set_QPBOC_IP_MODE-TMS_sign=%d\r\n",ipmode.TMS_sign);
// 	MSG_LOG("set_QPBOC_IP_MODE-IP_wait_time=%d\r\n",ipmode.IP_wait_time);
// 	MSG_LOG("set_QPBOC_IP_MODE-IP_MODE=%d\r\n",ipmode.IP_MODE);

}
unsigned char Get_QPBOC_IP_MODE(unsigned char member)
{
	unsigned char data=0;
	memcpy((unsigned char *)&data,(unsigned char *)&ipmode+member,1);
	return data;
}
//12 13 20 3 23
unsigned char  ip_test[4];
void switch_ip_time(unsigned char mode)
{
	unsigned int port;
	//unsigned char bu[4];
//	MSG_LOG("set_QPBOC_IP_MODE-ipmodde=%d\r\n",ipmode.IP_MODE);
	if(	Q_QPBOC_para.switch_control_SP!=0x31) //测试)
	{
		return;
	}
	if(ipmode.IP_ok !=0xCD || ipmode.Ip_frist_sign !=0xCD)
		return ;
	if(mode==0)
	{	
//	MSG_LOG("失败次数+++==%d\r\n",ipmode.IP_wait_time);
		ipmode.IP_wait_time++;
	}
	else 
	{	
	MSG_LOG("链接成功==%d\r\n",ipmode.IP_MODE);

	if(ipmode.IP_MODE!=3&&ipmode.IP_MODE!=23)	
	{
		port=10004;
//		SysTime.seconds;
		ip_test[0]=122;
		ip_test[1]=86+SysTime.seconds;
		ip_test[2]=116;
		ip_test[3]=100+SysTime.seconds;	

		MSG_LOG("域名得到的ip:%d.%d.%d.%d\n", ip_test[0], ip_test[1], ip_test[2], ip_test[3]);
		MSG_LOG("端口:%d\n", port);
	}
		if(Get_QPBOC_IP_MODE(IP_ok_shift)==0xCD)  //切回普通模式
		{
		//	MSG_LOG("切换轮询模式!\r\n");
			set_QPBOC_IP_MODE(IP_ok_shift,0xAB);
			isNetOK[1]=0;
			buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));
		}
	}



	if(	ipmode.IP_wait_time>6)
	{
	
		switch(ipmode.IP_MODE)
		{
		case 12:
		//	delayxms(3);
			ipmode.IP_MODE=13;
			ipmode.IP_wait_time=0;
			//	MSG_LOG("szzzzzzzzzzzzzzzzzz==%d\r\n",ipmode.IP_MODE);
			 buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));	
			break;
	
	
	
		default :
		//	ipmode.IP_MODE=12;
			break;
		}
/*		 buildSeverInfo_06(0x26);*/
	
	//	 delayxms(3);
	}

	if(ipmode.IP_wait_time>2)
	{	switch(ipmode.IP_MODE)
	{
			case 13:
			ipmode.IP_MODE=20;
			ipmode.IP_wait_time=0;
			 buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));	
			break;
			case 20:
				ipmode.IP_MODE=3;
				ipmode.IP_wait_time=0;
				ipmode.IP_wait_time=0;
				buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));
				break;
			case 3 :
			//	delayxs(3);
				ipmode.IP_MODE=23;
				ipmode.IP_wait_time=0;
				buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));
				break;
		case 23:
		//	MSG_LOG("五个都不行\r\n");
				ipmode.IP_wait_time=0;
				buildSeverInfo_06tls(0x26, 5,0xff);
			break;
		default :
		//	ipmode.IP_MODE=12;
			break;

		}
	
	}
}
//
void show_dns_ip(void)
{
	unsigned int port = 0;
	unsigned char bu[4];
	if(	Q_QPBOC_para.switch_control_SP!=0x31) //测试)
	{
		getMobileParameter(ipmode.IP_MODE, (unsigned char *)bu);
		getMobileParameter(25, (unsigned char *)&port);

		MSG_LOG("\n哪一个ip:%d\n", ipmode.IP_MODE);
		MSG_LOG("\nbuildSeverInfo_06 start1:%d.%d.%d.%d\n", bu[0], bu[1], bu[2], bu[3]);
		MSG_LOG("buildSeverInfo_06 start2:%d\n", port);
		return;
	}
//		MSG_LOG("set_QPBOC_IP_MODE-ipmodde=%d\r\n",ipmode.IP_MODE);
//		MSG_LOG("set_QPBOC_IP_MODE-ipmodde=%d\r\n",ipmode.Ip_frist_sign);

	if(ipmode.Ip_frist_sign==0xcd)
		return;
		
	if(ipmode.IP_MODE!=3&&ipmode.IP_MODE!=23)	
	{
		
			MSG_LOG("哪一个ip:%d\n", ipmode.IP_MODE);
			MSG_LOG("buildSeverInfo_06 start12:%d\n", 5);
			port=10004;
// 			bu[0]=122;
// 			bu[1]=226;
// 			bu[2]=116;
// 			bu[3]=137;	
			MSG_LOG("buildSeverInfo_06 start1:%d.%d.%d.%d\n", ip_test[0], ip_test[1], ip_test[2], ip_test[3]);
			MSG_LOG("buildSeverInfo_06 start2:%d\n", port);
	}
	else
	{
		getMobileParameter(ipmode.IP_MODE, (unsigned char *)bu);
		getMobileParameter(4, (unsigned char *)&port);

		MSG_LOG("\n哪一个ip:%d\n", ipmode.IP_MODE);
		MSG_LOG("\nbuildSeverInfo_06 start1:%d.%d.%d.%d\n", bu[0], bu[1], bu[2], bu[3]);
		MSG_LOG("buildSeverInfo_06 start2:%d\n", port);
		
		
	}
}
//
unsigned char para_buff[2000];
int QPBOC_TMS_DataDeal(char *pData, int datlen) {

#define TMS_HTTP_HEAD
#define COUNT_ST_JSON	20
	unsigned char jsonChs[3000];
	stJSON stJsons[COUNT_ST_JSON];
	stJSON *pTmpJson;
	int jsonNum = 0;
	int ret;
	unsigned char kek1[16];
	unsigned char kek2[16];
#ifdef _debug_
	int tmpI;
	//	unsigned char jsonChs1[500];
#endif
	unsigned int valUInt32 = 0;

	char *pTmpCh = NULL;
#ifdef TMS_HTTP_HEAD
	char * start_p = NULL;
	//	char *ret_p = NULL;
#endif
	int i, j;
	stTmsFileDown *pTmsFileDown = &s_tmsFileDown;
	unsigned int addr;
	unsigned char *buftemp = jsonChs + 1500;

	if (datlen < 10) {
		MSG_LOG("QPBOC_TMS_DataDeal发送失败\n");
		BCD_LOG((unsigned char *)pData, datlen, 1);
		if (s_tmsStatus == Download_File) {
			++s_errTime;
			goto DOWN_CONTINUE_ERROR;
		}
		return -1;
	}

	pData += 3;
	MSG_LOG("TMSQPBOC_TMS_DataDeal(%d):%s\n", datlen, pData);
	//BCD_LOG(pData, 10, 1);

#ifdef TMS_HTTP_HEAD
	/*
	HTTP/1.1 500 Internal Server Error
	Allow: POST, PUT
	Content-Type: x-ISO-TPDU/x-auth
	Content-Length: 0
	Server:                       Access-Guard-1000-Software/1.0
	Connection: close

	xxxxxxxxx
	*/

	start_p = NULL;
	start_p = NULL;

	start_p = strstr(pData, "\r\n\r\n");		//之后就是数据了
	if (start_p == NULL) {
		MSG_LOG("HTTP头与数据间隔行错误\n");
		if (s_tmsStatus == Download_File) {
			++s_errTime;
			if (datlen == LENUNIT_DOWN || datlen == (pTmsFileDown->tfd_fileLen - pTmsFileDown->tfd_downLen)) {
				memcpy(jsonChs, pData, datlen);
				goto DOWNFILE_WRITE;
			}
			goto DOWN_CONTINUE_ERROR;
		}
		return -1;
	}
	start_p[0] = '\0';	// 截断HTTP头与数据
	start_p += 4;

	if (s_tmsStatus == Download_File) {
		MSG_LOG("解析HTTP头:%d, %d, %d\n", start_p, pData, datlen);
		datlen -= start_p - pData;	// 1,2,3,4,5,6
		memcpy(jsonChs, start_p, datlen);
		jsonChs[datlen] = '\0';
	}
	else {
		pTmpCh = strstr(start_p, "{");		//之后就是数据了
		if (pTmpCh == NULL) {
			MSG_LOG("JSON数据不存在\n");
			return -1;
		}
		else {
			start_p = pTmpCh;

			MSG_LOG("解析HTTP头:%d, %d, %d\n", start_p, pData, datlen);
			datlen -= start_p - pData;	// 1,2,3,4,5,6
			memcpy(jsonChs, start_p, datlen);
			jsonChs[datlen] = '\0';

			jsonNum = COUNT_ST_JSON;
			ret = parseJsonCurLevel((char *)jsonChs, datlen, stJsons, &jsonNum);
			if (ret != 0) {
				MSG_LOG("Fail to parseJsonCurLevel1\n");
				return -2;
			}
#if 0
#ifdef _debug_
			tmpI = 500;
			ret = getJsonString(stJsons, jsonNum, jsonChs1, &tmpI);
			MSG_LOG("JSON数据1:");
			com_snd(DEBUG_COM, tmpI, (unsigned char *)jsonChs1);
#endif
#endif
			pTmpJson = getJsonObjByName(stJsons, jsonNum, "ReturnCode");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少应答码对象\n");
				return -2;
			}

			ret = -1;
			switch ((pTmpJson->pmValue[0] << 8) | pTmpJson->pmValue[1])
			{
			case 0x3030:
				ret = 0;
				MSG_LOG("成功\n");
				break;
			case 0x3031:
				MSG_LOG("无此交易类型/交易类型不匹配\n");
				break;
			case 0x3936:
				MSG_LOG("系统故障\n");
				break;
			case 0x3032:
				MSG_LOG("无此厂商标识\n");
				break;
			case 0x3033:
				MSG_LOG("无此终端型号标识\n");
				break;
			case 0x3034:
				MSG_LOG("无此硬件序列号\n");
				break;
			case 0x3035:
				MSG_LOG("无此下载类型\n");
				break;
			case 0x3036:
				MSG_LOG("应用名称错误\n");
				break;
			case 0x3037:
				MSG_LOG("无授权\n");
				break;
			case 0x3038:
				MSG_LOG("无此逻辑终端/无匹配逻辑终端\n");
				break;
			case 0x3130:
				MSG_LOG("不用下载\n");
				break;
			case 0x3131:
				MSG_LOG("终端状态异常\n");
				break;
			case 0x3132:
				MSG_LOG("密钥不存在\n");
				break;
			case 0x3133:
				MSG_LOG("双向认证失败\n");
				set_TMS_switch_control(17,set_Key_activation_FLAG);
				break;
			case 0x3134:
				MSG_LOG("该终端已做初始化\n");
				break;
			case 0x3135:
				MSG_LOG("策略未分配\n");
				break;
			case 0x3136:
				MSG_LOG("未上传该版本程序\n");
				break;
			case 0x3137:
				MSG_LOG("PC 工具未下载该版本程序\n");
				break;
			case 0x3138:
				MSG_LOG("激活码错误\n");
				break;
			case 0x3139:
				MSG_LOG("必传字段为空或不合法\n");
				break;
			default:
				MSG_LOG("未知应答码:%s\n", pTmpJson->pmValue);
				break;
			}

			if (ret != 0) {
				MSG_LOG("接口返回错误, 请检查\n");
				return ret;
			}
		}
	}

	switch (s_tmsStatus)
	{
	case Key_activation:

		pTmpJson = getJsonObjByName(stJsons, jsonNum, "MKey2");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少MKey2对象\n");
			return -2;
		}

#if test_
		memcpy(kek1, KEYM_1, 16);
#else
		getMobileParameter(9, kek1);
#endif

		tmpI = sizeof(kek1);
		ret = CharsToBytes(pTmpJson->pmValue, pTmpJson->len, jsonChs + datlen, &tmpI);
		if (ret != Ret_OK) {
			MSG_LOG("String2Bytes:%d\n", ret);
			return ret;
		}

		BCD_LOG(jsonChs + datlen, tmpI, 1);

		DES3_decrypt(jsonChs + datlen, kek1, kek2);
		DES3_decrypt(jsonChs + datlen + 8, kek1, kek2 + 8);

		MSG_LOG("KEK2:");
		BCD_LOG(kek2, tmpI, 1);
#if test_
	//	memcpy(kek1, KEYM_1, 16);
#else
		saveMobileParameter(7, kek2);
#endif

	
		set_TMS_switch_control(17,set_Key_activation_FLAG);
		// 发送下一个指令
		s_tmsStatus = Key_download;
		break;
	case Key_download:
		pTmpJson = getJsonObjByName(stJsons, jsonNum, "KEY1");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少KEY1对象\n");
			return -2;
		}

		tmpI = COUNT_ST_JSON - jsonNum;
		ret = parseJsonCurLevel(pTmpJson->pmValue, pTmpJson->len, stJsons + jsonNum, &tmpI);
		if (ret != 0) {
			MSG_LOG("Fail to parseJsonCurLevel1\n");
			return -2;
		}
		pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "KeyIndex");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少KeyIndex对象\n");
			return -2;
		}
		//if ((int)(pTmpJson->number) == 0) // KEK,用于签到

		pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "Key");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少Key对象\n");
			return -2;
		}
#if test_
		memcpy(kek2, KEYM_2, 16);
		
#else
	
		getMobileParameter(7, kek2);
#endif


		tmpI = sizeof(kek2);
		ret = String2Bytes(pTmpJson->pmValue, pTmpJson->len, jsonChs + datlen, &tmpI);
		if (ret != Ret_OK) {
			MSG_LOG("String2Bytes:%d\n", ret);
			return ret;
		}

		BCD_LOG(jsonChs + datlen, tmpI, 1);
		BCD_LOG(kek2,16,1);
		DES3_decrypt(jsonChs + datlen, kek2, kek1);
		DES3_decrypt(jsonChs + datlen + 8, kek2, kek1 + 8);

		MSG_LOG("消费秘钥:");
		BCD_LOG(kek1, tmpI, 1);
#if test_
;
		
#else
	
			saveMobileParameter(2, kek1);
#endif


		set_TMS_switch_control(18,set_Update_check_flag);
		s_tmsStatus = Update_check;
		break;
	case Update_check:
		
			MSG_LOG("开始处理更新检查\n");
		pTmpJson = getJsonObjByName(stJsons, jsonNum, "APP1");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少APP1对象\n");
			return -2;
		}

		tmpI = COUNT_ST_JSON - jsonNum;
		ret = parseJsonCurLevel(pTmpJson->pmValue, pTmpJson->len, stJsons + jsonNum, &tmpI);
	
		
		if (ret != 0) {
			MSG_LOG("Fail to parseJsonCurLevel3\n");
			return -2;
		}
		pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPIndex");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少APPIndex对象\n");
			return -2;
		}
		if (memcmp(pTmpJson->pmValue, APPIndex, strlen(APPIndex)) == 0) {	// 索引一致
			if (pTmpJson->len > sizeof(s_serverVer) - 1) {
				MSG_LOG("s_serverVer空间过小:%d\n", pTmpJson->len);
				return -2;
			}
			pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPVer");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少APPVer对象\n");
				return -2;
			}

			memcpy(s_serverVer, pTmpJson->pmValue, pTmpJson->len);
			s_serverVer[pTmpJson->len] = '\0';

		//参数
			pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "Parameter");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少Parameter对象\n");
				return -2;
			}
			
			memcpy(para_buff, pTmpJson->pmValue, pTmpJson->len);
			para_buff[pTmpJson->len] = '\0';
			MSG_LOG("JSON数据Parameter对象=%s\n",para_buff);
			deal_para((char *)para_buff,pTmpJson->len);//處理參數，add-hbguo

		}
		else {

			pTmpJson = getJsonObjByName(stJsons, jsonNum, "APP2");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少APP1对象\n");
			//	return -2;
			}

			tmpI = COUNT_ST_JSON - jsonNum;
			ret = parseJsonCurLevel(pTmpJson->pmValue, pTmpJson->len, stJsons + jsonNum, &tmpI);
			if (ret != 0) {
				MSG_LOG("Fail to parseJsonCurLevel3\n");
			//	return -2;
			}
			pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPIndex");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少APPIndex对象\n");
				return -2;
			}
			if (memcmp(pTmpJson->pmValue, APPIndex, strlen(APPIndex)) == 0) {	// 索引一致
				if (pTmpJson->len > sizeof(s_serverVer) - 1) {
					MSG_LOG("s_serverVer空间过小1:%d\n", pTmpJson->len);
					return -2;
				}

				pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPVer");

				if (pTmpJson == NULL) {
					MSG_LOG("JSON数据缺少APPVer对象\n");
					return -2;
				}

				memcpy(s_serverVer, pTmpJson->pmValue, pTmpJson->len);
				s_serverVer[pTmpJson->len] = '\0';
			//参数
			
			pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "Parameter");

			if (pTmpJson == NULL) {
				MSG_LOG("JSON数据缺少Parameter对象\n");
				return -2;
			}
			
			memcpy(para_buff, pTmpJson->pmValue, pTmpJson->len);
			para_buff[pTmpJson->len] = '\0';
			MSG_LOG("JSON数据Parameter对象=%s\n",para_buff);
			deal_para((char *)para_buff,pTmpJson->len);
			}
		}
#if 0
		tmpI = sprintf(jsonChs, "1.0.%03X", SOFT_VER_TIME_LOG);
		if (memcmp(s_serverVer, jsonChs, pTmpJson->len) <= 0) {
			s_tmsStatus = Tms_Over;
			break;
		}
#endif
		s_tmsStatus = Download_query;
		break;
	case Download_query:
	
		pTmpJson = getJsonObjByName(stJsons, jsonNum, "APP1");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少APP1对象\n");
			return -2;
		}

		tmpI = COUNT_ST_JSON - jsonNum;
		ret = parseJsonCurLevel(pTmpJson->pmValue, pTmpJson->len, stJsons + jsonNum, &tmpI);
		if (ret != 0) {
			MSG_LOG("Fail to parseJsonCurLevel.Download_query\n");
			return -2;
		}
		pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPPath");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少APPPath对象\n");
			return -2;
		}
		if (pTmpJson->len >= sizeof(pTmsFileDown->ipPortPath)) {
			MSG_LOG("s_downFilePath空间过小1:%d\n", pTmpJson->len);
			return -2;
		}
		memset(&s_tmsFileDown, 0x00, sizeof(stTmsFileDown));

		//memcpy(pTmsFileDown->filePath, pTmpJson->pmValue, pTmpJson->len);
		//pTmsFileDown->filePath[pTmpJson->len] = '\0';
		start_p = strstr(pTmpJson->pmValue, "//");
		if (start_p == NULL) {
			MSG_LOG("文件路径中无域名://\n");
			return -2;
		}
		start_p += 2;
		j = 0;
		for (i = start_p - pTmpJson->pmValue; i < pTmpJson->len; ++i)
		{
			if (pTmpJson->pmValue[i] == ' ') {
				pTmsFileDown->ipPortPath[j] = '%';
				++j;
				pTmsFileDown->ipPortPath[j] = '2';
				++j;
				pTmsFileDown->ipPortPath[j] = '0';
				++j;
			}
			else {
				pTmsFileDown->ipPortPath[j] = pTmpJson->pmValue[i];
				++j;
			}
		}
		pTmsFileDown->ipPortPath[j] = '\0';

		start_p = strstr(pTmsFileDown->ipPortPath, "/");
		if (start_p == NULL) {
			MSG_LOG("文件路径中无分隔符:/\n");
			return -2;
		}
		start_p[0] = '\0';
		pTmsFileDown->pFilePathPos = start_p + 1;


		start_p = strstr(pTmsFileDown->ipPortPath, ":");
		if (start_p == NULL) {
			MSG_LOG("文件路径中无端口号:/\n");
			pTmsFileDown->tmsport = 80;
		}
		else {
			start_p[0] = '\0';
			++start_p;
			ret = StringDecimal2UInt32(start_p, strlen(start_p), &pTmsFileDown->tmsport);
			if (ret != Ret_OK) {
				MSG_LOG("文件路径中StringDecimal2UInt32端口转换失败:\n");
				return -8;
			}
		}

		pTmpJson = getJsonObjByName(stJsons + jsonNum, tmpI, "APPFileSize");

		if (pTmpJson == NULL) {
			MSG_LOG("JSON数据缺少APPFileSize对象\n");
			return -2;
		}
		ret = StringDecimal2UInt32(pTmpJson->pmValue, pTmpJson->len, &valUInt32);
		if (ret != 0) {
			MSG_LOG("Fail to StringDecimal2UInt32.alue, pTmpJson->len, &valUInt32\n");
			return -2;
		}
		pTmsFileDown->tfd_fileLen = valUInt32;
		MSG_LOG("s_tmsFileDown:%s/%s\n", pTmsFileDown->ipPortPath, pTmsFileDown->pFilePathPos);
		BCD_LOG((unsigned char *)&s_tmsFileDown, 12, 1);
		sysfewrite(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char *)&s_tmsFileDown);
		s_tmsStatus = Download_File;
		break;
	case Download_File:
		start_p = strstr(pData, "Content-Range:");
		if (start_p == NULL) {
			MSG_LOG("下载响应数据包中无Content-Range:\n");
			goto DOWN_CONTINUE_ERROR;
		}
		start_p = strstr(start_p, "bytes");
		if (start_p == NULL) {
			MSG_LOG("下载响应数据包中Content-Range:格式非法\n");
			goto DOWN_CONTINUE_ERROR;
		}
		start_p += 5;
		while (start_p[0] < '0' || start_p[0] > '9')
		{
			if (start_p[0] == '\0') {
//				start_p == NULL;
				break;
			}
			++start_p;
		}
		pTmpCh = strstr(start_p, "-");
		if (pTmpCh == NULL) {
			MSG_LOG("下载响应数据包中Content-Range:格式非法,无-\n");
			goto DOWN_CONTINUE_ERROR;
		}
		ret = StringDecimal2UInt32(start_p, pTmpCh - start_p, &valUInt32);
		if (ret != Ret_OK) {
			MSG_LOG("Fail to StringDecimal2UInt32.t32(start_p, pTmpCh - start_p, &va\n");
			goto DOWN_CONTINUE_ERROR;
		}
		MSG_LOG("偏移:%d\n", valUInt32);
		if (pTmsFileDown->tfd_downLen != valUInt32) {
			MSG_LOG("下载响应数据包中偏移量与设备中偏移量不一致:%d\n", pTmsFileDown->tfd_downLen);
			goto DOWN_CONTINUE_ERROR;
		}
		start_p = pTmpCh + 1;

		// 判断包大小
		pTmpCh = strstr(start_p, "/");
		if (pTmpCh == NULL) {
			MSG_LOG("下载响应数据包中Content-Range:格式非法,无/\n");
			goto DOWN_CONTINUE_ERROR;
		}
		ret = StringDecimal2UInt32(start_p, pTmpCh - start_p, &valUInt32);
		if (ret != Ret_OK) {
			MSG_LOG("Fail to StringDecimal2UInt32.tnt32(start_p, start_p - pTmpCh, &val&va\n");
			goto DOWN_CONTINUE_ERROR;
		}
		MSG_LOG("偏移截止:%d\n", valUInt32);
		if ((pTmsFileDown->tfd_downLen + datlen - 1) != valUInt32) {
			MSG_LOG("下载响应数据包中偏截止与设备中偏截止不一致:%d,%d\n", LENUNIT_DOWN, datlen);
			goto DOWN_CONTINUE_ERROR;
		}

		++pTmpCh;
		start_p = pTmpCh;
		while (pTmpCh[0] >= '0' && pTmpCh[0] <= '9')
		{
			++pTmpCh;
		}
		ret = StringDecimal2UInt32(start_p, pTmpCh - start_p, &valUInt32);
		if (ret != Ret_OK) {
			MSG_LOG("Fail to St1111ringDecimal2UInt32.t32(start_p, pTmpCh - start_p, &va\n");
			goto DOWN_CONTINUE_ERROR;
		}
		if (pTmsFileDown->tfd_fileLen != valUInt32) {
			MSG_LOG("下载响应数据包中大小与设备中大小不一致:%d\n", pTmsFileDown->tfd_fileLen);
			goto DOWN_CONTINUE_ERROR;
		}
		MSG_LOG("大小:%d\n", valUInt32);

	DOWNFILE_WRITE:

		if (pTmsFileDown->tfd_downLen == 0) {
			addr = FLASH_PRO_START;//写入的地址 程序起始地址
			for (i = 0; i < 7; i++) {
#ifdef debug_GJ_TLVDeal_
				debugstring("清空存贮器X:");
				debugdata((unsigned char*)&addr, 4, 1);
#endif
				flasherase(ERASE64KBYTESECTOR, addr);//先擦除 256K
				addr += 0x10000;//下一个64K的块
			}
			flasherase(ERASE4KBYTESECTOR, FLASH_PRO_FLAG);//先擦除标志区4K
		}

		valUInt32 = pTmsFileDown->tfd_fileLen;
			
		
		sprintf((char*)buftemp, "P:%d%% ", ((pTmsFileDown->tfd_downLen * 100) / valUInt32));
		display(6, 0, "下载中", DIS_CENTER);
		display(10, 0, (char*)buftemp, DIS_CENTER);
		set_timer0(10, 0);//10ms钟进一次
		addr = (FLASH_PRO_START + pTmsFileDown->tfd_downLen);//写入的地址，
		if (addr > FLASH_PRO_END) {//地址出错，此包不写入，重新申请此包数据
			restore_flag = 1;
			goto DOWN_CONTINUE_ERROR;
		}
		else {
			for (i = 0; i < 3; i++) {
				flashwrite(addr, jsonChs, datlen);//写入
				flashread(addr, buftemp, datlen);
				if (memcmp(jsonChs, buftemp, datlen) == 0) {
					break;
				}
				else {
#ifdef debug_GJ_TLVDeal_
					debugstring("数据写入错误\r\n");
//					debugdata(dfiledata, datlen, 1);
					debugdata(buftemp, datlen, 1);
#endif
					continue;
				}
			}

			if (i >= 3)
			{
#ifdef debug_GJ_TLVDeal_
				debugstring("从头开始\r\n");
#endif
				addr = FLASH_PRO_START;//写入的地址 程序起始地址

				pTmsFileDown->tfd_downLen = 0;
				//memset((unsigned char*)pTmsFileDown, 0, sizeof(stTmsFileDown));
			}
		}
		s_errTime = 0;
		pTmsFileDown->tfd_downLen += datlen;
		if (pTmsFileDown->tfd_downLen >= pTmsFileDown->tfd_fileLen)//下载完成
		{

			MSG_LOG("file len:%d\r\n", pTmsFileDown->tfd_downLen);

#if 0
			flashread(FLASH_PRO_START + 0x1000, buftemp, 64);	//读出标识
			buftemp[63] = 0;
			if (memcmp(MachineCode, buftemp, strlen(MachineCode)) != 0) {//不是这个车载机的程序
				cls();
				MessageBox(1, "不是本设备程序");
				display(6, 0, (char*)buftemp, DIS_CENTER);
				beep(5, 50, 50);
				delayxms(1);
			}
#endif

			memset(buftemp, 0, 64);
			memcpy(buftemp, "程序需要更新", 12);
#warning ("py(buftemp + 12, gsl8583FileDownPara.Miss_v")
			//memcpy(buftemp + 12, gsl8583FileDownPara.Miss_ver, 2);
			memcpy(buftemp + 14, (unsigned char*)&SysTime, 7);
#ifdef debug_GJ_TLVDeal_
			debugstring("程序下载完成:");
			debugdata(buftemp, 21, 1);
#endif
#ifdef _AREA_ENG_
			display(3, 0, "Restart.....", DIS_CENTER);
#else
			display(3, 0, "完成,请重启...", DIS_CENTER);
#endif
			addr = FLASH_PRO_FLAG;

#ifdef debug_GJ_TLVDeal_
			debugstring("程序下载完成2:");
			debugdata(buftemp, 34, 1);
#endif
			flashwrite(addr, buftemp, 64);

#ifdef debug_GJ_TLVDeal_
			debugstring("程序下载完成3:");
			flashread(addr, buftemp + 200, 64);
			debugdata(buftemp + 200, 34, 1);
#endif

			memset(pTmsFileDown, 0, sizeof(stTmsFileDown));
			sysfewrite(BIT_TMS_FILE_DOWNLOAD, sizeof(stTmsFileDown), (unsigned char*)pTmsFileDown);//保存任务参数
			beep(1, 10, 10);
			delayxms(400);
			ReStartCPU();//下载完成，重启设备以便更新程序。
			while (1);
		}
		else {
			sysfewrite(BIT_TMS_FILE_DOWNLOAD + 4, 4, (unsigned char *)&pTmsFileDown->tfd_downLen);
		}
		goto DOWN_CONTINUE;
	DOWN_CONTINUE_ERROR:
		MSG_LOG("DOWN_CONTINUE_ERROR222222222:\n");
		++s_errTime;
	DOWN_CONTINUE:
		//s_tmsStatus = 
		s_isDownloadSend = 0;
		break;
	default:
		break;
	}


#endif

	return 0;

#undef TMS_HTTP_HEAD
#undef COUNT_ST_JSON
}

void one_updata(void)
{
		//buildSeverInfo_06tls_TMS(0x26, 5);
		ProcessTmsData();
		buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));//tms走完 轮训ip
}
void two_updata(void)
{
		//buildSeverInfo_06tls_TMS(0x26, 5);
		ProcessTmsData_2();
		buildSeverInfo_06tls(0x26, 5,Get_QPBOC_IP_MODE(IP_MODE_shift));//tms走完 轮训ip
}
extern void down_kek_TMS(void);
void TMS_SET(void)
{
	unsigned char ucMENU_NUM = 0;
	char menu[15][20];
	void(*function[15])(void);

	unsigned int i = 0;

	for (i = 0; i < 15; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}


	ucMENU_NUM = 0;
	strcpy(menu[ucMENU_NUM], "分量1导入     ");
	function[ucMENU_NUM++] = down_kek_TMS;
	strcpy(menu[ucMENU_NUM], "一键更新      ");
	function[ucMENU_NUM++] = one_updata;
	strcpy(menu[ucMENU_NUM], "应用更新      ");
	function[ucMENU_NUM++] = two_updata;
	strcpy(menu[ucMENU_NUM], "母pos密钥导入 ");
	function[ucMENU_NUM++] = two_updata;

	MenuFrame(menu, function, "  --TMS业务--  ", ucMENU_NUM, (100));

}

extern void Showcard(void);
extern void showGPRStR_add(void);
extern void StatsMoney(void);
void TMS_QU_RECODE(void)
{
	unsigned char ucMENU_NUM = 0;
	char menu[5][20];
	void(*function[15])(void);

	unsigned int i = 0;

	for (i = 0; i < 5; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}


	ucMENU_NUM = 0;
	strcpy(menu[ucMENU_NUM], "交易明细    ");
	function[ucMENU_NUM++] = Showcard;
	strcpy(menu[ucMENU_NUM], "交易统计    ");
	function[ucMENU_NUM++] = StatsMoney;
	strcpy(menu[ucMENU_NUM], "未上送信息  ");
	function[ucMENU_NUM++] = showGPRStR_add;

	MenuFrame(menu, function, "  --交易查询--  ", ucMENU_NUM, (100));

}
//extern void setdatetime(void);
extern void set_device(void);
void SET_Other(void)
{
	unsigned char ucMENU_NUM = 0;
	char menu[5][20];
	void(*function[15])(void);

	unsigned int i = 0;

	for (i = 0; i < 5; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}


	ucMENU_NUM = 0;
// 	strcpy(menu[ucMENU_NUM], "音量调节    ");
// 	function[ucMENU_NUM++] = Showcard;
	//strcpy(menu[ucMENU_NUM], "时间设置    ");
	//function[ucMENU_NUM++] = setdatetime;
	//strcpy(menu[ucMENU_NUM], "序列号设置  ");
	//function[ucMENU_NUM++] = set_device;

	MenuFrame(menu, function, "  --厂商设置--  ", ucMENU_NUM, (100));

}


void SET_TMS_DEV_INFOR(void)
{
	unsigned char ucMENU_NUM = 0;
	char menu[5][20];
	void(*function[15])(void);

	unsigned int i = 0;

	for (i = 0; i < 5; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}


	ucMENU_NUM = 0;
// 	strcpy(menu[ucMENU_NUM], "音量调节    ");
// 	function[ucMENU_NUM++] = Showcard;
	strcpy(menu[ucMENU_NUM], "终端信息查询  ");
	function[ucMENU_NUM++] = Show_pboc_minu;
	strcpy(menu[ucMENU_NUM], "终端信息设置  ");
	function[ucMENU_NUM++] = set_pboc_menu;

	MenuFrame(menu, function, "  --终端信息--  ", ucMENU_NUM, (100));

}
extern void Show_pboc_minu(void);
extern void showSetOtherPaymode(void);
extern void set_pboc_menu(void);
void QPBOC_menu(void)
{
	unsigned char ucMENU_NUM = 0;
	char menu[20][20];
	void(*function[20])(void);

	unsigned int i = 0;

	for (i = 0; i < 20; i++)
	{
		memset(menu[i], 0, sizeof(menu[i]));
	}


	ucMENU_NUM = 0;
	strcpy(menu[ucMENU_NUM], "终端信息          ");
	function[ucMENU_NUM++] =SET_TMS_DEV_INFOR ;
	strcpy(menu[ucMENU_NUM], "业务模式          ");
	function[ucMENU_NUM++] = showSetOtherPaymode;
	strcpy(menu[ucMENU_NUM], "TMS业务          ");
	function[ucMENU_NUM++] = TMS_SET;
	strcpy(menu[ucMENU_NUM], "交易记录查询   ");
	function[ucMENU_NUM++] = TMS_QU_RECODE;
	strcpy(menu[ucMENU_NUM], "通讯设置     ");
	function[ucMENU_NUM++] = showSetOtherPaymode;
	strcpy(menu[ucMENU_NUM], "厂商设置   ");
	function[ucMENU_NUM++] = SET_Other;

	MenuFrame(menu, function, "  --银商菜单--  ", ucMENU_NUM, (100));

}

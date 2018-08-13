#ifndef _TMS_H_
#define _TMS_H_

#if 0
#define HTTP_POST_POST "POST /TMSWebService/nostandard/%05d HTTP/1.1\r\n"
#define HTTP_POST_HOST  "HOST:qr-test1.chinaums.com\r\n"
#define HTTP_POST_FID   "FID:ESF3000L\r\n"
#define HTTP_POST_MID   "MID:%s\r\n"
#define HTTP_POST_SN     "SN:ESF3000L%s\r\n"
#define HTTP_POST_VER    "VER:01\r\n"
#define HTTP_POST_ENCODING  "Encoding:UTF-8\r\n"
#define HTTP_POST_TransCode  "TransCode:%03x\r\n"
#define HTTP_POST_Random     "Random:%s\r\n"
#define HTTP_POST_Authinfo    "Authinfo:%s\r\n"
#define HTTP_POST_CONTENT_TYPE  "Content-type:application/json\r\n"
#define HTTP_POST_CONTENT_LENGTH "Content-Length:%d\r\n"
#else
#define HTTP_POST_POST "POST /TMSWebService/nostandard/%05d HTTP/1.1\r\n"
#define HTTP_POST_HOST  "HOST: qr-test1.chinaums.com:8001\r\n"
#define HTTP_POST_FID   "FID: LANDI\r\n"
#define HTTP_POST_MID   "MID: %s\r\n"
#define HTTP_POST_SN     "SN: ESF3000L%s\r\n"
#define HTTP_POST_VER    "VER: 1.0\r\n"
#define HTTP_POST_ENCODING  "Encoding: utf-8\r\n"
#define HTTP_POST_TransCode  "TransCode: %03x\r\n"
#define HTTP_POST_Random     "Random: %s\r\n"
#define HTTP_POST_Authinfo    "Authinfo: %s\r\n"
#define HTTP_POST_CONTENT_TYPE  "Content-type: application/json\r\n"
#define HTTP_POST_CONTENT_LENGTH "Content-Length: %d\r\n"
#endif

// 
// #define HOST_10001 "58.247.0.18:8001/TMSWebService/standard/10001"
// #define HOST_10002 "58.247.0.18:8001/TMSWebService/standard/10002"
// #define HOST_10003 "58.247.0.18:8001/TMSWebService/standard/10003"
// #define HOST_10004 "58.247.0.18:8001/TMSWebService/standard/10004"
// #define HOST_10005 "58.247.0.18:8001/TMSWebService/standard/10005"
#define Update_check			0x10
#define Download_query			0x11
#define Key_activation			0x20
#define Key_download			0x30
#define Device_key_Download		0x31
#define Tms_Over				0xFE

#define Download_File			0xF0

#define set_Key_activation_FLAG		0xAB //ab tms初装机标志
#define set_Update_check_flag		0xAB
#define set_TMS_START_FLAGE_FLAG	0xAB


#define TMS_sign_shift 0
#define IP_MODE_shift 1
#define IP_wait_time_shift 2
#define IP_ok_shift 3

#define QPBOC_IP_MODE_FRIST 12
extern int ProcessTmsData(void);

extern int QPBOC_TMS_DataDeal(char *pData, int len);
extern void TMS_SET(void);
extern unsigned char  show_para_tms(void);
extern unsigned char switch_IP2INT(unsigned char mode,unsigned  char *dataip);


extern int ProcessTmsData_2(void);



extern void set_TMS_switch_control(unsigned char mode, unsigned char data);
extern unsigned char get_switch_control(unsigned char mode);
extern void GET_string_IP(unsigned char *dataip);

extern void QPBOC_IP_MODE_INIT(void);
extern void set_QPBOC_IP_MODE(unsigned char member ,unsigned char mode);
extern unsigned char Get_QPBOC_IP_MODE(unsigned char member);
extern unsigned char SWITCH_build_06(unsigned char mode);
extern void switch_ip_time(unsigned char mode);
extern void show_dns_ip(void);
extern void QPBOC_menu(void);
#endif

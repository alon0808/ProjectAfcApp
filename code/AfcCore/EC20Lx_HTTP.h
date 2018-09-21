#ifndef _INCLUDE_EC20LX_HTTP_H
#define _INCLUDE_EC20LX_HTTP_H

//#include "SlzrTypeDef.h"

#define BLKindex_LIST_FILE_NAME WorkDir"BLKindex"
#define BLACK_LIST_FILE_NAME WorkDir"blacklist"
#define BUSPRO_LIST_FILE_NAME WorkDir"buspro"
#define PROindex_LIST_FILE_NAME WorkDir"PROindex"
#define EC20PRO_LIST_FILE_NAME WorkDir"ec20pro"


#define SL8583FileFLAG_PRO	"PRO"	//�����ļ����ա���������POS�ն˽���淶v2.00.docx�� �޶�ͨѶ��ʽ 
#define SL8583FileFLAG_BLK "BLK"	//������
#define SL8583FileFLAG_GPS "GPS"	//��λ������Ϣ
#define SL8583FileFLAG_CSN "CSN"	//
#define SL8583FileFLAG_PRI "PRI"	//Ʊ����Ϣ�汾,��ʱû��
#define SL8583FileFLAG_PKI "PKI"	//��Կ��Ϣ
#define SL8583FileFLAG_WHT	"WHT"	//������ ����������������������������

#define SL8583FileFLAG_EC20	"EC2"	//Linux ģ���Ӧ�ó���
#define SL8583FileFLAG_BUS	"BUS"	//Linux ģ���Ӧ�ó���

#define HTTP_FINISH_DOWN 0
#define HTTP_NEED_DOWN 0x01
#define HTTP_START_DOWN 0x02


typedef struct
{
	int Dtype;	//��������
	long hasrecieve;		//�Ѿ����յ����ļ���С
	long allLen;			//�ļ��ܴ�С
	char FFlag[3];			//�ļ���ʶ
	char FVer[2];			//�ļ��汾
	char Filename[32];		//�ļ���
	char HttpAddr[2048];	//url
}stHttpDownInfo;

extern stHttpDownInfo gHttpDinfo;
extern pthread_t tidmain_mainDownload;

#ifdef __cplusplus
extern "C" {
#endif
	extern void* main_HTTPDataDown(void *arg);
#ifdef __cplusplus
}
#endif

#endif

